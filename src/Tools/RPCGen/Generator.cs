using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Text;
using Flood.RPC.Protocol;
using Flood.RPC.Metadata;

namespace Flood.Tools.RPCGen
{
    internal class Generator
    {
        private const uint DefaultIndent = 4;
        private const uint MaxIndent = 80;

        private readonly Options options;
        private readonly StringBuilder sb;
        private readonly Stack<uint> currentIndent;
        private bool isStartOfLine;
        private bool needsNewLine;

        public Generator(Options options)
        {
            this.options = options;
            sb = new StringBuilder();
            currentIndent = new Stack<uint>();
            isStartOfLine = false;
        }

        #region Text Helpers

        public void Write(string msg, params object[] args)
        {
            if (isStartOfLine)
                sb.Append(new string(' ', (int) currentIndent.Sum(u => u)));

            if (args.Length > 0)
                msg = string.Format(msg, args);

            if (msg.Length > 0)
                isStartOfLine = false;

            sb.Append(msg);
        }

        public void WriteLine(string msg, params object[] args)
        {
            Write(msg, args);
            NewLine();
        }

        public void WriteLineIndent(string msg, params object[] args)
        {
            PushIndent();
            WriteLine(msg, args);
            PopIndent();
        }

        public void NewLine()
        {
            sb.AppendLine(string.Empty);
            isStartOfLine = true;
        }

        public void NewLineIfNeeded()
        {
            if (!needsNewLine) return;

            NewLine();
            needsNewLine = false;
        }

        public void NeedNewLine()
        {
            needsNewLine = true;
        }

        public void PushIndent(uint indent = DefaultIndent)
        {
            currentIndent.Push(indent);
        }

        public void PopIndent()
        {
            currentIndent.Pop();
        }

        public void WriteStartBraceIndent()
        {
            WriteLine("{");
            PushIndent();
        }

        public void WriteCloseBraceIndent()
        {
            PopIndent();
            WriteLine("}");
        }

        public override string ToString()
        {
            return sb.ToString();
        }

        #endregion

        public void GenerateMessage(Type type)
        {
            GenerateUsings();
            GenerateMessageClass(type);
        }

        void GenerateMessageClass(Type type)
        {
            var typeName = string.Format("{0}Impl", type.Name);

            WriteLine("[Serializable]");
            WriteLine("public class {0} : Base", typeName);
            WriteStartBraceIndent();

            // Generate private fields.
            foreach (var field in type.GetFields())
            {
                WriteLine("private {0} _{1};",
                    ConvertToTypeString(field.FieldType), field.Name);
                NeedNewLine();
            }
            NewLineIfNeeded();

            var parameters = ConvertFieldToParametersList(type);
            GeneratePropertyList(parameters);
            GenerateIsSet(parameters);

            // Generate constructor
            WriteLine("public {0}()", typeName);
            WriteLine("{");
            WriteLine("}");
            NewLine();

            // Generate read method
            GenerateServiceMethodRead(parameters, isResult: false);
            NewLine();

            // Generate write method
            GenerateServiceMethodWrite(type.Name, parameters, isResult: false);

            WriteCloseBraceIndent();
        }

        private List<Parameter> ConvertFieldToParametersList(Type type)
        {
            var parameters = new List<Parameter>();
            foreach (var field in type.GetFields())
                parameters.Add(new Parameter(field));
            return parameters;
        }

        public void GenerateService(Type type)
        {
            // TODO: Generate namespace
            GenerateUsings();

            WriteLine("public partial class {0}Impl", type.Name);
            WriteStartBraceIndent();

            //GenerateInterface(type);

            GenerateServiceClient(type);
            GenerateServiceProcessor(type);

            var methods = type.GetMethods();
            for (var i = 0; i < methods.Length; i++)
            {
                var method = methods[i];

                GenerateServiceMethodArgs(method);
                NewLine();

                GenerateServiceMethodResult(method);

                if (i < methods.Length - 1)
                    NewLine();
            }

            WriteCloseBraceIndent();
        }

