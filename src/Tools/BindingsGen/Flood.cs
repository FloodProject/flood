using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using CppSharp;
using CppSharp.AST;
using CppSharp.AST.Extensions;
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
            options.GeneratorKind = GeneratorKind.CLI;
            options.OutputDir = @"../../../../src/EngineManaged/Bindings";
            options.IncludeDirs.Add(@"../../../../inc");
            options.LibraryDirs.Add(@"../../../../build/vs2012/lib/Debug_x32");
            options.WriteOnlyWhenChanged = true;
            options.GenerateFunctionTemplates = true;
            options.IgnoreParseWarnings = true;
            options.Verbose = true;

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
                    "Core/String.h",
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
                    "Graphics/RenderBackend.h",
                    "Graphics/RenderBuffer.h",
                    "Graphics/Texture.h",
                    "Engine/Geometry/DebugGeometry.h",
                    "Engine/Resources/Font.h",
                    "Engine/Scene/Entity.h",
                    "Engine/Scene/Group.h",
                    "Engine/Scene/Component.h",
                    "Engine/Scene/Transform.h",
                    "Engine/Scene/Geometry.h",
                    "Engine/Scene/Camera.h",
                    "Engine/Scene/Scene.h",
                    "Engine/Engine.h",
                    "Engine/PlatformManager.h",
                    "Engine/Window/Menu.h",
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
            driver.AddTranslationUnitPass(new CheckMacroPass("FLD"));
            driver.AddTranslationUnitPass(new CheckIgnoredDeclsPass());
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

        public void Preprocess(Driver driver, ASTContext ctx)
        {
            ctx.IgnoreHeadersWithName("API.h");
            ctx.IgnoreHeadersWithName("Concurrency.h");
            ctx.IgnoreHeadersWithName("ConcurrentQueue.h");
            ctx.IgnoreHeadersWithName("Delegate.h");
            ctx.IgnoreHeadersWithName("Event.h");
            ctx.IgnoreHeadersWithName("Handle.h");
            ctx.IgnoreHeadersWithName("Object.h");
            ctx.IgnoreHeadersWithName("Pointers.h");
            ctx.IgnoreHeadersWithName("References.h");
            ctx.IgnoreHeadersWithName("Reflection.h");
            ctx.IgnoreHeadersWithName("ReflectionHelpers.h");
            ctx.IgnoreHeadersWithName("Task.h");
            ctx.IgnoreHeadersWithName("Timer.h");
            ctx.IgnoreHeadersWithName("Swap.h");

            //Core
            ctx.IgnoreClassWithName("Object");
            ctx.IgnoreClassWithName("Class");
            ctx.IgnoreClassWithName("ReferenceCounted");
            ctx.IgnoreClassWithName("HandleManager");

            ctx.IgnoreClassWithName("Delegate0");
            ctx.IgnoreClassWithName("Delegate1");
            ctx.IgnoreClassWithName("Delegate2");

            ctx.IgnoreClassWithName("TaskPool");
            ctx.IgnoreClassWithName("ConcurrentQueue");

            ctx.SetClassAsValueType("StringHash");
            ctx.IgnoreClassWithName("RawStringCompare");

            ctx.SetClassAsValueType("LogEntry");
            ctx.IgnoreFunctionWithName("LogCreate");

            ctx.IgnoreFunctionWithName("AllocatorAllocate");
            ctx.IgnoreFunctionWithName("AllocatorDeallocate");
            ctx.SetNameOfFunction("AllocatorReset", "AllocatorResetMemory");

            ctx.IgnoreClassWithName("StreamFuncs");
            ctx.IgnoreClassWithName("FileStream");
            ctx.SetClassAsValueType("FileWatchEvent");

            ctx.IgnoreFunctionWithPattern(".+GetType");
            ctx.IgnoreFunctionWithName("ClassGetIdMap");

            ctx.IgnoreFunctionWithName("ReflectionSetHandleContext");
            ctx.IgnoreFunctionWithName("SerializerCreateJSON");
            ctx.IgnoreFunctionWithName("SerializerCreateBinary");
            ctx.IgnoreClassWithName("ReflectionContext");
            ctx.IgnoreClassWithName("ValueContext");

            ctx.SetClassAsValueType("ExtensionMetadata");

            ctx.IgnoreClassWithName("MemoryStream");
            ctx.IgnoreClassWithName("Serializer");
            ctx.IgnoreClassWithName("Thread");

            // Math
            ctx.SetClassAsValueType("ColorP");
            ctx.SetClassAsValueType("Color");
            ctx.SetClassAsValueType("Vector2P");
            ctx.SetClassAsValueType("Vector2");
            ctx.SetClassAsValueType("Vector2i");
            ctx.SetClassAsValueType("Vector3P");
            ctx.SetClassAsValueType("Vector3");
            ctx.SetClassAsValueType("Vector4");
            ctx.SetClassAsValueType("EulerAngles");
            ctx.SetClassAsValueType("QuaternionP");
            ctx.SetClassAsValueType("Quaternion");
            ctx.SetClassAsValueType("Matrix4x3");
            ctx.SetClassAsValueType("Matrix4x4");

            ctx.CopyClassFields("ColorP", "Color");
            ctx.CopyClassFields("Vector2P", "Vector2");
            ctx.CopyClassFields("Vector3P", "Vector3");
            ctx.CopyClassFields("QuaternionP", "Quaternion");

            ctx.IgnoreClassWithName("ColorP");
            ctx.IgnoreClassWithName("Vector2P");
            ctx.IgnoreClassWithName("Vector3P");
            ctx.IgnoreClassWithName("QuaternionP");

            ctx.SetNameOfClassMethod("Matrix4x3", "identity", "setIdentity");
            ctx.SetNameOfClassMethod("Matrix4x4", "identity", "setIdentity");
            ctx.SetNameOfClassMethod("Quaternion", "identity", "setIdentity");
            ctx.SetNameOfClassMethod("Vector2", "zero", "setZero");
            ctx.SetNameOfClassMethod("Vector3", "zero", "setZero");
            ctx.SetNameOfClassMethod("Vector4", "zero", "setZero");

            // Resources
            ctx.IgnoreFunctionWithName("ResourcesInitialize");
            ctx.IgnoreFunctionWithName("ResourcesDeinitialize");
            //lib.SetClassAsValueType("ResourceEvent");
            ctx.SetClassAsValueType("ResourceLoadOption");
            ctx.SetClassAsValueType("ResourceLoadOptions");
            ctx.SetNameOfClassMethod("Texture", "allocate", "alloc");
            ctx.ExcludeFromPass("ResourceHandleCreate", typeof (FunctionToInstanceMethodPass));

            // Set generic type constraints on template methods
            var resourceManager = ctx.FindClass("ResourceManager").FirstOrDefault();
            if (resourceManager != null)
                foreach (var template in resourceManager.Templates)
                {
                    for (var i = 0; i < template.Parameters.Count; ++i)
                    {
                        var param = template.Parameters[i];
                        param.Constraint = "Flood::Resource";
                        template.Parameters[i] = param;
                    }
                }

            // Graphics
            ctx.SetClassAsValueType("RenderContextSettings");
            ctx.SetClassAsValueType("RenderBatchRange");
            ctx.SetClassAsValueType("VertexElementP");
            ctx.SetClassAsValueType("VertexElement");
            ctx.SetClassAsValueType("UniformBufferElement");
            ctx.SetClassAsValueType("RenderState");
            ctx.SetClassAsValueType("LightState");
            ctx.IgnoreHeadersWithName("MaxRectsBinPack.h");
            ctx.IgnoreClassWithName("MaxRectsBinPack");
            ctx.IgnoreClassWithName("CompareHandle");
            ctx.SetClassAsValueType("Glyph");
            ctx.SetClassAsValueType("Rectangle");
            ctx.SetClassAsValueType("RectangleF");
            ctx.SetClassAsValueType("SubTexture");
            ctx.SetMethodParameterUsage("TextureAtlas", "getImageSubTexture", 2, ParameterUsage.Out);
            ctx.SetMethodParameterUsage("Font", "getGlyphInfo", 3, ParameterUsage.Out);

            // Engine
            ctx.IgnoreClassMethodWithName("Engine", "addSubsystem");
        }

        public void Postprocess(Driver driver, ASTContext lib)
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

    class FindEventsPass : TranslationUnitPass
    {
        readonly ITypeMapDatabase typeMapDatabase;

        public FindEventsPass(ITypeMapDatabase typeMapDatabase)
        {
            this.typeMapDatabase = typeMapDatabase;
        }

        public override bool VisitFieldDecl(Field field)
        {
            if (!VisitDeclaration(field))
                return false;

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
            if (!type.Arguments[0].Type.Type.TryGetClass(out @class))
                return;

            ctx.Return.Write("(::{0}*){1}->NativePtr",
                @class.QualifiedOriginalName, ctx.Parameter.Name);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            var type = Type as TemplateSpecializationType;

            Class @class;
            if (!type.Arguments[0].Type.Type.TryGetClass(out @class))
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
            type.Arguments[0].Type.Type.TryGetClass(out @class);

            ctx.Return.Write("new {0}({1})", @class.Name, ctx.ReturnVarName);
            //ctx.Return.Write("new IntPtr(&{0})", ctx.ReturnVarName);
        }
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

        public override void CLITypeReference(CLITypeReferenceCollector collector,
            ASTRecord<Declaration> loc)
        {
            if (!(Declaration is ClassTemplate))
                return;

            var typeRef = collector.GetTypeReference(Declaration);
            typeRef.Include = new Include()
            {
                File = "ResourceHandle.h",
                Kind = Include.IncludeKind.Quoted,
                InHeader = true
            };

            Debug.Assert(loc.Parent != null);

            var typeRecord = loc.FindAncestor<TemplateSpecializationType>();
            var templateType = typeRecord.Object as TemplateSpecializationType;
            var tag = templateType.Arguments[0].Type.Type as TagType;
            if (tag == null)
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

    [TypeMap("String")]
    public class String : TypeMap
    {
        public override string CLISignature(CLITypePrinterContext ctx)
        {
            return "System::String^";
    }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("StringMarshaller::marshalString({0})", ctx.Parameter.Name);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write("StringMarshaller::marshalString({0})", ctx.ReturnVarName);
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            return "Std.String";
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("new Std.String()");
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write(ctx.ReturnVarName);
        }

        public override void CLITypeReference(CLITypeReferenceCollector collector,
ASTRecord<Declaration> loc)
        {
            var typeRef = collector.GetTypeReference(Declaration);
            typeRef.Include = new Include()
            {
                File = "StringConverter.h",
                Kind = Include.IncludeKind.Quoted,
                InHeader = true
            };
        }
    }

    [TypeMap("Path")]
    [TypeMap("UTF8String")]
    public class UTF8String : TypeMap
    {
        public override string CLISignature(CLITypePrinterContext ctx)
        {
            return "System::String^";
        }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("StringMarshaller::marshalUTF8String({0})", ctx.Parameter.Name);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write("StringMarshaller::marshalString({0})", ctx.ReturnVarName);
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            return "Std.String";
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("new Std.String()");
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write(ctx.ReturnVarName);
        }

        public override void CLITypeReference(CLITypeReferenceCollector collector,
ASTRecord<Declaration> loc)
        {
            var typeRef = collector.GetTypeReference(Declaration);
            typeRef.Include = new Include()
            {
                File = "StringConverter.h",
                Kind = Include.IncludeKind.Quoted,
                InHeader = true
            };
        }
    }

    [TypeMap("StringWide")]
    [TypeMap("WString")]
    public class StringWide : TypeMap
    {
        public override string CLISignature(CLITypePrinterContext ctx)
        {
            return "System::String^";
        }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("StringMarshaller::marshalWString({0})", ctx.Parameter.Name);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write("StringMarshaller::marshalString({0})", ctx.ReturnVarName);
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            return "Std.String";
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            ctx.Return.Write("new Std.String()");
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            ctx.Return.Write(ctx.ReturnVarName);
        }

        public override void CLITypeReference(CLITypeReferenceCollector collector,
ASTRecord<Declaration> loc)
        {
            var typeRef = collector.GetTypeReference(Declaration);
            typeRef.Include = new Include()
            {
                File = "StringConverter.h",
                Kind = Include.IncludeKind.Quoted,
                InHeader = true
            };
        }
    }

    [TypeMap("HashMap")]
    public class HashMap : TypeMap
    {

        public override bool IsIgnored
        {
            get
            {
                var type = Type as TemplateSpecializationType;
                var pointeeType1 = type.Arguments[0].Type;
                var pointeeType2 = type.Arguments[1].Type;

                var checker1 = new TypeIgnoreChecker(TypeMapDatabase);
                var checker2 = new TypeIgnoreChecker(TypeMapDatabase);

                pointeeType1.Visit(checker1);
                pointeeType2.Visit(checker2);

                return checker1.IsIgnored || checker2.IsIgnored;
            }
        }

        public override string CLISignature(CLITypePrinterContext ctx)
        {
            var type = Type as TemplateSpecializationType;
            return string.Format("System::Collections::Generic::Dictionary<{0}, {1}>^",
                type.Arguments[0].Type, type.Arguments[1].Type);
        }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            var templateType = Type as TemplateSpecializationType;
            var type1 = templateType.Arguments[0].Type;
            var type2 = templateType.Arguments[1].Type;

            var entryString = (ctx.Parameter != null) ? ctx.Parameter.Name : ctx.ArgName;

            var tmpVarName = "_tmp" + entryString;

            var cppTypePrinter = new CppTypePrinter(ctx.Driver.TypeDatabase);
            var nativeType1 = type1.Type.Visit(cppTypePrinter);
            var nativeType2 = type2.Type.Visit(cppTypePrinter);

            ctx.SupportBefore.WriteLine("auto {0} = HashMap<{1}, {2}>();",
                tmpVarName, nativeType1, nativeType2);
            ctx.SupportBefore.WriteLine("auto _keys = {0}->Keys->GetEnumerator();", entryString);
            ctx.SupportBefore.WriteLine("while (_keys.MoveNext())");
            ctx.SupportBefore.WriteStartBraceIndent();
            {
                ctx.SupportBefore.WriteLine("{0} _key = _keys.Current;", type1.ToString());
                ctx.SupportBefore.WriteLine("{0} _val = {1}[_key];", type2.ToString(), entryString);
                var param1 = new Parameter
                {
                    Name = "_key",
                    QualifiedType = type1
                };

                var elementCtx1 = new MarshalContext(ctx.Driver)
                {
                    Parameter = param1,
                    ArgName = param1.Name,
                };

                var marshal1 = new CLIMarshalManagedToNativePrinter(elementCtx1);
                type1.Type.Visit(marshal1);

                if (!string.IsNullOrWhiteSpace(marshal1.Context.SupportBefore))
                    ctx.SupportBefore.Write(marshal1.Context.SupportBefore);

                ctx.SupportBefore.WriteLine("auto _marshalKey = {0};",
                    marshal1.Context.Return);

                var param2 = new Parameter
                {
                    Name = "_val",
                    QualifiedType = type2
                };

                var elementCtx2 = new MarshalContext(ctx.Driver)
                {
                    Parameter = param2,
                    ArgName = param2.Name,
                };


                var marshal2 = new CLIMarshalManagedToNativePrinter(elementCtx2);
                type2.Type.Visit(marshal2);

                if (!string.IsNullOrWhiteSpace(marshal2.Context.SupportBefore))
                    ctx.SupportBefore.Write(marshal2.Context.SupportBefore);

                ctx.SupportBefore.WriteLine("auto _marshalValue = {0};",
                    marshal2.Context.Return);


                ctx.SupportBefore.WriteLine("{0}[_marshalKey] = _marshalValue;", tmpVarName);
            }

            ctx.SupportBefore.WriteCloseBraceIndent();

            ctx.Return.Write(tmpVarName);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            var templateType = Type as TemplateSpecializationType;
            var keyType = templateType.Arguments[0].Type;
            var valueType = templateType.Arguments[1].Type;
            var tmpVarName = "_tmp" + ctx.ArgName;

            ctx.SupportBefore.WriteLine("auto {0} = gcnew System::Collections::Generic::Dictionary<{1}, {2}>();",
                tmpVarName, keyType.ToString(), valueType.ToString());
            ctx.SupportBefore.WriteLine("for(auto _it = {0}.Begin(); _it != {0}.End(); ++_it)", ctx.ReturnVarName);
            ctx.SupportBefore.WriteStartBraceIndent();
            {
                ctx.SupportBefore.WriteLine("auto& _key = _it->first;");
                ctx.SupportBefore.WriteLine("auto& _val = _it->second;");

                var keyCtx = new MarshalContext(ctx.Driver)
                {
                    ReturnVarName = "_key",
                    ReturnType = keyType
                };

                var valueCtx = new MarshalContext(ctx.Driver)
                {
                    ReturnVarName = "_val",
                    ReturnType = valueType
                };

                var marshalKey = new CLIMarshalNativeToManagedPrinter(keyCtx);
                keyType.Type.Visit(marshalKey);

                if (!string.IsNullOrWhiteSpace(marshalKey.Context.SupportBefore))
                    ctx.SupportBefore.Write(marshalKey.Context.SupportBefore);

                ctx.SupportBefore.WriteLine("auto _marshalKey = {0};", marshalKey.Context.Return);

                var marshalValue = new CLIMarshalNativeToManagedPrinter(valueCtx);
                valueType.Type.Visit(marshalValue);

                if (!string.IsNullOrWhiteSpace(marshalValue.Context.SupportBefore))
                    ctx.SupportBefore.Write(marshalValue.Context.SupportBefore);

                ctx.SupportBefore.WriteLine("auto _marshalValue = {0};", marshalValue.Context.Return);

                ctx.SupportBefore.WriteLine("{0}->Add(_marshalKey, _marshalValue);", tmpVarName);
            }
            ctx.SupportBefore.WriteCloseBraceIndent();

            ctx.Return.Write(tmpVarName);
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            var type = Type as TemplateSpecializationType;
            return string.Format("System.Collections.Generic.Dictionary<{0}, {1}>",
                type.Arguments[0].Type, type.Arguments[1].Type);
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            throw new System.NotImplementedException();
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            throw new System.NotImplementedException();
        }
    }

    [TypeMap("HashSet")]
    public class HashSet : TypeMap
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
            return string.Format("System::Collections::Generic::HashSet<{0}>^",
                type.Arguments[0].Type);
        }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            var templateType = Type as TemplateSpecializationType;
            var type = templateType.Arguments[0].Type;

            var entryString = (ctx.Parameter != null) ? ctx.Parameter.Name : ctx.ArgName;

            var tmpVarName = "_tmp" + entryString;

            var cppTypePrinter = new CppTypePrinter(ctx.Driver.TypeDatabase);
            var nativeType = type.Type.Visit(cppTypePrinter);

            ctx.SupportBefore.WriteLine("auto {0} = HashSet<{1}>();",
                tmpVarName, nativeType);
            ctx.SupportBefore.WriteLine("for each({0} _element in {1})",
                type.ToString(), entryString);
            ctx.SupportBefore.WriteStartBraceIndent();
            {
                var param = new Parameter
                {
                    Name = "_element",
                    QualifiedType = type
                };

                var elementCtx = new MarshalContext(ctx.Driver)
                {
                    Parameter = param,
                    ArgName = param.Name,
                };

                var marshal = new CLIMarshalManagedToNativePrinter(elementCtx);
                type.Type.Visit(marshal);

                if (!string.IsNullOrWhiteSpace(marshal.Context.SupportBefore))
                    ctx.SupportBefore.Write(marshal.Context.SupportBefore);

                ctx.SupportBefore.WriteLine("auto _marshalElement = {0};",
                    marshal.Context.Return);

                ctx.SupportBefore.WriteLine("{0}.Insert(_marshalElement);", tmpVarName);
            }

            ctx.SupportBefore.WriteCloseBraceIndent();

            ctx.Return.Write(tmpVarName);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            var templateType = Type as TemplateSpecializationType;
            var type = templateType.Arguments[0].Type;
            var tmpVarName = "_tmp" + ctx.ArgName;

            ctx.SupportBefore.WriteLine("auto {0} = gcnew System::Collections::Generic::HashSet<{1}>();",
                tmpVarName, type.ToString());
            ctx.SupportBefore.WriteLine("for(auto _element : {0})", ctx.ReturnVarName);
            ctx.SupportBefore.WriteStartBraceIndent();
            {
                var elementCtx = new MarshalContext(ctx.Driver)
                {
                    ReturnVarName = "_element",
                    ReturnType = type
                };

                var marshal = new CLIMarshalNativeToManagedPrinter(elementCtx);
                type.Type.Visit(marshal);

                if (!string.IsNullOrWhiteSpace(marshal.Context.SupportBefore))
                    ctx.SupportBefore.Write(marshal.Context.SupportBefore);

                ctx.SupportBefore.WriteLine("auto _marshalElement = {0};", marshal.Context.Return);

                ctx.SupportBefore.WriteLine("{0}.Add(_marshalElement);", tmpVarName);
            }
            ctx.SupportBefore.WriteCloseBraceIndent();

            ctx.Return.Write(tmpVarName);
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            var type = Type as TemplateSpecializationType;
            return string.Format("System.Collections.Generic.HashSet<{0}>",
                type.Arguments[0].Type);
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            throw new System.NotImplementedException();
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            throw new System.NotImplementedException();
        }
    }

    [TypeMap("Vector")]
    public class Vector : TypeMap
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
            return string.Format("System::Collections::Generic::List<{0}>^",
                type.Arguments[0].Type);
        }

        public override void CLIMarshalToNative(MarshalContext ctx)
        {
            var templateType = Type as TemplateSpecializationType;
            var type = templateType.Arguments[0].Type;

            var entryString = (ctx.Parameter != null) ? ctx.Parameter.Name : ctx.ArgName;

            var tmpVarName = "_tmp" + entryString;

            var cppTypePrinter = new CppTypePrinter(ctx.Driver.TypeDatabase);
            var nativeType = type.Type.Visit(cppTypePrinter);

            ctx.SupportBefore.WriteLine("auto {0} = Vector<{1}>();",
                tmpVarName, nativeType);
            ctx.SupportBefore.WriteLine("for each({0} _element in {1})",
                type.ToString(), entryString);
            ctx.SupportBefore.WriteStartBraceIndent();
            {
                var param = new Parameter
                {
                    Name = "_element",
                    QualifiedType = type
                };

                var elementCtx = new MarshalContext(ctx.Driver)
                {
                    Parameter = param,
                    ArgName = param.Name,
                };

                var marshal = new CLIMarshalManagedToNativePrinter(elementCtx);
                type.Type.Visit(marshal);

                if (!string.IsNullOrWhiteSpace(marshal.Context.SupportBefore))
                    ctx.SupportBefore.Write(marshal.Context.SupportBefore);

                ctx.SupportBefore.WriteLine("auto _marshalElement = {0};",
                    marshal.Context.Return);

                ctx.SupportBefore.WriteLine("{0}.Push(_marshalElement);", tmpVarName);
            }

            ctx.SupportBefore.WriteCloseBraceIndent();

            ctx.Return.Write(tmpVarName);
        }

        public override void CLIMarshalToManaged(MarshalContext ctx)
        {
            var templateType = Type as TemplateSpecializationType;
            var type = templateType.Arguments[0].Type;
            var tmpVarName = "_tmp" + ctx.ArgName;

            ctx.SupportBefore.WriteLine("auto {0} = gcnew System::Collections::Generic::List<{1}>();",
                tmpVarName, type.ToString());
            ctx.SupportBefore.WriteLine("for(auto _element : {0})", ctx.ReturnVarName);
            ctx.SupportBefore.WriteStartBraceIndent();
            {
                var elementCtx = new MarshalContext(ctx.Driver)
                {
                    ReturnVarName = "_element",
                    ReturnType = type
                };

                var marshal = new CLIMarshalNativeToManagedPrinter(elementCtx);
                type.Type.Visit(marshal);

                if (!string.IsNullOrWhiteSpace(marshal.Context.SupportBefore))
                    ctx.SupportBefore.Write(marshal.Context.SupportBefore);

                ctx.SupportBefore.WriteLine("auto _marshalElement = {0};", marshal.Context.Return);

                ctx.SupportBefore.WriteLine("{0}->Add(_marshalElement);", tmpVarName);
            }
            ctx.SupportBefore.WriteCloseBraceIndent();

            ctx.Return.Write(tmpVarName);
        }

        public override string CSharpSignature(CSharpTypePrinterContext ctx)
        {
            var type = Type as TemplateSpecializationType;
            return string.Format("System.Collections.Generic.List<{0}>",
                type.Arguments[0].Type);
        }

        public override void CSharpMarshalToNative(MarshalContext ctx)
        {
            throw new System.NotImplementedException();
        }

        public override void CSharpMarshalToManaged(MarshalContext ctx)
        {
            throw new System.NotImplementedException();
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
