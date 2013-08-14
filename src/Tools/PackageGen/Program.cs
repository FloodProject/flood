
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
        public string Assembly;
    }

    public class Program
    {
        static void ShowHelp(OptionSet options)
        {
            var module = System.Diagnostics.Process.GetCurrentProcess().MainModule;
            var exeName = Path.GetFileName(module.FileName);
            Console.WriteLine("Usage: " + exeName + " [options]+ assembly");
            Console.WriteLine("Generates package archive from a package dll.");
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
                options.Assembly = set.Parse(args)[0];
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

            if (!Directory.Exists(options.OutputDir))
                Directory.CreateDirectory(options.OutputDir);

            
            return 0;
        }
    }
}
