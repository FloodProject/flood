using DSLToolkit.Util;
using ICSharpCode.Decompiler;
using ICSharpCode.Decompiler.Ast;
using ICSharpCode.Decompiler.ILAst;
using ICSharpCode.NRefactory.CSharp;
using Mono.Cecil;
using Mono.Cecil.Cil;
using Mono.Collections.Generic;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;

namespace DSLToolkit.Weaver
{

    class TypeWeaver
    {

        public TypeDefinition DestinationType { get; private set; }
        public TypeDefinition OriginType { get; private set; }

        //Member prefix name
        public string NamePrefix { get; set; }

        private readonly List<Instruction> instructions;

        private Dictionary<MemberReference,MemberReference> referenceMap;

        private DeepCopier copier;

        private static TypeDefinition GetTypeDef(Type type)
        {
            var assemblyPath = type.Assembly.Location;
            var assembly = AssemblyDefinition.ReadAssembly(assemblyPath);
            var module = assembly.MainModule;
            return module.Import(type).Resolve();
        }

        public TypeWeaver(TypeDefinition destinationTypeDef, Type originType):
            this(destinationTypeDef, GetTypeDef(originType))
        {}

        public TypeWeaver(TypeDefinition originTypeDef, TypeDefinition destinationTypeDef)
        {
            this.DestinationType = destinationTypeDef;
            this.OriginType = originTypeDef;
            this.NamePrefix = "";

            instructions = new List<Instruction>();
            referenceMap = new Dictionary<MemberReference,MemberReference>();
            copier = new DeepCopier(this);
        }

        public void ProcessNestedTypes() //TODO something better
        {
            Map(OriginType,DestinationType);
            foreach(var t in OriginType.NestedTypes.ToList()){
                AddDefinition(t);              
            }
        }

        public void ProcessInstructions()
        {
            
            CopyConstructorInitInstructions();
            
            FixGeneratedType();

            ProcessNewInstructions();

            copier.CopyMap.Clear();  //clear new copies
            referenceMap.Clear();    //clear references
        }

       

        public void Map(MemberReference origin,MemberReference destination)
        {
            CheckOriginMember(origin);
            CheckDestinationMember(destination);

            referenceMap.Add(origin,destination);
        }

        private void ProcessNewInstructions(){

            foreach(var i in instructions){
                if(i.Operand == null){
                    continue;
                }
                if(i.Operand is TypeReference){
                    i.Operand = CopyReference(copier,(TypeReference)i.Operand);
                    continue;
                }
                if(i.Operand is MethodReference){
                    i.Operand = CopyReference(copier,(MethodReference)i.Operand);
                    continue;
                }
                if(i.Operand is FieldReference){
                    i.Operand = CopyReference(copier,(FieldReference)i.Operand);
                    continue;
                }
                if(i.Operand is VariableReference   ||
                   i.Operand is ParameterDefinition ||
                   i.Operand is Instruction)
                {
                    i.Operand = copier.CopyMap[i.Operand];
                    continue;
                }
                if(i.Operand.GetType().IsValueType || i.Operand is String){
                    continue;
                }
                throw new NotImplementedException("No handling for operands of type "+i.Operand.GetType());
            }

            instructions.Clear();

            foreach(var method in DestinationType.Methods){
                var offset = 0;
			    foreach (var instruction in method.Body.Instructions) {
				    instruction.Offset = offset;
				    offset += instruction.GetSize ();
			    }
            }
        }

