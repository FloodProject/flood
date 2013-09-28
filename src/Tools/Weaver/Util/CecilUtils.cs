using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using Mono.Cecil;
using Mono.Cecil.Pdb;
using System.IO;

namespace Weaver.Util
{
    public class CecilUtils
    {
        public static AssemblyDefinition CreateEmptyAssemblyDef()
        {
            var assemblyName = new AssemblyNameDefinition(Path.GetRandomFileName(), new Version(0, 0, 0));

            return AssemblyDefinition.CreateAssembly(assemblyName, assemblyName.Name, ModuleKind.Dll);
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

        public static TypeDefinition GetTypeDef(ModuleDefinition module, Type type, bool doReturnNull = false)
        {
            return GetTypeDef(module, GetFriendlyTypeName(type), doReturnNull);
        }

        public static TypeDefinition GetTypeDef(ModuleDefinition module, TypeReference type, bool doReturnNull = false)
        {
            return GetTypeDef(module, type.FullName, doReturnNull);
        }

        public static TypeDefinition GetTypeDef(ModuleDefinition module, string typeName, bool doReturnNull = false)
        {
            typeName = typeName.Replace("+", "/");
            var typeNames = typeName.Split('/');

            var type = module.Types.FirstOrDefault(t => t.FullName == typeNames[0]);
            for (int i = 1; i < typeNames.Length && type != null; i++)
                type = GetTypeDef(type, typeNames[i], doReturnNull);

            return type;
        }

        public static TypeDefinition GetTypeDef(TypeDefinition type, string typeName, bool doReturnNull = false)
        {
            type = type.NestedTypes.FirstOrDefault(t => t.Name == typeName);

            if(type != null || doReturnNull)
                return type;

            throw new Exception("Type not found.");
        }

        public static MethodDefinition GetTypeMethodDef(TypeDefinition destType, MethodInfo origMethod)
        {
            var origMethodParams = origMethod.GetParameters().Select(p => GetFriendlyTypeName(p.ParameterType));

            return GetTypeMethodDef(destType, origMethod.Name, origMethodParams.ToList());
        }

        public static MethodDefinition GetTypeMethodDef(TypeDefinition destType, MethodReference origMethod)
        {
            var origMethodParams = origMethod.Parameters.Select(p => p.ParameterType.FullName);

            return GetTypeMethodDef(destType, origMethod.Name, origMethodParams.ToList());
        }

        public static MethodDefinition GetTypeMethodDef(TypeDefinition destType, string origMethodName,
                                                        List<string> origMethodParams, bool doReturnNull = false)
        {
            foreach (var destMethod in destType.Methods)
            {
                if (destMethod.Name != origMethodName)
                    continue;

                if (destMethod.Parameters.Count != origMethodParams.Count)
                    continue;

                var @continue = false;
                for (var i = 0; i < destMethod.Parameters.Count; i++)
                {
                    if (destMethod.Parameters[i].ParameterType.FullName != origMethodParams[i])
                    {
                        @continue = true;
                        break;
                    }
                }

                if (@continue)
                    continue;

                return destMethod;
            }

            if (!doReturnNull)
                throw new Exception("Method not found.");

            return null;
        }

        public static FieldDefinition GetTypeFieldDef(TypeDefinition destType, FieldReference origField)
        {
            return GetTypeFieldDef(destType, origField.Name, origField.FieldType.FullName);
        }

        public static FieldDefinition GetTypeFieldDef(TypeDefinition destType, FieldInfo fieldInfo)
        {
            return GetTypeFieldDef(destType, fieldInfo.Name, GetFriendlyTypeName(fieldInfo.FieldType));
        }

        public static FieldDefinition GetTypeFieldDef(TypeDefinition destType, string fieldName, string fieldType, bool doReturnNull = false)
        {
            foreach (var destField in destType.Fields)
            {
                if (destField.Name != fieldName)
                    continue;

                if (destField.FieldType.FullName != fieldType)
                    continue;

                return destField;
            }

            if (!doReturnNull)
                throw new Exception("Field not found.");

            return null;
        }

        public static PropertyReference GetTypePropertyDef(TypeDefinition destType, PropertyReference origProperty)
        {
            return GetTypePropertyDef(destType, origProperty.Name, origProperty.PropertyType.FullName);
        }

        public static PropertyDefinition GetTypePropertyDef(TypeDefinition destType, PropertyInfo propertyInfo)
        {
            return GetTypePropertyDef(destType, propertyInfo.Name, GetFriendlyTypeName(propertyInfo.PropertyType));
        }

        public static PropertyDefinition GetTypePropertyDef(TypeDefinition destType, string propertyName,
                                                            string propertyType)
        {
            foreach (var destProperty in destType.Properties)
            {
                if (destProperty.Name != propertyName)
                    continue;

                if (destProperty.PropertyType.FullName != propertyType)
                    continue;

                return destProperty;
            }

            return null;
        }

        public static T GetMemberDef<T>(ModuleDefinition module, T @ref)
            where T : MemberReference
        {
            var memberRef = (MemberReference) @ref;

            if (memberRef is TypeReference)
                return (T)(MemberReference) GetTypeDef(module, (TypeReference) memberRef);

            if (memberRef.DeclaringType == null)
                throw new NullReferenceException("memberRef.DeclaringType is null.");

            var destTypeDef = GetTypeDef(module, memberRef.DeclaringType);

            return (T) GetMemberDef(destTypeDef, memberRef);
        }

        public static T GetMemberDef<T>(TypeDefinition declaringType, T @ref)
            where T : MemberReference
        {
            var memberRef = (MemberReference) @ref;

            if (memberRef is TypeReference)
                return (T)(MemberReference)GetTypeDef(declaringType, memberRef.Name);

            if (memberRef is FieldReference)
                return (T)(MemberReference) GetTypeFieldDef(declaringType, (FieldReference) memberRef);

            if (memberRef is PropertyReference)
                return (T)(MemberReference) GetTypePropertyDef(declaringType, (PropertyReference) memberRef);

            if (memberRef is MethodReference)
                return (T)(MemberReference) GetTypeMethodDef(declaringType, (MethodReference) memberRef);

            throw new NotImplementedException();
        }

        public static MemberReference GetReference(ModuleDefinition module, MemberReference @ref, bool addAssemblyReference = false)
        {
            List<AssemblyNameReference> assemblyNames = null;
            if (!addAssemblyReference)
                assemblyNames = module.AssemblyReferences.ToList();

            MemberReference ret;

            if (@ref is TypeReference)
                ret = module.Import((TypeReference) @ref);
            else if (@ref is FieldReference)
                ret = module.Import((FieldReference) @ref);
            else if (@ref is MethodReference)
                ret = module.Import((MethodReference) @ref);
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
            if (memberRef is TypeReference)
                return typeRef.Scope;

            return memberRef.DeclaringType.Scope;
        }

        public static bool AreScopesEqual(MemberReference member1, MemberReference member2)
        {
            var scope1 = GetScope(member1);
            var scope2 = GetScope(member2);

            return scope1.Name == scope2.Name;
        }

        public static bool DoMethodParametersTypeMatch(MethodReference method1, MethodReference method2)
        {
            if (method1.Parameters.Count != method2.Parameters.Count)
                return false;

            for (int i = 0; i < method1.Parameters.Count; i++)
            {
                if (!AreTypesEquivalent(method1.Parameters[i].ParameterType, method2.Parameters[i].ParameterType))
                    return false;
            }

            return true;
        }

        public static bool AreTypesEquivalent(TypeReference typeRef1, TypeReference typeRef2)
        {
            var pointer1 = typeRef1 as PointerType;
            var pointer2 = typeRef2 as PointerType;
            if (pointer1 != null && pointer2 != null)
                return AreTypesEquivalent(pointer1.ElementType, pointer2.ElementType);

            if (!AreScopesEqual(typeRef1,typeRef2))
                return false;

            return typeRef1.FullName == typeRef2.FullName;
        }

        public static void CheckEquivalentTypes(TypeReference typeRef1, TypeReference typeRef2)
        {
            if(!AreTypesEquivalent(typeRef1, typeRef2))
                throw new Exception("Types are not equivalent.");
        }
    }
}
