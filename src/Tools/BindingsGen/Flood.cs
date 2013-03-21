using System;
using System.Collections.Generic;
using System.Linq;
using Cxxi;
using Cxxi.Generators;
using Cxxi.Generators.CLI;
using Cxxi.Passes;
using Cxxi.Types;

namespace Flood
{
    class Flood : ILibrary
    {
        public void Preprocess(Library lib)
        {
            lib.IgnoreHeadersWithName("API.h");
            lib.IgnoreHeadersWithName("Concurrency.h");
            lib.IgnoreHeadersWithName("ConcurrentQueue.h");
            lib.IgnoreHeadersWithName("Delegate.h");
            lib.IgnoreHeadersWithName("Event.h");
            lib.IgnoreHeadersWithName("Handle.h");
            lib.IgnoreHeadersWithName("Object.h");
            lib.IgnoreHeadersWithName("Pointers.h");
            lib.IgnoreHeadersWithName("References.h");
            lib.IgnoreHeadersWithName("Reflection.h");
            lib.IgnoreHeadersWithName("ReflectionHelpers.h");

            //Core
            lib.IgnoreClassWithName("Object");
            lib.IgnoreClassWithName("Class");
            lib.IgnoreClassWithName("ReferenceCounted");
            lib.IgnoreClassWithName("HandleManager");

            lib.IgnoreClassWithName("Delegate0");
            lib.IgnoreClassWithName("Delegate1");
            lib.IgnoreClassWithName("Delegate2");

            lib.IgnoreClassWithName("TaskPool");
            lib.IgnoreClassWithName("ConcurrentQueue");
            
            lib.SetClassAsValueType("StringHash");
            lib.IgnoreClassWithName("RawStringCompare");

            lib.SetClassAsValueType("LogEntry");
            lib.IgnoreFunctionWithName("LogCreate");

            lib.IgnoreFunctionWithName("AllocatorAllocate");
            lib.IgnoreFunctionWithName("AllocatorDeallocate");
            lib.SetNameOfFunction("AllocatorReset", "AllocatorResetMemory");

            lib.IgnoreClassWithName("StreamFuncs");
            lib.SetClassAsOpaque("FileStream");
            lib.SetClassAsValueType("FileWatchEvent");

            lib.IgnoreFunctionWithPattern(".+GetType");
            lib.IgnoreFunctionWithName("ClassGetIdMap");

            lib.IgnoreFunctionWithName("ReflectionSetHandleContext");
            lib.IgnoreFunctionWithName("SerializerCreateJSON");
            lib.IgnoreFunctionWithName("SerializerCreateBinary");

            lib.SetClassAsValueType("ExtensionMetadata");

            // Math
            lib.SetClassAsValueType("ColorP");
            lib.SetClassAsValueType("Color");
            lib.SetClassAsValueType("Vector2P");
            lib.SetClassAsValueType("Vector2");
            lib.SetClassAsValueType("Vector2i");
            lib.SetClassAsValueType("Vector3P");
            lib.SetClassAsValueType("Vector3");
            lib.SetClassAsValueType("Vector4");
            lib.SetClassAsValueType("EulerAngles");
            lib.SetClassAsValueType("QuaternionP");
            lib.SetClassAsValueType("Quaternion");
            lib.SetClassAsValueType("Matrix4x4");

            lib.IgnoreClassWithName("ColorP");
            lib.IgnoreClassWithName("Vector2P");
            lib.IgnoreClassWithName("Vector3P");
            lib.IgnoreClassWithName("QuaternionP");

            lib.SetNameOfClassMethod("Quaternion", "identity", "setIdentity");
            lib.SetNameOfClassMethod("Vector2", "zero", "setZero");
            lib.SetNameOfClassMethod("Vector3", "zero", "setZero");
            lib.SetNameOfClassMethod("Vector4", "zero", "setZero");

            // Resources
            lib.IgnoreFunctionWithName("ResourcesInitialize");
            lib.IgnoreFunctionWithName("ResourcesDeinitialize");
            //lib.SetClassAsValueType("ResourceEvent");
            lib.SetClassAsValueType("ResourceLoadOption");
            lib.SetClassAsValueType("ResourceLoadOptions");
            lib.SetNameOfClassMethod("Texture", "allocate", "alloc");
            lib.ExcludeFromPass("ResourceHandleCreate", typeof(FunctionToInstanceMethodPass));

            // Graphics
            lib.SetClassAsValueType("RenderContextSettings");
            lib.SetClassAsValueType("RenderBatchRange");
            lib.SetClassAsValueType("VertexElementP");
            lib.SetClassAsValueType("VertexElement");
            lib.SetClassAsValueType("UniformBufferElement");
            lib.SetClassAsValueType("RenderBlock");
            lib.SetClassAsValueType("RenderState");
            lib.SetClassAsValueType("LightState");

            // Engine
            lib.IgnoreClassMethodWithName("Engine", "addSubsystem");
            lib.SetClassAsValueType("Settings");
            lib.SetClassAsValueType("WindowSettings");
            lib.SetEnumAsFlags("WindowStyles");
        }