        private void CopyConstructorInitInstructions()
        {
            var constructors = GetConstructors(DestinationType, true);

            var visitor = new ObservableAstVisitor();

            bool isNewConstructor = false;
            visitor.EnterConstructorDeclaration += exp => {
                var constructor = exp.Annotation<MethodReference>();
                if(!copier.CopyMap.ContainsKey(constructor)){
                    isNewConstructor = true;
                }
            };
            visitor.LeaveConstructorDeclaration += exp => {
                isNewConstructor = false;
            };
            visitor.EnterConstructorInitializer += exp => {
                isNewConstructor = false;
            };
            visitor.EnterAssignmentExpression += exp => {
                var member = exp.Left.Annotation<MemberReference>();
                if(isNewConstructor){
                    if(copier.CopyMap.ContainsKey(member)){
                        var argILRange = exp.Annotation<List<ILRange>>();
                        var newInstructions = GetInstructions(exp);
                        foreach(var constructor in constructors){
                            var newInstruction = GetInstruction(newInstructions, argILRange[0].To);
                            newInstruction = newInstruction.Previous;
                            while(newInstruction != null && newInstruction.Offset >= argILRange[0].From){
                                constructor.Body.Instructions.Insert(0, copier.Copy(newInstruction));
                                newInstruction = newInstruction.Previous;
                            }
                        }
                        Console.WriteLine("Copied to "+constructors.Count+" constructor assigment of "+member);
                    }
                }
            };

           ProcessNewInstructions();

           var d = new DecompilerContext(OriginType.Module);
           d.CurrentType = OriginType;
           var astBuilder = new AstBuilder(d);
           astBuilder.AddType(OriginType);
           astBuilder.SyntaxTree.AcceptVisitor(visitor);
        }

        private List<MethodDefinition> GetConstructors(TypeDefinition type, bool isBaseInitializer){
            List<MethodDefinition> constructors = new List<MethodDefinition>();
            var visitor = new ObservableAstVisitor();
            visitor.EnterConstructorDeclaration += exp => {
                var constructor = exp.Annotation<MethodReference>();
                if(!isBaseInitializer || exp.Initializer.ConstructorInitializerType == ConstructorInitializerType.Base){
                    constructors.Add(constructor.Resolve());
                }
            };

           ProcessNewInstructions();

           var d = new DecompilerContext(type.Module);
           d.CurrentType = type;
           var astBuilder = new AstBuilder(d);
           astBuilder.AddType(type);
           astBuilder.SyntaxTree.AcceptVisitor(visitor);
           return constructors;
        }

        private void FixGeneratedType()
        {
            var d = new DecompilerContext(DestinationType.Module);
            d.CurrentType = DestinationType;
            var visitor = new ObservableAstVisitor();

            visitor.EnterInvocationExpression += exp => {
                var target = exp.Target;
                var methodInvoke = exp.Annotation<MethodReference>();
                if(methodInvoke==null)
                    return;
                for(var i = 0; i< exp.Arguments.Count; i++){
                    var arg = exp.Arguments.ToList()[i];
                    var argType = arg.Annotation<TypeInformation>().InferredType;
                    var param = methodInvoke.Parameters[i];
                    var paramType = param.ParameterType;
                    if(argType.IsValueType && !paramType.IsValueType){
                        var argILRange = arg.Annotation<List<ILRange>>();
                        var insts = GetInstructions(arg);
                        var inst = GetInstruction(insts, argILRange[0].To);
                        if(inst.Previous.OpCode.Code != Code.Box){
                            var box = Instruction.Create(OpCodes.Box,argType);
                            insts.Insert(insts.IndexOf(inst),box);
                            Console.WriteLine("Box inserted! Method Arg "+ param.Name + " " +argType + " to " + paramType);
                        }
                    }
                }
            };

           ProcessNewInstructions();

           var astBuilder = new AstBuilder(d);
           astBuilder.AddType(DestinationType);
           // astBuilder.RunTransformations();
           astBuilder.SyntaxTree.AcceptVisitor(visitor);
        }

        private Collection<Instruction> GetInstructions(AstNode node)
        {
            while(node.NodeType != NodeType.Member && !(node is Accessor)){
                node = node.Parent;
            }
            return  node.Annotation<MethodDefinition>().Body.Instructions;
        }
        private Instruction GetInstruction(Collection<Instruction> instructions, int offset)
        {
            var i = instructions[0];
            while(i.Offset < offset){
                i = i.Next;
            }
            if(i.Offset != offset)
                throw new ArgumentOutOfRangeException();

             return i;
        }

        
        

