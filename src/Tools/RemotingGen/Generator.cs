﻿using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using Flood.Serialization;
using Weaver;
using Weaver.Util;

[assembly: InternalsVisibleToAttribute("RemotingGen.Tests.GeneratorTests")]
namespace RemotingGen
{
    internal class Generator : TextGenerator
    {
        private Assembly currentAssembly;


        private GenerationContext GenerationContext;
 
        public Generator(Assembly currentAssembly, GenerationContext generationContext)
        {

            GenerationContext = generationContext;

            this.currentAssembly = currentAssembly;
        }

#region Generate Data Objects

        public void GenerateDataObject(Type type)
        {
            GenerateUsings();
            var className = GetStubsClassName(type, false);
            var parameters = ConvertFieldToParametersList(type);
            GenerateDataObjectClass(className, parameters, type.Namespace, false, type);

            GenerationContext.DataObjects.Add(type);
            GenerationContext.DataObjectsMap.Add(new TypeSignature(type.Namespace, className), new TypeSignature(type));
        }

        private void GenerateDataObjectClass(string className, List<Parameter> parameters, string @namespace, bool isValueType, Type dataObjecDataType = null)
        {
            if (@namespace != null)
            {
                WriteLine("namespace  {0}", @namespace);
                WriteStartBraceIndent();
            }

            var structclass = (isValueType) ? "struct" : "class";

            var baseDataObjects = GetBaseDataObjects(dataObjecDataType);
            var baseDataObjectCount = (baseDataObjects == null) ? 0 : baseDataObjects.Count;
            var baseDataObject = (baseDataObjectCount == 0) ? null : baseDataObjects[0];

            if (baseDataObject != null)
            {
                WriteLine("public {0} {1} : {2}", structclass, className, GetStubsClassName(baseDataObject, true));
            }
            else
            {
                var @interface = (isValueType) ? "IDataObject" : "IObservableDataObject";
                WriteLine("public {0} {1} : {2}", structclass, className, @interface);
            }

            WriteStartBraceIndent();

            // Generate fields
            if (!isValueType)
            {
                WriteLine("public class Reference : {0}, IDataObjectReference", className);
                WriteStartBraceIndent();
                WriteLine("public RemotingPeer Peer { get; private set; }");
                WriteLine("public int RemoteId { get; private set; }");
                WriteLine("public ReferenceManager ReferenceManager { get; private set; }");
                NewLine();
                WriteLine("public Reference(RemotingPeer peer, int remoteId, ReferenceManager referenceManager)");
                WriteStartBraceIndent();
                WriteLine("Peer = peer;");
                WriteLine("RemoteId = remoteId;");
                WriteLine("ReferenceManager = referenceManager;");
                WriteCloseBraceIndent();
                WriteCloseBraceIndent();
                NewLine();
                if (baseDataObject == null)
                {
                    WriteLine("private event Action<Type, int> propertyChanged;");
                    WriteLine("event Action<Type,int> IObservableDataObject.PropertyChanged");
                    WriteStartBraceIndent();
                    WriteLine("add { propertyChanged += value; }");
                    WriteLine("remove { propertyChanged -= value; }");
                    WriteCloseBraceIndent();
                    NewLine();
                    WriteLine("protected void OnPropertyChanged(Type type, int propertyId)");
                    WriteStartBraceIndent();
                    WriteLine("if(propertyChanged != null)");
                    WriteLineIndent("propertyChanged(type, propertyId);");
                    WriteCloseBraceIndent();
                    NewLine();
                }
                WriteLine("private BitField changedProperties;");
                NewLine();
                foreach (var param in parameters)
                {
                    var backingFieldName = "__" + param.Name;
                    var paramTypeName = GetTypeName(param.ParameterType);

                    if (param.IsImplementedProperty)
                    {
                        WriteLine("private {0} {1} {{ get; set; }}", paramTypeName, backingFieldName);

                        var typeSig = new TypeSignature(param.ParameterType);
                        var declaringTypeSig = new TypeSignature(dataObjecDataType);

                        GenerationContext.MemberClones.Add(new MemberClone 
                        {
                            OriginMember = new PropertySignature(typeSig, declaringTypeSig, param.Name), 
                            Name = backingFieldName,
                            AccessModifier = AccessModifiers.Private,
                            NoCustomAttributes = true
                        });

                        var setParams = new List<ParameterSignature> {new ParameterSignature(typeSig, "value")};
                        var getMethodSig = new MethodSignature(typeSig, declaringTypeSig, "get_"+backingFieldName, new List<ParameterSignature>());
                        var setMethodSig = new MethodSignature(new TypeSignature(typeof(void)), declaringTypeSig, "set_"+backingFieldName, setParams);

                        GenerationContext.MemberOptions.Add(getMethodSig, Weaver.MemberOptions.UseOriginInstructions);
                        GenerationContext.MemberOptions.Add(setMethodSig, Weaver.MemberOptions.UseOriginInstructions);
                    }
                    else
                    {
                        WriteLine("private {0} {1};", paramTypeName, backingFieldName);
                    }

                    WriteLine("public {0} {1}", paramTypeName, ToTitleCase(param.Name));
                    WriteStartBraceIndent();
                    WriteLine("get {{ return {0}; }}", backingFieldName);
                    WriteLine("set");
                    WriteStartBraceIndent();
                    WriteLine("if( value == {0})", backingFieldName);
                    WriteLineIndent("return;");
                    NewLine();
                    WriteLine("{0} = value;", backingFieldName);
                    NewLine();
                    WriteLine("changedProperties.SetBit({0});", param.Id);
                    NewLine();
                    WriteLine("OnPropertyChanged(typeof({0}), {1});", className, param.Id);
                    WriteCloseBraceIndent();
                    WriteCloseBraceIndent();
                    NewLine();
                }

                WriteLine("public bool IsReference{ get; set; }");
                NewLine();

                WriteLine("public unsafe void GetResetChanges(BitField* bitFields)");
                WriteStartBraceIndent();
                WriteLine("bitFields[0] = changedProperties;");
                WriteLine("changedProperties.Reset();");
                if (baseDataObject != null)
                {
                    NewLine();
                    WriteLine("(({0}) (object) this).GetResetChanges(bitFields++);", GetStubsClassName(baseDataObject, true));
                }
                WriteCloseBraceIndent();
                NewLine();
            }
            else
            {
                foreach (var param in parameters)
                {
                    var paramTypeName = GetTypeName(param.ParameterType);

                    WriteLine("public {0} {1};", paramTypeName, ToTitleCase(param.Name));

                    if(IsDelegate(param.ParameterType))
                        GenerationContext.Delegates.Add(param.ParameterType);

                    NewLine();
                }
            }

            WriteLine("public int BaseDataObjectCount {{ get {{ return {0}; }} }}", baseDataObjectCount);
            NewLine();

            // Generate write methods
            GenerateDataObjectWriteBitField(parameters, baseDataObject);
            GenerateDataObjectWrite(className, baseDataObjectCount > 0, isValueType);
            NewLine();

            // Generate read method
            GenerateDataObjectRead(className,  baseDataObjectCount > 0, isValueType, parameters);

            WriteCloseBraceIndent();
            if (@namespace != null)
                WriteCloseBraceIndent();
        }