        void GenerateUsings()
        {
            WriteLine("using System;");
            WriteLine("using System.Collections.Generic;");

            WriteLine("using Flood.RPC;");
            WriteLine("using Flood.RPC.Metadata;");
            WriteLine("using Flood.RPC.Protocol;");
            WriteLine("using Flood.RPC.Transport;");
            NewLine();
        }

        void GenerateInterface(Type type)
        {
            WriteLine("public interface {0}");
            WriteStartBraceIndent();

            foreach (var method in type.GetMethods())
            {
                Write("{0} {1}(", method.ReturnType.Name, method.Name);
                WriteLine(");");
            }

            WriteCloseBraceIndent();
            NewLine();
        }

        void GenerateParameterList(IList<ParameterInfo> parameters)
        {
            for (var i = 0; i < parameters.Count; i++)
            {
                var param = parameters[i];
                Write("{0} {1}", ConvertToTypeString(param.ParameterType),
                      param.Name);

                if (i < parameters.Count - 1)
                    Write(", ");
            }
        }

        static bool GetInheritedService(Type type, out Type @base)
        {
            @base = null;

            foreach (var @interface in type.GetInterfaces())
            {
                var service = @interface.GetCustomAttribute<ServiceAttribute>();
                if (service == null)
                    continue;

                @base = @interface;
                return true;
            }

            return false;
        }

        void GenerateServiceClient(Type type)
        {
            Type baseType;
            GetInheritedService(type, out baseType);

            Write("public class Client");
            
            if (baseType != null)
                Write(" : {0}Impl.Client", baseType.Name);
            NewLine();
            WriteStartBraceIndent();

            // Generate client constructors
            WriteLine("public Client(Serializer prot) : this(prot, prot)");
            WriteStartBraceIndent();
            WriteCloseBraceIndent();
            NewLine();

            Write("public Client(Serializer iprot, Serializer oprot)");
            if (baseType != null)
                Write(" : base(iprot, oprot)", baseType.Name);
            NewLine();
            WriteStartBraceIndent();
            WriteLine("iprot_ = iprot;");
            WriteLine("oprot_ = oprot;");
            WriteCloseBraceIndent();
            NewLine();

            if (baseType == null)
            {
                WriteLine("protected Serializer iprot_;");
                WriteLine("protected Serializer oprot_;");
                WriteLine("protected int seqid_;");
                NewLine();

                WriteLine("public Serializer InputProtocol");
                WriteStartBraceIndent();
                WriteLine("get { return iprot_; }");
                WriteCloseBraceIndent();
                NewLine();

                WriteLine("public Serializer OutputProtocol");
                WriteStartBraceIndent();
                WriteLine("get { return oprot_; }");
                WriteCloseBraceIndent();
                NewLine();
            }

            foreach (var method in type.GetMethods())
                GenerateProtocolMethod(method);

            WriteCloseBraceIndent();
            NewLine();
        }

        void GenerateProtocolMethod(MethodInfo method)
        {
            Write("public {0} {1}(", ConvertToTypeString(method.ReturnType),
                method.Name);

            var parameters = method.GetParameters();
            GenerateParameterList(parameters);

            WriteLine(")");

            WriteStartBraceIndent();

            Write("send_{0}(", method.Name);
            for (var i = 0; i < parameters.Length; i++)
            {
                Write("{0}", parameters[i].Name);
                if (i < parameters.Length - 1)
                    Write(", ");
            }
            WriteLine(");");

            if (method.ReturnType != typeof(void))
                Write("return ");
            WriteLine("recv_{0}();", method.Name);

            WriteCloseBraceIndent();
            NewLine();

            // Generate send method
            GenerateProtocolSend(method, parameters);

            // Generate receive method
            GenerateProtocolReceive(method);
        }

