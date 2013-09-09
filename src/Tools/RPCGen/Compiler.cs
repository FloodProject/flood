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

using EngineWeaver;
using Microsoft.CSharp;
using RPCGen;
using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Text;
using System.Linq;

namespace Flood.Tools.RPCGen
{
    public class Compiler
    {
        private readonly string destAssemblyPath;
        public readonly List<string> GeneratedFiles;
        public readonly List<Type> RpcTypes;
        public readonly Dictionary<string, string> DataObjectsMap;

        public bool outputDebug;
        public string outputDir;
        public Assembly assembly;
        Logger log = new Logger(Logger.LogLevel.Info);

        public Compiler(string destAssemblyPath, string outputDir)
        {
            this.destAssemblyPath = destAssemblyPath;
            this.outputDir = outputDir;
            this.outputDebug = true;
            this.GeneratedFiles = new List<string>();
            this.RpcTypes = new List<Type>();
            this.DataObjectsMap = new Dictionary<string, string>();

            if(!ParseAssembly(destAssemblyPath, out assembly))
                throw new ArgumentException();
        }

        static bool ParseAssembly(string path, out Assembly assembly)
        {
            assembly = null;

            if (string.IsNullOrWhiteSpace(path))
            {
                Console.WriteLine("Error: no assembly provided");
                return false;
            }

            try
            {
                var fullPath = Path.GetFullPath(path);
                var pdbPath = Path.ChangeExtension(fullPath,".pdb");
                var assemblyBytes = File.ReadAllBytes(fullPath);
                if (File.Exists(pdbPath))
                {
                    var pdbBytes = File.ReadAllBytes(pdbPath);
                    assembly = Assembly.Load(assemblyBytes, pdbBytes);
                }
                else
                {
                    assembly = Assembly.Load(assemblyBytes);
                }
            }
            catch 
            {
                Console.WriteLine("Error: assembly '{0}' could not be loaded", path);
                return false;
            }

            return true;
        }

        public void Process()
        {
            foreach(var module in assembly.GetModules())
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
                        var dataObjectFullName = ProcessDataObject(type);
                        RpcTypes.Add(type);
                        DataObjectsMap.Add(dataObjectFullName, type.FullName);
                    }

