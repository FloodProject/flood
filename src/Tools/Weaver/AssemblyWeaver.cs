using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Mono.Cecil;
using Weaver.Util;

namespace Weaver
{
    public class AssemblyWeaverException : Exception
    {
        public List<IError> Errors;
        
        public AssemblyWeaverException(List<IError> errors)
            : base(GetErrorMeassage(errors))
        {
            Errors = errors;
        }

        private static string GetErrorMeassage(List<IError> errors)
        {
             var msg = new StringBuilder();
            msg.AppendLine("Errors found.");

            foreach (var error in errors)
            {
                msg.AppendLine(error.ToString());
            }

            return msg.ToString();
        }
    }

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

            foreach (var reference in origAssembly.MainModule.AssemblyReferences)
                if(!TargetModule.AssemblyReferences.Any(r => r.Name == reference.Name && r.Version == reference.Version))
                    TargetModule.AssemblyReferences.Add(reference);

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

        public void AddReference(string fromAssemblyPath, string referenceName)
        {
            var directory = Path.GetDirectoryName(fromAssemblyPath);
            AddSearchDirectory(directory);

            var assembly = CecilUtils.GetAssemblyDef(fromAssemblyPath);

            var reference = assembly.MainModule.AssemblyReferences.Single(r => r.Name == referenceName);

            TargetModule.AssemblyReferences.Add(reference);
        }

        public void AddReference(string assemblyPath)
        {
            var directory = Path.GetDirectoryName(assemblyPath);
            AddSearchDirectory(directory);

            var assembly = CecilUtils.GetAssemblyDef(assemblyPath);

            TargetModule.AssemblyReferences.Add(assembly.Name);
        }

        private void CheckErrors()
        {
            if(Copier.Errors.Any())
                throw new AssemblyWeaverException(Copier.Errors);
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
