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

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using Flood.RPC.Metadata;

namespace Flood.Tools.RPCGen
{
    public class Compiler
    {
        private readonly Options options;
        private readonly Assembly assembly;
        public readonly List<string> GeneratedFiles;

        public Compiler(Options options, Assembly assembly)
        {
            this.options = options;
            this.assembly = assembly;
            this.GeneratedFiles = new List<string>();
        }

        public void Process()
        {
            foreach (var type in assembly.GetTypes())
            {
                var service = type.GetCustomAttribute<ServiceAttribute>();
                if (service != null)
                {
                    Debug.Assert(type.IsInterface);
                    ProcessService(type);
                }

                var message = type.GetCustomAttribute<MessageAttribute>();
                if (message != null)
                {
                    Debug.Assert(type.IsValueType || type.IsClass);
                    ProcessMessage(type);
                }

                var exception = type.GetCustomAttribute<ExceptionAttribute>();
                if (exception != null)
                {
                    Debug.Assert(type.IsClass);
                    ProcessException(type);
                }
            }
        }

        private void ProcessService(Type type)
        {
            if (options.OutputDebug)
            {
                Console.WriteLine("Service: {0}", type.Name);

                foreach (var method in type.GetMethods())
                    Console.WriteLine("  Method: {0}", method.Name);
            }

            var gen = new Generator(options);
            gen.GenerateService(type);

            WriteGeneratorToFile(type, gen);
        }

        private void ProcessMessage(Type type)
        {
            if (options.OutputDebug)
            {
                Console.WriteLine("Message: {0}", type.Name);

                foreach (var field in type.GetFields())
                    Console.WriteLine("  Field: {0}", field.Name);
            }

            var gen = new Generator(options);
            gen.GenerateMessage(type);

            WriteGeneratorToFile(type, gen);
        }

        private void ProcessException(Type type)
        {
            if (options.OutputDebug)
            {
                Console.WriteLine("Exception: {0}", type.Name);

                foreach (var field in type.GetFields())
                    Console.WriteLine("  Field: {0}", field.Name);
            }

            var gen = new Generator(options);
            gen.GenerateMessage(type);

            WriteGeneratorToFile(type, gen);
        }

        private void WriteGeneratorToFile(Type type, Generator gen)
        {
            if (string.IsNullOrEmpty(options.OutputDir))
                options.OutputDir = ".";

            var filePath = Path.GetFullPath(options.OutputDir);
            var fileName = string.Format("{0}.cs", type.Name);

            filePath = Path.Combine(filePath, fileName);
            File.WriteAllText(filePath, gen.ToString());
            GeneratedFiles.Add(filePath);

            Console.WriteLine("Generated '{0}'", fileName);
        }
    }

}
