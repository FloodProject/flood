using EngineWeaver.Util;
using Mono.Cecil;
using Mono.Cecil.Cil;
using Mono.Collections.Generic;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EngineWeaver
{
    public class CecilCopier : DeepCopier
    {
        private ModuleDefinition originModule;
        private ModuleDefinition destinationModule;
        private readonly Dictionary<MemberReference,MemberReference> referenceMap;

        private readonly List<Instruction> instructions;
        private readonly Dictionary<TypeDefinition,TypeDefinition> types;

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
                CopyAll(t.Key,t.Value,t.Value,"Name","DeclaringType","BaseType","MetadataToken","Scope", "NestedTypes");
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

        private FieldReference CopyReference(FieldReference @ref)
        {
             if(IsLocalReference(@ref.DeclaringType.Scope.Name))
                return GetLocalReference(@ref);

            Log("Importing "+@ref.FullName+ " from "+ @ref.DeclaringType.Scope.Name);
            return destinationModule.Import(@ref);
        }

        private MethodReference CopyReference(MethodReference @ref)
        {
            if(IsLocalReference(@ref.DeclaringType.Scope.Name))
                return GetLocalReference(@ref);

            Log("Importing "+@ref.FullName+ " from "+ @ref.DeclaringType.Scope.Name);

            // Fix bad imports of generic arguments.
            CheckGenericArguments(@ref);

            return destinationModule.Import(@ref);
        }

        private TypeReference CopyReference(TypeReference @ref)
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

        private FieldDefinition Copy(FieldDefinition def, TypeDefinition declaringType)
        {
            var ret = new FieldDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.FieldType));

            Log("< Field "+ret.FullName);
            CopyAll(def,ret,declaringType,"Name","DeclaringType","FieldType","MetadataToken");

            return ret;
        }

        private void AddMethod(TypeDefinition type, MethodDefinition method)
        {
            if(type.Methods.Any(m => m.FullName == method.FullName))
                return;

            type.Methods.Add(method); 
        }

        private EventDefinition Copy(EventDefinition def, TypeDefinition declaringType)
        {
            var ret = new EventDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.EventType));

            Log("< Event "+ret.FullName);
            CopyAll(def,ret,declaringType,"Name","DeclaringType","EventType","MetadataToken");

            if(ret.RemoveMethod != null) AddMethod(declaringType, ret.RemoveMethod);
            if(ret.AddMethod    != null) AddMethod(declaringType, ret.AddMethod);

            return ret;
        }

        private PropertyDefinition Copy(PropertyDefinition def, TypeDefinition declaringType)
        {
            var ret = new PropertyDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.PropertyType));
            
            Log("< Property "+ret.FullName);
            CopyAll(def,ret,declaringType,"Name","DeclaringType","PropertyType","MetadataToken");

            if(ret.GetMethod != null) AddMethod(declaringType, ret.GetMethod);
            if(ret.SetMethod != null) AddMethod(declaringType, ret.SetMethod);

            return ret;
        }

        private MethodDefinition Copy(MethodDefinition def, TypeDefinition declaringType)
        {
            var ret = declaringType.Methods.FirstOrDefault(m => m.FullName == def.FullName);
            if(ret != null)
                return ret;

            ret = new MethodDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(def.ReturnType));

            Log("< Method "+ret.FullName);
            CopyAll(def,ret,ret,"Name","DeclaringType","ReturnType","MetadataToken");

            return ret;
        }

        private TypeDefinition Copy(TypeDefinition def, TypeDefinition declaringType)
        {
            var typeCollection = (declaringType != null)? declaringType.NestedTypes : destinationModule.Types;
            var ret = typeCollection.FirstOrDefault( t => t.FullName == def.FullName);
            if (ret != null)
            {
                Log("Cannot copy existing type "+ def.FullName);
                return ret;
            }

            ret = new TypeDefinition(def.Namespace, NamePrefix+def.Name, def.Attributes, CopyReference(def.BaseType));

            typeCollection.Add(ret);

            Map(def,ret);
            types.Add(def, ret);

            Log("> Type "+def.FullName+" delayed copy");

            foreach(var nestedType in def.NestedTypes)
                Copy(nestedType, ret);

            return ret;
        }

        private ParameterDefinition Copy(ParameterDefinition def)
        {
            var ret = new ParameterDefinition(def.Name, def.Attributes, CopyReference(def.ParameterType));

            Log("< Parameter " + ret.Name);
            CopyAll(def,ret,ret,"Method","ParameterType","MetadataToken");

            return ret;
        }

        private VariableDefinition Copy(VariableDefinition def)
        {
            var ret = new VariableDefinition(def.Name, CopyReference(def.VariableType));
            
            Log("< Variable " + ret.Name);
            CopyAll(def,ret,ret,"VariableType");

            return ret;
        }


        /*
         * COPIES
         */

        private CustomAttribute Copy(CustomAttribute def)
        {
            var ret = new CustomAttribute(CopyReference(def.Constructor),def.GetBlob());

            Log("< CopyAttributes ");
            CopyAll(def,ret,ret,"DeclaringType");

            return ret;
        }

        private CustomAttributeArgument Copy(CustomAttributeArgument def)
        {
            return new CustomAttributeArgument(CopyReference(def.Type), def.Value);
        }

        private MethodBody Copy(MethodBody def, MethodDefinition parent)
        {
            var ret = new MethodBody(parent);

            Log("< MethodBody ");
            CopyAll(def,ret,ret,"Method","Scope");

            return ret;
        }

        private MethodReturnType  Copy(MethodReturnType def, IMethodSignature parent)
        {
            var ret= new MethodReturnType(parent);
            ret.ReturnType = CopyReference(def.ReturnType);

            Log("< MethodReturnType ");
            CopyAll(def,ret,ret,"Method","ReturnType");

            return ret;
        }

        private ExceptionHandler  Copy(ExceptionHandler def)
        {
            var ret= new ExceptionHandler(def.HandlerType);
            if(def.CatchType != null)
                ret.CatchType = CopyReference(def.CatchType);

            Log("< ExceptionHandler ");
            CopyAll(def,ret,ret,"CatchType");

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

            instructions.Add(ret);
            return ret;
        }

        private System.Byte[] Copy(System.Byte[] value){
            return value;
        }

        #endregion

        # region MERGE METHODS

        private void MergeReference(FieldReference @ref1, FieldReference @ref2)
        {
            
        }

        private void MergeReference(MethodReference @ref1, MethodReference @re2)
        {
            
        }

        private void MergeReference(PropertyReference @ref1, PropertyReference @re2)
        {
            
        }

        private void MergeReference(TypeReference @ref1, TypeReference @re2)
        {
            
        }

        private void Merge(FieldDefinition def1, FieldDefinition def2)
        {
            MergeAll(def1,def2,def2,"Name","DeclaringType","FieldType","MetadataToken", "Module");
        }

        private void Merge(EventDefinition def1, EventDefinition def2)
        {
            MergeAll(def1,def2,def2,"Name","DeclaringType","EventType","MetadataToken", "Module", "Attributes");
        }

        private void Merge(PropertyDefinition def1, PropertyDefinition def2)
        {
            MergeAll(def1,def2,def2,"Name","DeclaringType","PropertyType","MetadataToken", "Module", "Attributes");
        }

        private void Merge(MethodDefinition def1, MethodDefinition def2)
        {
            MergeAll(def1,def2,def2,"Name","DeclaringType","ReturnType","MetadataToken", "Module", "Attributes");
        }

        private void Merge(TypeDefinition def1, TypeDefinition def2)
        {
            MergeAll(def1,def2,def2,"Name","DeclaringType","BaseType","MetadataToken", "Module");
        }

        private void Merge(ParameterDefinition def1, ParameterDefinition def2)
        {
            MergeAll(def1,def2,def2,"Name", "Method","ParameterType","MetadataToken", "Module");
        }

        private void Merge(VariableDefinition def1, VariableDefinition def2)
        {
            MergeAll(def1,def2,def2,"VariableType");
        }


        private void Merge(CustomAttribute def1, CustomAttribute def2)
        {
            MergeAll(def1,def2,def2,"DeclaringType");
        }

        private void Merge(GenericInstanceType def1, GenericInstanceType def2)
        {
           
        }

        private void Merge(Mono.Cecil.Cil.MethodBody def1, Mono.Cecil.Cil.MethodBody def2)
        {
            MergeAll(def1,def2,def2,"Method");
        }

        private void  Merge(MethodReturnType def1, MethodReturnType def2)
        {
            MergeAll(def1,def2,def2,"Method","ReturnType");
        }
