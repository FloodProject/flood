using System;
using System.Collections.Generic;
using System.IO;
using EngineWeaver.Util;
using Mono.Cecil;

namespace EngineWeaver
{
    public class AssemblyWeaver
    {
        internal readonly AssemblyDefinition DestinationAssembly;

        public AssemblyWeaver(string destAssemblyPath)
        {
            DestinationAssembly = CecilUtils.GetAssemblyDef(destAssemblyPath);
        }

        public void AddAssembly(string origAssemblyPath)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);
            var copier = new CecilCopier(origAssembly.MainModule, DestinationAssembly.MainModule);

            foreach (var origType in origAssembly.MainModule.Types)
                if(origType.BaseType != null)
                    copier.Copy(origType);

            copier.Process();
        }

        public void CopyTypes(string origAssemblyPath, IEnumerable<string> typeNames)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);
            var copier = new CecilCopier(origAssembly.MainModule, DestinationAssembly.MainModule);

            foreach (var typeName in typeNames)
            {
                var typeDef = CecilUtils.GetTypeDef(origAssembly.MainModule, typeName);

                copier.Copy(typeDef);
            }

            copier.Process();
        }

        public void Write(string outputAssemblyPath)
        {
            DestinationAssembly.Name.Name = Path.GetFileName(outputAssemblyPath);
            var writerParameters = new WriterParameters();
            writerParameters.WriteSymbols = DestinationAssembly.MainModule.HasSymbols;
            writerParameters.SymbolWriterProvider = new Mono.Cecil.Pdb.PdbWriterProvider();
            DestinationAssembly.Write(outputAssemblyPath, writerParameters);
        }

        public HashSet<string> GetReferences()
        {
            var ret = new HashSet<string>();
            foreach (var reference in DestinationAssembly.MainModule.AssemblyReferences)
            {
                ret.Add(reference.Name+".dll");
            }
            
            return ret;
        }

    }
}
