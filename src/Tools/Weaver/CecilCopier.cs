using System.Reflection;
using Mono.Cecil;
using Mono.Cecil.Cil;
using Mono.Collections.Generic;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Weaver.Util;
using MethodBody = Mono.Cecil.Cil.MethodBody;
using TypeAttributes = Mono.Cecil.TypeAttributes;

namespace Weaver
{
    public class CecilKey : CopyKey
    {
        public override bool Equals(object obj)
        {
            var key = obj as CecilKey;
            if (key == null)
                return false;

            var memberRef = Value as MemberReference;
            if (memberRef != null && !(memberRef is GenericParameter))
            {
                var memberKeyRef = key.Value as MemberReference;
                if( memberKeyRef == null ||
                    !CecilUtils.AreScopesEqual(memberKeyRef, memberRef) ||
                    memberKeyRef.FullName != memberRef.FullName)
                    return false;

                if (memberKeyRef is TypeReference)
                {
                    if (!(memberRef is TypeReference))
                        return false;
                }
                else if (memberKeyRef is FieldReference)
                {
                    if (!(memberRef is FieldReference))
                        return false;
                }
                else if (memberKeyRef is PropertyReference)
                {
                    if (!(memberRef is PropertyReference))
                        return false;
                }
                else if (memberKeyRef is EventReference)
                {
                    if (!(memberRef is EventReference))
                        return false;
                }
                else if (memberKeyRef is MethodReference)
                {
                    var methodKeyRef = (MethodReference)memberKeyRef;
                    var methodRef = memberRef as MethodReference;

                    if (methodRef == null)
                        return false;

                    return CecilUtils.DoMethodParametersTypeMatch(methodKeyRef, methodRef);
                }
                else
                {
                    throw new NotImplementedException();
                }

                return true;
            }

            return Value.Equals(key.Value);
        }

        public override int GetHashCode()
        {
            var hash = 0;

            var memberRef = Value as MemberReference;
            if (memberRef != null)
                hash ^= memberRef.FullName.GetHashCode();

            return hash != 0 ? hash : Value.GetHashCode();
        }
    }

    public interface IError
    {
        
    }

    public class UnexpectedImport : IError
    {
        public IMetadataScope Scope;
        public MemberReference Member;
        public object Source;
        
        public UnexpectedImport(IMetadataScope scope, MemberReference member, object source)
        {
            Scope = scope;
            Member = member;
            Source = source;
        }

        public override string ToString()
        {
            return "UnexpectedImport: " + Scope.Name + " Reference:"+Member.FullName+ " source:"+Source;
        }
    }

    public class CecilCopier : DeepCopier<CecilKey>
    {
        private ModuleDefinition destinationModule;

        private readonly Dictionary<TypeDefinition, TypeDefinition> stubTypes;

        //Member prefix name
        public string NamePrefix { get; set; }

        private TypeReference objectTypeRef;

        //Options
        public bool canImport = false;

        public List<IError> Errors;

        public CecilCopier(ModuleDefinition destinationModule)
        {
            this.destinationModule = destinationModule;
            
            NamePrefix = "";

            stubTypes = new Dictionary<TypeDefinition, TypeDefinition>();

            objectTypeRef = destinationModule.Import(typeof (object));

            Errors = new List<IError>();
        }

        protected override bool IsValidCopyValue(object value)
        {
            var memberRef = value as MemberReference;
            if (memberRef != null)
                return memberRef.Module == null || memberRef.Module == destinationModule;

            return true;
        }

        protected override bool SkipCopy(object from, out object copy)
        {
            var typeDef = from as TypeDefinition;
            if (typeDef != null && stubTypes.ContainsKey(typeDef))
            {
                copy = null;
                return false; //Replace stub
            }

            return base.SkipCopy(from, out copy);
        }

        public void Process()
        {
            ProcessDelayed();
            Update();
        }

        private void Update()
        {
            foreach(var type in destinationModule.Types)
                UpdateType(type);
        }

        private void UpdateType(TypeDefinition type)
        {
            UpdateInstructionsOffsets(type);

            foreach (var nestedType in type.NestedTypes)
                UpdateType(nestedType);
        }

