using System;
using System.Collections.Generic;
using System.Linq;
using CppSharp;
using CppSharp.Generators;
using CppSharp.Generators.CLI;
using Cxxi.Generators.CSharp;
using CppSharp.Passes;
using CppSharp.Types;
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
            lib.IgnoreClassWithName("FileStream");
            lib.SetClassAsValueType("FileWatchEvent");

            lib.IgnoreFunctionWithPattern(".+GetType");
            lib.IgnoreFunctionWithName("ClassGetIdMap");

            lib.IgnoreFunctionWithName("ReflectionSetHandleContext");
            lib.IgnoreFunctionWithName("SerializerCreateJSON");
            lib.IgnoreFunctionWithName("SerializerCreateBinary");
            lib.IgnoreClassWithName("ReflectionContext");
            lib.IgnoreClassWithName("ValueContext");

            lib.SetClassAsValueType("ExtensionMetadata");

            lib.IgnoreClassWithName("Stream");
            lib.IgnoreClassWithName("MemoryStream");
            lib.IgnoreClassWithName("Serializer");
            lib.IgnoreClassWithName("Thread");

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
            lib.SetClassAsValueType("Matrix4x3");
            lib.SetClassAsValueType("Matrix4x4");

            lib.CopyClassFields("ColorP", "Color");
            lib.CopyClassFields("Vector2P", "Vector2");
            lib.CopyClassFields("Vector3P", "Vector3");
            lib.CopyClassFields("QuaternionP", "Quaternion");

            lib.IgnoreClassWithName("ColorP");
            lib.IgnoreClassWithName("Vector2P");
            lib.IgnoreClassWithName("Vector3P");
            lib.IgnoreClassWithName("QuaternionP");

            lib.SetNameOfClassMethod("Matrix4x3", "identity", "setIdentity");
            lib.SetNameOfClassMethod("Matrix4x4", "identity", "setIdentity");
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
            lib.SetClassAsValueType("RenderState");
            lib.SetClassAsValueType("LightState");
            lib.IgnoreHeadersWithName("MaxRectsBinPack.h");
            lib.IgnoreClassWithName("MaxRectsBinPack");
            lib.IgnoreClassWithName("CompareHandle");
            lib.SetClassAsValueType("Glyph");
            lib.SetClassAsValueType("Rectangle");
            lib.SetClassAsValueType("RectangleF");
            lib.SetClassAsValueType("SubTexture");
            lib.SetMethodParameterUsage("TextureAtlas", "getImageSubTexture", 2, ParameterUsage.Out);
            lib.SetMethodParameterUsage("Font", "getGlyphInfo", 3, ParameterUsage.Out);

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
            options.GeneratorKind = LanguageGeneratorKind.CSharp;
            options.OutputDir = @"../../../../src/EngineManaged/BindingsCSharp";
            options.IncludeDirs.Add(@"../../../../inc");
            options.LibraryDirs.Add(@"../../../../build/vs2012/lib/Debug_x32");
            options.WriteOnlyWhenChanged = true;

            SetupLibraries(options.Libraries);
            SetupHeaders(options.Headers);
        }

        public void SetupLibraries(List<string> libraries)
        {
            libraries.AddRange(new string[]
                {
                    "Core.lib",
                    "Resources.lib",
                    "Graphics.lib",
                    "Engine.lib"
                });
        }

        public void SetupHeaders(List<string> headers)
        {
           headers.AddRange(new string[]
                {
                    "Core/Log.h",
                    "Core/Extension.h",
                    "Core/Reflection.h",
                    "Core/Serialization.h",
                    "Core/Math/Color.h",
                    "Core/Network/Network.h",
                    "Core/Network/Host.h",
                    "Core/Network/Peer.h",
                    "Core/Network/Packet.h",
                    "Resources/Resource.h",
                    "Resources/ResourceLoader.h",
                    "Resources/ResourceManager.h",
                    "Graphics/Resources/Image.h",
                    "Graphics/Graphics.h",
                    "Graphics/GeometryBuffer.h",
                    "Graphics/RenderContext.h",
                    "Graphics/RenderDevice.h",
                    "Graphics/RenderBatch.h",
                    "Graphics/RenderQueue.h",
                    "Graphics/RenderTarget.h",
                    "Graphics/RenderView.h",
                    "Graphics/Texture.h",
                    "Engine/Engine.h",
                    "Engine/Window/Window.h",
                    "Engine/Window/WindowManager.h",
                    "Engine/Input/InputManager.h",
                    "Engine/Input/Keyboard.h",
                    "Engine/Input/Mouse.h",
                    "Engine/Resources/TrueTypeFont.h",
                    "Engine/Texture/TextureAtlas.h"
                });
        }

        public void SetupPasses(Driver driver, PassBuilder builder)
        {
            const RenameTargets renameTargets = RenameTargets.Function
                | RenameTargets.Method | RenameTargets.Field;
            builder.RenameDeclsCase(renameTargets, RenameCasePattern.UpperCamelCase);

            builder.AddPass(new CheckMacroPass());

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

    class CheckMacroPass : TranslationUnitPass
    {
        public override bool VisitDeclaration(Declaration decl)
        {
            if (AlreadyVisited(decl))
                return false;

            var expansions = decl.PreprocessedEntities.OfType<MacroExpansion>();

            if (expansions.Any(e => e.Text == "FLD_IGNORE"))
                decl.ExplicityIgnored = true;

            return true;
        }

        public override bool VisitClassDecl(Class @class)
        {
            if (AlreadyVisited(@class))
                return false;

            var expansions = @class.PreprocessedEntities.OfType<MacroExpansion>();

            if (expansions.Any(e => e.Text == "FLD_VALUE_TYPE"))
                @class.Type = ClassType.ValueType;

            return true;
        }

        public override bool VisitParameterDecl(Parameter parameter)
        {
            if (AlreadyVisited(parameter))
                return false;

            var expansions = parameter.PreprocessedEntities.OfType<MacroExpansion>();

            if (expansions.Any(e => e.Text == "FLD_OUT"))
                parameter.Usage = ParameterUsage.Out;

            if (expansions.Any(e => e.Text == "FLD_IN_OUT"))
                parameter.Usage = ParameterUsage.InOut;

            return true;
        }

        public override bool VisitEnumDecl(Enumeration @enum)
        {
            if (AlreadyVisited(@enum))
                return false;

            var expansions = @enum.PreprocessedEntities.OfType<MacroExpansion>();

            if (expansions.Any(e => e.Text == "FLD_FLAGS"))
                @enum.SetFlags();

            return true;
        }
    }

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
        public override string CLISignature(CLITypePrinterContext ctx)
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

            var cliMarshal = ctx.MarshalToManaged as CLIMarshalNativeToManagedPrinter;
            cliMarshal.WriteClassInstance(@class, instance);
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            var type = Type as TemplateSpecializationType;
            return string.Format("{0}", type.Arguments[0].Type);
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("{0}.Instance", ctx.Parameter.Name);
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write("new IntPtr(&{0})", ctx.ReturnVarName);
        }
    }

    [TypeMap("Path")]
    [TypeMap("String")]
    public class String : CppSharp.Types.Std.String
    {
    }

    [TypeMap("StringWide")]
    public class StringWide : CppSharp.Types.Std.WString
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
        public override string CLISignature(CLITypePrinterContext ctx)
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

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            var type = Type as TemplateSpecializationType;
            var args = type.Arguments.Select(arg => arg.Type.ToString()).
                ToList();

            var output = "System.Action";

            if (args.Count > 0)
                output += string.Format("<{0}>", string.Join(", ", args));

            return output;
        }

        public override bool IsIgnored { get { return true; } }
    }

    [TypeMap("Handle")]
    [TypeMap("ResourceHandle")]
    public class HandleMap : TypeMap
    {
        public override string CLISignature(CLITypePrinterContext ctx)
        {
            var type = Type.Desugar() as TemplateSpecializationType;
            return string.Format("Flood::ResourceHandle<{0}>",
                type.Arguments[0].Type);
        }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("(HandleId){0}.Id", ctx.Parameter.Name);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write("{0}({1}.id)", CLISignature(null), ctx.ReturnVarName);
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            var type = Type.Desugar() as TemplateSpecializationType;
            return string.Format("Flood.ResourceHandle<{0}>",
                type.Arguments[0].Type);
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("{0}.Id", ctx.Parameter.Name);
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write("{0}.id", ctx.ReturnVarName);
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