        private void GenerateDataObjectWriteBitField(IEnumerable<Parameter> parameters, Type baseType)
        {
            WriteLine("public unsafe void Write(Serializer data, BitField* properties, int bitFieldCount, object customData = null)");
            WriteStartBraceIndent();
            WriteLine("data.WriteI64(properties[0].Bits);");
            foreach (var param in parameters)
            {
                NewLine();
                WriteLine("if(properties[0].GetBit({0}))",param.Id);
                WriteStartBraceIndent();
                GenerateTypeSerialization(param.ParameterType, ToTitleCase(param.Name), "data");
                WriteCloseBraceIndent();
            }
            if (baseType != null)
            {
                NewLine();
                WriteLine("if(--bitFieldCount > 0)");
                WriteLineIndent("(({0}) (object) this).Write(data, properties++, bitFieldCount, customData);", GetStubsClassName(baseType, true));
            }
            WriteCloseBraceIndent();
        }

        private void GenerateDataObjectWrite(string className, bool hasBaseType, bool isValueType)
        {
            GenerateDataObjectOverride("Write", className, hasBaseType, isValueType, () =>
            {
                WriteLine("unsafe");
                WriteStartBraceIndent();
                WriteLine("var bitField = BitField.AllSet;");
                WriteLine("Write(data, &bitField, 1, customData);");
                WriteCloseBraceIndent();
            });

        }

        private void GenerateDataObjectRead(string className, bool hasBaseType, bool isValueType, IEnumerable<Parameter> parameters)
        {
            GenerateDataObjectOverride("Read", className, hasBaseType, isValueType, () =>
            {
                WriteLine("var properties = new BitField();");
                WriteLine("properties.Bits = data.ReadI64();");

                foreach (var param in parameters)
                {
                    NewLine();
                    WriteLine("if(properties.GetBit({0}))",param.Id);
                    WriteStartBraceIndent();
                    GenerateTypeDeserialization(param.ParameterType,ToTitleCase(param.Name),"data");
                    WriteCloseBraceIndent();
                }
            });
        }