        # region DEFINITION MERGE

        public void Merge(FieldDefinition origin, FieldDefinition destination)
        {
            CheckOriginMember(origin);
            CheckDestinationMember(destination);
            var typeChange = MergeTypeChange(origin.FieldType,destination.FieldType);
            
            //TODO?
            //definitionsMap.Add(origin,destination);
        }

        public void Merge(EventDefinition origin, EventDefinition destination)
        {
            CheckOriginMember(origin);
            CheckDestinationMember(destination);
            var typeChange = MergeTypeChange(origin.EventType,destination.EventType);

            copier.Merge(origin,destination,OriginType);
        }

        public void Merge(MethodDefinition origin, MethodDefinition destination)
        {
            CheckOriginMember(origin);
            CheckDestinationMember(destination);
            var typeChange = MergeTypeChange(origin.ReturnType,destination.ReturnType);

        }

        public void Merge(PropertyDefinition origin, PropertyDefinition destination)
        {
            CheckOriginMember(origin);
            CheckDestinationMember(destination);
            var typeChange = MergeTypeChange(origin.PropertyType,destination.PropertyType);
           
            copier.Merge(origin,destination,OriginType);

        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private bool MergeTypeChange(TypeReference originType, TypeReference destinationType){
            if (originType.FullName != destinationType.FullName){
                if(originType.FullName.StartsWith("System.Object")){
                    return true;
                } else {
                    throw new ArgumentException(originType+" must be of type object.");
                }
            }
            return false;
        }

        # endregion



        public FieldDefinition AddDefinition(FieldDefinition def, string name = "")
        {
            CheckOriginMember(def);
            var ret = copier.Copy(def,DestinationType);
            DestinationType.Fields.Add(ret);
            if(name != "")
                ret.Name = name;
            return ret;
        }

        public EventDefinition AddDefinition(EventDefinition def, string name = "")
        {
            CheckOriginMember(def);
            var ret = copier.Copy(def,DestinationType);
            DestinationType.Events.Add(ret);
            if(name != "")
                ret.Name = name;
            return ret;
        }

        public PropertyDefinition AddDefinition(PropertyDefinition def, string name = "")
        {
            CheckOriginMember(def);
            var ret = copier.Copy(def,DestinationType);
            DestinationType.Properties.Add(ret);
            if(name != "")
                ret.Name = name;
            return ret;
        }

        public TypeDefinition AddDefinition(TypeDefinition def, string name = "")
        {
            CheckOriginMember(def);
            var ret = (TypeDefinition)copier.Copy(def,DestinationType);
            DestinationType.NestedTypes.Add(ret);
            if(name != "")
                ret.Name = name;
            return ret;
        }

        public MethodDefinition AddDefinition(MethodDefinition def, string name = "")
        {
            CheckOriginMember(def);
            var ret = (MethodDefinition)copier.Copy(def,DestinationType);
            DestinationType.Methods.Add(ret);
            if(name != "")
                ret.Name = name;
            return ret;
        }


        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private void CheckOriginMember(MemberReference member)
        {
            if(member.Module != OriginType.Module)
                throw new ArgumentException("Member "+member.Name+" from origin module");
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private void CheckDestinationMember(MemberReference member)
        {
            if(member.Module != DestinationType.Module)
                throw new ArgumentException("Member "+member.Name+" not from destination module");
        }



       # region COPY METHODS

        private FieldReference CopyReference(DeepCopier copier, FieldReference @ref)
        {
             if(@ref.DeclaringType.Scope == OriginType.Scope){
                if(copier.CopyMap.ContainsKey(@ref))
                    return  (FieldReference)copier.CopyMap[@ref];
                if(referenceMap.ContainsKey(@ref))
                    return  (FieldReference)referenceMap[@ref];
                throw new Exception("Cannot find Field "+@ref.FullName+". Try to Copy/Merge/Map it first.");
            }

            copier.Log("Importing "+@ref.FullName+ " from "+ @ref.DeclaringType.Scope.Name);
            return DestinationType.Module.Import(@ref);
        }

        private MethodReference CopyReference(DeepCopier copier, MethodReference @ref)
        {
            if(@ref.DeclaringType.Scope == OriginType.Scope){
                if(copier.CopyMap.ContainsKey(@ref))
                    return  (MethodReference)copier.CopyMap[@ref];
                if(referenceMap.ContainsKey(@ref))
                    return  (MethodReference)referenceMap[@ref];
                throw new Exception("Cannot find Method "+@ref.FullName+". Try to Copy/Merge/Map it first.");
            }

            copier.Log("Importing "+@ref.FullName+ " from "+ @ref.DeclaringType.Scope.Name);
            return DestinationType.Module.Import(@ref);
        }

        private TypeReference CopyReference(DeepCopier copier, TypeReference @ref)
        {
            if(@ref.Scope == OriginType.Scope){
                if(copier.CopyMap.ContainsKey(@ref))
                    return  (TypeReference)copier.CopyMap[@ref];
                if(referenceMap.ContainsKey(@ref))
                    return  (TypeReference)referenceMap[@ref];
                throw new Exception("Cannot find Type "+@ref.FullName+". Try to Copy/Merge/Map it first.");
            }

            copier.Log("Importing "+@ref.FullName+ " from "+ @ref.Scope.Name);
            return DestinationType.Module.Import(@ref);
        }

        private GenericInstanceType CopyReference(DeepCopier copier, GenericInstanceType def)
        {
            return CopyReference(copier,def);
        }


        /*
         * COPY DEFINITIONS
         */

        private FieldDefinition Copy(DeepCopier copier, FieldDefinition def, TypeDefinition declaringType)
        {
            var ret = new FieldDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(copier,def.FieldType));

            copier.Log("< Field "+ret.FullName);
            copier.CopyAll(def,ret,declaringType,"Name","DeclaringType","FieldType","MetadataToken");

            return ret;
        }

        private EventDefinition Copy(DeepCopier copier, EventDefinition def, TypeDefinition declaringType)
        {
            var ret = new EventDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(copier,def.EventType));

            copier.Log("< Event "+ret.FullName);
            copier.CopyAll(def,ret,declaringType,"Name","DeclaringType","EventType","MetadataToken");

            if(ret.RemoveMethod != null) declaringType.Methods.Add(ret.RemoveMethod); 
            if(ret.AddMethod != null)    declaringType.Methods.Add(ret.AddMethod); 

            return ret;
        }