                    if (Metadata.IsException(type))
                    {
                        Debug.Assert(type.IsClass);
                        ProcessException(type);
                        RpcTypes.Add(type);
                    }
                }
            }

            WriteDataObjectFactory();
        }

        private void ProcessService(Type type)
        {
            if (outputDebug)
            {
                log.Debug("Service: {0}", type.Name);

                foreach (var method in type.GetMethods())
                    log.Debug("  Method: {0}", method.Name);
            }

            var gen = new Generator(type.Assembly);
            gen.GenerateService(type);

            WriteGeneratorToFile(type, gen);
        }

        private string ProcessDataObject(Type type)
        {
            if (outputDebug)
            {
                log.Debug("DataObject: {0}", type.Name);

                foreach (var field in type.GetFields())
                    log.Debug("  Field: {0}", field.Name);
            }

            var gen = new Generator(type.Assembly);
            var dataObjectFullName = gen.GenerateDataObject(type);

            WriteGeneratorToFile(type, gen);

            return dataObjectFullName;
        }

        private void ProcessException(Type type)
        {
            if (outputDebug)
            {
                log.Debug("Exception: {0}", type.Name);

                foreach (var field in type.GetFields())
                    log.Debug("  Field: {0}", field.Name);
            }

            var gen = new Generator(type.Assembly);
            gen.GenerateDataObject(type);

            WriteGeneratorToFile(type, gen);
        }

        private void WriteDataObjectFactory()
        {
            var gen = new Generator(assembly);
            gen.GenerateDataObjectFactory(RpcTypes);

            WriteGeneratorToFile("DataObjectFactory", gen);
        }

        private void WriteGeneratorToFile(Type type, Generator gen)
        {
            WriteGeneratorToFile(type.FullName, gen);
        }

        private void WriteGeneratorToFile(string fileName, Generator gen)
        {
            if (string.IsNullOrEmpty(outputDir))
                outputDir = ".";

            var filePath = Path.GetFullPath(outputDir);
            if(!fileName.EndsWith(".cs"))
                fileName = string.Format("{0}.cs", fileName);

            filePath = Path.Combine(filePath, fileName);
            File.WriteAllText(filePath, gen.ToString());
            GeneratedFiles.Add(filePath);

            Console.WriteLine("Generated '{0}'", fileName);
        }

        public void Compile(string outputPath)
        {
            var references = GetAssemblyReferencesPaths(assembly);

            CompileIntoAssembly(destAssemblyPath, outputPath, references);
        }

        public void CompileApi(string outputPath)
        {
            using (var emptyAssemblyPaths = new TemporaryAssemblyPaths())
            using (var apiAssemblyPaths = new TemporaryAssemblyPaths())
            {
                CreateEmptyAssembly(emptyAssemblyPaths.DllPath);

                var weaver = new EngineWeaver.AssemblyWeaver(emptyAssemblyPaths.DllPath);
                var types = RpcTypes.Select(t => t.FullName);
                weaver.CopyTypes(destAssemblyPath, types);
                weaver.Write(apiAssemblyPaths.DllPath);

                var references = weaver.GetReferences();

                CompileIntoAssembly(apiAssemblyPaths.DllPath, outputPath, references);
            }
        }

        private void CompileIntoAssembly(string destPath, string outputPath, HashSet<string> references)
        {
            outputPath = Path.GetFullPath(outputPath);

            using (var generatedAssembly = new TemporaryAssemblyPaths())
            {
                
                FieldsToProperties(destPath, outputPath);

                CodeDomProvider provider = new CSharpCodeProvider();
                CompilerParameters cp = new CompilerParameters()
                {
                    GenerateExecutable = false,
                    OutputAssembly = generatedAssembly.DllPath,
                    IncludeDebugInformation = true,
                    CompilerOptions = "/unsafe"
                };

                references.Add(outputPath);
                foreach (var @ref in references)
                    cp.ReferencedAssemblies.Add(@ref);

                CompilerResults cr = provider.CompileAssemblyFromFile(cp, GeneratedFiles.ToArray());
            
                if (cr.Errors.HasErrors)
                {
                    var message = new StringBuilder();
                    message.Append("Error compiling generated files.\n");
                    foreach(var error in cr.Errors)
                        message.AppendFormat("  {0}\n",error.ToString());
                    throw new Exception(message.ToString());
                }

                var weaver = new AssemblyWeaver(outputPath);
                weaver.MergeTypes(generatedAssembly.DllPath, DataObjectsMap);
                weaver.AddAssembly(generatedAssembly.DllPath);
                weaver.Write(outputPath);
            }
        }

        private static HashSet<string>  GetAssemblyReferencesPaths(Assembly assembly)
        {
            var references = new HashSet<string>();

            foreach (var assemblyName in assembly.GetReferencedAssemblies())
            {
                references.Add(assemblyName.Name + ".dll");
            }

            return references;
        }

        private static void CreateEmptyAssembly(string outputPath)
        {
            var provider = new CSharpCodeProvider();
            var cp = new CompilerParameters()
            {
                GenerateExecutable = false,
                OutputAssembly = outputPath,
                IncludeDebugInformation = true
            };
            var cr = provider.CompileAssemblyFromFile(cp, new string[]{});

            if (cr.Errors.HasErrors)
            {
                var message = new StringBuilder();
                message.Append("Error creating empty assembly.\n");
                foreach(var error in cr.Errors)
                    message.AppendFormat("  {0}\n",error.ToString());
                throw new Exception(message.ToString());
            }
        }

        private void FieldsToProperties(string assemblyPath, string outputPath)
        {
            var fields = new List<FieldInfo>();
            foreach (var type in RpcTypes)
            {
                if (!Metadata.IsDataObject(type))
                    continue;

                foreach (var field in type.GetFields())
                {
                    if (field.DeclaringType != type)
                        continue;

                    if(Metadata.HasId(field))
                        fields.Add(field);
                }
            }

            var weaver = new AssemblyWeaver(assemblyPath);
            var fieldToProperties = new FieldsToProperties(weaver);
            fieldToProperties.ProcessFields(fields);
            weaver.Write(outputPath);
        }
    }
}