        void GenerateProtocolReceive(MethodInfo method)
        {
            Write("public {0} recv_{1}(", ConvertToTypeString(method.ReturnType),
                  method.Name);
            WriteLine(")");
            WriteStartBraceIndent();

            WriteLine("var msg = iprot_.ReadMessageBegin();");
            WriteLine("if (msg.Type == MessageType.Exception)");

            WriteStartBraceIndent();
            WriteLine("var x = RPCException.Read(iprot_);");
            WriteLine("iprot_.ReadMessageEnd();");
            WriteLine("throw x;");
            WriteCloseBraceIndent();

            WriteLine("var result = new {0}_result();", method.Name);
            WriteLine("result.Read(iprot_);");
            WriteLine("iprot_.ReadMessageEnd();");

            if (method.ReturnType != typeof (void))
            {
                WriteLine("if (result.__isset.success)");
                WriteLineIndent("return result.Success;");
            }

            var throws = method.GetCustomAttributes<ThrowsAttribute>();
            foreach (var exception in throws)
            {
                WriteLine("if (result.__isset.exception{0})", exception.Id);
                WriteStartBraceIndent();
                WriteLine("throw result.Exception{0};", exception.Id);
                WriteCloseBraceIndent();
            }

            if (method.ReturnType != typeof (void))
            {
                WriteLine("throw new RPCException(RPCException.ExceptionType.MissingResult,");
                WriteLineIndent(" \"{0} failed: unknown result\");", method.Name);
            }

            WriteCloseBraceIndent();
        }

        void GenerateProtocolSend(MethodInfo method, ParameterInfo[] parameters)
        {
            Write("public void send_{0}(", method.Name);
            GenerateParameterList(parameters);
            WriteLine(")");
            WriteStartBraceIndent();

            WriteLine("oprot_.WriteMessageBegin(new Message(\"{0}\", MessageType.Call, seqid_));",
                      method.Name);
            WriteLine("var args = new {0}_args();", method.Name);
            foreach (var param in method.GetParameters())
                WriteLine("args.{0} = {1};", ToTitleCase(param.Name), param.Name);
            WriteLine("args.Write(oprot_);");
            WriteLine("oprot_.WriteMessageEnd();");
            WriteLine("oprot_.Transport.Flush();");

            WriteCloseBraceIndent();
            NewLine();
        }

        void GenerateServiceProcessor(Type type)
        {
            Type baseType;
            GetInheritedService(type, out baseType);

            WriteLine("public class Processor : {0}",
                      baseType == null ? "SimpleProcessor" :
                      baseType.Name + "Impl.Processor");
            WriteStartBraceIndent();

            WriteLine("private readonly {0} iface_;", type.FullName);

            // Generate constructor
            {
                Write("public Processor({0} iface)", type.FullName);

                if (baseType != null)
                    Write(" : base(iface)");
                NewLine();
                WriteStartBraceIndent();

                WriteLine("iface_ = iface;");

                foreach (var method in type.GetMethods())
                    WriteLine("processMap_[\"{0}\"] = {0}_Process;", method.Name);

                WriteCloseBraceIndent();
                NewLine();
            }

            // Generate process methods
            foreach (var method in type.GetMethods())
            {
                GenerateServiceProcessMethod(method);
                NewLine();
            }

            WriteCloseBraceIndent();
            NewLine();
        }

