using Weaver.Util;
using Mono.Cecil;
using Mono.Cecil.Cil;
using System.Collections.Generic;
using System.Reflection;

using FieldAttributes = Mono.Cecil.FieldAttributes;
using ParameterAttributes = Mono.Cecil.ParameterAttributes;
using PropertyAttributes = Mono.Cecil.PropertyAttributes;

namespace Weaver
{
    public class FieldsToProperties
    {
        private readonly AssemblyWeaver weaver;
        private readonly Dictionary<FieldReference, PropertyDefinition> fieldProperties;

        public FieldsToProperties(string assemblyPath)
        {
            weaver = new AssemblyWeaver(assemblyPath);
            fieldProperties = new Dictionary<FieldReference, PropertyDefinition>();
        }

        public void ProcessFields(List<FieldInfo> fields)
        {
            foreach (var field in fields)
            {
                var typeDef = CecilUtils.GetTypeDef(weaver.TargetModule, new TypeSignature(field.DeclaringType));
                var fieldDef = CecilUtils.GetTypeFieldDef(typeDef, new FieldSignature(field));

                var propertyDef = AddProperty(typeDef, fieldDef.Name, fieldDef.FieldType);

                foreach (var attribute in fieldDef.CustomAttributes)
                    propertyDef.CustomAttributes.Add(attribute);

                fieldProperties.Add(fieldDef, propertyDef);
            }
        }

        public void Write(string outputPath)
        {
            foreach(var type in weaver.TargetModule.Types)
                ProcessTypeInstructions(type);

            foreach (var field in fieldProperties.Keys)
                field.DeclaringType.Resolve().Fields.Remove(field.Resolve());

            weaver.Write(outputPath);
        }

        private void ProcessTypeInstructions(TypeDefinition type)
        {
            foreach(var method in type.Methods)
            {
                if(method.Body == null)
                    continue;

                foreach (var instruction in method.Body.Instructions)
                {
                    var code = instruction.OpCode.Code;
                    if (code != Code.Stfld && code != Code.Ldfld)
                        continue;

                    PropertyDefinition propertyDef;
                    if (!fieldProperties.TryGetValue((FieldReference) instruction.Operand, out propertyDef))
                        continue;

                    instruction.OpCode = OpCodes.Call;
                    instruction.Operand = (code == Code.Stfld)? propertyDef.SetMethod : propertyDef.GetMethod;
                }
            }

            foreach (var nestedType in type.NestedTypes)
                ProcessTypeInstructions(nestedType);
        }

        private static PropertyDefinition AddProperty(TypeDefinition declaringTypeDef, string propertyName, TypeReference propertyTypeDef)
        {
            //Import the void type 
            var voidRef = declaringTypeDef.Module.Import(typeof(void)); 

            //define the field we store the value in 
            var field = new FieldDefinition(ConvertToFieldName(propertyName)
                , FieldAttributes.Private
                , propertyTypeDef); 
            declaringTypeDef.Fields.Add(field); 

            //Create the get method 
            var get = new MethodDefinition("get_" + propertyName
                , Mono.Cecil.MethodAttributes.Public 
                | Mono.Cecil.MethodAttributes.SpecialName
                | Mono.Cecil.MethodAttributes.HideBySig
                , propertyTypeDef); 

            var getIl = get.Body.GetILProcessor();
            getIl.Append(getIl.Create(OpCodes.Ldarg_0)); 
            getIl.Append(getIl.Create(OpCodes.Ldfld, field)); 
            getIl.Append(getIl.Create(OpCodes.Stloc_0)); 
            Instruction inst = getIl.Create(OpCodes.Ldloc_0); 
            getIl.Append(getIl.Create(OpCodes.Br_S, inst)); 
            getIl.Append(inst); 
            getIl.Append(getIl.Create(OpCodes.Ret)); 
            get.Body.Variables.Add(new VariableDefinition("V_0", propertyTypeDef)); 
            get.Body.InitLocals = true; 
            get.SemanticsAttributes = MethodSemanticsAttributes.Getter; 
            declaringTypeDef.Methods.Add(get); 

            //Create the set method 
            var set = new MethodDefinition("set_" + propertyName
                , Mono.Cecil.MethodAttributes.Public
                | Mono.Cecil.MethodAttributes.SpecialName
                | Mono.Cecil.MethodAttributes.HideBySig
                , voidRef); 

            var setIl = set.Body.GetILProcessor();
            setIl.Append(setIl.Create(OpCodes.Ldarg_0)); 
            setIl.Append(setIl.Create(OpCodes.Ldarg_1)); 
            setIl.Append(setIl.Create(OpCodes.Stfld, field)); 
            setIl.Append(setIl.Create(OpCodes.Ret)); 
            set.Parameters.Add(new ParameterDefinition("value", ParameterAttributes.None, propertyTypeDef)); 
            set.SemanticsAttributes = MethodSemanticsAttributes.Setter; 
            declaringTypeDef.Methods.Add(set); 

            //create the property 
            var propertyDefinition = new PropertyDefinition(propertyName, PropertyAttributes.None, propertyTypeDef);
            propertyDefinition.GetMethod = get;
            propertyDefinition.SetMethod = set;

            //add the property to the type. 
            declaringTypeDef.Properties.Add(propertyDefinition);

            return propertyDefinition;
        } 

        private static string ConvertToFieldName(string propertyName) 
        { 
            var fieldName = new System.Text.StringBuilder(); 
            fieldName.Append("_"); 
            fieldName.Append(propertyName[0].ToString().ToLower()); 
            if (propertyName.Length > 1) 
                fieldName.Append(propertyName.Substring(1)); 

            return fieldName.ToString(); 
        }
    }
}
