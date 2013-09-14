using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using CppSharp;
using CppSharp.AST;
using CppSharp.Generators;
using CppSharp.Generators.AST;
using CppSharp.Generators.CLI;
using CppSharp.Generators.CSharp;
using CppSharp.Passes;
using CppSharp.Types;

namespace Flood
{
    class Flood : ILibrary
    {
        Driver Driver;

        #region Setup

        public void Setup(Driver driver)
        {
            Driver = driver;

            var options = driver.Options;
            options.LibraryName = "Engine";
            options.OutputNamespace = "Flood";
            options.GeneratorKind = LanguageGeneratorKind.CLI;
            options.OutputDir = @"../../../../src/EngineManaged/Bindings";
            options.IncludeDirs.Add(@"../../../../inc");
            options.LibraryDirs.Add(@"../../../../build/vs2012/lib/Debug_x32");
            options.WriteOnlyWhenChanged = true;
            options.GenerateFunctionTemplates = true;
            options.IgnoreParseWarnings = false;

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
                    "Core/Archive.h",
                    "Core/Stream.h",
                    "Core/FileWatcher.h",
                    "Core/Math/Color.h",
                    "Core/Math/BoundingBox.h",
                    "Core/Math/Frustum.h",
                    "Core/Math/Plane.h",
                    "Core/Math/Ray.h",
                    "Core/Network/Network.h",
                    "Core/Network/Host.h",
                    "Core/Network/Session.h",
                    "Core/Network/SessionManager.h",
                    "Core/Network/Packet.h",
                    "Core/Network/Peer.h",
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
                    "Engine/Geometry/DebugGeometry.h",
                    "Engine/Resources/Font.h",
                    "Engine/Scene/Entity.h",
                    "Engine/Scene/Component.h",
                    "Engine/Scene/Transform.h",
                    "Engine/Scene/Geometry.h",
                    "Engine/Scene/Camera.h",
                    "Engine/Scene/Scene.h",
                    "Engine/Engine.h",
                    "Engine/PlatformManager.h",
                    "Engine/Window/Window.h",
                    "Engine/Window/WindowManager.h",
                    "Engine/Input/InputManager.h",
                    "Engine/Input/Keyboard.h",
                    "Engine/Input/Mouse.h",
                    "Engine/Resources/TrueTypeFont.h",
                    "Engine/Texture/TextureAtlas.h"
                });
        }

        public void SetupPasses(Driver driver)
        {
            const RenameTargets renameTargets = RenameTargets.Function
                                              | RenameTargets.Method 
                                              | RenameTargets.Field;

            driver.AddTranslationUnitPass(new CaseRenamePass(renameTargets, RenameCasePattern.UpperCamelCase));
            driver.AddTranslationUnitPass(new CheckMacroPass());
            driver.AddTranslationUnitPass(new FindEventsPass(driver.TypeDatabase));
            driver.AddTranslationUnitPass(new GetterSetterToPropertyPass());
            driver.AddTranslationUnitPass(new FieldToPropertyPass());
            if (driver.Options.IsCLIGenerator)
                driver.AddTranslationUnitPass(new ObjectOverridesPass());
            driver.AddTranslationUnitPass(new FunctionToInstanceMethodPass());
            driver.AddTranslationUnitPass(new FunctionToStaticMethodPass());

            driver.AddGeneratorOutputPass(new FLDObjectOverridesPass());
        }

        #endregion

        #region Processing

        public void Preprocess(Driver driver, Library lib)
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
            lib.IgnoreHeadersWithName("Task.h");
            lib.IgnoreHeadersWithName("Timer.h");

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
            lib.ExcludeFromPass("ResourceHandleCreate", typeof (FunctionToInstanceMethodPass));

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
        }

        public void Postprocess(Library lib)
        {
            Driver.Generator.OnUnitGenerated += ProcessUnit;
        }

        public void WriteHeader(TextGenerator g)
        {
            g.WriteLine("/************************************************************************");
            g.WriteLine("*");
            g.WriteLine("* Flood Project \u00A9 (2008-201x)");
            g.WriteLine("* Licensed under the simplified BSD license. All rights reserved.");
            g.WriteLine("*");
            g.WriteLine("************************************************************************/");
            g.NewLine();
        }

        public void ProcessUnit(GeneratorOutput output)
        {
            foreach (var template in output.Templates)
                WriteHeader(template.FindBlock(BlockKind.Header).Text);
        }

        #endregion
    }

    #region Passes

    class CheckMacroPass : TranslationUnitPass
    {
        public override bool VisitDeclaration(Declaration decl)
        {
            var expansions = decl.PreprocessedEntities.OfType<MacroExpansion>();

            if (expansions.Any(e => e.Text == "FLD_IGNORE" &&
                                    e.Location != MacroLocation.ClassBody &&
                                    e.Location != MacroLocation.FunctionBody &&
                                    e.Location != MacroLocation.FunctionParameters))
                decl.ExplicityIgnored = true;

            return true;
        }

        public override bool VisitClassDecl(Class @class)
        {
            var expansions = @class.PreprocessedEntities.OfType<MacroExpansion>();

            if (expansions.Any(e => e.Text == "FLD_VALUE_TYPE"))
                @class.Type = ClassType.ValueType;

            return base.VisitClassDecl(@class);
        }

        public override bool VisitParameterDecl(Parameter parameter)
        {
            var expansions = parameter.PreprocessedEntities.OfType<MacroExpansion>();

            if (expansions.Any(e => e.Text == "FLD_OUT"))
                parameter.Usage = ParameterUsage.Out;

            if (expansions.Any(e => e.Text == "FLD_IN_OUT"))
                parameter.Usage = ParameterUsage.InOut;

            return true;
        }

        public override bool VisitEnumDecl(Enumeration @enum)
        {
            var expansions = @enum.PreprocessedEntities.OfType<MacroExpansion>();

            if (expansions.Any(e => e.Text == "FLD_FLAGS"))
                @enum.SetFlags();

            return true;
        }

        public override bool VisitMethodDecl(Method method)
        {
            var expansions = method.PreprocessedEntities.OfType<MacroExpansion>();

            if (expansions.Any(e => e.Text == "FLD_HASHCODE" || e.Text == "FLD_EQUALS"))
                method.ExplicityIgnored = true;

            return base.VisitMethodDecl(method);
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
            if(field.Ignore)
                return false;

            TypeMap typeMap;
            if (!typeMapDatabase.FindTypeMapRecursive(field.Type, out typeMap))
                return false;

            if (!(typeMap is EventMap))
                return false;

            field.ExplicityIgnored = true;

            ITypedDecl decl = field;

            var typedef = field.Type as TypedefType;
            if (typedef != null)
                return false;

            var template = decl.Type as TemplateSpecializationType;
            var @params = template.Arguments.Select(param => 
                new Parameter(){ QualifiedType = param.Type }).ToList();

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
                Access = field.Access,
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

    class FLDObjectOverridesPass : GeneratorOutputPass
    {
        public override void VisitGeneratorOutput(GeneratorOutput output)
        {
            foreach (var template in output.Templates)
            {
                var blocks = template.FindBlocks(CLIBlockKind.MethodBody);

                foreach (var block in blocks)
                    VisitMethodBody(block);
            }
        }

        void VisitMethodBody(Block block)
        {
            var method = block.Declaration as Method;
            var @class = (Class) method.Namespace;

            if (method.Name == "GetHashCode" && method.Parameters.Count == 0)
                GenerateGetHashCode(@class, block);

            if (method.Name == "Equals" && method.Parameters.Count == 1)
                GenerateEquals(@class, block, method);
        }

        void GenerateGetHashCode(Class @class, Block block)
        {
            var hashcodeOverride = @class.Methods.FirstOrDefault(m =>
            {
                var expansions = m.PreprocessedEntities.OfType<MacroExpansion>();
                return expansions.Any(e => e.Text == "FLD_HASHCODE");
            });

            if (hashcodeOverride == null)
                return;

            var cppTypePrinter = new CppTypePrinter(Driver.TypeDatabase);
            var classCppType = @class.Visit(cppTypePrinter);

            block.Text.StringBuilder.Clear();
            block.WriteLine("return (({0}*)NativePtr)->{1}();",
                classCppType, hashcodeOverride.OriginalName);
        }

        void GenerateEquals(Class @class, Block block, Method method)
        {
            var equalsOverride = @class.Methods.FirstOrDefault(m =>
            {
                var expansions = m.PreprocessedEntities.OfType<MacroExpansion>();
                return expansions.Any(e => e.Text == "FLD_EQUALS");
            });

            if (equalsOverride == null)
                return;

            var cliTypePrinter = new CLITypePrinter(Driver);
            var cppTypePrinter = new CppTypePrinter(Driver.TypeDatabase);
            var paramCppType = equalsOverride.Parameters[0].Visit(cppTypePrinter);
            var classCppType = @class.Visit(cppTypePrinter);
            var classCliType = @class.Visit(cliTypePrinter);

            block.Text.StringBuilder.Clear();
            block.WriteLine("if (!object) return false;");
            block.WriteLine("auto obj = dynamic_cast<{0}>({1});",
                classCliType, method.Parameters[0].Name);
            block.NewLine();

            block.WriteLine("if (!obj) return false;");
            block.WriteLine("auto param = ({0})obj->NativePtr;", paramCppType);
            block.WriteLine("return (({0}*)NativePtr)->{1}(param);",
                classCppType, equalsOverride.OriginalName);
        }
    }

    #endregion

    #region Type Maps
    [TypeMap("RefPtr")]
    public class RefPtrMap : TypeMap
    {
        public override bool IsIgnored
        {
            get
            {
                var type = Type as TemplateSpecializationType;
                var pointeeType = type.Arguments[0].Type;

                var checker = new TypeIgnoreChecker(TypeMapDatabase);
                pointeeType.Visit(checker);

                return checker.IsIgnored;
            }
        }

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
            if (!type.Arguments[0].Type.Type.Desugar().IsTagDecl(out @class))
                throw new Exception("Cannot marshal RefPtr: unsupported type");

            var instance = string.Format("{0}.get()", ctx.ReturnVarName);

            var cliMarshal = ctx.MarshalToManaged as CLIMarshalNativeToManagedPrinter;
            cliMarshal.WriteClassInstance(@class, instance);
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            var fullType = ctx.FullType.Type;
            var isPointer = (fullType != null) && (fullType is PointerType
                || fullType.Desugar() is TemplateSpecializationType);

            if (ctx.CSharpKind == CSharpTypePrinterContextKind.Native && isPointer)
                return "global::System.IntPtr";

            var type = Type as TemplateSpecializationType;
            return string.Format("{0}", type.Arguments[0].Type);
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            var type = Type as TemplateSpecializationType;
            ctx.Return.Write("{0}._Instance", ctx.Parameter.Name);
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            var type = Type as TemplateSpecializationType;

            Class @class;
            type.Arguments[0].Type.Type.Desugar().IsTagDecl(out @class);

            ctx.Return.Write("new {0}({1})", @class.Name, ctx.ReturnVarName);
            //ctx.Return.Write("new IntPtr(&{0})", ctx.ReturnVarName);
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
    }

    [TypeMap("Handle")]
    [TypeMap("ResourceHandle")]
    public class HandleMap : TypeMap
    {
        public override string CLISignature(CLITypePrinterContext ctx)
        {
            var type = Type.Desugar() as TemplateSpecializationType;
            var arg = type.Arguments[0].Type.ToString();
            if (string.IsNullOrEmpty(arg))
                arg = type.Template.Parameters[0].Name;
            
            return string.Format("Flood::ResourceHandle<{0}>", arg);
        }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("(HandleId){0}.Id", ctx.Parameter.Name);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write("{0}({1}.id)", CLISignature(null), ctx.ReturnVarName);
        }

        public override void CLITypeReference(CLITypeReferenceCollector collector, ASTRecord<Declaration> loc)
        {
            if(!(Declaration is ClassTemplate))
                return;

            var typeRef = collector.GetTypeReference(Declaration);
            typeRef.Include = new Include()
            {
                File = "ResourceHandle.h",
                Kind = Include.IncludeKind.Quoted,
                InHeader = true
            };

            Debug.Assert(loc.Parent != null);

            var type = loc.Parent.Object as CppSharp.AST.Type;
            var templateType = type.Desugar() as TemplateSpecializationType;
            var tag = templateType.Arguments[0].Type.Type as TagType;
            if(tag == null)
                return; // TODO Fix this

            var argDecl = tag.Declaration;
            var argTypeDef = collector.GetTypeReference(argDecl);
            argTypeDef.Include = new Include()
            {
                File = argDecl.Namespace.TranslationUnit.FileName,
                Kind = Include.IncludeKind.Quoted,
                InHeader = true
            };
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            if (ctx.CSharpKind == CSharpTypePrinterContextKind.Native)
            {
                if (ctx.FullType.Type is PointerType)
                    return "Flood.ResourceHandle*";

                // Handles only contain a single 32-bit unsigned int handle id.
                return "Flood.ResourceHandle";
            }

            var type = Type.Desugar() as TemplateSpecializationType;
            return string.Format("Flood.ResourceHandle<{0}>",
                type.Arguments[0].Type);
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            var marshal = ctx as CSharpMarshalContext;
            if (marshal.FullType.Type is PointerType)
                marshal.ArgumentPrefix.Write("&");

            ctx.Return.Write("new Flood.ResourceHandle({0}.Id)", ctx.Parameter.Name);
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            var marshal = ctx as CSharpMarshalContext;
            var fullType = marshal.FullType.Type;

            var type = Type.Desugar() as TemplateSpecializationType;
            ctx.Return.Write("new Flood.ResourceHandle<{0}>({1}{2}Id)",
                type.Arguments[0].Type, ctx.ReturnVarName,
                (fullType is PointerType) ? "->" : ".");
        }
    }
    #endregion

    static class Program
    {
        public static void Main(string[] args)
        {
            ConsoleDriver.Run(new Flood());
        }
    }
}