        void GenerateServiceProcessMethod(MethodInfo method)
        {
            WriteLine("public void {0}_Process(int seqid, Serializer iprot, Serializer oprot)",
                      method.Name);
            WriteStartBraceIndent();

            WriteLine("var args = new {0}_args();", method.Name);
            WriteLine("args.Read(iprot);");
            WriteLine("iprot.ReadMessageEnd();");
            WriteLine("var result = new {0}_result();", method.Name);

            // If the method throws exceptions, we need to call it inside a try-catch.
            var throws = method.GetCustomAttributes<ThrowsAttribute>();
            var hasExceptions = throws.Any();

            if (hasExceptions)
            {
                WriteLine("try {");
                PushIndent();
            }

            if (method.ReturnType != typeof (void))
                Write("result.Success = ");

            // Call the service method
            Write("iface_.{0}(", method.Name);

            var parameters = method.GetParameters();
            for (var i = 0; i < parameters.Length; i++)
            {
                var param = parameters[i];
                Write("args.{0}", ToTitleCase(param.Name));

                if (i < parameters.Length - 1)
                    Write(", ");
            }

            WriteLine(");");

            if (hasExceptions)
                PopIndent();

            // Write the catch part of the exception handling.
            foreach (var exception in throws)
            {
                WriteLine("}} catch ({0} ex{1}) {{", exception.Exception.FullName,
                    exception.Id);
                PushIndent();
                WriteLine("result.Exception{0} = ex{0};", exception.Id);
                PopIndent();
            }

            if (hasExceptions)
                WriteLine("}");

            // Create a new message and reply to the RPC call
            WriteLine("oprot.WriteMessageBegin(new Message(\"{0}\", MessageType.Reply, seqid));",
                      method.Name);

            WriteLine("result.Write(oprot);");
            WriteLine("oprot.WriteMessageEnd();");
            WriteLine("oprot.Transport.Flush();");

            WriteCloseBraceIndent();
        }

        void GenerateServiceMethodArgs(MethodInfo method)
        {
            WriteLine("[Serializable]");
            WriteLine("public partial class {0}_args : Base", method.Name);
            WriteStartBraceIndent();

            // Generate private fields.
            foreach (var param in method.GetParameters())
            {
                WriteLine("private {0} _{1};",
                    ConvertToTypeString(param.ParameterType), param.Name);
                NeedNewLine();
            }
            NewLineIfNeeded();

            var parameters = ConvertToParametersList(method);
            GeneratePropertyList(parameters);
            GenerateIsSet(parameters);

            // Generate constructor
            WriteLine("public {0}_args()", method.Name);
            WriteLine("{");
            WriteLine("}");
            NewLine();

            // Generate read method
            GenerateServiceMethodRead(parameters, isResult: false);
            NewLine();

            // Generate write method
            GenerateServiceMethodWrite(method.Name, parameters, isResult: false);

            WriteCloseBraceIndent();
        }

        static List<Parameter> ConvertToParametersList(MethodInfo method)
        {
            // Convert from PropertyInfo to our own struct. This makes it easier
            // to use the same code for argument and result marshaling.
            var parameters = new List<Parameter>();
            foreach (var param in method.GetParameters())
                parameters.Add(new Parameter(param));
            return parameters;
        }

        void GenerateIsSet(IEnumerable<Parameter> parameters)
        {
            // Generate an __isset structure that keeps track of what variables
            // have been set.
            WriteLine("public Isset __isset;");
            NewLine();

            WriteLine("[Serializable]");
            WriteLine("public struct Isset");
            WriteStartBraceIndent();
            foreach (var param in parameters)
                WriteLine("public bool {0};", param.Name);
            WriteCloseBraceIndent();
            NewLine();
        }

        void GenerateFieldSerialize(Parameter param)
        {
            var type = param.ParameterType;
            var thriftType = ConvertFromTypeToThrift(param.ParameterType);

            switch (thriftType)
            {
                case TType.Void:
                    throw new NotSupportedException();
                case TType.Struct:
                    GenerateStructSerialize(param);
                    break;
                case TType.Exception:
                    GenerateStructSerialize(param);
                    break;
                case TType.List:
                case TType.Map:
                case TType.Set:
                    GenerateContainerSerialize(param);
                    break;
                case TType.Bool:
                case TType.Byte:
                case TType.Double:
                case TType.I16:
                case TType.I32:
                case TType.I64:
                case TType.String:
                    Write("oprot.Write{0}(", thriftType.ToString());
                    if (type.IsEnum)
                        Write("({0})", Enum.GetUnderlyingType(type).ToString());
                    WriteLine("_{0});", param.Name);
                    break;
                default:
                    throw new NotImplementedException();
            }
        }

        void GenerateStructSerialize(Parameter param)
        {
            WriteLine("var {0}Impl = new {1}Impl()", ToTitleCase(param.Name),
                param.ParameterType.Name);
            WriteStartBraceIndent();

            foreach (var field in param.ParameterType.GetFields())
                WriteLine("{1} = {0}.{1},", ToTitleCase(param.Name), field.Name);

            PopIndent();
            WriteLine("};");
            WriteLine("{0}Impl.Write(oprot);", ToTitleCase(param.Name));
        }

