using System;
using System.IO;
using EngineWeaver.Util;
using Mono.Cecil;
using System.Collections.Generic;

namespace EngineWeaver
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

            var attributeType = typeof (System.Runtime.CompilerServices.TypeForwardedToAttribute);
            var attributeCtor = attributeType.GetConstructor(new []{typeof (Type)});
            var attributeCtorRef = weaver.TargetModule.Import(attributeCtor);
            var systemTypeRef = weaver.TargetModule.Import(typeof(Type));

            foreach (var type in types)
            {
                if(fowardedTypes.Contains(type))
                    throw new Exception("Type "+type+" has alredy been fowarded.");

                fowardedTypes.Add(type);

                var fromTypeDef = CecilUtils.GetTypeDef(fromModule, type);
                var toTypeDef = CecilUtils.GetTypeDef(toAssembly.MainModule, type);

                var fromTypeRef = CecilUtils.GetReference(fromModule, fromTypeDef);
                var toTypeRef = CecilUtils.GetReference(weaver.TargetModule, toTypeDef);

                //Map type to another assembly type
                weaver.Copier.SetCopy(fromTypeRef, toTypeRef);

                //Add TypeForwardedToAttribute
                var attributeArgument = new CustomAttributeArgument(systemTypeRef, toTypeRef);
                var forwardAttribute = new CustomAttribute(attributeCtorRef);
                forwardAttribute.ConstructorArguments.Add(attributeArgument); 
                weaver.TargetModule.Assembly.CustomAttributes.Add(forwardAttribute);
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
