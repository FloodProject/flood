using System;
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
        private Dictionary<Type, string> delegateNames;
        private int delegateCounter;
 
        public Generator()
        {
            delegateNames = new Dictionary<Type, string>();
        }

#region Generate Data Objects

        public void GenerateDataObject(Type type)
        {
            GenerateUsings();
            var className = GetStubsClassName(type, false);
            var parameters = ConvertFieldToParametersList(type);
            GenerateDataObjectClass(className, parameters, type.Namespace);
        }

        private void GenerateDataObjectClass(string className, List<Parameter> parameters, string @namespace)
        {
            if (@namespace != null)
            {
                WriteLine("namespace  {0}", @namespace);
                WriteStartBraceIndent();
            }

            WriteLine("public class {0} : IDataObject", className);
            WriteStartBraceIndent();

            // Generate fields
            foreach (var param in parameters)
            {
                WriteLine("public {0} {1};", PrettyName(param.ParameterType), ToTitleCase(param.Name));
                NeedNewLine();
            }

            WriteLine("private RPCStub stub;");
            NewLine();

            // Generate constructor
            WriteLine("public {0}(RPCStub stub)", className);
            WriteLine("{");
            WriteLine("this.stub = stub;");
            WriteLine("}");
            NewLine();

            // Generate read method
            GenerateServiceMethodRead(parameters, "stub");
            NewLine();

            // Generate write method
            GenerateServiceMethodWrite(parameters, "stub");

            WriteCloseBraceIndent();
            if (@namespace != null)
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

                if (!IsTask(method.ReturnType))
                    throw new Exception("Method " + method.Name+ " doesn't return a Task");

                var argsClassName = GetProcedureArgsClassName(method);
                var resultClassName = GetProcedureResultClassName(method);
                GenerateMethodSerializers(method, argsClassName, resultClassName);

                if (i < methods.Length - 1)
                    NewLine();
            }

            GenerateDelegateClasses();

            WriteCloseBraceIndent();
            WriteCloseBraceIndent();
        }

        private void GenerateMethodSerializers(MethodInfo method, string argsClassName, string resultClassName)
        {
            
            if (HasArgsSerializer(method))
            {
                var argsParameters = ConvertToParametersList(method);
                GenerateDataObjectClass(argsClassName, argsParameters, null);
            }

            if (!HasResultSerializer(method))
                return;

            var retType = GetMethodReturnType(method);
            var resultParams = new List<Parameter>();
            if(retType != typeof (void))
                resultParams.Add(new Parameter {Name = "success", ParameterType = retType});

            List<ExceptionInfo> exceptionsInfos;
            var throwsException = Metadata.TryGetThrows(method, out exceptionsInfos);
            if (throwsException != null)
            {
                 foreach (var exceptionsInfo in exceptionsInfos)
                {
                    var exceptionName = "Exception" + exceptionsInfo.Id;
                    resultParams.Add(new Parameter {Name = exceptionName, ParameterType = exceptionsInfo.Type});
                }
            }

            NewLine();
            GenerateDataObjectClass(resultClassName, resultParams, null);
        }

        private bool HasArgsSerializer(MethodInfo method)
        {
            return method.GetParameters().Length > 0;
        }

        private bool HasResultSerializer(MethodInfo method)
        {
            return GetMethodReturnType(method) != typeof (void) || Metadata.HasThrows(method);
        }


        #region Service Proxy

        private void GenerateServiceProxy(Type type)
        {
            Write("public class Proxy : RPCProxy, {0}", PrettyName(type));
            WriteStartBraceIndent();

            // Generate client constructors
            WriteLine("public Proxy(RPCPeer peer, RPCStubId implId, RPCStubId proxyId)");
            WriteLine("    : base(peer, implId, proxyId)");
            WriteStartBraceIndent();
            WriteCloseBraceIndent();
            NewLine();

            foreach (var method in type.GetMethods())
            {
                if (IsEventMethod(type, method))
                    continue;

                var procedureId = GetProcedureCallId(method);

                GenerateProtocolMethod(method
                    , procedureId
                    , method.Name
                    , GetProcedureArgsClassName(method)
                    , GetProcedureResultClassName(method)
                    , "this"
                    , "RPCData.Call"
                    , PrettyName(type));
            }

            foreach (var @event in type.GetEvents())
            {
                GenerateProxyEvent(type, @event);
            }

            WriteCloseBraceIndent();
            NewLine();
        }

        private void GenerateProtocolMethod(MethodInfo method, int? methodId, string methodName, string argsClassName, string resultClassName, string stubName, string callType, string methodClassName = null)
        {
            var methodPrefix = "";
            if (!string.IsNullOrEmpty(methodClassName))
                methodPrefix = methodClassName+".";

            if (!IsTask(method.ReturnType) && method.ReturnType != typeof(void))
                throw new Exception();

            Write((IsTask(method.ReturnType))? "async " : "public ");
            Write("{0} {1}{2}(", PrettyName(method.ReturnType), methodPrefix, methodName);

            var parameters = method.GetParameters();
            GenerateParameterList(parameters);

            WriteLine(")");

            WriteStartBraceIndent();

            var methodIdText = (methodId.HasValue) ? methodId.Value.ToString() : "";
            var sendMethodName = methodName + "Send" + methodIdText;
            var receiveMethodName = methodName + "Receive" + methodIdText;

            Write("var call = {0}(", sendMethodName);
            for (var i = 0; i < parameters.Length; i++)
            {
                if (i > 0)
                    Write(", ");
                Write("{0}", parameters[i].Name);
            }
            WriteLine(");");

            if(IsTask(method.ReturnType))
            {
                WriteLine("var response = await DispatchCall(call);");
                if (method.ReturnType != typeof(Task))
                    Write("return ");
                WriteLine("{0}(response);", receiveMethodName);
            }
            else
            {
                WriteLine("DispatchCall(call);");
            }

            WriteCloseBraceIndent();
            NewLine();

            // Generate send method
            GenerateProtocolSend(method, parameters, methodIdText, sendMethodName, argsClassName, stubName, callType);

            // Generate receive method
            GenerateProtocolReceive(method, receiveMethodName, resultClassName, stubName);
        }

        private void GenerateProtocolReceive(MethodInfo method, string methodName, string resultClassName, string stubName)
        {
            var retType = GetMethodReturnType(method);
            Write("private {0} {1}(RPCData response)", PrettyName(retType), methodName);
            WriteStartBraceIndent();

            WriteLine("if (response.Header.CallType == RPCDataType.Exception)");
            WriteStartBraceIndent();
            WriteLine("var x = RPCException.Read(response.Serializer);");
            WriteLine("throw x;");
            WriteCloseBraceIndent();

            if (!HasResultSerializer(method))
            {
                WriteCloseBraceIndent();
                return;
            }

            WriteLine("var result = new {0}({1});", resultClassName, stubName);
            WriteLine("result.Read(response);");

            List<ExceptionInfo> exceptionsInfo;
            if (Metadata.TryGetThrows(method, out exceptionsInfo))
            { 
                foreach (var exception in exceptionsInfo)
                {
                    WriteLine("if (result.Exception{0} != null)", exception.Id);
                    PushIndent();
                    WriteLine("throw result.Exception{0};", exception.Id);
                    PopIndent();
                }
            }

            if (retType != typeof(void))
                WriteLineIndent("return result.Success;");

            WriteCloseBraceIndent();
        }

        private void GenerateProtocolSend(MethodInfo method, ParameterInfo[] parameters, string methodId, string methodName, string argsClassName, string stubName, string callType)
        {
            Write("private {0} {1}(", callType, methodName);
            GenerateParameterList(parameters);
            WriteLine(")");
            WriteStartBraceIndent();

            WriteLine("var call = CreateCall({0});", methodId);

            var flags = GetRPCDataFlags(method);
            if (flags.Count > 0)
            {
                NewLine();
                WriteLine("call.Data.Flags = {0};", string.Join(" | ", flags));
            }  

            NewLine();
            if (HasArgsSerializer(method))
            {
                WriteLine("var args = new {0}({1});", argsClassName, stubName);
                foreach (var param in method.GetParameters())
                    WriteLine("args.{0} = {1};", ToTitleCase(param.Name), param.Name);
                WriteLine("args.Write(call.Data);");
            }
            WriteLine("return call;");
            WriteCloseBraceIndent();
            NewLine();
        }

        private void GenerateProxyEvent(Type type, EventInfo eventInfo)
        {
            var eventType = PrettyName(eventInfo.EventHandlerType);
            var eventName = eventInfo.Name;
            var eventName2 = "_" + eventName;

            var @delegate = eventInfo.EventHandlerType;
            var eventId = GetEventId(eventInfo);

            WriteLine("private event {0} {1};", eventType, eventName2);
            WriteLine("public event {0} {1}", eventType, eventName);
            WriteStartBraceIndent();
            WriteLine("add");
            WriteStartBraceIndent();
            WriteLine("if({0} == null)", eventName2);
            WriteStartBraceIndent();
            WriteLine("Subscribe<{0}>({1}, value);", GetDelegateImplClassName(@delegate), eventId);
            WriteCloseBraceIndent();
            WriteLine("{0} += value;", eventName2);
            WriteCloseBraceIndent();
            WriteLine("remove");
            WriteStartBraceIndent();
            WriteLine("{0} -= value;", eventName2);
            WriteCloseBraceIndent();
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
                WriteLine("public Impl({0} iface, RPCStubId id)", PrettyName(type));
                WriteLine(" : base(iface, id)");
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
            WriteLine("public async Task {0}(RPCData.Call call)",
                      GetProcedureProcessMethodName(method));
            WriteStartBraceIndent();

            if (HasArgsSerializer(method))
            {
                WriteLine("var args = new {0}(this);", GetProcedureArgsClassName(method));
                WriteLine("args.Read(call.Data);");
            }

            // If the method throws exceptions, we need to call it inside a try-catch.
            if (Metadata.HasThrows(method))
            {
                WriteLine("try {");
                PushIndent();
            }

            if (HasResultSerializer(method))
            {
                WriteLine("var result = new {0}(this);", GetProcedureResultClassName(method));
                var retType = GetMethodReturnType(method);
                if (retType != typeof(void))
                    Write("result.Success = ");
            }

            // Call the service method
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

            WriteLine("var reply = new RPCData.Reply(call);");
            if (HasResultSerializer(method))
                WriteLine("result.Write(reply.Data);");
            WriteLine("reply.Data.Dispatch();");

            WriteCloseBraceIndent();
        }

        private void GenerateEventSubscribeMethod(EventInfo @event)
        {
            WriteLine("void {0}(RPCPeer peer, RPCStubId remoteId, int remoteDelegateId)", GetEventSubscribeMethodName(@event));
            WriteStartBraceIndent();

            var @delegate = @event.EventHandlerType;
            WriteLine("var del = CreateDelegateProxy<{0}>(peer, remoteId, remoteDelegateId);", GetDelegateProxyClassName(@delegate));
            WriteLine("iface_.{0} += ({1}) del.Delegate;", @event.Name, PrettyName(@delegate));
            WriteCloseBraceIndent();
        }

        private void GenerateEventUnsubscribeMethod(EventInfo @event)
        {
            WriteLine("void {0}(RPCPeer peer, RPCStubId remoteId, int remoteDelegateId)", GetEventUnsubscribeMethodName(@event));
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

        private static bool CheckParametersIds(MethodInfo method, out bool hasIds)
        {
            var @params = method.GetParameters();
            if (@params.Length == 0)
            {
                hasIds = false;
                return false;
            }  

            hasIds = Metadata.HasId(@params[0]);

            for (var i = 1; i < @params.Length; i++)
            {
                int Id = i;
                if (Metadata.HasId(@params[i]) != hasIds)
                {
                    var msg = string.Format("Method {0}.{1} need to have all or none of his parameter with the Id attribute", 
                        method.DeclaringType.FullName, method.Name);
                    throw new Exception(msg);
                }
            }

            return true;
        }

        private static List<Parameter> ConvertToParametersList(MethodInfo method)
        {
             // Convert from PropertyInfo to our own struct. This makes it easier
            // to use the same code for argument and result marshaling.
            var parameters = new List<Parameter>();

            bool hasIds;
            if(!CheckParametersIds(method, out hasIds))
                return parameters;

            var @params = method.GetParameters();

            for (var i = 0; i < @params.Length; i++)
            {
                var param = @params[i];

                int id = i;
                if (hasIds)
                    Metadata.TryGetId(param, out id);

                parameters.Add(new Parameter(param.Name, param.ParameterType, id));
            }
                
            return parameters;
        }
        
        #endregion

        #region Service Method Result

        private static string GetProcedureResultClassName(MethodInfo method)
        {
            var procedureId = GetProcedureCallId(method);
            return string.Format("{0}_{1}_result", method.Name, procedureId);
        }

        #endregion

        #region Delegates

        private static bool IsDelegate(Type type)
        {
            return type.IsSubclassOf(typeof(Delegate));
        }

        private string GetDelegateName(Type type, bool doCreate = true)
        {
            if (!IsDelegate(type))
                throw new ArgumentException("Type " + type + " is not a delegate");

            string name;
            if (!delegateNames.TryGetValue(type, out name))
            {
                if (!doCreate)
                    throw new Exception("Unknowed delegate name for " + type.FullName);

                name = "Delegate" + delegateCounter++;
                delegateNames.Add(type, name);
            }

            return name;
        }

        private string GetDelegateImplClassName(Type type, bool doCreate = true)
        {
            return GetDelegateName(type, doCreate) + "Impl";
        }

        private string GetDelegateProxyClassName(Type type, bool doCreate = true)
        {
            return GetDelegateName(type, doCreate) + "Proxy";
        }

        private void GenerateDelegateClasses()
        {
            foreach(var type in delegateNames.Keys)
            {
                GenerateDelegateClass(type);
            }
        }

        private void GenerateDelegateClass(Type type)
        {

            var methodInvoke = type.GetMethod("Invoke");
            if(methodInvoke == null)
                throw new Exception();

            var stubName = "Stub";
            var proxyName = GetDelegateProxyClassName(type, false);
            var implName = GetDelegateImplClassName(type, false);
            var argsName = GetDelegateName(type, false) + "_args";
            var resultName = GetDelegateName(type, false) + "_result";

            WriteLine("public class {0} : RPCDelegateProxy", proxyName);
            WriteStartBraceIndent();
            WriteLine("public {0}()", proxyName);
            WriteStartBraceIndent();
            WriteLine("SetDelegate(new {0}(Invoke));", PrettyName(type));
            WriteCloseBraceIndent();
            GenerateProtocolMethod(methodInvoke, null, "Invoke", argsName, resultName, stubName, "RPCData.DelegateCall");
            WriteCloseBraceIndent();
            NewLine();

            WriteLine("public class {0} : RPCDelegateImpl", implName);
            WriteStartBraceIndent();
            WriteLine("public {0}()", implName);
            WriteStartBraceIndent();
            WriteCloseBraceIndent();
            NewLine();
            GenerateDelegateProxyInvoke(methodInvoke, argsName, resultName, stubName);
            WriteCloseBraceIndent();
            NewLine();

            GenerateMethodSerializers(methodInvoke, argsName, resultName);
        }

        private void GenerateDelegateProxyInvoke(MethodInfo methodInvoke, string argsName, string resultName, string stubName)
        {
            //Invoke
            WriteLine("public override void Invoke(RPCData.DelegateCall call)");
            WriteStartBraceIndent();

            if (HasArgsSerializer(methodInvoke))
            {
                WriteLine("var args = new {0}({1});", argsName, stubName);
                WriteLine("args.Read(call.Data);");
            }

            var returnType = methodInvoke.ReturnType;

            if (IsTask(methodInvoke.ReturnType))
            {
                if (returnType != typeof(Task))
                    Write("var task = ({0})", PrettyName(returnType));

                Write("Delegate.DynamicInvoke(new object[]{");
                var parameters = methodInvoke.GetParameters();
                for (var i = 0; i < parameters.Length; i++)
                {
                    var param = parameters[i];
                    Write("args.{0}", ToTitleCase(param.Name));

                    if (i < parameters.Length - 1)
                        Write(", ");
                }
                WriteLine("});");

                if (returnType != typeof(Task))
                {
                    WriteLine("task.ContinueWith( t =>");
                    WriteStartBraceIndent();
                    WriteLine("var result = new {0}({1});", resultName, stubName);
                    WriteLine("result.Success = t.Result;");
                    WriteLine("var reply = new RPCData.DelegateReply(call);");
                    WriteLine("result.Write(reply.Data);");
                    WriteLine("reply.Data.Dispatch();");
                    PopIndent();
                    WriteLine("});");
                }

            }
            else
            {
                if (HasResultSerializer(methodInvoke))
                {
                    WriteLine("var result = new {0}({1});", resultName, stubName);
                    if (returnType != typeof(void))
                        Write("result.Success = ({0})", PrettyName(returnType));
                }

                Write("Delegate.DynamicInvoke(new object[]{");
                var parameters = methodInvoke.GetParameters();
                for (var i = 0; i < parameters.Length; i++)
                {
                    var param = parameters[i];
                    Write("args.{0}", ToTitleCase(param.Name));

                    if (i < parameters.Length - 1)
                        Write(", ");
                }
                WriteLine("});");

                if (returnType != typeof(void))
                {
                    WriteLine("var reply = new RPCData.Reply(call);");
                    WriteLine("result.Write(reply.Data);");
                    WriteLine("reply.Data.Dispatch();");
                }
            }
            WriteCloseBraceIndent();
        }

        #endregion

        #region General Auxiliaries

        public static bool IsTask(Type type)
        {
            return type.IsAssignableFrom(typeof(Task)) 
                || IsInstanceOfGenericType(typeof(Task<>), type);
        }

        public static Type GetMethodReturnType(MethodInfo method)
        {
            if (method.ReturnType == typeof (Task))
                return typeof(void);

            if (IsInstanceOfGenericType(typeof (Task<>), method.ReturnType))
                return method.ReturnType.GenericTypeArguments[0];

            return method.ReturnType;
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
            var argsCount = method.GetParameters().Length;
            if (argsCount > 1)
                return null;
            if (argsCount == 1)
                return method.DeclaringType.GetProperties().FirstOrDefault(prop => prop.GetSetMethod() == method);

            return method.DeclaringType.GetProperties().FirstOrDefault(prop => prop.GetGetMethod() == method);
        }

        private IEnumerable<Parameter> ConvertToActualParameters(IEnumerable<Parameter> parameters)
        {
            var actualParams = new List<Parameter>();
            foreach (var param in parameters)
            {
                if (param.ParameterType == typeof(void))
                    continue;
                actualParams.Add(param);
            }
            return actualParams;
        }

        private static bool IsNullableType(Type type)
        {
            switch (ConvertFromTypeToThrift(type))
            {
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
        
        private void GenerateServiceMethodWrite(IEnumerable<Parameter> parameters, string stubName)
        {
            parameters = ConvertToActualParameters(parameters);

            WriteLine("public void Write(RPCData data)");
            WriteStartBraceIndent();
            WriteLine("Write(data, new []{{{0}}});", String.Join(", ", parameters.Select(p => p.Id)));
            WriteCloseBraceIndent();

            WriteLine("public void Write(RPCData data, int[] propertyIds)");
            WriteStartBraceIndent();
            WriteLine("data.Serializer.WriteI32(propertyIds.Length);");
            WriteLine("foreach(var propertyId in propertyIds)");
            WriteStartBraceIndent();
            WriteLine("data.Serializer.WriteI32(propertyId);");
            WriteLine("switch (propertyId)");
            WriteStartBraceIndent();
            foreach (var param in parameters)
            {
                WriteLine("case {0}:", param.Id);
                PushIndent();
                GenerateTypeSerialization(param.ParameterType, ToTitleCase(param.Name), "data", stubName);
                WriteLine("break;");
                PopIndent();
            }
            WriteLine("default:");
            PushIndent();
            WriteLine("throw new Exception(\"Received unexpected property id.\");", "data");
            PopIndent();
            WriteCloseBraceIndent();
            WriteCloseBraceIndent();
            WriteCloseBraceIndent();
        }

        private void GenerateServiceMethodRead(IEnumerable<Parameter> parameters, string stubName)
        {
            WriteLine("public void Read(RPCData data)");
            WriteStartBraceIndent();

            WriteLine("var propCount = data.Serializer.ReadI32();");
            WriteLine("for (var i = 0; i<propCount; i++)");
            WriteStartBraceIndent();
            WriteLine("var propertyId = data.Serializer.ReadI32();");

            WriteLine("switch (propertyId)");
            WriteStartBraceIndent();
            foreach (var param in parameters)
            {
                WriteLine("case {0}:", param.Id);
                PushIndent();
                GenerateTypeDeserialization(param.ParameterType, ToTitleCase(param.Name), "data", stubName);
                WriteLine("break;");
                PopIndent();
            }
            WriteLine("default:");
            PushIndent();
            WriteLine("throw new Exception(\"Received unexpected property id.\");", "data");
            PopIndent();
            WriteCloseBraceIndent();

            WriteCloseBraceIndent();
            WriteCloseBraceIndent();
        }

        #endregion

        #region Serialization

        /// <summary>
        /// Generates the code to serialize a type 
        /// </summary>
        private void GenerateTypeSerialization(Type type, string varName, string dataName, string stubName)
        {
            var thriftType = ConvertFromTypeToThrift(type);

            switch (thriftType)
            {
                case TType.List:
                    GenerateListSerialize(type, varName, dataName, stubName);
                    break;
                case TType.Map:
                    GenerateMapSerialize(type, varName, dataName, stubName);
                    break;
                case TType.DataObject:
                case TType.Exception:
                    GenerateStructSerialize(type, varName, dataName, stubName);
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
                    WriteLine("{0}.Serializer.Write{1}({2}{3});", dataName, thriftType.ToString(), enumCast, varName);
                    break;
                case TType.Guid:
                    WriteLine("{0}.Serializer.WriteString({1}.ToString());", dataName, varName);
                    break;
                case TType.DateTime:
                    WriteLine("{0}.Serializer.WriteI64({1}.Ticks);", dataName, varName);
                    break;
                case TType.Delegate:
                    WriteLine("var del = {0}.CreateDelegateImpl<{1}>({2}.Peer, {2}.Header.RemoteId, {3});", stubName, GetDelegateImplClassName(type), dataName, varName);
                    WriteLine("{0}.Serializer.WriteI32(del.Id);", dataName);
                    // TODO: Serialize Peer and RemoteId so we can create delegate proxies to other peer's delegates.
                    break;
                case TType.Service:
                    WriteLine("var serviceImpl = {0}.RPCManager.GetCreateImplementation<{1}>({2});", stubName, PrettyName(type), varName);
                    WriteLine("{0}.Serializer.WriteI32(serviceImpl.Id.Id);", dataName, varName);
                    // TODO: Serialize Peer so we can create proxies to other peer's services.
                    break;
                case TType.Void:
                default:
                    System.Diagnostics.Debugger.Break();
                    throw new NotImplementedException();
            }

        }

        /// <summary>
        /// Generates the code to serialize a field of type struct
        /// </summary>
        private void GenerateStructSerialize(Type type, string varName, string dataName, string stubName)
        {
            var implObjName = varName + "Impl";
            implObjName = implObjName.Replace(".", "");

            WriteLine("var {0} = new {1}({2});", implObjName, GetStubsClassName(type, true), stubName);
            GenerateStructInit(type, varName, implObjName);
            WriteLine("{0}.Write({1});", implObjName, dataName);
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
        private void GenerateListSerialize(Type type, string name, string dataName, string stubName)
        {
            var listElemType = (type.IsArray)? type.GetElementType() : type.GetGenericArguments()[0];

             var countName = (type.IsArray)? "Length" : "Count";

            WriteLine("{0}.Serializer.WriteListBegin(new TList(TType.{1}, {2}.{3}));",
                      dataName,
                      ConvertFromTypeToThrift(type).ToString(),
                      ToTitleCase(name),
                      countName);

            var iterName = string.Format("_iter{0}", GenericIndex++);
            WriteLine("foreach (var {0} in {1})", iterName, ToTitleCase(name));
            WriteStartBraceIndent();

            GenerateTypeSerialization(listElemType, iterName, dataName, stubName);

            WriteCloseBraceIndent();

            WriteLine("{0}.Serializer.WriteListEnd();", dataName);
        }

        /// <summary>
        /// Generates the code to serialize a map  
        /// </summary>
        private void GenerateMapSerialize(Type type, string name, string dataName, string stubName)
        {
            var mapElemType1 = type.GetGenericArguments()[0];
            var mapElemType2 = type.GetGenericArguments()[1];

            WriteLine("{0}.Serializer.WriteMapBegin(new TMap(TType.{1}, TType.{2}, {3}.Count));",
                      dataName,
                      ConvertFromTypeToThrift(mapElemType1).ToString(),
                      ConvertFromTypeToThrift(mapElemType2).ToString(),
                      ToTitleCase(name));

            var iterName = string.Format("_iter{0}", GenericIndex++);
            WriteLine("foreach (var {0} in {1})", iterName, ToTitleCase(name));
            WriteStartBraceIndent();

            GenerateContainerDoubleElementSerialization(mapElemType1, mapElemType2, iterName, dataName, stubName);

            WriteCloseBraceIndent();

            WriteLine("{0}.Serializer.WriteMapEnd();", dataName);

        }

        /// <summary>
        /// Generates the code to serialize a dual element container  
        /// </summary>
        private void GenerateContainerDoubleElementSerialization(Type elemType1, Type elemType2, string iterName, string dataName, string stubName)
        {
            GenerateTypeSerialization(elemType1, iterName + ".Key", dataName, stubName);
            GenerateTypeSerialization(elemType2, iterName + ".Value", dataName, stubName);
        }

        #endregion

        #region Deserialization

        /// <summary>
        /// Generates the code to deserialize a single element of an array 
        /// </summary>
        private void GenerateTypeDeserialization(Type type, string varName, string dataName, string stubName, bool varExists = true)
        {
            var thriftType = ConvertFromTypeToThrift(type);
            switch (thriftType)
            {
                case TType.List:
                    GenerateListDeserialize(type, varName, dataName, stubName);
                    break;
                case TType.Map:
                    GenerateMapDeserialize(type, varName, dataName, stubName);
                    break;
                case TType.DataObject:
                case TType.Exception:
                    GenerateStructDeserialize(type, varName, dataName, stubName, varExists);
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
                    WriteLine("{0} = {1}{2}.Serializer.Read{3}();", varName, cast, dataName, thriftType.ToString());
                    break;
                case TType.Guid:
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = new System.Guid({1}.Serializer.ReadString());", varName, dataName);
                    break;
                case TType.DateTime:
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = new System.DateTime({1}.Serializer.ReadI64());", varName, dataName);
                    break;
                case TType.Delegate:
                    WriteLine("var remoteDelegateId = {0}.Serializer.ReadI32();", dataName);
                    WriteLine("var del = {0}.CreateDelegateProxy<{1}>({2}.Peer, {2}.Header.RemoteId, remoteDelegateId);", stubName, GetDelegateProxyClassName(type), dataName);
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = ({1})del.Delegate;", varName, PrettyName(type));
                    break;
                case TType.Service:
                    WriteLine("var remoteId = {0}.Serializer.ReadI32();", dataName);
                    WriteLine("{0} = {1}.RPCManager.GetService<{2}>({3}.Peer, new RPCStubId(remoteId));", varName, stubName, PrettyName(type), dataName);
                    break;
                case TType.Void:
                default:
                    throw new NotImplementedException();
            }

        }

        /// <summary>
        /// Generates the code to deserialize a field of type struct
        /// </summary>
        private void GenerateStructDeserialize(Type type, string varName, string dataName, string stubName, bool varExists)
        {
            var origObjName = varName+"Impl";
            WriteLine("var {0} = new {1}({2});", origObjName, GetStubsClassName(type, true), stubName);
            WriteLine("{0}.Read({1});", origObjName, dataName);

            var elemName2 = string.Format("_elem{0}", GenericIndex++);
            WriteLine("var {0} = new {1}();", elemName2, PrettyName(type));
            GenerateStructInit(type, origObjName, elemName2, true);
            if (!varExists)
                Write("var ");
            WriteLine("{0} = {1};", varName, elemName2);
        }

        /// <summary>
        /// Generates the code to deserialize a list  
        /// </summary>
        private void GenerateListDeserialize(Type type, string name, string dataName, string stubName)
        {
            var listElemType = (type.IsArray)? type.GetElementType() : type.GetGenericArguments()[0];

            var listName = string.Format("_list{0}", GenericIndex++);
            WriteLine("var {0} = {1}.Serializer.ReadListBegin();", listName, dataName);

            if (type.IsArray)
            {
                WriteLine("{0} = new {1}[{2}.Count];", ToTitleCase(name), PrettyName(listElemType), listName);
            }
            else
            {
                WriteLine("{0} = new {1}();", ToTitleCase(name), PrettyName(type));
            }

            var iterName = string.Format("_i{0}", GenericIndex++);
            WriteLine("for (var {0} = 0; {0} < {1}.Count; ++{0})", iterName, listName);

            WriteStartBraceIndent();

            var elemName = string.Format("_elem{0}", GenericIndex++);

            GenerateTypeDeserialization(listElemType, elemName, dataName, stubName, false);

            if (type.IsArray)
            {
                WriteLine("{0}[{1}] = {2};", name, iterName, elemName);
            }
            else
            {
                WriteLine("{0}.Add({1});", name, elemName);
            }

            WriteCloseBraceIndent();
            WriteLine("{0}.Serializer.ReadListEnd();", dataName);
        }

        /// <summary>
        /// Generates the code to deserialize a map  
        /// </summary>
        private void GenerateMapDeserialize(Type type, string name, string dataName, string stubName)
        {
            var mapElemType1 = type.GetGenericArguments()[0];
            var mapElemType2 = type.GetGenericArguments()[1];

            WriteLine("{0} = new {1}();", ToTitleCase(name),
                      PrettyName(type));

            var mapName = string.Format("_set{0}", GenericIndex++);
            WriteLine("var {0} = {1}.Serializer.ReadMapBegin();", mapName, dataName);

            var iterName = string.Format("_i{0}", GenericIndex++);
            WriteLine("for (var {0} = 0; {0} < {1}.Count; ++{0})",
                      iterName, mapName);

            WriteStartBraceIndent();

            GenerateContainerDoubleElementDeserialization(mapElemType1, mapElemType2, ToTitleCase(name), dataName, stubName);

            WriteCloseBraceIndent();
            WriteLine("{0}.Serializer.ReadMapEnd();", dataName);
        }

        /// <summary>
        /// Generates the code to deserialize a double element container  
        /// </summary>
        private void GenerateContainerDoubleElementDeserialization(Type elemType1, Type elemType2, string containerName, string dataName, string stubName)
        {
            var elemName1 = string.Format("_elem{0}", GenericIndex++);
            var elemName2 = string.Format("_elem{0}", GenericIndex++);

            GenerateTypeDeserialization(elemType1, elemName1, dataName, stubName, false);
            GenerateTypeDeserialization(elemType2, elemName2, dataName, stubName, false);

            WriteLine("{0}.Add({1}, {2});", containerName, elemName1, elemName2);
        }

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
            else if (IsInstanceOfGenericType(typeof(ICollection<>), type) || type.IsArray)
                return TType.List;
            else if (IsDelegate(type))
                return TType.Delegate;
            else if (Metadata.IsService(type))
                return TType.Service;

            throw new NotImplementedException("Unhandle type "+type);
        }

        #endregion

        #region Data Types

        private struct Parameter
        {
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
