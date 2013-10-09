using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using Mono.Cecil;
using Mono.Cecil.Pdb;
using System.IO;
using Mono.Collections.Generic;

namespace Weaver.Util
{
    public class CecilUtils
    {
        public static AssemblyDefinition CreateEmptyAssemblyDef()
        {
            using (var dllPaths = new TemporaryAssemblyPaths())
            {
                DotNetCompiler.CompileIntoAssembly(dllPaths.DllPath, new List<string>(), new List<string>());

                return GetAssemblyDef(dllPaths.DllPath);
            }
        }

        public static AssemblyDefinition GetAssemblyDef(Assembly assembly)
        {
            var assemblyPath = assembly.Location;
            if (assemblyPath == null)
                throw new NullReferenceException();

            return GetAssemblyDef(assemblyPath);
        }

        public static AssemblyDefinition GetAssemblyDef(string assemblyPath)
        {
            assemblyPath = Path.GetFullPath(assemblyPath);

            var assemblyResolver = new DefaultAssemblyResolver();
            var assemblyLocation = Path.GetDirectoryName(assemblyPath);
            assemblyResolver.AddSearchDirectory(assemblyLocation);

            var readerParameters = new ReaderParameters {AssemblyResolver = assemblyResolver};

            var pdbName = Path.ChangeExtension(assemblyPath, "pdb");
            if (File.Exists(pdbName))
            {
                var symbolReaderProvider = new PdbReaderProvider();
                readerParameters.SymbolReaderProvider = symbolReaderProvider;
                readerParameters.ReadSymbols = true;
            }

            var assemblyDef = AssemblyDefinition.ReadAssembly(assemblyPath, readerParameters);

            return assemblyDef;
        }

        public static TypeDefinition GetTypeDef(ModuleDefinition module, TypeSignature typeSignature, bool doReturnNull = false)
        {
            if (typeSignature.DeclaringType == null)
                return GetTypeDef(module.Types, typeSignature, doReturnNull);

            var declaringType = GetTypeDef(module, typeSignature.DeclaringType, doReturnNull);
            return GetTypeDef(declaringType.NestedTypes, typeSignature, doReturnNull);
        }

        private static TypeDefinition GetTypeDef(Collection<TypeDefinition> types, TypeSignature typeSignature, bool doReturnNull = false)
        {
            foreach (var type in types)
                if (new TypeSignature(type) == typeSignature)
                    return type;

            if (!doReturnNull)
                throw new Exception("Type not found.");

            return null;
        }

        public static MethodDefinition GetTypeMethodDef(TypeDefinition destType, MethodSignature method, bool doReturnNull = false)
        {
            foreach (var destMethod in destType.Methods)
                if (new MethodSignature(destMethod) == method)
                    return destMethod;

            if (!doReturnNull)
                throw new Exception("Method not found.");

            return null;
        }

        public static FieldDefinition GetTypeFieldDef(TypeDefinition destType, FieldSignature field, bool doReturnNull = false)
        {
            foreach (var destField in destType.Fields)
                if (new FieldSignature(destField) == field)
                    return destField;

            if (!doReturnNull)
                throw new Exception("Field not found.");

            return null;
        }

        public static PropertyDefinition GetTypePropertyDef(TypeDefinition destType, PropertySignature property, bool doReturnNull = false)
        {
            foreach (var destProperty in destType.Properties)
                if (new PropertySignature(destProperty) == property)
                    return destProperty;

            if (!doReturnNull)
                throw new Exception("Property not found.");

            return null;
        }

        public static T GetMemberDef<T>(ModuleDefinition module, T @ref)
            where T : MemberReference
        {
            var memberRef = (MemberReference) @ref;

            if (memberRef is TypeReference)
                return (T)(MemberReference) GetTypeDef(module, new TypeSignature((TypeReference)memberRef));

            if (memberRef.DeclaringType == null)
                throw new NullReferenceException("memberRef.DeclaringType is null.");

            var destTypeDef = GetTypeDef(module, new TypeSignature(memberRef.DeclaringType));

            return (T) GetMemberDef(destTypeDef, memberRef);
        }

        public static T GetMemberDef<T>(TypeDefinition declaringType, T @ref, bool doReturnNull = false)
            where T : MemberReference
        {
            var memberRef = (MemberReference) @ref;

            if (memberRef is TypeReference)
                return (T)(MemberReference)GetTypeDef(declaringType.NestedTypes, new TypeSignature((TypeReference)memberRef), doReturnNull);

            if (memberRef is FieldReference)
                return (T)(MemberReference) GetTypeFieldDef(declaringType, new FieldSignature((FieldReference) memberRef));

            if (memberRef is PropertyReference)
                return (T)(MemberReference) GetTypePropertyDef(declaringType, new PropertySignature((PropertyReference) memberRef));

            if (memberRef is MethodReference)
                return (T)(MemberReference) GetTypeMethodDef(declaringType, new MethodSignature((MethodReference) memberRef));

            throw new NotImplementedException();
        }

