
using Ionic.Zip;
using Mono.Options;
using System;
using System.IO;

namespace Flood.Tools.PackageGen
{
    public class Options
    {
        public bool Verbose = false;
        public bool ShowHelpText = false;
        public string OutputDir;
        public string PackageName;
        public string PackageDir;
    }

    public class Program
    {
        static void ShowHelp(OptionSet options)
        {
            var module = System.Diagnostics.Process.GetCurrentProcess().MainModule;
            var exeName = Path.GetFileName(module.FileName);
            Console.WriteLine("Usage: " + exeName + " [options]+ packageName packageDir");
            Console.WriteLine("Generates package archive from a package directory.");
            Console.WriteLine();
            Console.WriteLine("Options:");
            options.WriteOptionDescriptions(Console.Out);
        }

        static bool ParseCommandLineOptions(String[] args, Options options)
        {
            var set = new OptionSet()
                {
                    { "o|outdir=", v => options.OutputDir = v },
                    // Misc. options
                    { "v|verbose",  v => { options.Verbose = true; } },
                    { "h|?|help",   v => options.ShowHelpText = v != null },
                };

            if (args.Length == 0 || options.ShowHelpText)
            {
                ShowHelp(set);
                return false;
            }

            try
            {
                options.PackageName = set.Parse(args)[0].Replace("\"", "");
                options.PackageDir = set.Parse(args)[1].Replace("\"", "");
            }
            catch (OptionException)
            {
                Console.WriteLine("Error parsing the command line.");
                ShowHelp(set);
                return false;
            }

            return true;
        }

        public static int Main(string[] args)
        {
            var options = new Options();

            if (!ParseCommandLineOptions(args, options))
                return 1;

            if (options.OutputDir == null)
                options.OutputDir = options.PackageDir;

            if (!Directory.Exists(options.OutputDir))
                Directory.CreateDirectory(options.OutputDir);

            var packageDllName = options.PackageName + ".dll";
            var packageApiDllName = options.PackageName + ".API.dll";
            var packageArchiveName = options.PackageName + ".fld";
            var packageDllPath = Path.Combine(options.PackageDir, packageDllName);
            var packageApiDllPath = Path.Combine(options.PackageDir, packageApiDllName);
            var packageArchivePath = Path.Combine(options.OutputDir, packageArchiveName);

            System.Console.WriteLine("{0}", packageDllPath);

            //Generate RPC classes
            var rpcCompiler = new RPCGen.Compiler(packageDllPath, options.PackageDir);
            rpcCompiler.Process();
            rpcCompiler.Compile(packageDllPath);

            System.Console.WriteLine("{0}", packageApiDllPath);

            //Generate a PackageName.API.dll with only the PackageName.dll RPC types.
            rpcCompiler.CompileApi(packageApiDllPath);

            //Create an archive
            using (var zip = new ZipFile())
            {
                zip.AddDirectory(options.PackageDir, "");
                zip.Save(packageArchivePath);
            }

            return 0;
        }
    }
}
