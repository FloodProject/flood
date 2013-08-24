﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Flood.RPC.Serialization;
using RPCGen;

[assembly: InternalsVisibleToAttribute("Flood.Tools.RPCGen.Tests.GeneratorTests")]
namespace Flood.Tools.RPCGen
{
    internal class Generator : TextGenerator
    {

#region Generate Data Objects

        public void GenerateDataObject(Type type)
        {
            GenerateUsings();
            GenerateDataObjectClass(type);
        }

        public void GenerateDataObjectClass(Type type)
        {
            WriteLine("namespace  {0}", type.Namespace);
            WriteStartBraceIndent();

            WriteLine("[Serializable]");
            WriteLine("public class {0}", GetStubsClassName(type, false));
            WriteStartBraceIndent();

            // Generate fields
            foreach (var field in GetAllFields(type))
            {
                if (!Metadata.HasId(field))
                    continue;

                WriteLine("private {0} _{1};", PrettyName(field.FieldType), field.Name);
                NeedNewLine();
            }

            // Generate properties.
            foreach (var property in GetAllProperties(type))
            {
                if (!Metadata.HasId(property))
                    continue;

                WriteLine("private {0} _{1};",
                    PrettyName(property.PropertyType), property.Name);
                NeedNewLine();
            }
            NewLineIfNeeded();

            var parameters = ConvertFieldToParametersList(type);
            GeneratePropertyList(parameters);
            GenerateIsSet(parameters);

            // Generate constructor
            WriteLine("public {0}()", GetStubsClassName(type, false));
            WriteLine("{");
            WriteLine("}");
            NewLine();

            // Generate read method
            GenerateServiceMethodRead(parameters, isResult: false);
            NewLine();

            // Generate write method
            GenerateServiceMethodWrite(type.Name, parameters, isResult: false);

            WriteCloseBraceIndent();
            WriteCloseBraceIndent();
        }

        private List<Parameter> ConvertFieldToParametersList(Type type)
        {
            var parameters = new List<Parameter>();
            foreach(var field in GetAllFields(type))
            {
                if (!Metadata.HasId(field))
                    continue;

                parameters.Add(new Parameter(field));
            }

            foreach (var property in GetAllProperties(type))
            {
                if (!Metadata.HasId(property))
                    continue;
                
                parameters.Add(new Parameter(property));
            }

            return parameters;
        }

#endregion

#region Generate Services

        public void GenerateService(Type type)
        {
            GenerateUsings();

            WriteLine("namespace  {0}", type.Namespace);
            WriteStartBraceIndent();

            WriteLine("public class {0}", GetStubsClassName(type, false));
            WriteStartBraceIndent();

            //GenerateInterface(type);

            GenerateServiceProxy(type);
            GenerateServiceProcessor(type);

            var methods = type.GetMethods();
            for (var i = 0; i < methods.Length; i++)
            {
                var method = methods[i];

                if (IsEventMethod(type, method))
                    continue;

                // Throws exeption if type is not Task
                GetMethodReturnType(method);

                var argsClassName = GetProcedureArgsClassName(method);
                GenerateServiceMethodArgs(method, argsClassName);
                NewLine();

                var retType = GetMethodReturnType(method);
                var resultClassName = GetProcedureResultClassName(method);
                GenerateServiceMethodResult(method, resultClassName, retType);

                if (i < methods.Length - 1)
                    NewLine();
            }

            WriteCloseBraceIndent();
            WriteCloseBraceIndent();
        }

        #region Service Proxy

        private void GenerateServiceProxy(Type type)
        {
            Write("public class Proxy : RPCProxy, {0}", PrettyName(type));
            WriteStartBraceIndent();

            // Generate client constructors
            WriteLine("public Proxy(RPCPeer peer, int implId, int proxyId)");
            WriteLine("    : base(peer, implId, proxyId)");
            WriteStartBraceIndent();
            WriteCloseBraceIndent();
            NewLine();

            foreach (var method in type.GetMethods())
            {
                if (IsEventMethod(type, method))
                    continue;

                GenerateProtocolMethod(type, method);
            }

            foreach (var @event in type.GetEvents())
            {
                GenerateProxyEvent(type, @event);
            }

            WriteCloseBraceIndent();
            NewLine();
        }

        private static string GetProcedureSendMethodName(MethodInfo method)
        {
            var procedureId = GetProcedureCallId(method);
            return string.Format("send_{0}_{1}", method.Name, procedureId);
        }

        private static string GetProcedureReceiveMethodName(MethodInfo method)
        {
            var procedureId = GetProcedureCallId(method);
            return string.Format("recv_{0}_{1}", method.Name, procedureId);
        }

        private void GenerateProtocolMethod(Type type, MethodInfo method)
        {
            var retType = GetMethodReturnType(method);
            if (retType == typeof (void))
            {
                Write("async Task {0}.{1}(", PrettyName(type), method.Name);
            }
            else
            {
                var typeString = PrettyName(retType);
                Write("async Task<{0}> {1}.{2}(", typeString, PrettyName(type),
                    method.Name);
            }

            var parameters = method.GetParameters();
            GenerateParameterList(parameters);

            WriteLine(")");

            WriteStartBraceIndent();

            WriteLine("var seqNum = GetNextSequenceNumber();");
            Write("var request = {0}(seqNum", GetProcedureSendMethodName(method));
            for (var i = 0; i < parameters.Length; i++)
            {
                Write(", {0}", parameters[i].Name);
            }
            WriteLine(");");
            WriteLine("var response = await DispatchCall(request, seqNum);");
            if (retType != typeof(void))
                Write("return ");
            WriteLine("{0}(response);", GetProcedureReceiveMethodName(method));

            WriteCloseBraceIndent();
            NewLine();

            // Generate send method
            GenerateProtocolSend(method, parameters);

            // Generate receive method
            GenerateProtocolReceive(method);
        }

