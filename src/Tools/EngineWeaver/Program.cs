
using DSLToolkit.Weaver;
using Mono.Cecil;
using Mono.Cecil.Pdb;
using Mono.Options;
using System;
using System.Collections.Generic;
using System.IO;


namespace DSLToolkit
{
    class Program
    {
        static void Main(string[] args)
        {
            bool show_help = false;
		    string outputDir = "";

		    var p = new OptionSet () {
			    "Usage: FloodBuilder [OPTIONS]+ assembly",
			    "Greet a list of individuals with an optional message.",
			    "If no message is specified, a generic greeting is used.",
			    "",
			    /*"Options:",
			    { 
                    "n|name=", 
                    "the {NAME} of someone to greet.",
			        v => names.Add (v) 
                },{ 
                    "r|repeat=", 
				    "the number of {TIMES} to repeat the greeting.\n" + 
					"this must be an integer.",
			        (int v) => repeat = v 
                },{ 
                    "v", 
                    "increase debug message verbosity",
			        v => { if (v != null) ++verbosity; } 
                },*/{
                    "h|help",  
                    "show this message and exit", 
			         v => show_help = v != null 
                },
		    };

		    List<string> extra;
		    try {
			    extra = p.Parse (args);
		    }
		    catch (OptionException e) {
			    Console.Write ("greet: ");
			    Console.WriteLine (e.Message);
			    Console.WriteLine ("Try `greet --help' for more information.");
			    return;
		    }

            if (show_help || extra.Count == 0) {
			    p.WriteOptionDescriptions (Console.Out);
                Console.ReadLine();
			    return;
		    }

            foreach(var assemblyPath in extra)
            {
                try 
                {
                    WeaveAssembly(assemblyPath);
                } 
                catch (Exception e)
                {
                    Console.WriteLine(e);
                    Console.WriteLine(e.InnerException);
                }
            }  

            //WeaveAssembly(@"C:\Users\Marcos\Desktop\vapor\old\BestGUI\Test\bin\Debug\Test.exe");
              

            Console.ReadLine();
        }


        private static void WeaveAssembly(string assemblyPath) 
        {
            var assemblyResolver = new DefaultAssemblyResolver();
            var assemblyLocation = Path.GetDirectoryName(assemblyPath);
            assemblyResolver.AddSearchDirectory(assemblyLocation);
            //if (!string.IsNullOrEmpty(HintPath))
            //{
           // assemblyResolver.AddSearchDirectory(HintPath);
           // }
           // var silverlightAssemblyPath = Environment.ExpandEnvironmentVariables(@”%ProgramFiles%\Reference Assemblies\Microsoft\Framework\Silverlight\v4.0\”);
           // assemblyResolver.AddSearchDirectory(silverlightAssemblyPath);
            var readerParameters = new ReaderParameters { AssemblyResolver = assemblyResolver };
            var writerParameters = new WriterParameters();
            var pdbName = Path.ChangeExtension(assemblyPath, "pdb");
            if (File.Exists(pdbName))
            {
                var symbolReaderProvider = new PdbReaderProvider();
                readerParameters.SymbolReaderProvider = symbolReaderProvider;
                readerParameters.ReadSymbols = true;
                writerParameters.WriteSymbols = true;
            }
            var assemblyDefinition = AssemblyDefinition.ReadAssembly(assemblyPath, readerParameters);

            var weaver = new NotifierWeaver();
            weaver.WeaveModule(assemblyDefinition.MainModule);

            assemblyDefinition.Write(assemblyPath, writerParameters);
        }
    }
}