        // Used to generate unique names when de/serializing collections.
        int GenericIndex = 0;

        void GenerateContainerSerialize(Parameter param)
        {
            GenerateListSerialize(param);
        }

        void GenerateFieldDeserialize(Parameter param)
        {
            var type = param.ParameterType;
            var thriftType = ConvertFromTypeToThrift(param.ParameterType);

            switch (thriftType)
            {
                case TType.Void:
                    throw new NotSupportedException();
                case TType.Struct:
                    GenerateStructDeserialize(param);
                    break;
                case TType.Exception:
                    GenerateStructDeserialize(param);
                    break;
                case TType.List:
                case TType.Map:
                case TType.Set:
                    GenerateContainerDeserialize(param);
                    break;
                case TType.Bool:
                case TType.Byte:
                case TType.Double:
                case TType.I16:
                case TType.I32:
                case TType.I64:
                case TType.String:
                    Write("{0} = ", ToTitleCase(param.Name));
                    // If it is an enum, then we need to cast the value.
                    if (type.IsValueType && !type.IsPrimitive)
                        Write("({0}) ", type.FullName);
                    WriteLine("iprot.Read{0}();", thriftType.ToString());
                    break;
                default:
                    throw new NotImplementedException();
            }
        }

        void GenerateStructDeserialize(Parameter param)
        {
            WriteLine("var {0}Impl = new {1}Impl();", ToTitleCase(param.Name),
                param.ParameterType.Name);
            WriteLine("{0}Impl.Read(iprot);", ToTitleCase(param.Name));

            WriteLine("{0} = new {1}()", ToTitleCase(param.Name),
                ConvertToTypeString(param.ParameterType));
            WriteStartBraceIndent();

            foreach (var field in param.ParameterType.GetFields())
                WriteLine("{1} = {0}Impl.{1},", ToTitleCase(param.Name), field.Name);

            PopIndent();
            WriteLine("};");
        }

        void GenerateContainerDeserialize(Parameter param)
        {
            GenerateListDeserialize(param);
        }

        void GenerateListSerialize(Parameter param)
        {
            var paramType = param.ParameterType;
            var listElemType = param.ParameterType.GetGenericArguments()[0];

            WriteLine("oprot.WriteListBegin(new TList(TType.{0}, {1}.Count));",
                      ConvertFromTypeToThrift(paramType).ToString(),
                      ToTitleCase(param.Name));

            var iterName = string.Format("_iter{0}", GenericIndex++);
            WriteLine("foreach (var {0} in {1})", iterName, ToTitleCase(param.Name));
            WriteStartBraceIndent();

            var elemName = string.Format("_elem{0}", GenericIndex++);
            WriteLine("var {0} = new {1}Impl()", elemName, listElemType.Name);
            WriteStartBraceIndent();
            foreach (var field in listElemType.GetFields())
                WriteLine("{0} = {1}.{0},", field.Name, iterName);
            PopIndent();
            WriteLine("};");

            WriteLine("{0}.Write(oprot);", elemName);
            WriteCloseBraceIndent();

            WriteLine("oprot.WriteListEnd();");
        }