        private void GenerateProtocolReceive(MethodInfo method)
        {
            var retType = GetMethodReturnType(method);
            Write("private {0} {1}(RPCData response)", PrettyName(retType),
                  GetProcedureReceiveMethodName(method));
            WriteStartBraceIndent();

            WriteLine("if (response.Header.CallType == RPCDataType.Exception)");
            WriteStartBraceIndent();
            WriteLine("var x = RPCException.Read(response.Serializer);");
            WriteLine("response.Serializer.ReadProcedureCallEnd();");
            WriteLine("throw x;");
            WriteCloseBraceIndent();

            WriteLine("var result = new {0}();", GetProcedureResultClassName(method));
            WriteLine("result.Read(response.Serializer);");
            WriteLine("response.Serializer.ReadProcedureCallEnd();");

            if (retType != typeof(void))
            {
                WriteLine("if (result.__isset.success)");
                WriteLineIndent("return result.Success;");
            }

            List<ExceptionInfo> exceptionsInfo;
            if (Metadata.TryGetThrows(method, out exceptionsInfo))
            { 
                foreach (var exception in exceptionsInfo)
                {
                    WriteLine("if (result.__isset.exception{0})", exception.Id);
                    WriteStartBraceIndent();
                    WriteLine("throw result.Exception{0};", exception.Id);
                    WriteCloseBraceIndent();
                }
            }

            if (retType != typeof(void))
            {
                WriteLine("throw new RPCException(RPCException.ExceptionType.MissingResult,");
                WriteLineIndent(" \"{0} failed: unknown result\");", method.Name);
            }

            WriteCloseBraceIndent();
        }

        private void GenerateProtocolSend(MethodInfo method, ParameterInfo[] parameters)
        {
            var seqParam = (parameters.Length == 0) ? "int seqNum" : "int seqNum, ";
            Write("private RPCData {0}({1}", GetProcedureSendMethodName(method), seqParam);
            GenerateParameterList(parameters);
            WriteLine(")");
            WriteStartBraceIndent();

            WriteLine("var request = RPCData.CreateCall(Peer, ImplId, ProxyId);");

            var flags = GetRPCDataFlags(method);
            if (flags.Count > 0)
            {
                NewLine();
                WriteLine("request.Flags = {0};", string.Join(" | ", flags));
            }  

            NewLine();
            int procedureId = GetProcedureCallId(method);
            WriteLine("var procedureCall = new Flood.RPC.Serialization.ProcedureCall({0}, seqNum);", procedureId);
            WriteLine("request.Serializer.WriteProcedureCallBegin(procedureCall);");

            WriteLine("var args = new {0}();", GetProcedureArgsClassName(method));
            foreach (var param in method.GetParameters())
                WriteLine("args.{0} = {1};", ToTitleCase(param.Name), param.Name);
            WriteLine("args.Write(request.Serializer);");
            WriteLine("request.Serializer.WriteProcedureCallEnd();");
            WriteLine("return request;");
            WriteCloseBraceIndent();
            NewLine();
        }

        private void GenerateProxyEvent(Type type, EventInfo eventInfo)
        {
            var eventType = PrettyName(eventInfo.EventHandlerType);
            var eventName = eventInfo.Name;
            var eventName2 = "_" + eventName;

            WriteLine("private event {0} {1};", eventType, eventName2);

            WriteLine("public event {0} {1}", eventType, eventName);
            WriteStartBraceIndent();
            WriteLine("add");
            WriteStartBraceIndent();
            WriteLine("if({0} == null)", eventName2);
            WriteStartBraceIndent();
            WriteLine("var eventSubscribe = RPCData.CreateEventSubscribe(Peer, ImplId, ProxyId);");
            WriteLine("eventSubscribe.Serializer.WriteI32({0});", GetEventId(eventInfo));
            WriteLine("Peer.Dispatch(eventSubscribe);");
            WriteCloseBraceIndent();
            WriteLine("{0} += value;", eventName2);
            WriteCloseBraceIndent();
            WriteLine("remove");
            WriteStartBraceIndent();
            WriteLine("{0} -= value;", eventName2);
            WriteCloseBraceIndent();
            WriteCloseBraceIndent();
            WriteLine("private void Invoke_{0}(RPCData data)", eventName);
            WriteStartBraceIndent();
            WriteCloseBraceIndent();
            NewLine();
        }

        private static int GetProcedureCallId(MethodInfo method)
        {
            int id;
            if (!Metadata.TryGetId(method, out id))
            {
                var msg = string.Format("Method {0}.{1} require an Id attribute.", method.DeclaringType.FullName, method.Name);
                throw new Exception(msg);
            }
            return id;
        }

