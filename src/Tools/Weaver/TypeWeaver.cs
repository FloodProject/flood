using EngineWeaver.Util;
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

namespace EngineWeaver
{

    public class TypeWeaver
    {

        public TypeDefinition DestinationType { get; private set; }
        public TypeDefinition OriginType { get; private set; }

        private readonly Dictionary<FieldReference, object> constants; 

        private readonly CecilCopier copier;

        public TypeWeaver(TypeDefinition originTypeDef, TypeDefinition destinationTypeDef, CecilCopier copier)
        {
            this.DestinationType = destinationTypeDef;
            this.OriginType = originTypeDef;
            this.copier = copier;

            constants = new Dictionary<FieldReference, object>();
        }

        public void ProcessNestedTypes() //TODO something better
        {
            copier.Map(OriginType,DestinationType);
            foreach(var t in OriginType.NestedTypes.ToList()){
                AddDefinition(t);              
            }
        }

        public void ProcessInstructions()
        {
            ReplaceInstructionsWithConstant();

            CopyConstructorInitInstructions();
            
            FixGeneratedType();

            copier.Process();

            copier.Clear();
            constants.Clear();       //clear constants
        }

        private void ReplaceInstructionsWithConstant()
        {
            foreach(var i in copier.CopyMap.Values.OfType<Instruction>())
            {
                var fieldRef = i.Operand as FieldReference;
                if (fieldRef == null || !constants.ContainsKey(fieldRef))
                    continue;

                var opCode = i.OpCode;
                if (opCode != OpCodes.Ldfld && opCode != OpCodes.Ldsfld)
                    throw new Exception(string.Format("Can't apply operation {0} to constant {1},"+
                        " only ldfld can be processed.", opCode,fieldRef));

                var constant = constants[fieldRef];

                Console.WriteLine("Replacing field {0} by constant value {1}", fieldRef,constant);

                i.Operand = constant;

                if (constant is int)
                {
                    i.OpCode = OpCodes.Ldc_I4;
                    continue;
                }
                if (constant is float)
                {
                    i.OpCode = OpCodes.Ldc_R4;
                    continue;
                }
                if (constant is string)
                {
                    i.OpCode = OpCodes.Ldstr;
                    continue;
                }

                throw new NotImplementedException(
                    "Constants of type " + constant.GetType().FullName +" not implemented.");
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

           copier.Process();

           var d = new DecompilerContext(OriginType.Module) {CurrentType = OriginType};
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

           copier.Process();

           var d = new DecompilerContext(type.Module) {CurrentType = type};
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

           copier.Process();

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

        # region Constants

        public void AddConstant(FieldDefinition def, int constant)
        {
            AddConstant2(def, constant);
        }
        public void AddConstant(FieldDefinition def, float constant)
        {
            AddConstant2(def, constant);
        }
        public void AddConstant(FieldDefinition def, string constant)
        {
            AddConstant2(def, constant);
        }

        private void AddConstant2(FieldDefinition def, object constant)
        {
            CheckOriginMember(def);
            if(def.FieldType.FullName != constant.GetType().FullName)
                throw new ArgumentException(string.Format("Field {0} is not of type {1}",def, constant.GetType().FullName));
            
            constants.Add(def,constant);
        }

        #endregion

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

            copier.Merge(origin,destination);
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
           
            copier.Merge(origin,destination);

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
            var ret = copier.Copy(def);
            DestinationType.Fields.Add(ret);
            if(name != "")
                ret.Name = name;
            return ret;
        }

        public EventDefinition AddDefinition(EventDefinition def, string name = "")
        {
            CheckOriginMember(def);
            var ret = copier.Copy(def);
            DestinationType.Events.Add(ret);
            if(name != "")
                ret.Name = name;
            return ret;
        }

        public PropertyDefinition AddDefinition(PropertyDefinition def, string name = "")
        {
            CheckOriginMember(def);
            var ret = copier.Copy(def);
            DestinationType.Properties.Add(ret);
            if(name != "")
                ret.Name = name;
            return ret;
        }

        public TypeDefinition AddDefinition(TypeDefinition def, string name = "")
        {
            CheckOriginMember(def);
            var ret = (TypeDefinition)copier.Copy(def);
            DestinationType.NestedTypes.Add(ret);
            if(name != "")
                ret.Name = name;
            return ret;
        }

        public MethodDefinition AddDefinition(MethodDefinition def, string name = "")
        {
            CheckOriginMember(def);
            var ret = (MethodDefinition)copier.Copy(def);
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


    }

}