        void GenerateListDeserialize(Parameter param)
        {
            var listElemType = param.ParameterType.GetGenericArguments()[0];

            WriteLine("{0} = new List<{1}>();", ToTitleCase(param.Name),
                      ConvertToGenericArgsString(param.ParameterType));

            var listName = string.Format("_list{0}", GenericIndex++);
            WriteLine("var {0} = iprot.ReadListBegin();", listName);

            var loopName = string.Format("_i{0}", GenericIndex++);
            WriteLine("for (var {0} = 0; {0} < {1}.Count; ++{0})",
                      loopName, listName);

            WriteStartBraceIndent();

            var elemName = string.Format("_elem{0}", GenericIndex++);
            WriteLine("var {0} = new {1}Impl();", elemName, listElemType.Name);
            WriteLine("{0}.Read(iprot);", elemName);

            var elemName2 = string.Format("_elem{0}", GenericIndex++);
            WriteLine("var {0} = new {1}()", elemName2, listElemType.FullName);
            WriteStartBraceIndent();
            foreach (var field in listElemType.GetFields())
                WriteLine("{0} = {1}.{0},", field.Name, elemName);
            PopIndent();
            WriteLine("};");

            WriteLine("{0}.Add({1});", ToTitleCase(param.Name), elemName2);

            WriteCloseBraceIndent();
            WriteLine("iprot.ReadListEnd();");
        }

        void GenerateServiceMethodRead(IEnumerable<Parameter> parameters, bool isResult)
        {
            WriteLine("public void Read(Serializer iprot)");
            WriteStartBraceIndent();

            WriteLine("iprot.ReadStructBegin();");

            WriteLine("while (true)");
            WriteStartBraceIndent();

            GenerateServiceMethodReadFields(parameters, isResult);

            WriteCloseBraceIndent();
            WriteLine("iprot.ReadStructEnd();");

            WriteCloseBraceIndent();
        }

        void GenerateServiceMethodReadFields(IEnumerable<Parameter> parameters, bool isResult)
        {
            WriteLine("var field = iprot.ReadFieldBegin();");
            WriteLine("if (field.Type == TType.Stop)");
            WriteLineIndent("break;");
            NewLine();

            WriteLine("switch (field.ID)");
            WriteStartBraceIndent();

            foreach (var param in parameters)
            {
                if (isResult && param.ParameterType == typeof (void))
                    continue;

                var typeName = ConvertFromTypeToThrift(param.ParameterType);

                WriteLine("case {0}:", param.Id);
                PushIndent();

                WriteLine("if (field.Type == TType.{0})", typeName.ToString());
                WriteStartBraceIndent();

                GenerateFieldDeserialize(param);

                var required = param.GetType().GetCustomAttribute<RequiredAttribute>();
                if (required != null)
                    WriteLine("isset_{0} = true;", param.Name);

                WriteCloseBraceIndent();
                WriteLine("else");
                WriteStartBraceIndent();
                WriteLine("{0}.Skip(iprot, field.Type);", typeof (ProtocolUtil).Name);
                WriteCloseBraceIndent();
                WriteLine("break;");

                PopIndent();
            }

            WriteLine("default:");
            PushIndent();
            WriteLine("{0}.Skip(iprot, field.Type);", typeof (ProtocolUtil).Name);
            WriteLine("break;");
            PopIndent();

            WriteCloseBraceIndent();
            WriteLine("iprot.ReadFieldEnd();");
        }

        IEnumerable<Parameter> ConvertToActualParameters(IEnumerable<Parameter> parameters,
            bool isResult)
        {
            var actualParams = new List<Parameter>();
            foreach (var param in parameters)
            {
                if (isResult && param.ParameterType == typeof (void))
                    continue;
                actualParams.Add(param);
            }
            return actualParams;
        }

        static bool IsNullableType(Type type)
        {
            switch (ConvertFromTypeToThrift(type))
            {
                case TType.List:
                case TType.Map:
                case TType.Set:
                case TType.String:
                    return true;
                default:
                    return false;
            }
        }