        private void GenerateDataObjectOverride(string name, string className, bool hasBaseType, bool isValueType, Action printer)
        {
            var modifier = (isValueType)? "" : (hasBaseType) ? "override" : "virtual";

            WriteLine("public {0} void {1}(Serializer data, Type baseType = null, object customData = null)", modifier, name);
            WriteStartBraceIndent();

            if (!hasBaseType)
            {
                WriteLine("if(baseType != null && baseType != typeof({0}))", className);
                WriteLineIndent("throw new ArgumentException();");
                NewLine();
            }
            else
            {
               WriteLine("if(baseType == null || baseType == typeof({0}))", className);
               WriteStartBraceIndent();
            }

            printer.Invoke();

            if (hasBaseType)
            {
                NewLine();
                WriteLine("baseType = null;");
                WriteCloseBraceIndent();
                NewLine();
                WriteLine("base.{0}(data, baseType);", name);
            }
            WriteCloseBraceIndent();
        }

        private List<Parameter> ConvertFieldToParametersList(Type type)
        {
            var parameters = new List<Parameter>();
            foreach(var field in GetAllFields(type))
            {
                if (!Metadata.HasId(field) || field.DeclaringType != type)
                    continue;

                parameters.Add(new Parameter(field));
            }

            foreach (var property in GetAllProperties(type))
            {
                if (!Metadata.HasId(property) || property.DeclaringType != type)
                    continue;
                
                parameters.Add(new Parameter(property));
            }

            return parameters;
        }

        private List<Type> GetBaseDataObjects(Type type)
        {
            if (type == null)
                return null;

            var baseTypes = new List<Type>();

            while (type.BaseType != null)
            {
                type = type.BaseType;
                if(Metadata.IsDataObject(type))
                    baseTypes.Add(type);
            }

            return baseTypes;
        }

        public void GenerateDataObjectFactory()
        {
            GenerateUsings();

            WriteLine("public class DataObjectFactory : IDataObjectFactory");
            WriteStartBraceIndent();

            WriteLine("public IObservableDataObject CreateDataObjectReference(ushort id, RemotingPeer peer, int remoteId, ReferenceManager referenceManager)");
            WriteStartBraceIndent();
            WriteLine("switch(id)");
            WriteStartBraceIndent();
            foreach (var dataObject in GenerationContext.DataObjects)
            {
                ushort id;
                if(!Metadata.TryGetDataObjectId(dataObject, out id))
                    throw new Exception();

                WriteLine("case {0}:", id);
                WriteLineIndent("return new {0}.Reference(peer, remoteId, referenceManager);", GetStubsClassName(dataObject, true));
            }
            WriteLine("default:");
            WriteLineIndent("throw new NotImplementedException();");
            WriteCloseBraceIndent();
            WriteCloseBraceIndent();

            WriteLine("public RemotingDelegateImpl CreateDelegateImpl(Type delegateType)");
            WriteStartBraceIndent();
            foreach (var delegateType in GenerationContext.Delegates)
            {
                WriteLine("if(delegateType == typeof({0}))", GetTypeName(delegateType));
                WriteLineIndent("return new {0}();", GetDelegateImplClassName(delegateType, false));
            }
            WriteLine("throw new NotImplementedException();");
            WriteCloseBraceIndent();

            WriteLine("public RemotingDelegateProxy CreateDelegateProxy(Type delegateType)");
            WriteStartBraceIndent();
            foreach (var delegateType in GenerationContext.Delegates)
            {
                WriteLine("if(delegateType == typeof({0}))", GetTypeName(delegateType));
                WriteLineIndent("return new {0}();", GetDelegateProxyClassName(delegateType, false));
            }
            WriteLine("throw new NotImplementedException();");
            WriteCloseBraceIndent();

            WriteCloseBraceIndent();
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

            WriteCloseBraceIndent();
            WriteCloseBraceIndent();
        }

        private void GenerateMethodSerializers(MethodInfo method, string argsClassName, string resultClassName)
        {
            
            if (HasArgsSerializer(method))
            {
                var argsParameters = ConvertToParametersList(method);
                GenerateDataObjectClass(argsClassName, argsParameters, null, true);
            }

            if (!HasResultSerializer(method))
                return;

            var reDataType = GetMethodReturnType(method);
            var resultParams = new List<Parameter>();
            if(reDataType != typeof (void))
                resultParams.Add(new Parameter {Name = "success", ParameterType = reDataType});

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
            GenerateDataObjectClass(resultClassName, resultParams, null, true);
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
            WriteLine("public class Proxy : ServiceProxy, {0}", GetTypeName(type));
            WriteStartBraceIndent();

            // Generate client constructors
            WriteLine("public Proxy(RemotingPeer peer, int implId, int proxyId)");
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
                    , "Message.Call"
                    , GetTypeName(type));
            }