        private static int GetEventId(EventInfo @event)
        {
            int id;
            if (!Metadata.TryGetId(@event, out id))
            {
                var msg = string.Format("Event {0}.{1} require an Id attribute.", @event.DeclaringType.FullName, @event.Name);
                throw new Exception(msg);
            }
            return id;
        }

        private List<string> GetRPCDataFlags(MethodInfo method)
        {
            var flags = new List<string>();
            if(Metadata.HasEncrypted(method))
                flags.Add("PacketFlags.Ecrypted");
            if(Metadata.HasCompressed(method))
                flags.Add("PacketFlags.Compressed");
            if(Metadata.HasSigned(method))
                flags.Add("PacketFlags.Signed");
            return flags;
        }

        private void GenerateParameterList(IList<ParameterInfo> parameters)
        {
            for (var i = 0; i < parameters.Count; i++)
            {
                var param = parameters[i];
                Write("{0} {1}", PrettyName(param.ParameterType),
                      param.Name);

                if (i < parameters.Count - 1)
                    Write(", ");
            }
        }

        #endregion

        #region Service Processor

        private void GenerateServiceProcessor(Type type)
        {
            WriteLine("public class Impl : RPCImpl");
            WriteStartBraceIndent();

            WriteLine("private readonly {0} iface_;", PrettyName(type));

            // Generate constructor
            {
                WriteLine("public Impl({0} iface)", PrettyName(type));
                WriteLine(" : base(iface)");
                WriteStartBraceIndent();

                WriteLine("iface_ = iface;");

                foreach (var method in type.GetMethods())
                {
                    if (IsEventMethod(type, method))
                        continue;

                    var procedureId = GetProcedureCallId(method);

                    WriteLine("processors[{0}] = new Processors({1});", procedureId,
                        GetProcedureProcessMethodName(method));
                }

                foreach (var @event in type.GetEvents())
                {
                    var eventId = GetEventId(@event);

                    WriteLine("processors[{0}] = new Processors({1}, {2});", eventId,
                        GetEventSubscribeMethodName(@event),
                        GetEventUnsubscribeMethodName(@event)
                        );
                }

                WriteCloseBraceIndent();
                NewLine();
            }

            // Generate process methods
            foreach (var method in type.GetMethods())
            {
                if (IsEventMethod(type, method))
                    continue;

                GenerateServiceProcessMethod(method);
                NewLine();
            }

            foreach (var @event in type.GetEvents())
            {
                GenerateEventSubscribeMethod(@event);
                GenerateEventUnsubscribeMethod(@event);
            }

            WriteCloseBraceIndent();
            NewLine();
        }

        private static string GetProcedureProcessMethodName(MethodInfo method)
        {
            var procedureId = GetProcedureCallId(method);
            return string.Format("{0}_{1}_Process", method.Name, procedureId);
        }

        private static string GetEventSubscribeMethodName(EventInfo @event)
        {
            return string.Format("{0}_Subscribe", @event.Name);
        }

        private static string GetEventUnsubscribeMethodName(EventInfo @event)
        {
            return string.Format("{0}_Unsubscribe", @event.Name);
        }

        private void GenerateServiceProcessMethod(MethodInfo method)
        {
            WriteLine("public async Task {0}(RPCData request, ProcedureCall msg)",
                      GetProcedureProcessMethodName(method));
            WriteStartBraceIndent();

            WriteLine("var args = new {0}();", GetProcedureArgsClassName(method));
            WriteLine("args.Read(request.Serializer);");
            WriteLine("var result = new {0}();", GetProcedureResultClassName(method));

            // If the method throws exceptions, we need to call it inside a try-catch.
            if (Metadata.HasThrows(method))
            {
                WriteLine("try {");
                PushIndent();
            }

            var retType = GetMethodReturnType(method);
            if (retType != typeof(void))
                Write("result.Success = ");


            // Call the service method
            PropertyInfo prop = GetProperty(method);
            if (prop == null)
            {
                Write("await iface_.{0}(", method.Name);
                var parameters = method.GetParameters();
                for (var i = 0; i < parameters.Length; i++)
                {
                    var param = parameters[i];
                    Write("args.{0}", ToTitleCase(param.Name));

                    if (i < parameters.Length - 1)
                        Write(", ");
                }

                WriteLine(");");
            }
            else
            {
                if (prop.GetGetMethod() == method)
                    Write("iface_.{0}", prop.Name);
                else
                    Write("iface_.{0} = args.{1}", prop.Name, ToTitleCase(method.GetParameters()[0].Name));

                WriteLine(";");

            }

            List<ExceptionInfo> exceptionsInfo;
            if (Metadata.TryGetThrows(method, out exceptionsInfo))
            {
                PopIndent();

                // Write the catch part of the exception handling.
                foreach (var exception in exceptionsInfo)
                {
                    WriteLine("}} catch ({0} ex{1}) {{", PrettyName(exception.Type),
                              exception.Id);
                    PushIndent();
                    WriteLine("result.Exception{0} = ex{0};", exception.Id);
                    PopIndent();
                }
            
                WriteLine("}");
            }

            WriteLine("var reply = RPCData.Create(request, RPCDataType.Reply);");

            var procedureId = GetProcedureCallId(method);
            // Create a new ProcedureCall and reply to the RPC call
            WriteLine("reply.Serializer.WriteProcedureCallBegin(msg);");

            WriteLine("result.Write(reply.Serializer);");
            WriteLine("reply.Serializer.WriteProcedureCallEnd();");
            WriteLine("reply.Dispatch();");

            WriteCloseBraceIndent();
        }