        private void UpdateInstructionsOffsets(TypeDefinition type)
        {
            foreach(var method in type.Methods)
            {
                if(method.Body == null)
                    continue;

                var offset = 0;
                foreach (var instruction in method.Body.Instructions)
                {
                    instruction.Offset = offset;
                    offset += instruction.GetSize ();
                }
            }
        }

        private TypeDefinition GetDeclaringType(MemberReference member, bool throwMapException = true)
        {
            if (member.DeclaringType == null)
                return null;

            TypeReference localDeclaringType;
            if (TryGetLocalReference(member.DeclaringType, out localDeclaringType))
                return (TypeDefinition) localDeclaringType;

            if(throwMapException)
                throw new Exception("Could not found destination TypeDefinition. Map it.");

            return null;
        }

        public void AddMethod(TypeDefinition type, MethodDefinition method)
        {
            if(type.Methods.Any(m => m.FullName == method.FullName))
                return;

            type.Methods.Add(method); 
        }

        # region COPY METHODS

        private bool TryGetLocalReference(TypeReference @ref, out TypeReference localRef)
        {
            MemberReference @out;
            var ret = TryGetLocalReference(@ref, out @out);
            localRef = (TypeReference) @out;
            return ret;
        }

        private bool TryGetLocalReference(MethodReference @ref, out MethodReference localRef)
        {
            MemberReference @out;
            var ret = TryGetLocalReference(@ref, out @out);
            localRef = (MethodReference) @out;
            return ret;
        }

        private bool TryGetLocalReference(FieldReference @ref, out FieldReference localRef)
        {
            MemberReference @out;
            var ret = TryGetLocalReference(@ref, out @out);
            localRef = (FieldReference) @out;
            return ret;
        }

        private bool TryGetLocalReference(MemberReference @ref, out MemberReference localRef) 
        {
            if (TryGetCopy(@ref, out localRef))
            {
                UpdateScope(localRef);
                return true;
            }

            if(@ref.FullName.Contains("Reference"))
                Console.WriteLine();

            if (CecilUtils.GetScope(@ref).Name == destinationModule.Assembly.Name.Name)
            {
                localRef = CecilUtils.GetMemberDef(destinationModule, @ref);
            }
            else
            {
                TypeReference localDeclaringType;
                if (@ref.DeclaringType == null ||
                    !TryGetLocalReference(@ref.DeclaringType, out localDeclaringType))
                    return false;

                if (CecilUtils.AreScopesEqual(localDeclaringType, @ref.DeclaringType))
                    return false;

                var remoteDef = CecilUtils.GetMemberDef(localDeclaringType.Resolve(), @ref);
                localRef = CecilUtils.GetReference(destinationModule, remoteDef);
            }

            //Try to find local copy
            MemberReference localRefCopy;
            if (TryGetCopy(localRef, out localRefCopy))
                localRef = localRefCopy;

            UpdateScope(localRef);

            return true;
        }

        #region CopyReference

        public ModuleReference CopyReference(ModuleReference @ref)
        {
            var ret = destinationModule.ModuleReferences.SingleOrDefault(m => m.Name == @ref.Name);
            if (ret != null)
                return ret;

            ret =  new ModuleReference(@ref.Name);
            destinationModule.ModuleReferences.Add(ret);

            return ret;
        }

        public MemberReference CopyReference(MemberReference @ref)
        {
            if (@ref is TypeReference)
                return CopyReference((TypeReference) @ref);

            if (@ref is MethodReference)
                return CopyReference((MethodReference) @ref);

            if (@ref is FieldReference)
                return CopyReference((FieldReference) @ref);

            throw new NotImplementedException();
        }

        public TypeReference CopyReference(TypeReference @ref)
        {
            if (@ref is GenericParameter)
                return Copy((GenericParameter)@ref);

            if (@ref is GenericInstanceType)
                return Copy((GenericInstanceType)@ref);

            if (@ref is ArrayType)
                return Copy((ArrayType)@ref);

            if (@ref is PointerType)
                return Copy((PointerType)@ref);

            if (@ref is ByReferenceType)
                return Copy((ByReferenceType)@ref);

            return Copy(@ref);
        }

