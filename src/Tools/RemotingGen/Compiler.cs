/*
 * Licensed under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

using System.Runtime.CompilerServices;
using Weaver;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Linq;
using Weaver.Util;

namespace RemotingGen
{
    class GeneratorWriter
    {
        public String OutputDir;
        public List<string> GeneratedFiles; 

        public GeneratorWriter(string outputDir)
        {
            OutputDir = outputDir;
            GeneratedFiles = new List<string>();

            if (!Directory.Exists(outputDir))
                Directory.CreateDirectory(outputDir);
        }

        public void WriteGeneratorToFile(Type type, Generator gen)
        {
            WriteGeneratorToFile(type.FullName, gen);
        }

        public void WriteGeneratorToFile(string fileName, Generator gen)
        {
            if (string.IsNullOrEmpty(OutputDir))
                OutputDir = ".";

            var filePath = Path.GetFullPath(OutputDir);
            if(!fileName.EndsWith(".cs"))
                fileName = string.Format("{0}.cs", fileName);

            filePath = Path.Combine(filePath, fileName);
            File.WriteAllText(filePath, gen.ToString());
            GeneratedFiles.Add(filePath);

            Console.WriteLine("Generated '{0}'", fileName);
        }
    }

    public class GenerationContext
    {
        public readonly List<Type> DataObjects;
        public readonly List<Type> Delegates;

        public readonly List<MemberClone> MemberClones;
        public readonly Dictionary<MemberSignature, MemberOptions> MemberOptions;

        public readonly Dictionary<TypeSignature, TypeSignature> DataObjectsMap;

        public GenerationContext()
        {
            DataObjects = new List<Type>();
            Delegates = new List<Type>();

            MemberClones = new List<MemberClone>();
            MemberOptions = new Dictionary<MemberSignature, MemberOptions>();

            DataObjectsMap = new Dictionary<TypeSignature, TypeSignature>();
        }
    }

    public class Compiler
    {
        public Assembly Assembly;
        public string OutputDir;

        private GeneratorWriter apiWriter;

        public readonly GenerationContext GenerationContext;
        public readonly List<Type> RpcTypes;

        Logger log = new Logger(Logger.LogLevel.Info);

        public Compiler(Assembly assembly, string outputDir)
        {
            Assembly = assembly;
            OutputDir = outputDir;

            GenerationContext = new GenerationContext();
            RpcTypes = new List<Type>();

            apiWriter = new GeneratorWriter(Path.Combine(outputDir, "API"));
        }

        public void Process()
        {
            foreach(var module in Assembly.GetModules())
            {
                foreach (var type in module.GetTypes())
                {
                    if (Metadata.IsService(type))
                    {
                        Debug.Assert(type.IsInterface);
                        ProcessService(type);
                        RpcTypes.Add(type);
                    }

                    if (Metadata.IsDataObject(type))
                    {
                        Debug.Assert(type.IsValueType || type.IsClass);
                        ProcessDataObject(type);
                        RpcTypes.Add(type);
                    }

                    if (Metadata.IsException(type))
                    {
                        Debug.Assert(type.IsClass);
                        ProcessException(type);
                        RpcTypes.Add(type);
                    }
                }
            }

            WriteDelegates();
            WriteDataObjectFactory();
        }

        private void ProcessService(Type type)
        {
            log.Debug("Service: {0}", type.Name);

            foreach (var method in type.GetMethods())
                log.Debug("  Method: {0}", method.Name);

            var gen = new Generator(type.Assembly, GenerationContext);
            gen.GenerateService(type);

            apiWriter.WriteGeneratorToFile(type, gen);
        }

        private void ProcessDataObject(Type type)
        {
            log.Debug("DataObject: {0}", type.Name);

            foreach (var field in type.GetFields())
                log.Debug("  Field: {0}", field.Name);

            var gen = new Generator(type.Assembly, GenerationContext);
            gen.GenerateDataObject(type);

            apiWriter.WriteGeneratorToFile(type, gen);
        }

        private void ProcessException(Type type)
        {
            log.Debug("Exception: {0}", type.Name);

            foreach (var field in type.GetFields())
                log.Debug("  Field: {0}", field.Name);

            var gen = new Generator(type.Assembly, GenerationContext);
            gen.GenerateDataObject(type);

            apiWriter.WriteGeneratorToFile(type, gen);
        }

        private void WriteDataObjectFactory()
        {
            var gen = new Generator(Assembly, GenerationContext);
            gen.GenerateDataObjectFactory();

            apiWriter.WriteGeneratorToFile("DataObjectFactory", gen);
        }

        private void WriteDelegates()
        {
            var gen = new Generator(Assembly, GenerationContext);
            gen.GenerateDelegateClasses();

            apiWriter.WriteGeneratorToFile("Delegates", gen);
        }

        public void Compile(string dllPath, bool createApi)
        {
            FieldsToProperties(dllPath);

            var apiPath = (createApi)? Path.ChangeExtension(dllPath, "API.dll") : dllPath;

            var references = AssemblyWeaver.GetReferences(dllPath);
            references.Remove(Path.GetFileName(apiPath));

            var apiTypes = ReferenceCollector.CollectReferences(dllPath, RpcTypes);

            using (var apiGenPaths = new TemporaryAssemblyPaths())
            {
                var apiReferences = references.Where(s => s.EndsWith(".API.dll")).ToList();

                AssemblyWeaver weaver;

                if (createApi)
                {
                    //Create assembly with required types by the generated API assembly
                    weaver = new AssemblyWeaver();

                    weaver.AddReference(dllPath, "EngineManaged");
                    weaver.AddReference(dllPath, "EngineBindings");
                    weaver.AddReference(dllPath, "System");
                    weaver.AddReference(dllPath, "System.Core");
                    foreach (var apiReference in apiReferences)
                        weaver.AddReference(dllPath, Path.GetFileNameWithoutExtension(apiReference));

                    weaver.CopyTypes(dllPath, apiTypes);
                    weaver.Write(apiPath);
                }

                //API generated assembly
                var apiGenReferences = apiReferences.ToList();
                apiGenReferences.Add("EngineManaged.dll");
                apiGenReferences.Add(apiPath);
                DotNetCompiler.CompileIntoAssembly(apiGenPaths.DllPath, apiGenReferences, apiWriter.GeneratedFiles);

                //Add API generated assembly to API assembly
                weaver = new AssemblyWeaver(apiPath);
                weaver.CloneMembers(GenerationContext.MemberClones);
                weaver.AddMemberOptions(GenerationContext.MemberOptions);
                //Merge some generated files.
                weaver.MergeTypes(apiGenPaths.DllPath, GenerationContext.DataObjectsMap);
                //Add all the other generated types.
                weaver.CopyAssembly(apiGenPaths.DllPath);

                if (createApi)
                {
                    //Add attribute InternalsVisibleToAttribute
                    var dllName = Path.GetFileNameWithoutExtension(dllPath);
                    weaver.AddAttribute(typeof(InternalsVisibleToAttribute), new []{typeof(string)}, new object[]{dllName});
                }

                weaver.Write(apiPath);
            }

            if (!createApi) 
                return;

            var typeFowarder = new TypeFowarder(dllPath);
            typeFowarder.FowardTypes(apiPath, apiTypes);
            typeFowarder.Write(dllPath);
        }

        private void FieldsToProperties(string assemblyPath)
        {
            var fields = RpcTypes.Where(Metadata.IsDataObject).SelectMany(t => 
                    t.GetFields(BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Public)
                    ).Where(Metadata.HasId);

            var fieldToProperties = new FieldsToProperties(assemblyPath);
            fieldToProperties.ProcessFields(fields.ToList());
            fieldToProperties.Write(assemblyPath);
        }
    }
}