        private void GenerateEventSubscribeMethod(EventInfo @event)
        {
            WriteLine("void {0}()", GetEventSubscribeMethodName(@event));
            WriteStartBraceIndent();
            WriteCloseBraceIndent();
        }

        private void GenerateEventUnsubscribeMethod(EventInfo @event)
        {
            WriteLine("void {0}()", GetEventUnsubscribeMethodName(@event));
            WriteStartBraceIndent();
            WriteCloseBraceIndent();
        }

        #endregion

        #region Service Method Args

        private static string GetProcedureArgsClassName(MethodInfo method)
        {
            var procedureId = GetProcedureCallId(method);
            return string.Format("{0}_{1}_args", method.Name, procedureId);
        }

        private void GenerateServiceMethodArgs(MethodInfo method, string name)
        {
            WriteLine("[Serializable]");
            WriteLine("public class {0}", name);
            WriteStartBraceIndent();

            // Generate private fields.
            foreach (var param in method.GetParameters())
            {
                WriteLine("private {0} _{1};",
                    PrettyName(param.ParameterType), param.Name);
                NeedNewLine();
            }
            NewLineIfNeeded();

            var parameters = ConvertToParametersList(method);
            GeneratePropertyList(parameters);
            GenerateIsSet(parameters);

            // Generate constructor
            WriteLine("public {0}()", name);
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

        private static List<Parameter> ConvertToParametersList(MethodInfo method)
        {
            // Convert from PropertyInfo to our own struct. This makes it easier
            // to use the same code for argument and result marshaling.
            var parameters = new List<Parameter>();
            foreach (var param in method.GetParameters())
                parameters.Add(new Parameter(param));
            return parameters;
        }
        
        #endregion

        #region Service Method Result

        private static string GetProcedureResultClassName(MethodInfo method)
        {
            var procedureId = GetProcedureCallId(method);
            return string.Format("{0}_{1}_result", method.Name, procedureId);
        }

        private void GenerateServiceMethodResult(MethodInfo method, string name, Type retType)
        {
            WriteLine("[Serializable]");
            WriteLine("public class {0}", name);
            WriteStartBraceIndent();

            var parameters = new List<Parameter>();

            var hasReturn = retType != typeof(void);
            if (hasReturn)
            {
                parameters.Add(new Parameter
                {
                    Name = "success",
                    ParameterType = retType
                });
            }

            List<ExceptionInfo> exceptionsInfo;
            if (Metadata.TryGetThrows(method, out exceptionsInfo))
            {
                foreach (var exception in exceptionsInfo)
                {
                    var param = new Parameter()
                    {
                        Name = "exception" + exception.Id,
                        ParameterType = exception.Type,
                        Id = exception.Id
                    };
                    parameters.Add(param);
                }
            }

            foreach (var param in parameters)
            {
                WriteLine("private {0} _{1};",
                    PrettyName(param.ParameterType), param.Name);
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
            foreach (var param in parameters)
                WriteLine("public bool {0};", param.Name);
            WriteCloseBraceIndent();
            NewLine();

            // Generate constructor
            WriteLine("public {0}()", name);
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

        #endregion

        #region General Auxiliaries

        public static Type GetMethodReturnType(MethodInfo method)
        {
            if (method.ReturnType == typeof (Task))
                return typeof(void);

            if (IsInstanceOfGenericType(typeof (Task<>), method.ReturnType))
                return method.ReturnType.GenericTypeArguments[0];

            throw new Exception(string.Format("{0}.{1} does not return a Task", 
                        method.DeclaringType.FullName, method.Name));
        }

        private static bool IsEventMethod(Type type, MethodInfo method)
        {
            foreach (var @event in type.GetEvents())
            {
                if (@event.AddMethod == method
                    || @event.RemoveMethod == method
                    || @event.RaiseMethod == method)
                    return true;
            }
            return false;
        }

        private static bool GetInheritedService(Type type, out Type @base)
        {
            @base = null;

            foreach (var @interface in type.GetInterfaces())
            {
                if (!Metadata.IsService(@interface))
                    continue;

                @base = @interface;
                return true;
            }

            return false;
        }

        private static string ToTitleCase(string str)
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

        /// <summary>
        /// If method is getter or setter of a property returns the PropertyInfo associated
        /// </summary>
        /// <returns>PropertyInfo or null if not a property getter/setter</returns>
        private static PropertyInfo GetProperty(MethodInfo method)
        {
            var retType = GetMethodReturnType(method);
            bool takesArg = method.GetParameters().Length == 1;
            bool hasReturn = retType != typeof(void);
            if (takesArg == hasReturn) return null;
            if (takesArg)
                return method.DeclaringType.GetProperties().FirstOrDefault(prop => prop.GetSetMethod() == method);

            return method.DeclaringType.GetProperties().FirstOrDefault(prop => prop.GetGetMethod() == method);
        }

        private IEnumerable<Parameter> ConvertToActualParameters(IEnumerable<Parameter> parameters, bool isResult)
        {
            var actualParams = new List<Parameter>();
            foreach (var param in parameters)
            {
                if (isResult && param.ParameterType == typeof(void))
                    continue;
                actualParams.Add(param);
            }
            return actualParams;
        }

        private static bool IsNullableType(Type type)
        {
            switch (ConvertFromTypeToThrift(type))
            {
                case TType.Array:
                case TType.List:
                case TType.Map:
                case TType.String:
                    return true;
                default:
                    return false;
            }
        }

        private static IEnumerable<FieldInfo> GetAllFields(Type t)
        {
            if (t == null)
                return Enumerable.Empty<FieldInfo>();

            BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance | BindingFlags.DeclaredOnly;
            return t.GetFields(flags).Union(GetAllFields(t.BaseType));
        }

        private static IEnumerable<PropertyInfo> GetAllProperties(Type t)
        {
            if (t == null)
                return Enumerable.Empty<PropertyInfo>();

            BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance | BindingFlags.DeclaredOnly;
            return t.GetProperties(flags).Union(GetAllProperties(t.BaseType));
        }

        #endregion

        #region Code Generation

        // Used to generate unique names when (de)serializing collections.
        int GenericIndex = 0;

        private void GenerateUsings()
        {
            WriteLine("using System;");
            WriteLine("using System.Collections.Generic;");
            WriteLine("using Flood;");
            WriteLine("using Flood.RPC;");
            WriteLine("using Flood.RPC.Metadata;");
            WriteLine("using Flood.RPC.Serialization;");
            WriteLine("using System.Threading.Tasks;");

            NewLine();
        }
        
        private void GenerateIsSet(IEnumerable<Parameter> parameters)
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
        
        //TODO : this function seems to generate useless code if there are no actual parameters, 
        //TODO : should it even be called in that case?
        private void GenerateServiceMethodWrite(string typeName,
            IEnumerable<Parameter> parameters, bool isResult)
        {
            WriteLine("public void Write(Serializer oprot)");
            WriteStartBraceIndent();

            WriteLine("var struc = new DataObject(\"{0}_{1}\");",
                typeName, isResult ? "result" : "args");

            WriteLine("oprot.WriteDataObjectBegin(struc);");

            parameters = ConvertToActualParameters(parameters, isResult);
            if (parameters.Any())
                WriteLine("var field = new Field();");

            foreach (var param in parameters)
            {
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
                var className = "\"\"";
                
                if (ConvertFromTypeToThrift(param.ParameterType) == TType.DataObject)
                    className = ToTitleCase(param.Name) + ".GetType().Name";

                WriteLine("field.ClassName = {0};", className);
                WriteLine("oprot.WriteFieldBegin(field);");
                GenerateTypeSerialization(param.ParameterType, ToTitleCase(param.Name));

                WriteLine("oprot.WriteFieldEnd();");

                WriteCloseBraceIndent();
            }

            WriteLine("oprot.WriteFieldStop();");
            WriteLine("oprot.WriteDataObjectEnd();");

            WriteCloseBraceIndent();
        }

        private void GenerateServiceMethodRead(IEnumerable<Parameter> parameters, bool isResult)
        {
            WriteLine("public void Read(Serializer iprot)");
            WriteStartBraceIndent();

            WriteLine("iprot.ReadDataObjectBegin();");

            WriteLine("while (true)");
            WriteStartBraceIndent();

            GenerateServiceMethodReadFields(parameters, isResult);

            WriteCloseBraceIndent();
            WriteLine("iprot.ReadDataObjectEnd();");

            WriteCloseBraceIndent();
        }

        private void GenerateServiceMethodReadFields(IEnumerable<Parameter> parameters, bool isResult)
        {
            WriteLine("var field = iprot.ReadFieldBegin();");
            WriteLine("if (field.Type == TType.Stop)");
            WriteLineIndent("break;");
            NewLine();

            WriteLine("switch (field.ID)");
            WriteStartBraceIndent();

            foreach (var param in parameters)
            {
                if (isResult && param.ParameterType == typeof(void))
                    continue;

                var typeName = ConvertFromTypeToThrift(param.ParameterType);

                WriteLine("case {0}:", param.Id);
                PushIndent();

                WriteLine("if (field.Type == TType.{0})", typeName.ToString());
                WriteStartBraceIndent();

                GenerateTypeDeserialization(param.ParameterType, ToTitleCase(param.Name));

                if (Metadata.IsRequired(param.GetType()))
                    WriteLine("isset_{0} = true;", param.Name);

                WriteCloseBraceIndent();
                WriteLine("else");
                WriteStartBraceIndent();
                WriteLine("SerializerUtil.Skip(iprot, field.Type);");
                WriteCloseBraceIndent();
                WriteLine("break;");

                PopIndent();
            }

            WriteLine("default:");
            PushIndent();
            WriteLine("SerializerUtil.Skip(iprot, field.Type);");
            WriteLine("break;");
            PopIndent();

            WriteCloseBraceIndent();
            WriteLine("iprot.ReadFieldEnd();");
        }

        private void GeneratePropertyList(IEnumerable<Parameter> parameters)
        {
            foreach (var param in parameters)
            {
                WriteLine("public {0} {1}", PrettyName(param.ParameterType),
                          ToTitleCase(param.Name));
                WriteStartBraceIndent();

                WriteLine("get {{ return _{0}; }}", param.Name);
                WriteLine("set {{ __isset.{0} = true; this._{0} = value; }}", param.Name);

                WriteCloseBraceIndent();
                NewLine();
            }
        }

        #region Serialization

        /// <summary>
        /// Generates the code to serialize a type 
        /// </summary>
        private void GenerateTypeSerialization(Type type, string name)
        {
            var thriftType = ConvertFromTypeToThrift(type);

            switch (thriftType)
            {
                case TType.Array:
                    GenerateArraySerialize(type, name);
                    break;
                case TType.List:
                    GenerateListSerialize(type, name);
                    break;
                case TType.Map:
                    GenerateMapSerialize(type, name);
                    break;
                case TType.DataObject:
                case TType.Exception:
                    GenerateStructSerialize(type, name);
                    break;
                case TType.Bool:
                case TType.Byte:
                case TType.Double:
                case TType.I16:
                case TType.I32:
                case TType.I64:
                case TType.String:
                    var enumCast = "";
                    if (type.IsEnum)
                        enumCast = String.Format("({0})", Enum.GetUnderlyingType(type).ToString());
                    WriteLine("oprot.Write{0}({1}{2});", thriftType.ToString(), enumCast, name);
                    break;
                case TType.Guid:
                    WriteLine("oprot.WriteString({0}.ToString());", name);
                    break;
                case TType.DateTime:
                    WriteLine("oprot.WriteI64({0}.Ticks);", name);
                    break;
                case TType.Void:
                default:
                    System.Diagnostics.Debugger.Break();
                    throw new NotImplementedException();
            }

        }

        /// <summary>
        /// Generates the code to serialize an array
        /// </summary>
        private void GenerateArraySerialize(Type type, string name)
        {
            var arrayElemType = type.GetElementType();

            WriteLine("oprot.WriteArrayBegin(new TArray(TType.{0}, {1}.Length));",
                      ConvertFromTypeToThrift(type).ToString(),
                      ToTitleCase(name));

            var iterName = string.Format("_iter{0}", GenericIndex++);
            WriteLine("foreach (var {0} in {1})", iterName, ToTitleCase(name));
            WriteStartBraceIndent();

            GenerateTypeSerialization(arrayElemType, iterName);

            WriteCloseBraceIndent();

            WriteLine("oprot.WriteArrayEnd();");
        }

        /// <summary>
        /// Generates the code to serialize a field of type struct
        /// </summary>
        private void GenerateStructSerialize(Type type, string varName)
        {
            var implObjName = varName + "Impl";
            implObjName = implObjName.Replace(".", "");

            WriteLine("var {0} = new {1}();", implObjName, GetStubsClassName(type, true));
            GenerateStructInit(type, varName, implObjName);
            WriteLine("{0}.Write(oprot);", implObjName);
        }

        /// <summary>
        /// Generates the code to serialize a field of type struct
        /// </summary>
        private void GenerateStructInit(Type type, string origObjName, string destObjName, bool fromProperties = false)
        {
            foreach (var field in GetAllFields(type))
            {
                if (!Metadata.HasId(field))
                    continue;

                var origName = (fromProperties) ? ToTitleCase(field.Name) : field.Name;
                var destName = (!fromProperties) ? ToTitleCase(field.Name) : field.Name;
                WriteLine("{0}.{1} = {2}.{3};", destObjName, destName, origObjName, origName);
            }

            foreach (var property in GetAllProperties(type))
            {
                if (!Metadata.HasId(property))
                    continue;

                WriteLine("{0}.{1} = {2}.{3};", destObjName, property.Name, origObjName, property.Name);
            }
        }

        /// <summary>
        /// Generates the code to serialize a list  
        /// </summary>
        private void GenerateListSerialize(Type type, string name)
        {
            var listElemType = type.GetGenericArguments()[0];

            WriteLine("oprot.WriteListBegin(new TList(TType.{0}, {1}.Count));",
                      ConvertFromTypeToThrift(type).ToString(),
                      ToTitleCase(name));

            var iterName = string.Format("_iter{0}", GenericIndex++);
            WriteLine("foreach (var {0} in {1})", iterName, ToTitleCase(name));
            WriteStartBraceIndent();

            GenerateTypeSerialization(listElemType, iterName);

            WriteCloseBraceIndent();

            WriteLine("oprot.WriteListEnd();");
        }

        /// <summary>
        /// Generates the code to serialize a map  
        /// </summary>
        private void GenerateMapSerialize(Type type, string name)
        {
            var mapElemType1 = type.GetGenericArguments()[0];
            var mapElemType2 = type.GetGenericArguments()[1];

            WriteLine("oprot.WriteMapBegin(new TMap(TType.{0}, TType.{1}, {2}.Count));",
                      ConvertFromTypeToThrift(mapElemType1).ToString(),
                      ConvertFromTypeToThrift(mapElemType2).ToString(),
                      ToTitleCase(name));

            var iterName = string.Format("_iter{0}", GenericIndex++);
            WriteLine("foreach (var {0} in {1})", iterName, ToTitleCase(name));
            WriteStartBraceIndent();

            GenerateContainerDoubleElementSerialization(mapElemType1, mapElemType2, iterName);

            WriteCloseBraceIndent();

            WriteLine("oprot.WriteMapEnd();");

        }

        /// <summary>
        /// Generates the code to serialize a dual element container  
        /// </summary>
        private void GenerateContainerDoubleElementSerialization(Type elemType1, Type elemType2, string iterName)
        {
            GenerateTypeSerialization(elemType1, iterName + ".Key");
            GenerateTypeSerialization(elemType2, iterName + ".Value");
        }

        #endregion

        #region Deserialization

        /// <summary>
        /// Generates the code to deserialize a single element of an array 
        /// </summary>
        private void GenerateTypeDeserialization(Type type, string name, bool varExists = true)
        {

            var elemName = string.Format("_elem{0}", GenericIndex++);

            var thriftType = ConvertFromTypeToThrift(type);

            switch (thriftType)
            {
                case TType.Array:
                    GenerateArrayDeserialize(type, name);
                    break;
                case TType.List:
                    GenerateListDeserialize(type, name);
                    break;
                case TType.Map:
                    GenerateMapDeserialize(type, name);
                    break;
                case TType.DataObject:
                case TType.Exception:
                    WriteLine("var {0} = new {1}();", elemName, GetStubsClassName(type, true));
                    WriteLine("{0}.Read(iprot);", elemName);

                    var elemName2 = string.Format("_elem{0}", GenericIndex++);
                    WriteLine("var {0} = new {1}();", elemName2, PrettyName(type));
                    GenerateStructInit(type, elemName, elemName2, true);
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = {1};", name, elemName2);
                    break;
                case TType.Bool:
                case TType.Byte:
                case TType.Double:
                case TType.I16:
                case TType.I32:
                case TType.I64:
                case TType.String:
                    var cast = "";
                    if (type.IsEnum || type == typeof(float))
                        cast = String.Format("({0})", PrettyName(type));
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = {1}iprot.Read{2}();", name, cast, thriftType.ToString());
                    break;
                case TType.Guid:
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = new System.Guid(iprot.ReadString());", name);
                    break;
                case TType.DateTime:
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = new System.DateTime(iprot.ReadI64());", name);
                    break;
                case TType.Void:
                default:
                    throw new NotImplementedException();
            }

        }

        /// <summary>
        /// Generates the code to deserialize an array 
        /// </summary>
        private void GenerateArrayDeserialize(Type type, string name)
        {
            var arrayElemType = type.GetElementType();

            var arrayName = string.Format("_array{0}", GenericIndex++);
            WriteLine("var {0} = iprot.ReadArrayBegin();", arrayName);

            WriteLine("{0} = new {1}[{2}.Count];", ToTitleCase(name),
                      PrettyName(arrayElemType), arrayName);

            var iterName = string.Format("_i{0}", GenericIndex++);
            WriteLine("for (var {0} = 0; {0} < {1}.Count; ++{0})",
                      iterName, arrayName);

            WriteStartBraceIndent();

            GenerateTypeDeserialization(arrayElemType, ToTitleCase(name) + "[" + iterName + "]");

            WriteCloseBraceIndent();
            WriteLine("iprot.ReadArrayEnd();");
        }

        /// <summary>
        /// Generates the code to deserialize a field of type struct
        /// </summary>
        private void GenerateStructDeserialize(Type type, string varName)
        {
            var origObjName = varName+"Impl";
            WriteLine("var {0} = new {1}();", origObjName, GetStubsClassName(type, true));
            WriteLine("{0}.Read(iprot);", origObjName);

            var elemName = string.Format("_elem{0}", GenericIndex++);

            WriteLine("var {0} = new {1}();", elemName, PrettyName(type));
            GenerateStructInit(type, origObjName, elemName, true);
            WriteLine("var {0} = {1};", varName, elemName);
        }

        /// <summary>
        /// Generates the code to deserialize a list  
        /// </summary>
        private void GenerateListDeserialize(Type type, string name)
        {
            var listElemType = type.GetGenericArguments()[0];

            WriteLine("{0} = new {1}();", ToTitleCase(name),
                      PrettyName(type));

            var listName = string.Format("_list{0}", GenericIndex++);
            WriteLine("var {0} = iprot.ReadListBegin();", listName);

            var iterName = string.Format("_i{0}", GenericIndex++);
            WriteLine("for (var {0} = 0; {0} < {1}.Count; ++{0})",
                      iterName, listName);

            WriteStartBraceIndent();

            var elemName = string.Format("_elem{0}", GenericIndex++);

            GenerateTypeDeserialization(listElemType, elemName, false);
            WriteLine("{0}.Add({1});", name, elemName);

            WriteCloseBraceIndent();
            WriteLine("iprot.ReadListEnd();");
        }

        /// <summary>
        /// Generates the code to deserialize a map  
        /// </summary>
        private void GenerateMapDeserialize(Type type, string name)
        {
            var mapElemType1 = type.GetGenericArguments()[0];
            var mapElemType2 = type.GetGenericArguments()[1];

            WriteLine("{0} = new {1}();", ToTitleCase(name),
                      PrettyName(type));

            var mapName = string.Format("_set{0}", GenericIndex++);
            WriteLine("var {0} = iprot.ReadMapBegin();", mapName);

            var iterName = string.Format("_i{0}", GenericIndex++);
            WriteLine("for (var {0} = 0; {0} < {1}.Count; ++{0})",
                      iterName, mapName);

            WriteStartBraceIndent();

            GenerateContainerDoubleElementDeserialization(mapElemType1, mapElemType2, ToTitleCase(name));

            WriteCloseBraceIndent();
            WriteLine("iprot.ReadMapEnd();");
        }

        /// <summary>
        /// Generates the code to deserialize a double element container  
        /// </summary>
        private void GenerateContainerDoubleElementDeserialization(Type elemType1, Type elemType2, string containerName)
        {
            var elemName1 = string.Format("_elem{0}", GenericIndex++);
            var elemName2 = string.Format("_elem{0}", GenericIndex++);

            GenerateTypeDeserialization(elemType1, elemName1, false);
            GenerateTypeDeserialization(elemType2, elemName2, false);

            WriteLine("{0}.Add({1}, {2});", containerName, elemName1, elemName2);
        }

        #endregion

        #endregion

        #region Type Conversion 

        /// Returns a c# string representation o a type
        private static string PrettyName(Type type)
        {
            if (type.GetGenericArguments().Length == 0)
            {
                if (type == typeof(void))
                    return "void";

                return type.FullName.Replace("+",".");
            }
            var genericArguments = type.GetGenericArguments();
            var typeDefeninition = type.Name;
            var unmangledName = typeDefeninition.Substring(0, typeDefeninition.IndexOf("`"));
            return unmangledName + "<" + String.Join(",", genericArguments.Select(PrettyName)) + ">";
        }

        private static string GetStubsClassName(Type type, bool fullName)
        {
            var name = type.FullName;
            if (!fullName && name.Contains('.'))
            {
                var names = name.Split('.');
                name = names[names.Length - 1];
            }

            return name.Replace("+", "_") + "Stubs";
        }

        /// <summary>
        /// Checks if type of instance is a generic type and if it is of the type of genericType 
        /// or assignable to it
        /// </summary>
        internal static bool IsInstanceOfGenericType(Type genericType, object instance)
        {
            Type type = instance.GetType();

            while (type != null)
            {
                if (type.IsGenericType &&
                    type.GetGenericTypeDefinition() == genericType)
                {
                    return true;
                }

                if (genericType.IsAssignableFrom(type))
                    return true;

                type = type.BaseType;
            }

            return false;
        }
        
        /// <summary>
        /// Checks if type is a generic type and if it is of the type of genericType 
        /// or assignable to it
        /// </summary>
        internal static bool IsInstanceOfGenericType(Type genericType, Type type)
        {
            while (type != null)
            {
                if (type.IsGenericType &&
                    type.GetGenericTypeDefinition() == genericType)
                {
                    return true;
                }

                if (genericType.IsAssignableFrom(type))
                    return true;

                if (type.GetInterfaces().Any(i => i.IsGenericType && i.GetGenericTypeDefinition() == genericType))
                    return true;

                type = type.BaseType;
            }

            return false;
        }

        /// <summary>
        /// Converts from regular type to equivalent TType.
        /// </summary>
        internal static TType ConvertFromTypeToThrift(Type type)
        {
            if (type == typeof(void))
                return TType.Void;
            else if (type == typeof(bool))
                return TType.Bool;
            else if (type == typeof(byte))
                return TType.Byte;
            else if (type == typeof(double) || type == typeof(float))
                return TType.Double;
            else if (type == typeof(short))
                return TType.I16;
            else if (type == typeof(int))
                return TType.I32;
            else if (type == typeof(long))
                return TType.I64;
            else if (type == typeof(string))
                return TType.String;
            else if (type.IsArray)
                return TType.Array;
            else if (type == typeof(Guid) || typeof(Guid).IsAssignableFrom(type))
                return TType.Guid;
            else if (type == typeof(DateTime) || typeof(DateTime).IsAssignableFrom(type))
                return TType.DateTime;
            else if (type.IsEnum)
                return ConvertFromTypeToThrift(Enum.GetUnderlyingType(type));
            else if (Metadata.IsDataObject(type))
                return TType.DataObject;
            else if (type == typeof(Exception) || type.IsSubclassOf(typeof(Exception)))
                return TType.Exception;
            else if (IsInstanceOfGenericType(typeof(IDictionary<,>), type))
                return TType.Map;
            else if (IsInstanceOfGenericType(typeof(ICollection<>), type))
                return TType.List;

            throw new NotImplementedException("Unhandle type "+type);
        }

        #endregion

        #region Data Types

        private struct Parameter
        {
            public Parameter(ParameterInfo info)
            {
                Name = info.Name;
                ParameterType = info.ParameterType;

                if (Generator.GetProperty((info.Member as MethodInfo)) != null)
                {
                    Id = 0;
                    return;
                }

                int id;
                if (!Metadata.TryGetId(info, out id))
                    throw new Exception("Service Method's Parameters require an Id attribute.");

                Id = id;
            }

            public Parameter(FieldInfo info)
            {
                Name = info.Name;
                ParameterType = info.FieldType;

                int id;
                if (!Metadata.TryGetId(info, out id))
                    throw new Exception("DataObject's Fields require an Id attribute.");

                Id = id;
            }

            public Parameter(PropertyInfo info)
            {
                Name = info.Name;
                ParameterType = info.PropertyType;

                int id;
                if (!Metadata.TryGetId(info, out id))
                    throw new Exception("DataObject's Properties require an Id attribute.");

                Id = id;
            }

            public Parameter(string  name, Type type, int id)
            {
                Name = name;
                ParameterType = type;           
                Id = id;
            }
            public string Name;
            public Type ParameterType;
            public int Id;
        }

        #endregion

#endregion
    }
}
