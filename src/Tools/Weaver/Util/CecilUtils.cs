using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Mono.Cecil;
using Mono.Cecil.Pdb;
using System.IO;

namespace EngineWeaver.Util
{
    public class CecilUtils
    {
        private static Dictionary<string, AssemblyDefinition> assemblies = new Dictionary<string, AssemblyDefinition>();

        public static AssemblyDefinition GetAssemblyDef(Assembly assembly)
        {
            var assemblyPath = assembly.Location;
            if(assemblyPath == null)
                throw new NullReferenceException();

            return GetAssemblyDef(assemblyPath);
        }

        public static AssemblyDefinition GetAssemblyDef(string assemblyPath)
        {
            if (assemblies.ContainsKey(assemblyPath))
                return assemblies[assemblyPath];
          
            var assemblyResolver = new DefaultAssemblyResolver();
            var assemblyLocation = Path.GetDirectoryName(assemblyPath);
            assemblyResolver.AddSearchDirectory(assemblyLocation);

            var readerParameters = new ReaderParameters { AssemblyResolver = assemblyResolver };

            var pdbName = Path.ChangeExtension(assemblyPath, "pdb");
            if (File.Exists(pdbName))
            {
                var symbolReaderProvider = new PdbReaderProvider();
                readerParameters.SymbolReaderProvider = symbolReaderProvider;
                readerParameters.ReadSymbols = true;
            }

            var assemblyDef = AssemblyDefinition.ReadAssembly(assemblyPath, readerParameters);

            assemblies.Add(assemblyPath,assemblyDef);

            return assemblyDef;
        }

        public static TypeDefinition GetTypeDef(ModuleDefinition module, Type type)
        {
            return GetTypeDef(module, GetFriendlyTypeName(type));
        }

        public static TypeDefinition GetTypeDef(ModuleDefinition module, TypeReference type)
        {
            return GetTypeDef(module, type.FullName);
        }

        public static TypeDefinition GetTypeDef(ModuleDefinition module, string typeName)
        {
            typeName = typeName.Replace("+", "/");
            var typeNames = typeName.Split('/');

            var type = module.Types.First(t => t.FullName == typeNames[0]);
            for(int i = 1; i < typeNames.Length; i++)
                type = type.NestedTypes.First(t => t.Name == typeNames[i]);

            return type;
        }

        public static MethodDefinition GetTypeMethodDef(TypeDefinition destType, MethodReference origMethod)
        {
            foreach (var destMethod in destType.Methods)
            {
                if(destMethod.Name != origMethod.Name)
                    continue;

                if(destMethod.Parameters.Count != origMethod.Parameters.Count)
                    continue;

                var @continue = false;
                for (var i = 0; i < destMethod.Parameters.Count; i++)
                {
                    if (destMethod.Parameters[i].ParameterType.FullName != origMethod.Parameters[i].ParameterType.FullName)
                    {
                        @continue = true;
                        break;
                    }
                }

                if(@continue)
                    continue;

                return destMethod;
            }

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

        public static FieldDefinition GetTypeFieldDef(TypeDefinition destType, string fieldName, string fieldType)
        {
            foreach (var destField in destType.Fields)
            {
                if(destField.Name != fieldName)
                    continue;

                if(destField.FieldType.FullName != fieldType)
                    continue;

                return destField;
            }

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

        public static PropertyDefinition GetTypePropertyDef(TypeDefinition destType, string propertyName, string propertyType)
        {
            foreach (var destProperty in destType.Properties)
            {
                if(destProperty.Name != propertyName)
                    continue;

                if(destProperty.PropertyType.FullName != propertyType)
                    continue;

                return destProperty;
            }

            return null;
        }

        private static string GetFriendlyTypeName(Type type) {
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


    }
}