        public void Postprocess(Library lib)
        {
        }

        public void Setup(DriverOptions options)
        {
            options.OutputInteropIncludes = false;
            options.LibraryName = "Engine";
            options.OutputNamespace = "Flood";
            options.OutputDir = @"../../../../src/EngineManaged/Bindings";
            options.IncludeDirs.Add(@"../../../../inc");
            options.GeneratorKind = LanguageGeneratorKind.CPlusPlusCLI;

            SetupHeaders(options.Headers);
        }

        public void SetupHeaders(List<string> headers)
        {
            var sources = new string[]
                {
                    "Core/Log.h",
                    "Core/Extension.h",
                    "Core/Reflection.h",
                    "Core/Serialization.h",
                    "Resources/Resource.h",
                    "Resources/ResourceLoader.h",
                    "Resources/ResourceManager.h",
                    "Graphics/Graphics.h",
                    "Graphics/GeometryBuffer.h",
                    "Graphics/RenderContext.h",
                    "Graphics/RenderDevice.h",
                    "Graphics/RenderBatch.h",
                    "Graphics/RenderTarget.h",
                    "Graphics/RenderView.h",
                    "Graphics/Texture.h",
                    "Engine/Engine.h",

                    "Engine/Window/Window.h",
                    "Engine/Window/WindowManager.h",
                    "Engine/Input/InputManager.h",
                    "Engine/Input/Keyboard.h",
                    "Engine/Input/Mouse.h",
                };

            headers.AddRange(sources);
        }

        public void SetupPasses(Driver driver, PassBuilder builder)
        {
            const RenameTargets renameTargets = RenameTargets.Function
                | RenameTargets.Method | RenameTargets.Field;
            builder.RenameDeclsCase(renameTargets, RenameCasePattern.UpperCamelCase);

            builder.FindEvents(driver.TypeDatabase);
            builder.FunctionToInstanceMethod();
            builder.FunctionToStaticMethod();
            builder.CheckDuplicateNames();
        }

        public void GenerateStart(TextTemplate template)
        {
            template.WriteLine("/************************************************************************");
            template.WriteLine("*");
            template.WriteLine("* Flood Project \u00A9 (2008-201x)");
            template.WriteLine("* Licensed under the simplified BSD license. All rights reserved.");
            template.WriteLine("*");
            template.WriteLine("************************************************************************/");
            template.NewLine();

            if (template is CLISourcesTemplate)
            {
                template.WriteLine("#include \"_Marshal.h\"");
            }

            if (template is CLITextTemplate)
            {
                var cliTemplate = template as CLITextTemplate;

                var include = new Include()
                    {
                        File = "ResourceHandle.h",
                        Kind = Include.IncludeKind.Quoted
                    };

                cliTemplate.Includes.Add(include);
            }
        }

        public void GenerateAfterNamespaces(TextTemplate template)
        {
            if (template is CLISourcesTemplate)
                template.WriteLine("using namespace clix;");
        }
    }

    #region Passes
    class FindEventsPass : TranslationUnitPass
    {
        readonly ITypeMapDatabase typeMapDatabase;