        private PropertyDefinition Copy(DeepCopier copier, PropertyDefinition def, TypeDefinition declaringType)
        {
            var ret = new PropertyDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(copier,def.PropertyType));
            
            copier.Log("< Property "+ret.FullName);
            copier.CopyAll(def,ret,declaringType,"Name","DeclaringType","PropertyType","MetadataToken");

            if(ret.GetMethod != null) declaringType.Methods.Add(ret.GetMethod); 
            if(ret.SetMethod != null) declaringType.Methods.Add(ret.SetMethod); 

            return ret;
        }

        private MethodDefinition Copy(DeepCopier copier, MethodDefinition def, TypeDefinition declaringType)
        {
            var ret = new MethodDefinition(NamePrefix+def.Name, def.Attributes, CopyReference(copier,def.ReturnType));
            //ret.DeclaringType = DestinationType;

            copier.Log("< Method "+ret.FullName);
            copier.CopyAll(def,ret,ret,"Name","DeclaringType","ReturnType","MetadataToken");

            return ret;
        }

        private TypeDefinition Copy(DeepCopier copier, TypeDefinition def, TypeDefinition declaringType)
        {
            var ret = new TypeDefinition("", NamePrefix+def.Name, def.Attributes, CopyReference(copier,def.BaseType));
            //ret.Scope = declaringType.Scope;

            copier.Log("> Type "+def.FullName);
            copier.Log("< Type "+ret.FullName);
            copier.CopyAll(def,ret,ret,"Name","DeclaringType","BaseType","MetadataToken","Scope");

            return ret;
        }