/*
        private void  Merge(ExceptionHandler def1, ExceptionHandler def2)
        {
            MergeAll(def1,def2,def2,"CatchType");
        }

        private void Merge(Instruction def, Instruction def)
        {
            if(!instructions.Contains(def)){
                if(def.Operand is MemberReference){
                    instructions.Add(def);
                }
            }
            return def;
        }
*/

        private void  Merge(Collection<SecurityDeclaration>def1, Collection<SecurityDeclaration> def2)
        {

        }

        private void  Merge(Collection<CustomAttribute>def1, Collection<CustomAttribute> def2)
        {

        }

        private void  Merge(Collection<Instruction>def1, Collection<Instruction> def2)
        {
            //TODO dont override destination instructions always, we migth gona need prepend
            def2.Clear();
            foreach(var i in def1){
                def2.Add(Copy(i));
            }
        }

        private void  Merge(Collection<ExceptionHandler>def1, Collection<ExceptionHandler> def2)
        {

        }

        private void  Merge(Collection<VariableDefinition>def1, Collection<VariableDefinition> def2)
        {
            //TODO dont override destination instructions always
            def2.Clear();
            foreach(var v in def1){
                def2.Add(Copy(v));
            }
        }

        private void  Merge(Collection<MethodReference>def1, Collection<MethodReference> def2)
        {

        }

        private void  Merge(Collection<MethodDefinition>def1, Collection<MethodDefinition> def2)
        {

        }

        private void  Merge(Collection<GenericParameter>def1, Collection<GenericParameter> def2)
        {

        }

         private void  Merge(Collection<ParameterDefinition>def1, Collection<ParameterDefinition> def2)
        {
            //check if different
        }

        #endregion

    }
}
