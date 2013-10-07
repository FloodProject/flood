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
                var typeDef = CecilUtils.GetTypeDef(origAssembly.MainModule, new TypeSignature(type));

                Copier.Copy(typeDef, areStubTypes);
            }

            Copier.Process();
            CheckErrors();
        }

        public void MergeTypes(string origAssemblyPath, Dictionary<TypeSignature, TypeSignature> types)
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

        public void CloneMembers(List<MemberClone> clones)
        {
            foreach (var clone in clones)
            {
                var memberRef = CecilUtils.GetMember(TargetModule, clone.OriginMember);

                var propertyRef = memberRef as PropertyReference;
                if (propertyRef != null)
                {
                    var propertyDef = propertyRef.Resolve();

                    var clone1 = clone;
                    Copier.InitCopy = (origin, copy) =>
                    {
                        if (origin == propertyDef)
                            ((PropertyReference) copy).Name = clone1.Name;

                        if ( origin == propertyDef.GetMethod)
                            ((MethodReference) copy).Name = "get_" + clone1.Name;

                        if (origin == propertyDef.SetMethod)
                            ((MethodReference) copy).Name = "set_" + clone1.Name;
                    };

                    Copier.IsMemoizationEnabled = false;
                    var @ref = Copier.Copy<PropertyDefinition>(propertyDef);
                    Copier.IsMemoizationEnabled = true;
                    Copier.InitCopy = null;

                    var declaringTypeDef = CecilUtils.GetMemberDef(TargetModule, propertyRef.DeclaringType.Resolve());
                    declaringTypeDef.Properties.Add(@ref);
                    continue;
                }

                throw new NotImplementedException();
            }

            Copier.Process();
        }

        public void AddMemberOptions(Dictionary<MemberSignature,MemberOptions> options)
        {
            foreach (var optionsKV in options)
            {
                var memberRef = CecilUtils.GetMember(TargetModule, optionsKV.Key);
                Copier.AddMemberOptions(memberRef, optionsKV.Value);
            }
        }

        public void Write(string outputAssemblyPath)
        {
            var fileName = Path.GetFileName(outputAssemblyPath);
            var fileNameNoExt = Path.GetFileNameWithoutExtension(outputAssemblyPath);

            TargetModule.Name = fileName;
            TargetModule.Assembly.Name.Name = fileNameNoExt;

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

        public void AddAttribute(Type attributeType, Type[] ctorParamTypes, object[] paramArguments)
        {
            var internalAttributeCtor = attributeType.GetConstructor(ctorParamTypes);
            var internalAttributeCtorRef = TargetModule.Import(internalAttributeCtor);
            var internalAttribute = new CustomAttribute(internalAttributeCtorRef);

            for (int i = 0; i < ctorParamTypes.Count(); i++)
            {
                var paramType = internalAttributeCtorRef.Parameters[i].ParameterType;
                var internalAttributeArgument = new CustomAttributeArgument(paramType, paramArguments[i]);
                internalAttribute.ConstructorArguments.Add(internalAttributeArgument); 
            }

            TargetModule.Assembly.CustomAttributes.Add(internalAttribute);
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