        private ParameterDefinition Copy(DeepCopier copier, ParameterDefinition def)
        {
            var ret = new ParameterDefinition(def.Name, def.Attributes, CopyReference(copier,def.ParameterType));

            copier.Log("< Parameter " + ret.Name);
            copier.CopyAll(def,ret,ret,"Method","ParameterType","MetadataToken");

            return ret;
        }

        private VariableDefinition Copy(DeepCopier copier, VariableDefinition def)
        {
            var ret = new VariableDefinition(def.Name, CopyReference(copier,def.VariableType));
            
            copier.Log("< Variable " + ret.Name);
            copier.CopyAll(def,ret,ret,"VariableType");

            return ret;
        }


        /*
         * COPIES
         */

        private CustomAttribute Copy(DeepCopier copier, CustomAttribute def)
        {
            var ret = new CustomAttribute(CopyReference(copier,def.Constructor),def.GetBlob());

            copier.Log("< CopyAttributes ");
            copier.CopyAll(def,ret,ret,"DeclaringType");

            return ret;
        }

        private Mono.Cecil.Cil.MethodBody Copy(DeepCopier copier, Mono.Cecil.Cil.MethodBody def, MethodDefinition parent)
        {
            var ret = new Mono.Cecil.Cil.MethodBody(parent);

            copier.Log("< MethodBody ");
            copier.CopyAll(def,ret,ret,"Method");

            return ret;
        }

        private MethodReturnType  Copy(DeepCopier copier, MethodReturnType def, IMethodSignature parent)
        {
            var ret= new MethodReturnType(parent);
            ret.ReturnType = CopyReference(copier,def.ReturnType);

            copier.Log("< MethodReturnType ");
            copier.CopyAll(def,ret,ret,"Method","ReturnType");

            return ret;
        }

        private ExceptionHandler  Copy(DeepCopier copier, ExceptionHandler def)
        {
            var ret= new ExceptionHandler(def.HandlerType);
            if(def.CatchType != null)
                ret.CatchType = CopyReference(copier,def.CatchType);

            copier.Log("< ExceptionHandler ");
            copier.CopyAll(def,ret,ret,"CatchType");

            return ret;
        }

        private Instruction Copy(DeepCopier copier, Instruction def)
        {
            //copy of operands is delayed to ProcessInstructions
            Instruction ret= null;
            if(def.Operand == null){
                ret = Instruction.Create(def.OpCode);
            } else {
                var methodInfo = typeof(Instruction).GetMethod("Create",new Type[]{def.OpCode.GetType(),def.Operand.GetType()});
                ret = (Instruction) methodInfo.Invoke(null,new object[]{def.OpCode,def.Operand});
            }
            instructions.Add(ret);
            return ret;
        }

        private System.Byte[] Copy(DeepCopier copier, System.Byte[] value){
            return value;
        }

        #endregion


        # region MERGE METHODS


        private void MergeReference(DeepCopier copier, FieldReference @ref1, FieldReference @ref2)
        {
            
        }

        private void MergeReference(DeepCopier copier, MethodReference @ref1, MethodReference @re2)
        {
            
        }

        private void MergeReference(DeepCopier copier, PropertyReference @ref1, PropertyReference @re2)
        {
            
        }

        private void MergeReference(DeepCopier copier, TypeReference @ref1, TypeReference @re2)
        {
            
        }

        private void Merge(DeepCopier copier, FieldDefinition def1, FieldDefinition def2)
        {
            copier.MergeAll(def1,def2,def2,"Name","DeclaringType","FieldType","MetadataToken", "Module");
        }