        public MethodReference CopyReference(MethodReference @ref)
        {
            if (@ref is GenericInstanceMethod)
                return Copy((GenericInstanceMethod) @ref);

            return Copy(@ref);
        }

        public FieldReference CopyReference(FieldReference @ref)
        {
            FieldReference ret;
            if (TryGetLocalReference(@ref, out ret))
                return ret;

            ret = new FieldReference(@ref.Name, @ref.FieldType);

            CopyAll(@ref, ret, "FieldType", "DeclaringType");

            if (@ref.DeclaringType != null)
                ret.DeclaringType = CopyReference(@ref.DeclaringType);

            ret.FieldType = CopyReference(@ref.FieldType);

            UpdateScope(ret.DeclaringType);

            return ret;
        }

        public MethodReference Copy(MethodReference @ref)
        {
            MethodReference ret;
            if (TryGetLocalReference(@ref, out ret))
                return ret;

            ret = new MethodReference(@ref.Name, CopyReference(@ref.ReturnType), CopyReference(@ref.DeclaringType));

            CopyAll(@ref, ret, "MethodReturnType", "ReturnType", "DeclaringType");

            UpdateScope(ret.DeclaringType);

            return ret;
        }

        public GenericInstanceMethod Copy(GenericInstanceMethod @ref)
        {
            var methodRef = CopyReference(@ref.ElementMethod);
            var ret = new GenericInstanceMethod(methodRef);

            for (int i = 0; i < @ref.GenericArguments.Count; i++)
            {
                var genericArgument = @ref.GenericArguments[i];
                ret.GenericArguments.Add(CopyReference(genericArgument));
            }

            return ret;
        }

        public TypeReference Copy(TypeReference @ref)
        {
            TypeReference ret;
            if (TryGetLocalReference(@ref, out ret))
                return ret;

            ret = new TypeReference(@ref.Namespace, @ref.Name, destinationModule, @ref.Scope);

            CopyAll(@ref, ret, "Module", "Scope", "DeclaringType");

            if(@ref.DeclaringType != null)
                ret.DeclaringType = CopyReference(@ref.DeclaringType);

            ret.Scope = CopyScope(@ref.Scope, @ref);
            UpdateScope(ret);

            var field = ret.GetType().GetField("etype", BindingFlags.NonPublic | BindingFlags.Instance);
            var etype = field.GetValue(@ref);
            field.SetValue(ret, etype);

            return ret;
        }

        public GenericParameter Copy(GenericParameter @ref)
        {
            GenericParameter ret;
            if (TryGetCopy(@ref, out ret))
                return ret;

            ret = new GenericParameter(@ref.Position, @ref.Type, destinationModule);

            SetCopy(@ref, ret);

            ret.Name = @ref.Name;

            AddDelayedCopy(@ref, ret,
                (originObject, destObject) =>
                    {
                        var owner = (@ref.Type == GenericParameterType.Type)
                            ? (object) CopyReference((TypeReference) @ref.Owner)
                            : CopyReference((MethodReference) @ref.Owner);

                        var field = ret.GetType().GetField("owner", BindingFlags.NonPublic | BindingFlags.Instance);
                        field.SetValue(ret, owner);
                    });

            return ret;
        }

        public GenericInstanceType Copy(GenericInstanceType @ref)
        {
            var typeRef = CopyReference(@ref.ElementType);
            var ret = new GenericInstanceType(typeRef);

            foreach (var genericArgument in @ref.GenericArguments)
                ret.GenericArguments.Add(CopyReference(genericArgument));

            return ret;
        }

        public ArrayType Copy(ArrayType @ref)
        {
            var typeRef = CopyReference(@ref.ElementType);
            var ret = new ArrayType(typeRef);

            ret.Dimensions.Clear();
            foreach (var dim in @ref.Dimensions)
                ret.Dimensions.Add(new ArrayDimension(dim.LowerBound, dim.UpperBound));

            return ret;
        }

        public PointerType Copy(PointerType @ref)
        {
            var typeRef = CopyReference(@ref.ElementType);

            return new PointerType(typeRef);
        }

