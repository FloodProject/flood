using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Text;
using Microsoft.CSharp;

namespace Weaver.Util
{
    public class DotNetCompiler
    {
        public static void CompileIntoAssembly(string outputPath, IEnumerable<string> references, List<string> files)
        {
            CodeDomProvider provider = new CSharpCodeProvider();
            CompilerParameters cp = new CompilerParameters()
            {
                GenerateExecutable = false,
                OutputAssembly = outputPath,
                IncludeDebugInformation = true,
                CompilerOptions = "/unsafe"
            };

            foreach (var @ref in references)
                cp.ReferencedAssemblies.Add(@ref);

            CompilerResults cr = provider.CompileAssemblyFromFile(cp, files.ToArray());

            if (cr.Errors.HasErrors)
            {
                var message = new StringBuilder();
                message.Append("Error compiling generated files.\n");
                foreach(var error in cr.Errors)
                    message.AppendFormat("  {0}\n",error.ToString());
                throw new Exception(message.ToString());
            }
        }
    }
}
