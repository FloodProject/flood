using System;
using System.Runtime.CompilerServices;
using Weaver.Util;
using Mono.Cecil;
using System.Collections.Generic;

namespace Weaver
{
    public class TypeFowarder
    {
        private readonly string assemblyPath;
        private readonly AssemblyWeaver weaver;
        private readonly HashSet<Type> fowardedTypes;
        private AssemblyDefinition fromAssembly;

        public TypeFowarder(string assemblyPath)
        {
            this.assemblyPath = assemblyPath;

            weaver = new AssemblyWeaver();
            fowardedTypes = new HashSet<Type>();
            fromAssembly = CecilUtils.GetAssemblyDef(assemblyPath);
        }

        public void FowardTypes(string toAssemblyPath, IEnumerable<Type> types)
        {
            var fromModule = fromAssembly.MainModule;

            var toAssembly = CecilUtils.GetAssemblyDef(toAssemblyPath);

            weaver.AddReference(toAssemblyPath);

            foreach (var type in types)
            {
                if(fowardedTypes.Contains(type))
                    throw new Exception("Type "+type+" has alredy been fowarded.");

                fowardedTypes.Add(type);

                var fromTypeDef = CecilUtils.GetTypeDef(fromModule, new TypeSignature(type));
                var toTypeDef = CecilUtils.GetTypeDef(toAssembly.MainModule, new TypeSignature(type));

                var fromTypeRef = CecilUtils.GetReference(fromModule, fromTypeDef);
                var toTypeRef = CecilUtils.GetReference(weaver.TargetModule, toTypeDef);

                //Map type to another assembly type
                weaver.Copier.SetCopy(fromTypeRef, toTypeRef);

                //Add TypeForwardedToAttribute
                weaver.AddAttribute(typeof(TypeForwardedToAttribute), new [] {typeof (Type)}, new object[]{toTypeRef});
            }
        }

        public void Write(string outputPath)
        {
            //Will not copy types previously setted copied.
            weaver.CopyAssembly(assemblyPath);

            weaver.Write(outputPath);
        }

    }
}