        public ByReferenceType Copy(ByReferenceType @ref)
        {
            var typeRef = CopyReference(@ref.ElementType);

            return new ByReferenceType(typeRef);
        }

        #endregion

        private IMetadataScope CopyScope(IMetadataScope scope, MemberReference member)
        {
            string scopeName = scope.Name;
            Version scopeVersion;

            //Avoid self reference
            if (scopeName == destinationModule.Name ||
                scopeName == destinationModule.Assembly.Name.Name)
                return scope;

            switch (scope.MetadataScopeType) {
            case MetadataScopeType.AssemblyNameReference:
                scopeVersion = ((AssemblyNameReference) scope).Version;
                break;
            case MetadataScopeType.ModuleDefinition:
                scopeVersion = (((ModuleDefinition) scope).Assembly.Name).Version;
                break;
            default:
                throw new NotImplementedException ();
            }

            if(scopeName.EndsWith(".dll"))
                scopeName = Path.GetFileNameWithoutExtension(scopeName);

            var assemblyName = destinationModule.AssemblyReferences.FirstOrDefault(
                reference => reference.Name == scopeName && reference.Version == scopeVersion);

            if (assemblyName != null)
                return assemblyName;

            if (!canImport)
            {
                Errors.Add(new UnexpectedImport(scope, member, GetCurrentCopy(typeof(IMemberDefinition))));
                return scope;
            }

            var ret = new AssemblyNameReference(scopeName, scopeVersion);

            if(scope.MetadataScopeType == MetadataScopeType.AssemblyNameReference)
                CopyAll(((AssemblyNameReference) scope),ret, "Name","Version");

            destinationModule.AssemblyReferences.Add(ret);

            return ret;
        }

        private void UpdateScope(MemberReference memberRef)
        {
            if (memberRef is TypeReference)
            {
                UpdateScope((TypeReference) memberRef);
                return;
            }

            if (memberRef.DeclaringType != null)
                UpdateScope(memberRef.DeclaringType);
        }

        private void UpdateScope(TypeReference typeRef)
        {
            var typeSpec = typeRef as TypeSpecification;
            if (typeSpec != null)
                typeRef = typeSpec.ElementType;

            typeRef.Scope = CopyScope(typeRef.Scope, typeRef);

            if (typeRef.DeclaringType != null)
            {
                if (typeRef.Scope.Name!= typeRef.DeclaringType.Scope.Name)
                    throw new Exception("Declaring type has diferent scope name.");

                UpdateScope(typeRef.DeclaringType);
            }
        }

        /*
         * COPY DEFINITIONS
         */

        [MemoizeCopy]
        public FieldDefinition Copy(FieldDefinition def)
        {
            var ret = new FieldDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.FieldType));
            ret.DeclaringType = GetDeclaringType(def);

            CopyAll(def,ret.Constant,"Name","DeclaringType","FieldType","MetadataToken", "Constant");

            if(def.HasConstant)
                ret.Constant = def.Constant;