        public FindEventsPass(ITypeMapDatabase typeMapDatabase)
        {
            this.typeMapDatabase = typeMapDatabase;
        }

        public override bool VisitFieldDecl(Field field)
        {
            TypeMap typeMap;
            if (!typeMapDatabase.FindTypeMap(field.Type, out typeMap))
                return false;

            if (!(typeMap is EventMap))
                return false;

            ITypedDecl decl = field;

            var typedef = field.Type as TypedefType;
            if (typedef != null)
                return false;

            var template = decl.Type as TemplateSpecializationType;
            var @params = template.Arguments.Select(param => param.Type).ToList();

            if (template.Template.TemplatedDecl.Name.StartsWith("Delegate"))
                return false;

            // Clean up the event name.
            var names = StringHelpers.SplitCamelCase(field.Name);

            if (names.Length > 1 &&
                names[0].Equals("On", StringComparison.InvariantCultureIgnoreCase))
            {
                names = names.Skip(1).ToArray();
            }

            var @event = new Event()
            {
                Name = string.Join(string.Empty, names),
                OriginalName = field.OriginalName,
                Namespace = field.Namespace,
                QualifiedType = field.QualifiedType,
                Parameters = @params
            };

            field.Class.Events.Add(@event);
            return true;
        }
    }

    static class FindEventsPassExtensions
    {
        public static void FindEvents(this PassBuilder builder,
            TypeMapDatabase database)
        {
            var pass = new FindEventsPass(database);
            builder.AddPass(pass);
        }
    }
    #endregion

    #region Type Maps
    [TypeMap("RefPtr")]
    public class RefPtrMap : TypeMap
    {
        public override string CLISignature()
        {
            var type = Type as TemplateSpecializationType;
            return string.Format("{0}", type.Arguments[0].Type);
        }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            var type = Type as TemplateSpecializationType;

            Class @class;
            if (!type.Arguments[0].Type.Type.IsTagDecl(out @class))
                return;

            ctx.Return.Write("(::{0}*){1}->NativePtr",
                @class.QualifiedOriginalName, ctx.Parameter.Name);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            var type = Type as TemplateSpecializationType;

            Class @class;
            if (!type.Arguments[0].Type.Type.IsTagDecl(out @class))
                return;

            var instance = string.Format("{0}.get()", ctx.ReturnVarName);
            ctx.MarshalToManaged.WriteClassInstance(@class, instance);
        }
    }

    [TypeMap("Path")]
    [TypeMap("String")]
    public class String : Cxxi.Types.Std.String
    {
    }

    [TypeMap("StringWide")]
    public class StringWide : Cxxi.Types.Std.WString
    {
    }

    [TypeMap("Event0")]
    [TypeMap("Event1")]
    [TypeMap("Event2")]
    [TypeMap("Event3")]
    [TypeMap("Event4")]
    [TypeMap("Delegate0")]
    [TypeMap("Delegate1")]
    [TypeMap("Delegate2")]
    [TypeMap("Delegate3")]
    [TypeMap("Delegate4")]
    public class EventMap : TypeMap
    {
        public override string CLISignature()
        {
            var type = Type as TemplateSpecializationType;
            var args = type.Arguments.Select(arg => arg.Type.ToString()).
                ToList();

            var output = "System::Action";

            if (args.Count > 0)
                output += string.Format("<{0}>", string.Join(", ", args));

            output += "^";

            return output;
        }

        public override bool IsIgnored { get { return true; } }
    }

    [TypeMap("Handle")]
    [TypeMap("ResourceHandle")]
    public class HandleMap : TypeMap
    {
        public override string CLISignature()
        {
            var type = Type.Desugar() as TemplateSpecializationType;
            return string.Format("{0}::ResourceHandle<{1}>", "Flood",
                type.Arguments[0].Type);
        }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("(HandleId){0}.Id", ctx.Parameter.Name);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write("{0}({1}.id)", CLISignature(), ctx.ReturnVarName);
        }
    }
    #endregion

    static class Program
    {
        public static void Main(string[] args)
        {
            Driver.Run(new Flood());
        }
    }
}
