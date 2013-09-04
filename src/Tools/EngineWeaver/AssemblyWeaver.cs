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
        private Dictionary<ModuleDefinition, CecilCopier> copiers; 

        public AssemblyWeaver(string destAssemblyPath)
        {
            DestinationAssembly = CecilUtils.GetAssemblyDef(destAssemblyPath);
            copiers = new Dictionary<ModuleDefinition, CecilCopier>();
        }

        private CecilCopier GetCreateCopier(ModuleDefinition origModuleDef)
        {
            CecilCopier copier;
            if (copiers.TryGetValue(origModuleDef, out copier))
                return copier;

            copier = new CecilCopier(origModuleDef, DestinationAssembly.MainModule);
            copiers.Add(origModuleDef, copier);

            return copier;
        }

        public void AddAssembly(string origAssemblyPath)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);
            var copier = GetCreateCopier(origAssembly.MainModule);

            foreach (var origType in origAssembly.MainModule.Types)
                if(origType.BaseType != null)
                    copier.Copy(origType);

            copier.Process();
        }

        public void CopyTypes(string origAssemblyPath, IEnumerable<string> typeNames)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);
            var copier = GetCreateCopier(origAssembly.MainModule);

            foreach (var typeName in typeNames)
            {
                var typeDef = CecilUtils.GetTypeDef(origAssembly.MainModule, typeName);

                copier.Copy(typeDef);
            }

            copier.Process();
        }

        public void MergeTypes(string origAssemblyPath, Dictionary<string, string> typeNames)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);
            var copier = GetCreateCopier(origAssembly.MainModule);

            foreach (var typeNameKV in typeNames)
            {
                var origTypeDef = CecilUtils.GetTypeDef(origAssembly.MainModule, typeNameKV.Key);
                var destTypeDef = CecilUtils.GetTypeDef(DestinationAssembly.MainModule, typeNameKV.Value);
                copier.Merge(origTypeDef, destTypeDef);
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