        void GenerateServiceMethodWrite(string typeName,
            IEnumerable<Parameter> parameters, bool isResult)
        {
            WriteLine("public void Write({0} oprot)", typeof(Serializer).Name);
            WriteStartBraceIndent();

            WriteLine("var struc = new Struct(\"{0}_{1}\");",
                typeName, isResult ? "result" : "args");

            WriteLine("oprot.WriteStructBegin(struc);");

            parameters = ConvertToActualParameters(parameters, isResult);
            if (parameters.Any())
                WriteLine("var field = new Field();");

            foreach (var param in parameters)
            {
                if (isResult && param.ParameterType == typeof(void))
                    continue;

                if (IsNullableType(param.ParameterType))
                    WriteLine("if ({0} != null && __isset.{1})", ToTitleCase(param.Name),
                        param.Name);
                else
                    WriteLine("if (__isset.{0})", param.Name);

                WriteStartBraceIndent();

                WriteLine("field.Name = \"{0}\";", param.Name);
                WriteLine("field.Type = TType.{0};",
                    ConvertFromTypeToThrift(param.ParameterType).ToString());
                WriteLine("field.ID = {0};", param.Id);
                WriteLine("oprot.WriteFieldBegin(field);");
                
                GenerateFieldSerialize(param);
                
                WriteLine("oprot.WriteFieldEnd();");

                WriteCloseBraceIndent();
            }

            WriteLine("oprot.WriteFieldStop();");
            WriteLine("oprot.WriteStructEnd();");

            WriteCloseBraceIndent();
        }

        void GeneratePropertyList(IEnumerable<Parameter> parameters)
        {
            foreach (var param in parameters)
            {
                WriteLine("public {0} {1}", ConvertToTypeString(param.ParameterType),
                          ToTitleCase(param.Name));
                WriteStartBraceIndent();

                WriteLine("get {{ return _{0}; }}", param.Name);
                WriteLine("set {{ __isset.{0} = true; this._{0} = value; }}", param.Name);

                WriteCloseBraceIndent();
                NewLine();
            }
        }

        void GenerateServiceMethodResult(MethodInfo method)
        {
            WriteLine("[Serializable]");
            WriteLine("public partial class {0}_result : Base", method.Name);
            WriteStartBraceIndent();

            var parameters = new List<Parameter>();

            var hasReturn = method.ReturnType != typeof (void);
            if (hasReturn)
            {
                parameters.Add(new Parameter
                    {
                        Name = "success",
                        ParameterType = method.ReturnType
                    });
            }

            var throws = method.GetCustomAttributes<ThrowsAttribute>();
            foreach (var exception in throws)
            {
                var param = new Parameter()
                    {
                        Name = "exception" + exception.Id,
                        ParameterType = exception.Exception,
                        Id = exception.Id
                    };
                parameters.Add(param);
            }

            foreach(var param in parameters)
            {
                WriteLine("private {0} _{1};",
                    ConvertToTypeString(param.ParameterType), param.Name);
            }
            NewLine();

            GeneratePropertyList(parameters);

            // Generate an __isset structure that keeps track of what variables
            // have been set.
            WriteLine("public Isset __isset;");
            NewLine();

            WriteLine("[Serializable]");
            WriteLine("public struct Isset");
            WriteStartBraceIndent();
            foreach(var param in parameters)
                WriteLine("public bool {0};", param.Name);
            WriteCloseBraceIndent();
            NewLine();

            // Generate constructor
            WriteLine("public {0}_result()", method.Name);
            WriteLine("{");
            WriteLine("}");
            NewLine();

            // Generate read method
            GenerateServiceMethodRead(parameters, isResult: true);
            NewLine();

            // Generate write method
            GenerateServiceMethodWrite(method.Name, parameters, isResult: true);

            WriteCloseBraceIndent();
        }

        static bool IsInstanceOfGenericType(Type genericType, object instance)
        {
            Type type = instance.GetType();

            while (type != null)
            {
                if (type.IsGenericType &&
                    type.GetGenericTypeDefinition() == genericType)
                {
                    return true;
                }

                foreach (var @interface in type.GetInterfaces())
                {
                    if (@interface.IsAssignableFrom(type))
                        return true;
                }

                type = type.BaseType;
            }

            return false;
        }