        public static MemberReference GetMember(ModuleDefinition module, MemberSignature memberSignature, bool doReturnNull = false)
        {
            if(memberSignature is TypeSignature)
                return GetTypeDef(module, (TypeSignature)memberSignature, doReturnNull);

            var declaringType = GetTypeDef(module, memberSignature.DeclaringType);

            if(memberSignature is FieldSignature)
                return GetTypeFieldDef(declaringType, (FieldSignature) memberSignature, doReturnNull);

            if(memberSignature is PropertySignature)
                return GetTypePropertyDef(declaringType, (PropertySignature) memberSignature, doReturnNull);

            if(memberSignature is MethodSignature)
                return GetTypeMethodDef(declaringType, (MethodSignature) memberSignature, doReturnNull);

            throw new NotImplementedException();
        }

        public static MemberReference GetReference(ModuleDefinition module, object @ref, bool addAssemblyReference = false)
        {
            MemberReference ret;

            List<AssemblyNameReference> assemblyNames = null;
            if (!addAssemblyReference)
                assemblyNames = module.AssemblyReferences.ToList();

            if (@ref is TypeReference)
                ret = module.Import((TypeReference) @ref);
            else if (@ref is FieldReference)
                ret = module.Import((FieldReference) @ref);
            else if (@ref is MethodReference)
                ret = module.Import((MethodReference) @ref);
            else if (@ref is Type)
                ret = module.Import((Type) @ref);
            else if (@ref is FieldInfo)
                ret = module.Import((FieldInfo) @ref);
            else if (@ref is MethodBase)
                ret = module.Import((MethodBase) @ref);
            else
                throw new NotImplementedException();

            if (!addAssemblyReference)
            {
                module.AssemblyReferences.Clear();
                foreach (var assemblyName in assemblyNames)
                    module.AssemblyReferences.Add(assemblyName);
            }

            return ret;
        }

        public static string GetFriendlyTypeName(Type type)
        {
            if (type.IsGenericParameter)
                return type.Name;

            if (!type.IsGenericType)
                return type.FullName;

            var builder = new StringBuilder();
            var name = type.Name;
            builder.AppendFormat("{0}.{1}", type.Namespace, name);
            builder.Append('<');
            var first = true;
            foreach (var arg in type.GetGenericArguments())
            {
                if (!first)
                {
                    builder.Append(',');
                }
                builder.Append(GetFriendlyTypeName(arg));
                first = false;
            }
            builder.Append('>');
            return builder.ToString();
        }

        public static IMetadataScope GetScope(MemberReference memberRef)
        {
            var typeRef = memberRef as TypeReference;
            if (typeRef != null)
                return typeRef.Scope;

            return memberRef.DeclaringType.Scope;
        }

        public static string GetScopeName(MemberReference memberRef)
        {
            var scope = GetScope(memberRef);
            var name = scope.Name;
            if (name.EndsWith(".dll"))
                name = name.Substring(0, name.Length - 4);

            return name;
        }

        public static bool AreScopesEqual(MemberReference member1, MemberReference member2)
        {
            var scope1 = GetScope(member1);
            var scope2 = GetScope(member2);

            return scope1.Name == scope2.Name;
        }

        public static bool AreTypesEquivalent(TypeReference typeRef1, TypeReference typeRef2, Func<TypeReference, TypeReference> typeMapper = null)
        {
            if (typeMapper != null)
            {
                typeRef1 = typeMapper(typeRef1);
                typeRef2 = typeMapper(typeRef2);
            }

            var pointer1 = typeRef1 as PointerType;
            var pointer2 = typeRef2 as PointerType;
            if (pointer1 != null && pointer2 != null)
                return AreTypesEquivalent(pointer1.ElementType, pointer2.ElementType);

            if (!AreScopesEqual(typeRef1,typeRef2))
                return false;

            if(typeRef1.IsGenericInstance != typeRef2.IsGenericInstance)
                return false;

            if (typeRef1.IsGenericInstance)
            {
                var type1Generic = (GenericInstanceType) typeRef1;
                var type2Generic = (GenericInstanceType) typeRef2;

                if (type1Generic.GenericArguments.Count != type2Generic.GenericArguments.Count)
                    return false;

                for (int i = 0; i < type1Generic.GenericArguments.Count; i++)
                    if (!AreTypesEquivalent(type1Generic.GenericArguments[i], type2Generic.GenericArguments[i], typeMapper))
                        return false;

                return type1Generic.ElementType.FullName == type2Generic.ElementType.FullName;
            }

            return typeRef1.FullName == typeRef2.FullName;
        }
    }
}