        private void Merge(DeepCopier copier, EventDefinition def1, EventDefinition def2)
        {
            copier.MergeAll(def1,def2,def2,"Name","DeclaringType","EventType","MetadataToken", "Module", "Attributes");
        }

        private void Merge(DeepCopier copier, PropertyDefinition def1, PropertyDefinition def2)
        {
            copier.MergeAll(def1,def2,def2,"Name","DeclaringType","PropertyType","MetadataToken", "Module", "Attributes");
        }

        private void Merge(DeepCopier copier, MethodDefinition def1, MethodDefinition def2)
        {
            copier.MergeAll(def1,def2,def2,"Name","DeclaringType","ReturnType","MetadataToken", "Module", "Attributes");
        }

        private void Merge(DeepCopier copier, TypeDefinition def1, TypeDefinition def2)
        {
            copier.MergeAll(def1,def2,def2,"Name","DeclaringType","BaseType","MetadataToken", "Module");
        }

        private void Merge(DeepCopier copier, ParameterDefinition def1, ParameterDefinition def2)
        {
            copier.MergeAll(def1,def2,def2,"Name", "Method","ParameterType","MetadataToken", "Module");
        }

        private void Merge(DeepCopier copier, VariableDefinition def1, VariableDefinition def2)
        {
            copier.MergeAll(def1,def2,def2,"VariableType");
        }


        private void Merge(DeepCopier copier, CustomAttribute def1, CustomAttribute def2)
        {
            copier.MergeAll(def1,def2,def2,"DeclaringType");
        }

        private void Merge(DeepCopier copier, GenericInstanceType def1, GenericInstanceType def2)
        {
           
        }

        private void Merge(DeepCopier copier, Mono.Cecil.Cil.MethodBody def1, Mono.Cecil.Cil.MethodBody def2)
        {
            copier.MergeAll(def1,def2,def2,"Method");
        }

        private void  Merge(DeepCopier copier, MethodReturnType def1, MethodReturnType def2)
        {
            copier.MergeAll(def1,def2,def2,"Method","ReturnType");
        }
/*
        private void  Merge(DeepCopier copier, ExceptionHandler def1, ExceptionHandler def2)
        {
            copier.MergeAll(def1,def2,def2,"CatchType");
        }

        private void Merge(DeepCopier copier, Instruction def, Instruction def)
        {
            if(!instructions.Contains(def)){
                if( def.Operand is MemberReference){
                    instructions.Add(def);
                }
            }
            return def;
        }
*/

        private void  Merge(DeepCopier copier, Collection<SecurityDeclaration>def1, Collection<SecurityDeclaration> def2)
        {

        }

        private void  Merge(DeepCopier copier, Collection<CustomAttribute>def1, Collection<CustomAttribute> def2)
        {

        }

        private void  Merge(DeepCopier copier, Collection<Instruction>def1, Collection<Instruction> def2)
        {
            //TODO dont override destination instructions always, we migth gona need prepend
            def2.Clear();
            foreach(var i in def1){
                def2.Add(copier.Copy(i));
            }
        }

        private void  Merge(DeepCopier copier, Collection<ExceptionHandler>def1, Collection<ExceptionHandler> def2)
        {

        }

        private void  Merge(DeepCopier copier, Collection<VariableDefinition>def1, Collection<VariableDefinition> def2)
        {
            //TODO dont override destination instructions always
            def2.Clear();
            foreach(var v in def1){
                def2.Add(copier.Copy(v));
            }
        }

        private void  Merge(DeepCopier copier, Collection<MethodReference>def1, Collection<MethodReference> def2)
        {

        }

        private void  Merge(DeepCopier copier, Collection<MethodDefinition>def1, Collection<MethodDefinition> def2)
        {

        }

        private void  Merge(DeepCopier copier, Collection<GenericParameter>def1, Collection<GenericParameter> def2)
        {

        }

         private void  Merge(DeepCopier copier, Collection<ParameterDefinition>def1, Collection<ParameterDefinition> def2)
        {
            //check if different
        }

        #endregion


    }

}