        static TType ConvertFromTypeToThrift(Type type)
        {
            if (type == typeof(void))
                return TType.Void;
            else if (type == typeof(bool))
                return TType.Bool;
            else if (type == typeof(byte))
                return TType.Byte;
            else if (type == typeof(double))
                return TType.Double;
            else if (type == typeof(short))
                return TType.I16;
            else if (type == typeof(int))
                return TType.I32;
            else if (type == typeof(long))
                return TType.I64;
            else if (type == typeof(double))
                return TType.Double;
            else if (type == typeof(string))
                return TType.String;
            else if (type.IsEnum)
                return ConvertFromTypeToThrift(Enum.GetUnderlyingType(type));
            else if (type.IsValueType && !type.IsPrimitive)
                return TType.Struct;
            else if (type.IsSubclassOf(typeof(Exception)))
                return TType.Exception;
            else if (IsInstanceOfGenericType(typeof (IList<>), type))
                return TType.List;
            else if (IsInstanceOfGenericType(typeof (IDictionary<,>), type))
                return TType.Map;
            else if (IsInstanceOfGenericType(typeof (ISet<>), type))
                return TType.Set;

            throw new NotImplementedException();
        }

        static string ConvertToTypeString(TType type)
        {
            switch (type)
            {
                case TType.Void:
                    return "void";
                case TType.Bool:
                    return "bool";
                case TType.Byte:
                    return "byte";
                case TType.Double:
                    return "double";
                case TType.I16:
                    return "short";
                case TType.I32:
                    return "int";
                case TType.I64:
                    return "long";
                case TType.String:
                    return "string";
                default:
                    throw new NotImplementedException();
            }
        }

        static string ConvertToGenericArgsString(Type type)
        {
            var thriftType = ConvertFromTypeToThrift(type);
            return ConvertToGenericArgsString(type, thriftType);
        }

        static string ConvertToGenericArgsString(Type type, TType thriftType)
        {
            var sb = new StringBuilder();

            var parameters = type.GetGenericArguments();
            for (var i = 0; i < parameters.Length; i++)
            {
                var param = parameters[i];
                sb.Append(param.FullName);
                if (i < parameters.Length - 1)
                    sb.Append(", ");
            }

            return sb.ToString();
        }

        static string ConvertToGenericTypeString(Type type, TType thriftType)
        {
            var sb = new StringBuilder();
            Debug.Assert(type.IsGenericType);

            switch (thriftType)
            {
                default:
                    throw new NotSupportedException();
                case TType.List:
                    sb.Append("IList<");
                    break;
                case TType.Map:
                    sb.Append("IDictionary<");
                    break;
                case TType.Set:
                    sb.Append("ISet<");
                    break;
            }

            sb.Append(ConvertToGenericArgsString(type, thriftType));
            sb.Append(">");

            return sb.ToString();
        }

        static string ConvertToTypeString(Type type)
        {
            if (type.IsEnum)
                return type.FullName;

            var thriftType = ConvertFromTypeToThrift(type);

            switch (thriftType)
            {
                case TType.Struct:
                case TType.Exception:
                    return type.FullName;
                case TType.List:
                case TType.Map:
                case TType.Set:
                    return ConvertToGenericTypeString(type, thriftType);
            }

            return ConvertToTypeString(thriftType);
        }

        static string ToTitleCase(string str)
        {
            var result = str;
            if (!string.IsNullOrEmpty(str))
            {
                var words = str.Split(' ');
                for (int index = 0; index < words.Length; index++)
                {
                    var s = words[index];
                    if (s.Length > 0)
                    {
                        words[index] = s[0].ToString(CultureInfo.InvariantCulture)
                            .ToUpper() + s.Substring(1);
                    }
                }
                result = string.Join(" ", words);
            }
            return result;
        }

        struct Parameter
        {
            public Parameter(ParameterInfo info)
            {
                Name = info.Name;
                ParameterType = info.ParameterType;

                var id = info.GetCustomAttribute<IdAttribute>();
                if (id == null)
                    throw new Exception("expected an Id() attribute");

                Id = id.Id;
            }

            public Parameter(FieldInfo info)
            {
                Name = info.Name;
                ParameterType = info.FieldType;

                var id = info.GetCustomAttribute<IdAttribute>();
                if (id == null)
                    throw new Exception("expected an Id() attribute");

                Id = id.Id;
            }

            public string Name;
            public Type ParameterType;
            public int Id;
        }
    }
}
