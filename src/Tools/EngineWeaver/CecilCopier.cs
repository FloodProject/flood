using EngineWeaver.Util;
using Mono.Cecil;
using Mono.Cecil.Cil;
using Mono.Collections.Generic;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace EngineWeaver
{
    public class CecilCopier : DeepCopier
    {
        private ModuleDefinition originModule;
        private ModuleDefinition destinationModule;
        private readonly Dictionary<MemberReference,MemberReference> referenceMap;

        private readonly List<Instruction> instructions;
        private readonly Dictionary<TypeDefinition,TypeDefinition> types;
        private readonly Dictionary<TypeDefinition, TypeDefinition> stubTypes;

        //Member prefix name
        public string NamePrefix { get; set; }

        public CecilCopier(ModuleDefinition originModule, ModuleDefinition destinationModule)
        {
            this.originModule = originModule;
            this.destinationModule = destinationModule;
            
            NamePrefix = "";
            referenceMap = new Dictionary<MemberReference,MemberReference>();
            instructions = new List<Instruction>();
            types = new Dictionary<TypeDefinition,TypeDefinition>();
            stubTypes = new Dictionary<TypeDefinition, TypeDefinition>();
        }

        public void Clear()
        {
            CopyMap.Clear();
            referenceMap.Clear();
        }

        public void Map(MemberReference origin,MemberReference destination)
        {
            if(origin.Module != originModule || destination.Module != destinationModule)
                throw new ArgumentException();

            referenceMap.Add(origin,destination);
        }

        public void Process()
        {
            CopyTypes();
            CopyInstructionsOperands();
            Update();
        }

        private void CopyTypes()
        {
            foreach (var t in types)
            { 
                Log("> Type "+t.Key.FullName);
                Log("< Type "+t.Value.FullName);
                CopyAll(t.Key,t.Value,"Name","DeclaringType","BaseType","MetadataToken","Scope", "NestedTypes");
            }
            types.Clear();
        }

        private void CopyInstructionsOperands()
        {

            foreach(var i in instructions){
                if(i.Operand == null){
                    continue;
                }
                if(i.Operand.GetType().IsValueType || i.Operand is String){
                    continue;
                }
                if(i.Operand is TypeReference){
                    i.Operand = CopyReference((TypeReference)i.Operand);
                    continue;
                }
                if(i.Operand is MethodReference){
                    i.Operand = CopyReference((MethodReference)i.Operand);
                    continue;
                }
                if(i.Operand is FieldReference){
                    i.Operand = CopyReference((FieldReference)i.Operand);
                    continue;
                }
                if(i.Operand is VariableReference   ||
                   i.Operand is ParameterDefinition ||
                   i.Operand is Instruction)
                {
                    i.Operand = CopyMap[i.Operand];
                    continue;
                }
                var operandI = i.Operand as Instruction[];
                if (operandI != null) //i.e: switch
                {
                    var l = new Instruction[operandI.Length];
                    for(var k = 0; k <  operandI.Length; k++)
                        l[k] = (Instruction) CopyMap[operandI[k]];
                    i.Operand = l;
                    continue;
                }
               
                throw new NotImplementedException("No handling for operands of type "+i.Operand.GetType());
            }

            instructions.Clear();
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
            if(member.Module != originModule)
                throw new ArgumentException();

            if (member.DeclaringType == null)
                return null;

            if (referenceMap.ContainsKey(member.DeclaringType))
                return (TypeDefinition) referenceMap[member.DeclaringType];

            if (CopyMap.ContainsKey(member.DeclaringType))
                return (TypeDefinition) CopyMap[member.DeclaringType];

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

        private T GetLocalReference<T>(T @ref) where T : MemberReference
        {
            if(CopyMap.ContainsKey(@ref))
                return  (T)CopyMap[@ref];
            if(referenceMap.ContainsKey(@ref))
                return  (T)referenceMap[@ref];

            if (@ref is TypeReference)
            {
                var typeRef = destinationModule.GetType(@ref.FullName);
                if(typeRef != null) 
                    return (T)(MemberReference)typeRef;
            }

            var destTypeDef = destinationModule.GetType(@ref.DeclaringType.FullName);
            if (destTypeDef != null)
            {
                var origMethodRef = @ref as MethodReference;
                if (origMethodRef != null)
                {
                    var destMethodDef = CecilUtils.GetTypeMethodDef(destTypeDef, origMethodRef);
                    if(destMethodDef != null) 
                        return (T)(MemberReference)destMethodDef;
                }

                var origFieldRef = @ref as FieldReference;
                if (origFieldRef != null)
                {
                    var destFieldDef = CecilUtils.GetTypeFieldDef(destTypeDef, origFieldRef);
                    if(destFieldDef != null) 
                        return (T)(MemberReference)destFieldDef;
                }
            }

            throw new Exception("Cannot find Field "+@ref.FullName+". Try to Copy/Merge/TMap it first.");
        }

        private bool IsLocalReference(string scopeName)
        {
            if(Path.GetExtension(scopeName) == ".dll")
                scopeName = Path.GetFileNameWithoutExtension(scopeName);

            return scopeName ==  Path.GetFileNameWithoutExtension(originModule.Name) || 
                   scopeName == Path.GetFileNameWithoutExtension(destinationModule.Name);
        }

        public FieldReference CopyReference(FieldReference @ref)
        {
             if(IsLocalReference(@ref.DeclaringType.Scope.Name))
                return GetLocalReference(@ref);

            Log("Importing "+@ref.FullName+ " from "+ @ref.DeclaringType.Scope.Name);
            return destinationModule.Import(@ref);
        }

        public MethodReference CopyReference(MethodReference @ref)
        {
            if(IsLocalReference(@ref.DeclaringType.Scope.Name))
                return GetLocalReference(@ref);

            Log("Importing "+@ref.FullName+ " from "+ @ref.DeclaringType.Scope.Name);

            // Fix bad imports of generic arguments.
            CheckGenericArguments(@ref);

            return destinationModule.Import(@ref);
        }

        public TypeReference CopyReference(TypeReference @ref)
        {
            if (IsLocalReference(@ref.Scope.Name))
                return GetLocalReference(@ref);

            Log("Importing " + @ref.FullName + " from " + @ref.Scope.Name);

            // Fix bad imports of generic arguments.
            CheckGenericArguments(@ref);

            return destinationModule.Import(@ref);
        }

        private void CheckGenericArguments(TypeReference typeRef)
        {
            if (typeRef == null)
                return;

            var genericTypeRef = typeRef as GenericInstanceType;
            if (genericTypeRef != null)
            {
                for (int i = 0; i < genericTypeRef.GenericArguments.Count; i++)
                {
                    var genericArgument = genericTypeRef.GenericArguments[i];
                    if (genericArgument is GenericParameter) continue;
                    genericTypeRef.GenericArguments[i] = CopyReference(genericArgument);
                }
            }

            CheckGenericArguments(typeRef.DeclaringType as GenericInstanceType);
        }

        private void CheckGenericArguments(MethodReference methodRef)
        {
            if (methodRef == null)
                return;

            var genericMethodRef = methodRef as GenericInstanceMethod;
            if (genericMethodRef != null)
            {
                for (int i = 0; i < genericMethodRef.GenericArguments.Count; i++)
                {
                    var genericArgument = genericMethodRef.GenericArguments[i];
                    if (genericArgument is GenericParameter) continue;
                    genericMethodRef.GenericArguments[i] = CopyReference(genericArgument);
                }
            }

            CheckGenericArguments(methodRef.ReturnType as GenericInstanceType);
            CheckGenericArguments(methodRef.DeclaringType as GenericInstanceType);
        }

        /*
         * COPY DEFINITIONS
         */

        public FieldDefinition Copy(FieldDefinition def)
        {
            var ret = new FieldDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.FieldType));

            Log("< Field "+ret.FullName);
            CopyAll(def,ret,"Name","DeclaringType","FieldType","MetadataToken");

            return ret;
        }


        public EventDefinition Copy(EventDefinition def)
        {
            var ret = new EventDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.EventType));

            Log("< Event "+ret.FullName);
            CopyAll(def,ret,"Name","DeclaringType","EventType","MetadataToken");

            var declaringType = GetDeclaringType(def);
            if(ret.RemoveMethod != null) AddMethod(declaringType, ret.RemoveMethod);
            if(ret.AddMethod    != null) AddMethod(declaringType, ret.AddMethod);

            return ret;
        }

        public PropertyDefinition Copy(PropertyDefinition def)
        {
            var ret = new PropertyDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.PropertyType));
            
            Log("< Property "+ret.FullName);
            CopyAll(def,ret,"Name","DeclaringType","PropertyType","MetadataToken");

            var declaringType = GetDeclaringType(def);
            if(ret.GetMethod != null) AddMethod(declaringType, ret.GetMethod);
            if(ret.SetMethod != null) AddMethod(declaringType, ret.SetMethod);

            return ret;
        }

        public MethodDefinition Copy(MethodDefinition def)
        {
            var declaringType = GetDeclaringType(def);
            var ret = declaringType.Methods.FirstOrDefault(m => m.FullName == def.FullName);
            if(ret != null)
                return ret;

            ret = new MethodDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.ReturnType));

            Log("< Method "+ret.FullName);
            CopyAll(def,ret,"Name","DeclaringType","ReturnType","MetadataToken");

            return ret;
        }

        public TypeDefinition Copy(TypeDefinition def)
        {
            if (CopyMap.ContainsKey(def))
                return (TypeDefinition)CopyMap[def];

            return TypeCopy(def, false);
        }

        private TypeDefinition TypeCopy(TypeDefinition def, bool isStubType)
        {
            TypeDefinition declaringType = GetDeclaringType(def, false);
            if (def.IsNested && declaringType == null)
                declaringType = TypeCopy(def.DeclaringType, true);

            var typeCollection = (declaringType == null) ? this.destinationModule.Types : declaringType.NestedTypes;
            
            TypeDefinition ret;
            if (stubTypes.ContainsKey(def))
            {
                ret = stubTypes[def];
            }
            else
            {
                ret = typeCollection.FirstOrDefault((TypeDefinition t) => t.FullName == def.FullName);
                if (ret != null)
                {
                    Log("Cannot copy existing type " + def.FullName);
                    return ret;
                }

                var baseRef = (def.BaseType == null) ? null : this.CopyReference(def.BaseType);
                ret = new TypeDefinition(def.Namespace, this.NamePrefix + def.Name, def.Attributes, baseRef);
                typeCollection.Add(ret);

                if (isStubType)
                    stubTypes.Add(def, ret);
            }

            if (!isStubType)
            {
                types.Add(def, ret);
                CopyMap.Add(def, ret);
                foreach (TypeDefinition nestedType in def.NestedTypes)
                    Copy(nestedType);
            }

            return ret;
        }

        public ParameterDefinition Copy(ParameterDefinition def)
        {
            var ret = new ParameterDefinition(def.Name, def.Attributes, CopyReference(def.ParameterType));

            Log("< Parameter " + ret.Name);
            CopyAll(def,ret,"Method","ParameterType","MetadataToken");

            return ret;
        }

        public VariableDefinition Copy(VariableDefinition def)
        {
            var ret = new VariableDefinition(def.Name, CopyReference(def.VariableType));
            
            Log("< Variable " + ret.Name);
            CopyAll(def,ret,"VariableType");

            return ret;
        }

        public CustomAttribute Copy(CustomAttribute def)
        {
            var ret = new CustomAttribute(CopyReference(def.Constructor),def.GetBlob());

            Log("< CopyAttributes ");
            CopyAll(def,ret,"DeclaringType");

            return ret;
        }

        public CustomAttributeArgument Copy(CustomAttributeArgument def)
        {
            return new CustomAttributeArgument(CopyReference(def.Type), def.Value);
        }

        public MethodBody Copy(MethodBody def)
        {
            var parent = (MethodDefinition) CopyMap[def.Method];
            var ret = new MethodBody(parent);

            Log("< MethodBody ");
            CopyAll(def,ret,"Method","Scope");

            return ret;
        }

        public MethodReturnType  Copy(MethodReturnType def)
        {
            var parent = (IMethodSignature) CopyMap[def.Method];
            var ret= new MethodReturnType(parent);
            ret.ReturnType = CopyReference(def.ReturnType);

            Log("< MethodReturnType ");
            CopyAll(def,ret,"Method","ReturnType");

            return ret;
        }

        public ExceptionHandler  Copy(ExceptionHandler def)
        {
            var ret= new ExceptionHandler(def.HandlerType);
            if(def.CatchType != null)
                ret.CatchType = CopyReference(def.CatchType);

            Log("< ExceptionHandler ");
            CopyAll(def,ret,"CatchType");

            return ret;
        }

        public Instruction Copy(Instruction def)
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

            CopyMap.Add(def,ret);

            instructions.Add(ret);
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
            MergeAll(def1,def2,"Name","DeclaringType","FieldType","MetadataToken", "Module");
        }

        public void Merge(EventDefinition def1, EventDefinition def2)
        {
            MergeAll(def1,def2,"Name","DeclaringType","EventType","MetadataToken", "Module", "Attributes");
        }

        public void Merge(PropertyDefinition def1, PropertyDefinition def2)
        {
            MergeAll(def1,def2,"Name","DeclaringType","PropertyType","MetadataToken", "Module", "Attributes");
        }

        public void Merge(MethodDefinition def1, MethodDefinition def2)
        {
            MergeAll(def1,def2,"Name","DeclaringType","ReturnType","MetadataToken", "Module", "Attributes");
        }

        public void Merge(TypeDefinition def1, TypeDefinition def2)
        {
            MergeAll(def1,def2,"Name","DeclaringType","BaseType","MetadataToken", "Module");
        }

        public void Merge(ParameterDefinition def1, ParameterDefinition def2)
        {
            MergeAll(def1,def2,"Name", "Method","ParameterType","MetadataToken", "Module");
        }

        public void Merge(VariableDefinition def1, VariableDefinition def2)
        {
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
            MergeAll(def1,def2,"Method");
        }

        public void  Merge(MethodReturnType def1, MethodReturnType def2)
        {
            MergeAll(def1,def2,"Method","ReturnType");
        }

        public void  Merge(Collection<TypeReference>def1, Collection<TypeReference> def2)
        {
            foreach (var origTypeRef in def1)
            {
                if (def2.Any(i => i.FullName == origTypeRef.FullName))
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

                def2.Add(Copy(origDef));
            }
        }

        public void  Merge(Collection<MethodDefinition>def1, Collection<MethodDefinition> def2)
        {
            foreach (var origMethodDef in def1)
            {
                var destMethodName = origMethodDef.Name;

                var destMethodDef = def2.SingleOrDefault(
                    m => m.Name == destMethodName
                         && DoMethodParametersTypeMatch(origMethodDef, m));

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
            //TODO dont override destination instructions always, we migth gona need prepend
            def2.Clear();
            foreach(var i in def1){
                def2.Add(Copy(i));
            }
        }

        public void  Merge(Collection<ExceptionHandler>def1, Collection<ExceptionHandler> def2)
        {

        }

        public void  Merge(Collection<VariableDefinition>def1, Collection<VariableDefinition> def2)
        {
            //TODO dont override destination instructions always
            def2.Clear();
            foreach(var v in def1){
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
            //check if different
        }

        #endregion

        private bool DoMethodParametersTypeMatch(MethodDefinition method1, MethodDefinition method2)
        {
            if (method1.Parameters.Count != method2.Parameters.Count)
                return false;

            for (int i = 0; i < method1.Parameters.Count; i++)
            {
                if (method1.Parameters[i].ParameterType.FullName != method2.Parameters[i].ParameterType.FullName)
                    return false;
            }

            return true;
        }
    }
}