            foreach (var @event in type.GetEvents())
            {
                GenerateProxyEvent(type, @event);
            }

            WriteCloseBraceIndent();
            NewLine();
        }

        private void GenerateProtocolMethod(MethodInfo method, int? methodId, string methodName, string argsClassName, string resultClassName, string callType, string methodClassName = null)
        {
            var methodPrefix = "";
            if (!string.IsNullOrEmpty(methodClassName))
                methodPrefix = methodClassName+".";

            if (!IsTask(method.ReturnType) && method.ReturnType != typeof(void))
                throw new Exception("Method return type must be a Task");

            Write((IsTask(method.ReturnType))? "async " : "public ");
            Write("{0} {1}{2}(", GetTypeName(method.ReturnType, false), methodPrefix, methodName);

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
                    Write("return {0}", GetTypeCast(GetMethodReturnType(method)));
                WriteLine("{0}(response);", receiveMethodName);
            }
            else
            {
                WriteLine("DispatchCall(call);");
            }

            WriteCloseBraceIndent();
            NewLine();

            // Generate send method
            GenerateProtocolSend(method, parameters, methodIdText, sendMethodName, argsClassName, callType);

            // Generate receive method
            GenerateProtocolReceive(method, receiveMethodName, resultClassName);
        }

        private void GenerateProtocolReceive(MethodInfo method, string methodName, string resultClassName)
        {
            var reDataType = GetMethodReturnType(method);
            Write("private {0} {1}(Message response)", GetTypeName(reDataType), methodName);
            WriteStartBraceIndent();

            WriteLine("if (response.Header.CallType == MessageType.Exception)");
            WriteStartBraceIndent();
            WriteLine("var x = RemotingException.Read(response.Serializer);");
            WriteLine("throw x;");
            WriteCloseBraceIndent();

            if (!HasResultSerializer(method))
            {
                WriteCloseBraceIndent();
                return;
            }

            WriteLine("var result = new {0}();", resultClassName);
            WriteLine("result.Read(response.Serializer, null, response);");

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

            if (reDataType != typeof(void))
                WriteLine("return result.Success;");

            WriteCloseBraceIndent();
        }

        private void GenerateProtocolSend(MethodInfo method, ParameterInfo[] parameters, string methodId, string methodName, string argsClassName, string callType)
        {
            Write("private {0} {1}(", callType, methodName);
            GenerateParameterList(parameters);
            WriteLine(")");
            WriteStartBraceIndent();

            WriteLine("var call = CreateCall({0});", methodId);

            var flags = GetMessageFlags(method);
            if (flags.Count > 0)
            {
                NewLine();
                WriteLine("call.Data.Flags = {0};", string.Join(" | ", flags));
            }  

            NewLine();
            if (HasArgsSerializer(method))
            {
                WriteLine("var args = new {0}();", argsClassName);
                foreach (var param in method.GetParameters())
                    WriteLine("args.{0} = {1} {2};", ToTitleCase(param.Name), GetTypeCastToStub(param.ParameterType), param.Name);
                WriteLine("args.Write(call.Data.Serializer, null, call.Data);");
            }
            WriteLine("return call;");
            WriteCloseBraceIndent();
            NewLine();
        }

        private void GenerateProxyEvent(Type type, EventInfo eventInfo)
        {
            var evenDataType = GetTypeName(eventInfo.EventHandlerType);
            var eventName = eventInfo.Name;
            var eventName2 = "_" + eventName;

            var @delegate = eventInfo.EventHandlerType;
            var eventId = GetEventId(eventInfo);

            WriteLine("private event {0} {1};", evenDataType, eventName2);
            WriteLine("public event {0} {1}", evenDataType, eventName);
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

        private List<string> GetMessageFlags(MethodInfo method)
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
                Write("{0} {1}", GetTypeName(param.ParameterType, false),
                      param.Name);

                if (i < parameters.Count - 1)
                    Write(", ");
            }
        }

        #endregion

        #region Service Processor

        private void GenerateServiceProcessor(Type type)
        {
            WriteLine("public class Impl : ServiceImpl");
            WriteStartBraceIndent();

            WriteLine("private readonly {0} iface_;", GetTypeName(type));

            // Generate constructor
            {
                WriteLine("public Impl({0} iface, int id)", GetTypeName(type));
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
            WriteLine("public async Task {0}(Message.Call call)",
                      GetProcedureProcessMethodName(method));
            WriteStartBraceIndent();

            if (HasArgsSerializer(method))
            {
                WriteLine("var args = new {0}();", GetProcedureArgsClassName(method));
                WriteLine("args.Read(call.Data.Serializer, null, call.Data);");
            }

            // If the method throws exceptions, we need to call it inside a try-catch.
            if (Metadata.HasThrows(method))
            {
                WriteLine("try {");
                PushIndent();
            }

            if (HasResultSerializer(method))
            {
                WriteLine("var result = new {0}();", GetProcedureResultClassName(method));
                var reDataType = GetMethodReturnType(method);
                if (reDataType != typeof(void))
                    Write("result.Success = {0}", GetTypeCastToStub(reDataType));
            }

            // Call the service method
            Write("await iface_.{0}(", method.Name);
            var parameters = method.GetParameters();
            for (var i = 0; i < parameters.Length; i++)
            {

                var param = parameters[i];
                Write("{0} args.{1}", GetTypeCast(param.ParameterType), ToTitleCase(param.Name));

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
                    WriteLine("}} catch ({0} ex{1}) {{", GetTypeName(exception.Type),
                              exception.Id);
                    PushIndent();
                    WriteLine("result.Exception{0} = ex{0};", exception.Id);
                    PopIndent();
                }
            
                WriteLine("}");
            }

            WriteLine("var reply = new Message.Reply(call);");
            if (HasResultSerializer(method))
                WriteLine("result.Write(reply.Data.Serializer, null, reply.Data);");
            WriteLine("reply.Data.Dispatch();");

            WriteCloseBraceIndent();
        }

        private void GenerateEventSubscribeMethod(EventInfo @event)
        {
            WriteLine("void {0}(RemotingPeer peer, int remoteDelegateId)", GetEventSubscribeMethodName(@event));
            WriteStartBraceIndent();

            var @delegate = @event.EventHandlerType;
            WriteLine("var del = RemotingManager.DelegateManager.CreateDelegateProxy<{0}>(peer, remoteDelegateId);", GetDelegateProxyClassName(@delegate));
            WriteLine("iface_.{0} += ({1}) del.Delegate;", @event.Name, GetTypeName(@delegate));
            WriteCloseBraceIndent();
        }

        private void GenerateEventUnsubscribeMethod(EventInfo @event)
        {
            WriteLine("void {0}(RemotingPeer peer, int remoteDelegateId)", GetEventUnsubscribeMethodName(@event));
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

            if (GenerationContext.Delegates.Contains(type))
            {
                var index = GenerationContext.Delegates.IndexOf(type);
                return "Delegate" + index;
            }

            if (!doCreate)
                throw new Exception("Unknowed delegate name for " + type.FullName);

            GenerationContext.Delegates.Add(type);

            return "Delegate" + (GenerationContext.Delegates.Count-1);
        }

        private string GetDelegateImplClassName(Type type, bool doCreate = true)
        {
            return GetDelegateName(type, doCreate) + "Impl";
        }

        private string GetDelegateProxyClassName(Type type, bool doCreate = true)
        {
            return GetDelegateName(type, doCreate) + "Proxy";
        }

        public void GenerateDelegateClasses()
        {
            GenerateUsings();

            foreach(var type in GenerationContext.Delegates)
            {
                GenerateDelegateClass(type);
            }
        }

        private void GenerateDelegateClass(Type type)
        {

            var methodInvoke = type.GetMethod("Invoke");
            if(methodInvoke == null)
                throw new Exception();

            var proxyName = GetDelegateProxyClassName(type, false);
            var implName = GetDelegateImplClassName(type, false);
            var argsName = GetDelegateName(type, false) + "_args";
            var resultName = GetDelegateName(type, false) + "_result";

            WriteLine("class {0} : RemotingDelegateProxy", proxyName);
            WriteStartBraceIndent();
            WriteLine("public {0}()", proxyName);
            WriteStartBraceIndent();
            WriteLine("SetDelegate(new {0}(Invoke));", GetTypeName(type));
            WriteCloseBraceIndent();
            GenerateProtocolMethod(methodInvoke, null, "Invoke", argsName, resultName, "Message.DelegateCall");
            WriteCloseBraceIndent();
            NewLine();

            WriteLine("class {0} : RemotingDelegateImpl", implName);
            WriteStartBraceIndent();
            WriteLine("public {0}()", implName);
            WriteStartBraceIndent();
            WriteCloseBraceIndent();
            NewLine();
            GenerateDelegateProxyInvoke(methodInvoke, argsName, resultName);
            WriteCloseBraceIndent();
            NewLine();

            GenerateMethodSerializers(methodInvoke, argsName, resultName);
        }

        private void GenerateDelegateProxyInvoke(MethodInfo methodInvoke, string argsName, string resultName)
        {
            //Invoke
            WriteLine("public override void Invoke(Message.DelegateCall call)");
            WriteStartBraceIndent();

            if (HasArgsSerializer(methodInvoke))
            {
                WriteLine("var args = new {0}();", argsName);
                WriteLine("args.Read(call.Data.Serializer, null, call.Data);");
            }

            var returnType = methodInvoke.ReturnType;

            if (IsTask(methodInvoke.ReturnType))
            {
                if (returnType != typeof(Task))
                    Write("var task = ({0})", GetTypeName(returnType));

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
                    WriteLine("var result = new {0}();", resultName);
                    WriteLine("result.Success = t.Result;");
                    WriteLine("var reply = new Message.DelegateReply(call);");
                    WriteLine("result.Write(reply.Data.Serializer, null, reply.Data);");
                    WriteLine("reply.Data.Dispatch();");
                    PopIndent();
                    WriteLine("});");
                }

            }
            else
            {
                if (HasResultSerializer(methodInvoke))
                {
                    WriteLine("var result = new {0}({1});", resultName);
                    if (returnType != typeof(void))
                        Write("result.Success = ({0})", GetTypeName(returnType));
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
                    WriteLine("var reply = new Message.Reply(call);");
                    WriteLine("result.Write(reply.Data.Serializer, null, reply.Data);");
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
                case DataType.List:
                case DataType.Map:
                case DataType.String:
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
            WriteLine("using Flood.Remoting;");
            WriteLine("using Flood.Remoting.Metadata;");
            WriteLine("using Flood.Serialization;");
            WriteLine("using System.Threading.Tasks;");

            NewLine();
        }

        #endregion

        #region Serialization

        /// <summary>
        /// Generates the code to serialize a type 
        /// </summary>
        private void GenerateTypeSerialization(Type type, string varName, string dataName)
        {
            var thrifDataType = ConvertFromTypeToThrift(type);

            switch (thrifDataType)
            {
                case DataType.List:
                    GenerateListSerialize(type, varName, dataName);
                    break;
                case DataType.Map:
                    GenerateMapSerialize(type, varName, dataName);
                    break;
                case DataType.Bool:
                case DataType.Byte:
                case DataType.Double:
                case DataType.I16:
                case DataType.I32:
                case DataType.I64:
                case DataType.String:
                    var enumCast = "";
                    if (type.IsEnum)
                        enumCast = String.Format("({0})", Enum.GetUnderlyingType(type).ToString());
                    WriteLine("{0}.Write{1}({2}{3});", dataName, thrifDataType.ToString(), enumCast, varName);
                    break;
                case DataType.Guid:
                    WriteLine("{0}.WriteString({1}.ToString());", dataName, varName);
                    break;
                case DataType.DateTime:
                    WriteLine("{0}.WriteI64({1}.Ticks);", dataName, varName);
                    break;
                case DataType.Custom:
                    WriteLine("var c = new CustomData(this.GetType().Assembly ,typeof({0}), customData);", GetTypeName(type));
                    WriteLine("{0}.WriteCustom({1}, c);", dataName, varName);
                    break;
                case DataType.Void:
                default:
                    System.Diagnostics.Debugger.Break();
                    throw new NotImplementedException();
            }

        }

        /// <summary>
        /// Generates the code to serialize a list  
        /// </summary>
        private void GenerateListSerialize(Type type, string name, string dataName)
        {
            var listElemType = (type.IsArray)? type.GetElementType() : type.GetGenericArguments()[0];

             var countName = (type.IsArray)? "Length" : "Count";

            WriteLine("{0}.WriteListBegin(new TList(DataType.{1}, {2}.{3}));",
                      dataName,
                      ConvertFromTypeToThrift(type).ToString(),
                      ToTitleCase(name),
                      countName);

            var iterName = string.Format("_iter{0}", GenericIndex++);
            WriteLine("foreach (var {0} in {1})", iterName, ToTitleCase(name));
            WriteStartBraceIndent();

            GenerateTypeSerialization(listElemType, iterName, dataName);

            WriteCloseBraceIndent();

            WriteLine("{0}.WriteListEnd();", dataName);
        }

        /// <summary>
        /// Generates the code to serialize a map  
        /// </summary>
        private void GenerateMapSerialize(Type type, string name, string dataName)
        {
            var mapElemType1 = type.GetGenericArguments()[0];
            var mapElemType2 = type.GetGenericArguments()[1];

            WriteLine("{0}.WriteMapBegin(new TMap(DataType.{1}, DataType.{2}, {3}.Count));",
                      dataName,
                      ConvertFromTypeToThrift(mapElemType1).ToString(),
                      ConvertFromTypeToThrift(mapElemType2).ToString(),
                      ToTitleCase(name));

            var iterName = string.Format("_iter{0}", GenericIndex++);
            WriteLine("foreach (var {0} in {1})", iterName, ToTitleCase(name));
            WriteStartBraceIndent();

            GenerateContainerDoubleElementSerialization(mapElemType1, mapElemType2, iterName, dataName);

            WriteCloseBraceIndent();

            WriteLine("{0}.WriteMapEnd();", dataName);

        }

        /// <summary>
        /// Generates the code to serialize a dual element container  
        /// </summary>
        private void GenerateContainerDoubleElementSerialization(Type elemType1, Type elemType2, string iterName, string dataName)
        {
            GenerateTypeSerialization(elemType1, iterName + ".Key", dataName);
            GenerateTypeSerialization(elemType2, iterName + ".Value", dataName);
        }

        #endregion

        #region Deserialization

        /// <summary>
        /// Generates the code to deserialize a single element of an array 
        /// </summary>
        private void GenerateTypeDeserialization(Type type, string varName, string dataName, bool varExists = true)
        {
            var thrifDataType = ConvertFromTypeToThrift(type);
            switch (thrifDataType)
            {
                case DataType.List:
                    GenerateListDeserialize(type, varName, dataName);
                    break;
                case DataType.Map:
                    GenerateMapDeserialize(type, varName, dataName);
                    break;
                case DataType.Bool:
                case DataType.Byte:
                case DataType.Double:
                case DataType.I16:
                case DataType.I32:
                case DataType.I64:
                case DataType.String:
                    var cast = "";
                    if (type.IsEnum || type == typeof(float))
                        cast = String.Format("({0})", GetTypeName(type));
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = {1}{2}.Read{3}();", varName, cast, dataName, thrifDataType.ToString());
                    break;
                case DataType.Guid:
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = new System.Guid({1}.ReadString());", varName, dataName);
                    break;
                case DataType.DateTime:
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = new System.DateTime({1}.ReadI64());", varName, dataName);
                    break;
                case DataType.Custom:
                    WriteLine("var c = new CustomData(this.GetType().Assembly ,typeof({0}), customData);", GetTypeName(type));
                    if (!varExists)
                        Write("var ");
                    WriteLine("{0} = ({1}){2}.ReadCustom(c);", varName, GetTypeName(type), dataName);
                    break;
                case DataType.Void:
                default:
                    throw new NotImplementedException();
            }

        }

        /// <summary>
        /// Generates the code to deserialize a list  
        /// </summary>
        private void GenerateListDeserialize(Type type, string name, string dataName)
        {
            var listElemType = (type.IsArray)? type.GetElementType() : type.GetGenericArguments()[0];

            var listName = string.Format("_list{0}", GenericIndex++);
            WriteLine("var {0} = {1}.ReadListBegin();", listName, dataName);

            if (type.IsArray)
            {
                WriteLine("{0} = new {1}[{2}.Count];", ToTitleCase(name), GetTypeName(listElemType), listName);
            }
            else
            {
                WriteLine("{0} = new {1}();", ToTitleCase(name), GetTypeName(type));
            }

            var iterName = string.Format("_i{0}", GenericIndex++);
            WriteLine("for (var {0} = 0; {0} < {1}.Count; ++{0})", iterName, listName);

            WriteStartBraceIndent();

            var elemName = string.Format("_elem{0}", GenericIndex++);

            GenerateTypeDeserialization(listElemType, elemName, dataName, false);

            if (type.IsArray)
            {
                WriteLine("{0}[{1}] = {2};", name, iterName, elemName);
            }
            else
            {
                WriteLine("{0}.Add({1});", name, elemName);
            }

            WriteCloseBraceIndent();
            WriteLine("{0}.ReadListEnd();", dataName);
        }

        /// <summary>
        /// Generates the code to deserialize a map  
        /// </summary>
        private void GenerateMapDeserialize(Type type, string name, string dataName)
        {
            var mapElemType1 = type.GetGenericArguments()[0];
            var mapElemType2 = type.GetGenericArguments()[1];

            WriteLine("{0} = new {1}();", ToTitleCase(name),
                      GetTypeName(type));

            var mapName = string.Format("_set{0}", GenericIndex++);
            WriteLine("var {0} = {1}.ReadMapBegin();", mapName, dataName);

            var iterName = string.Format("_i{0}", GenericIndex++);
            WriteLine("for (var {0} = 0; {0} < {1}.Count; ++{0})",
                      iterName, mapName);

            WriteStartBraceIndent();

            GenerateContainerDoubleElementDeserialization(mapElemType1, mapElemType2, ToTitleCase(name), dataName);

            WriteCloseBraceIndent();
            WriteLine("{0}.ReadMapEnd();", dataName);
        }

        /// <summary>
        /// Generates the code to deserialize a double element container  
        /// </summary>
        private void GenerateContainerDoubleElementDeserialization(Type elemType1, Type elemType2, string containerName, string dataName)
        {
            var elemName1 = string.Format("_elem{0}", GenericIndex++);
            var elemName2 = string.Format("_elem{0}", GenericIndex++);

            GenerateTypeDeserialization(elemType1, elemName1, dataName, false);
            GenerateTypeDeserialization(elemType2, elemName2, dataName, false);

            WriteLine("{0}.Add({1}, {2});", containerName, elemName1, elemName2);
        }

        #endregion

        #region Type Conversion 

        private string GetStubsClassName(Type type, bool fullName)
        {
            var name = type.FullName;
            if (!fullName && name.Contains('.'))
            {
                var names = name.Split('.');
                name = names[names.Length - 1];
            }

            name = name.Replace("+", "_");
            
            if(type.Assembly.FullName == currentAssembly.FullName)
                name += "Stubs";

            return name;
        }

        private string GetTypeName(Type type, bool replaceWithStubs = true)
        {
            if (replaceWithStubs && type.Assembly == currentAssembly && Metadata.IsDataObject(type))
                return GetStubsClassName(type, true);

            if (type.GetGenericArguments().Length == 0)
            {
                if (type == typeof(void))
                    return "void";

                return type.FullName.Replace("+",".");
            }

            var genericArguments = type.GetGenericArguments();
            var unmangledName = type.Name.Substring(0, type.Name.IndexOf("`"));
            return unmangledName + "<" + String.Join(",", genericArguments.Select(t => GetTypeName(t, replaceWithStubs))) + ">";
        }

        private string GetTypeCast(Type type)
        {
            if (type.Assembly == currentAssembly && Metadata.IsDataObject(type))
                return string.Format("({0})(object)", GetTypeName(type, false));

            return "";
        }

        private string GetTypeCastToStub(Type type)
        {
            if (type.Assembly == currentAssembly && Metadata.IsDataObject(type))
                return string.Format("({0})(object)", GetTypeName(type));

            return "";
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
        /// Converts from regular type to equivalent DataType.
        /// </summary>
        internal static DataType ConvertFromTypeToThrift(Type type)
        {
            if (type == typeof(void))
                return DataType.Void;
            else if (type == typeof(bool))
                return DataType.Bool;
            else if (type == typeof(byte))
                return DataType.Byte;
            else if (type == typeof(double) || type == typeof(float))
                return DataType.Double;
            else if (type == typeof(short))
                return DataType.I16;
            else if (type == typeof(int))
                return DataType.I32;
            else if (type == typeof(long))
                return DataType.I64;
            else if (type == typeof(string))
                return DataType.String;
            else if (type == typeof(Guid) || typeof(Guid).IsAssignableFrom(type))
                return DataType.Guid;
            else if (type == typeof(DateTime) || typeof(DateTime).IsAssignableFrom(type))
                return DataType.DateTime;
            else if (type.IsEnum)
                return ConvertFromTypeToThrift(Enum.GetUnderlyingType(type));
            else if (IsInstanceOfGenericType(typeof(IDictionary<,>), type))
                return DataType.Map;
            else if (IsInstanceOfGenericType(typeof(ICollection<>), type) || type.IsArray)
                return DataType.List;
            else 
                return DataType.Custom;
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
                IsImplementedProperty = false;
            }

            public Parameter(PropertyInfo info)
            {
                Name = info.Name;
                ParameterType = info.PropertyType;

                int id;
                if (!Metadata.TryGetId(info, out id))
                    throw new Exception("DataObject's Properties require an Id attribute.");

                Id = id;
                IsImplementedProperty = IsPropertyImplemented(info);
            }

            public Parameter(string  name, Type type, int id)
            {
                Name = name;
                ParameterType = type;           
                Id = id;
                IsImplementedProperty = false;
            }

            public static bool IsPropertyImplemented(PropertyInfo property)
            {
                var backingFieldName = "<" + property.Name + ">k__BackingField";
                var fields = property.DeclaringType.GetFields(BindingFlags.Instance|BindingFlags.NonPublic);

                return fields.All(f => f.Name != backingFieldName);
            }

            public string Name;
            public Type ParameterType;
            public int Id;
            public bool IsImplementedProperty; 
        }

        #endregion

#endregion
    }
}