            return ret;
        }

        [MemoizeCopy]
        public EventDefinition Copy(EventDefinition def)
        {
            var ret = new EventDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.EventType));

            CopyAll(def,ret,"Name","DeclaringType","EventType","MetadataToken");

            var declaringType = GetDeclaringType(def);
            if(ret.RemoveMethod != null) AddMethod(declaringType, ret.RemoveMethod);
            if(ret.AddMethod    != null) AddMethod(declaringType, ret.AddMethod);

            return ret;
        }

        [MemoizeCopy]
        public PropertyDefinition Copy(PropertyDefinition def)
        {
            var ret = new PropertyDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.PropertyType));

            CopyAll(def,ret,"Name","DeclaringType","PropertyType","MetadataToken");

            var declaringType = GetDeclaringType(def);
            if(ret.GetMethod != null) AddMethod(declaringType, ret.GetMethod);
            if(ret.SetMethod != null) AddMethod(declaringType, ret.SetMethod);

            return ret;
        }

        [MemoizeCopy]
        public MethodDefinition Copy(MethodDefinition def)
        {
            Copy(def.DeclaringType, true);

            var declaringType = GetDeclaringType(def);

            var ret = declaringType.Methods.FirstOrDefault(
                m => m.FullName == def.FullName && 
                     CecilUtils.DoMethodParametersTypeMatch(m, def));
            if (ret != null)
            {
                Warn("Equivalent method already exists using it instead: "+def.FullName);
                return ret;
            }

            if(def.FullName.Contains("RPCGen.Tests.Services.DataObjectBaseClass.IService.GetDataObject"))
                Console.WriteLine();

            ret = new MethodDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.ReturnType));

            //ReturnType derive form MethodReturnType
            CopyAll(def,ret,"Name","DeclaringType", "MetadataToken", "Overrides", "ReturnType");

            AddDelayedCopy(def, ret,
                (originObject, destObject) =>
                    {
                        foreach (var @override in def.Overrides)
                            ret.Overrides.Add(CopyReference(@override));
                    });

            return ret;
        }

        [MemoizeCopy]
        public TypeDefinition Copy(TypeDefinition def)
        {
            return Copy(def, false);
        }

        [MemoizeCopy]
        public TypeDefinition Copy(TypeDefinition def, bool isStubType)
        {
            TypeReference typeRef;
            if (TryGetCopy(def, out typeRef))
                return typeRef as TypeDefinition;

            TypeDefinition declaringType = GetDeclaringType(def, false);
            if (def.IsNested && declaringType == null)
                declaringType = Copy(def.DeclaringType, true);

            var typeCollection = (declaringType == null) ? this.destinationModule.Types : declaringType.NestedTypes;

            TypeDefinition ret;
            if (stubTypes.ContainsKey(def))
            {
                ret = stubTypes[def];
            }
            else
            {
                ret = typeCollection.FirstOrDefault(t => t.Name == def.Name);
                if (ret != null)
                {
                    Warn("Type already exists merging: " + def.FullName);
                    Merge(def, ret);
                    return ret;
                }

                var baseType = ((def.Attributes & TypeAttributes.Interface) != 0)? null :objectTypeRef;

                ret = new TypeDefinition(def.Namespace, this.NamePrefix + def.Name, def.Attributes, baseType);
                ret.DeclaringType = declaringType;

                typeCollection.Add(ret);

                SetCopy(def, ret);
            }

            if (isStubType)
            {
                if(!stubTypes.ContainsKey(def))
                    stubTypes.Add(def, ret);

                Log("< Type Stub "+def.FullName);

                return ret;
            }

            if (stubTypes.ContainsKey(def))
                stubTypes.Remove(def);

            Log("< Type Delayed "+def.FullName);

            foreach (TypeDefinition nestedType in def.NestedTypes)
                Copy(nestedType);

            AddDelayedCopy(def, ret,
                (originObject, destObject) =>
                    {
                        CopyAll(def,ret,"Name","DeclaringType","MetadataToken","Scope", "NestedTypes", "BaseType", "Interfaces");

                        if(def.BaseType != null)
                            ret.BaseType = CopyReference(def.BaseType);

                        foreach (var @interface in def.Interfaces)
                            ret.Interfaces.Add(CopyReference(@interface));
                    });

            return ret;
        }

        [MemoizeCopy]
        public ParameterDefinition Copy(ParameterDefinition def)
        {
            var ret = new ParameterDefinition(def.Name, def.Attributes, CopyReference(def.ParameterType));

            CopyAll(def,ret, "ParameterType", "MetadataToken", "Constant");

            if(def.HasConstant)
                ret.Constant = def.Constant;

            return ret;
        }

        [MemoizeCopy]
        public VariableDefinition Copy(VariableDefinition def)
        {
            var ret = new VariableDefinition(def.Name, CopyReference(def.VariableType));

            CopyAll(def,ret,"VariableType");

            return ret;
        }

        [MemoizeCopy]
        public CustomAttribute Copy(CustomAttribute def)
        {
            var ret = new CustomAttribute(CopyReference(def.Constructor), def.GetBlob());

            CopyAll(def,ret, "Constructor");

            return ret;
        }

        [MemoizeCopy]
        public CustomAttributeArgument Copy(CustomAttributeArgument def)
        {
            return new CustomAttributeArgument(CopyReference(def.Type), def.Value);
        }

        [MemoizeCopy]
        public MethodBody Copy(MethodBody def)
        {
            var parent = (MethodDefinition) GetCopy(def.Method);
            var ret = new MethodBody(parent);

            CopyAll(def,ret,"Method","Scope", "Instructions", "ExceptionHandlers");

            AddDelayedCopy(def, ret,
                (originObject, destObject) =>
                    {
                        destObject.Instructions.Clear();

                        var instructions = new Dictionary<Instruction, Instruction>();
                        foreach (var instruction in originObject.Instructions)
                        {
                            var copy = Copy(instruction);
                            instructions.Add(instruction, copy);
                            destObject.Instructions.Add(copy);
                        }

                        UpdateInstructionsOperand(instructions);

                        foreach (var exHandler in originObject.ExceptionHandlers)
                            destObject.ExceptionHandlers.Add(Copy(exHandler, instructions));
                    });

            return ret;
        }

        [MemoizeCopy]
        public MethodReturnType Copy(MethodReturnType def)
        {
            var parent = (MethodReference) GetCopy(def.Method);

            var ret = new MethodReturnType(parent);

            ret.ReturnType = CopyReference(def.ReturnType);

            CopyAll(def,ret,"Method", "ReturnType");

            return ret;
        }

        private Instruction Copy(Instruction def)
        {
            //copy of operands is delayed to ProcessInstructions
            Instruction ret= null;
            if(def.Operand == null){
                ret = Instruction.Create(def.OpCode);
            } else {
                var methodInfo = typeof(Instruction).GetMethod("Create",new Type[]{def.OpCode.GetType(),def.Operand.GetType()});
                ret = (Instruction) methodInfo.Invoke(null,new object[]{def.OpCode,def.Operand});
            }

            ret.SequencePoint = def.SequencePoint;

            return ret;
        }

        private void UpdateInstructionsOperand(Dictionary<Instruction, Instruction> instructionMap)
        {
            foreach (var i in instructionMap.Values)
            {
                if (i.Operand == null)
                    continue;

                if (i.Operand.GetType().IsValueType || i.Operand is String)
                    continue;

                if (i.Operand is TypeReference)
                {
                    var ret = CopyReference((TypeReference) i.Operand);
                    i.Operand = ret;
                    continue;
                }
                if (i.Operand is MethodReference)
                {
                    i.Operand = CopyReference((MethodReference) i.Operand);
                    continue;
                }
                if (i.Operand is FieldReference)
                {
                    i.Operand = CopyReference((FieldReference) i.Operand);
                    continue;
                }
                if (i.Operand is VariableReference ||
                    i.Operand is ParameterDefinition)
                {
                    i.Operand = GetCopy(i.Operand);
                    continue;
                }
                if(i.Operand is Instruction)
                {
                    i.Operand = instructionMap[(Instruction)i.Operand];
                    continue;
                }
                var operandI = i.Operand as Instruction[];
                if (operandI != null) //i.e: switch
                {
                    var l = new Instruction[operandI.Length];
                    for (var k = 0; k < operandI.Length; k++)
                        l[k] = instructionMap[operandI[k]];
                    i.Operand = l;
                    continue;
                }

                throw new NotImplementedException("No handling for operands of type " + i.Operand.GetType());
            }
        }

        private ExceptionHandler  Copy(ExceptionHandler def, Dictionary<Instruction, Instruction> instructionMap)
        {
            var ret= new ExceptionHandler(def.HandlerType);
            if(def.CatchType != null)
                ret.CatchType = CopyReference(def.CatchType);

            CopyAll(def,ret,"CatchType", "TryStart", "TryEnd", "HandlerStart", "HandlerEnd");

            if(def.TryStart != null)
                ret.TryStart = instructionMap[def.TryStart];

            if(def.TryEnd != null)
                ret.TryEnd = instructionMap[def.TryEnd];
            
            if(def.HandlerStart != null)
                ret.HandlerStart = instructionMap[def.HandlerStart];

            if(def.HandlerEnd != null)
                ret.HandlerEnd = instructionMap[def.HandlerEnd];

            return ret;
        }

        public Byte[] Copy(Byte[] value){
            return value;
        }

        #endregion

        # region MERGE METHODS

        public void MergeReference(FieldReference @ref1, FieldReference @ref2)
        {
            
        }

        public void MergeReference(MethodReference @ref1, MethodReference @re2)
        {
            
        }

        public void MergeReference(PropertyReference @ref1, PropertyReference @re2)
        {
            
        }

        public void MergeReference(TypeReference @ref1, TypeReference @re2)
        {
            
        }

        public void Merge(FieldDefinition def1, FieldDefinition def2)
        {
            CecilUtils.CheckEquivalentTypes(def1.FieldType, def2.FieldType);

            MergeAll(def1,def2,"Name","DeclaringType","FieldType","MetadataToken", "Module");
        }

        public void Merge(EventDefinition def1, EventDefinition def2)
        {
            CecilUtils.CheckEquivalentTypes(def1.EventType, def2.EventType);

            MergeAll(def1,def2,"Name","DeclaringType","EventType","MetadataToken", "Module", "Attributes");
        }

        public void Merge(PropertyDefinition def1, PropertyDefinition def2)
        {
            CecilUtils.CheckEquivalentTypes(def1.PropertyType, def2.PropertyType);

            MergeAll(def1,def2,"Name","DeclaringType","PropertyType","MetadataToken", "Module", "Attributes");
        }

        public void Merge(MethodDefinition def1, MethodDefinition def2)
        {
            CecilUtils.CheckEquivalentTypes(def1.ReturnType, def2.ReturnType);

            MergeAll(def1,def2,"Name","DeclaringType","ReturnType","MetadataToken", "Module", "Attributes");
        }

        public void Merge(TypeDefinition def1, TypeDefinition def2)
        {
            MergeAll(def1,def2,"Name","DeclaringType", "MetadataToken", "Module", "Scope", "BaseType");

            AddDelayedCopy(def1, def2, 
                (originObject, destObject) =>
                    {
                        var hasType1BaseClass = originObject.BaseType != null && originObject.BaseType.FullName != "System.Object";
                        var hasType2BaseClass = destObject.BaseType != null && destObject.BaseType.FullName != "System.Object";
                        if (hasType1BaseClass && hasType2BaseClass)
                        {
                            TypeReference destObjectBaseRef;
                            if (!TryGetLocalReference(destObject.BaseType, out destObjectBaseRef))
                                destObjectBaseRef = destObject.BaseType;

                            TypeReference origObjectBaseRef;
                            if (!TryGetLocalReference(originObject.BaseType, out origObjectBaseRef))
                                origObjectBaseRef = originObject.BaseType;

                            if(!CecilUtils.AreTypesEquivalent(origObjectBaseRef, destObjectBaseRef))
                                throw new Exception("Cannot merge classes with diferent base types.");
                        }

                        if (hasType1BaseClass && !hasType2BaseClass)
                        {
                            destObject.BaseType = CopyReference(originObject.BaseType);
                        }
                    });
        }

        public void Merge(ParameterDefinition def1, ParameterDefinition def2)
        {
            CecilUtils.CheckEquivalentTypes(def1.ParameterType, def2.ParameterType);

            MergeAll(def1,def2,"Name", "Method","ParameterType","MetadataToken", "Module");
        }

        public void Merge(VariableDefinition def1, VariableDefinition def2)
        {
            CecilUtils.CheckEquivalentTypes(def1.VariableType, def2.VariableType);

            MergeAll(def1,def2,"VariableType");
        }

        public void Merge(CustomAttribute def1, CustomAttribute def2)
        {
            MergeAll(def1,def2,"DeclaringType");
        }

        public void Merge(GenericInstanceType def1, GenericInstanceType def2)
        {
           
        }

        public void Merge(MethodBody def1, MethodBody def2)
        {
            MergeAll(def1,def2,"Method", "Scope", "ThisParameter");
        }

        public void  Merge(MethodReturnType def1, MethodReturnType def2)
        {
            MergeAll(def1,def2,"Method","ReturnType");
        }

        public void  Merge(Collection<TypeReference>def1, Collection<TypeReference> def2)
        {
            foreach (var origTypeRef in def1)
            {
                if (def2.Any(i => CecilUtils.AreTypesEquivalent(i,origTypeRef)))
                    continue;

                def2.Add(CopyReference(origTypeRef));
            }
        }

        public void  Merge(Collection<TypeDefinition>def1, Collection<TypeDefinition> def2)
        {
            MembersMerge(def1, def2);
        }

        public void  Merge(Collection<FieldDefinition>def1, Collection<FieldDefinition> def2)
        {
            MembersMerge(def1, def2);
        }

        public void  Merge(Collection<PropertyDefinition>def1, Collection<PropertyDefinition> def2)
        {
            MembersMerge(def1, def2);
        }

        public void  Merge(Collection<EventDefinition>def1, Collection<EventDefinition> def2)
        {
            MembersMerge(def1, def2);
        }

        private void  MembersMerge<T>(Collection<T>def1, Collection<T> def2)
            where T : IMemberDefinition
        {
            foreach (var origDef in def1)
            {
                var destDef = def2.SingleOrDefault(i => i.Name == origDef.Name);
                if (destDef != null)
                {
                    Merge(origDef, destDef);
                    continue;
                }

                var copy = Copy(origDef);

                // Copy(TypeDefinition def) already adds def to declaring type.
                if(!(origDef is TypeDefinition))
                    def2.Add(copy);
            }
        }

        public void  Merge(Collection<MethodDefinition>def1, Collection<MethodDefinition> def2)
        {
            foreach (var origMethodDef in def1)
            {
                var destMethodName = origMethodDef.Name;

                var count = def2.Count(
                    m => m.Name == destMethodName
                         && CecilUtils.DoMethodParametersTypeMatch(origMethodDef, m));

                if(count > 1)
                    System.Diagnostics.Debugger.Break();

                var destMethodDef = def2.SingleOrDefault(
                    m => m.Name == destMethodName
                         && CecilUtils.DoMethodParametersTypeMatch(origMethodDef, m));

                if (destMethodDef != null)
                {
                    Merge(origMethodDef, destMethodDef);
                    continue;
                }

                def2.Add(Copy(origMethodDef));
            }
        }

        public void  Merge(Collection<SecurityDeclaration>def1, Collection<SecurityDeclaration> def2)
        {

        }

        public void  Merge(Collection<CustomAttribute>def1, Collection<CustomAttribute> def2)
        {

        }

        public void  Merge(Collection<Instruction>def1, Collection<Instruction> def2)
        {
            AddDelayedCopy(def1, def2,
                (originObject, destObject) =>
                    {
                        destObject.Clear();
                        var instructionMap = new Dictionary<Instruction, Instruction>();
                        foreach(var i in originObject)
                        {
                            var copy = Copy(i);
                            instructionMap.Add(i, copy);
                            destObject.Add(copy);
                        }

                        UpdateInstructionsOperand(instructionMap);
                    });
        }

        public void  Merge(Collection<ExceptionHandler>def1, Collection<ExceptionHandler> def2)
        {

        }

        public void  Merge(Collection<VariableDefinition>def1, Collection<VariableDefinition> def2)
        {
            var variables = def1.ToList();

            //TODO dont override destination instructions always
            def2.Clear();
            foreach(var v in variables){
                def2.Add(Copy(v));
            }
        }

        public void  Merge(Collection<MethodReference>def1, Collection<MethodReference> def2)
        {

        }

        public void  Merge(Collection<GenericParameter>def1, Collection<GenericParameter> def2)
        {

        }

        public void  Merge(Collection<ParameterDefinition>def1, Collection<ParameterDefinition> def2)
        {
            if(def1.Count != def2.Count)
                throw new Exception("Expecting same parameters number.");

            for (int i = 0; i < def1.Count; i++)
                Merge(def1[i],def2[i]);
        }

        public void  Merge(Byte[] def1, Byte[] def2)
        {

        }
        #endregion
    }
}
