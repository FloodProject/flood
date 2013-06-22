/*
 * Licensed under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

using Mono.Options;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace Flood.Tools.RPCGen
{
    public class Options
    {
        public bool Verbose = false;
        public bool ShowHelpText = false;
        public bool OutputDebug = false;
        public string OutputNamespace;
        public string OutputDir;
        public string MSBuildProjectPath;
        public string Assembly;
    }

    public class Program
    {
        static void ShowHelp(OptionSet options)
        {
            var module = System.Diagnostics.Process.GetCurrentProcess().MainModule;
            var exeName = Path.GetFileName(module.FileName);
            Console.WriteLine("Usage: " + exeName + " [options]+ assembly");
            Console.WriteLine("Generates Thrift code from .NET assembly files.");
            Console.WriteLine();
            Console.WriteLine("Options:");
            options.WriteOptionDescriptions(Console.Out);
        }

        static bool ParseCommandLineOptions(String[] args, Options options)
        {
            var set = new OptionSet()
                {
                    // Compiler options
                    { "ns|namespace=", v => options.OutputNamespace = v },
                    { "o|outdir=", v => options.OutputDir = v },
                    { "debug", v => options.OutputDebug = true },
                    // Misc. options
                    { "m|msbuild=",  v => { options.MSBuildProjectPath = v; } },
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

        static bool ParseAssembly(string path, out Assembly assembly)
        {
            assembly = null;

            if (string.IsNullOrWhiteSpace(path))
            {
                Console.WriteLine("Error: no assembly provided");
                return false;
            }

            try
            {
                var fullPath = Path.GetFullPath(path);
                var pdbPath = Path.ChangeExtension(fullPath,".pdb");
                var assemblyBytes = File.ReadAllBytes(fullPath);
                if (File.Exists(pdbPath))
                {
                    var pdbBytes = File.ReadAllBytes(pdbPath);
                    assembly = Assembly.Load(assemblyBytes, pdbBytes);
                }
                else
                {
                    assembly = Assembly.Load(assemblyBytes);
                }
            }
            catch 
            {
                Console.WriteLine("Error: assembly '{0}' could not be loaded", path);
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

            Assembly assembly;
            if (!ParseAssembly(options.Assembly, out assembly))
                return 1;

            var compiler = new Compiler(options, assembly);

            try
            {
                compiler.Process();
                compiler.Compile(options.Assembly);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                return 1;
            }
            

            if(!string.IsNullOrEmpty(options.MSBuildProjectPath)){
                MSBuildUpdater.UpdateMSBuild(options.MSBuildProjectPath,compiler.GeneratedFiles);
            }
            
            return 0;
        }
    }
}
