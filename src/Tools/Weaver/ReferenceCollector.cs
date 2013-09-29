
using System;
using System.Collections.Generic;
using System.Linq;
using Mono.Cecil;
using Weaver.Util;

namespace Weaver
{
    public class ReferenceCollector
    {
        public static HashSet<Type> CollectReferences(string assemblyLocation, List<Type> types)
        {
            var result = new HashSet<Type>();

            if (types.Count == 0)
                return result;

            foreach (var type in types)
                result.Add(type);

            CollectReferences(assemblyLocation, result);

            return result;
        }

        public static void CollectReferences(string assemblyLocation, HashSet<Type> types)
        {
            var origAssemblyDef = CecilUtils.GetAssemblyDef(assemblyLocation);
            var assemblyDef = CecilUtils.CreateEmptyAssemblyDef();
            var copier = new CecilCopier(assemblyDef.MainModule);

            var count = types.Count;
            if (count == 0)
                return;

            var assembly = types.First().Assembly;

            foreach (var type in types)
            {
                if(type.Assembly != assembly)
                    throw new Exception("Expected types of same assembly.");

                var typeDef = CecilUtils.GetTypeDef(origAssemblyDef.MainModule, type);

                copier.Copy(typeDef, false);
            }

            copier.Process();

            var scopeName = origAssemblyDef.Name.Name + ".dll";

            

            foreach (var error in copier.Errors)
            {
                var import = error as UnexpectedImport;
                if (import == null)
                    continue;

                if (import.Scope.Name != scopeName)
                    continue;

                var type = import.Member as TypeReference ?? import.Member.DeclaringType;

                var typeName = type.FullName;
                types.Add(assembly.GetType(typeName));
            }

            if (count < types.Count)
                CollectReferences(assemblyLocation, types);
        }
    }
}
