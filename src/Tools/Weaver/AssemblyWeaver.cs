using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using EngineWeaver.Util;
using Mono.Cecil;

namespace Weaver
{
    public class AssemblyWeaver
    {
        internal readonly ModuleDefinition TargetModule;

        internal CecilCopier Copier;

        public AssemblyWeaver()
            : this(CecilUtils.CreateEmptyAssemblyDef())
        {
        }

        public AssemblyWeaver(string targetAssemblyPath)
            : this(CecilUtils.GetAssemblyDef(targetAssemblyPath))
        {
        }

        private AssemblyWeaver(AssemblyDefinition targetAssemblyDef)
        {
            TargetModule = targetAssemblyDef.MainModule;
            Copier = new CecilCopier(targetAssemblyDef.MainModule);

            AddSearchDirectory(Directory.GetCurrentDirectory());
        }

        public void CopyAssembly(string origAssemblyPath)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);

            foreach (var origType in origAssembly.MainModule.Types)
                Copier.Copy(origType);

            Copier.Process();
            CheckErrors();
        }

        public void CopyTypes(string origAssemblyPath, IEnumerable<Type> types, bool areStubTypes = false)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);

            foreach (var type in types)
            {
                var typeDef = CecilUtils.GetTypeDef(origAssembly.MainModule, type);

                Copier.Copy(typeDef, areStubTypes);
            }

            Copier.Process();
            CheckErrors();
        }

        public void MergeTypes(string origAssemblyPath, Dictionary<string, string> types)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);

            foreach (var typeKV in types)
            {
                var origTypeDef = CecilUtils.GetTypeDef(origAssembly.MainModule, typeKV.Key);
                var destTypeDef = CecilUtils.GetTypeDef(TargetModule, typeKV.Value);

                Copier.Merge(origTypeDef, destTypeDef);
            }

            Copier.Process();
            CheckErrors();
        }

        public void Write(string outputAssemblyPath)
        {
            var fileName = Path.GetFileNameWithoutExtension(outputAssemblyPath);
            TargetModule.Assembly.Name.Name = fileName;
            var writerParameters = new WriterParameters();
            writerParameters.WriteSymbols = TargetModule.HasSymbols;
            writerParameters.SymbolWriterProvider = new Mono.Cecil.Pdb.PdbWriterProvider();
            TargetModule.Assembly.Write(outputAssemblyPath, writerParameters);
        }

        private void CheckErrors()
        {
            if (Copier.errors.Count == 0)
                return;

            var msg = new StringBuilder();
            msg.AppendLine("Errors found.");

            foreach (var error in Copier.errors)
            {
                msg.AppendLine(error.ToString());
            }
            
            throw new Exception(msg.ToString());
        }

        public static HashSet<string> GetReferences(string assemblyPath)
        {
            var assemblyDef = CecilUtils.GetAssemblyDef(assemblyPath);

            var ret = new HashSet<string>();
            foreach (var reference in assemblyDef.MainModule.AssemblyReferences)
            {
                var name = reference.Name;

                if (Path.GetExtension(reference.Name) != ".dll")
                    name += ".dll";

                ret.Add(name);
            }
            
            return ret;
        }

        public void AddSearchDirectory(string directory)
        {
            ((DefaultAssemblyResolver)TargetModule.AssemblyResolver).AddSearchDirectory(directory);
        }
    }
}
