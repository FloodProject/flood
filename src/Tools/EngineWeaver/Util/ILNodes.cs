using Mono.Cecil;
using Mono.Cecil.Cil;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DSLToolkit.Util
{
#if def
   class MSILtoCSharpConverter
    {
       
        public static CodeMemberMethod ToAST(MethodDefinition method)
        {
            Debug.WriteLine(method.Name);
            var methodDom = method.IsConstructor
                ? new CodeConstructor()
                : new CodeMemberMethod {Name = method.Name};

            methodDom.Parameters.AddRange(method.Parameters.Select(x => new CodeParameterDeclarationExpression(x.ParameterType.Name, x.Name)).ToArray());
            
            if (method.IsFamilyOrAssembly)
            {
                methodDom.Attributes = (methodDom.Attributes & ~MemberAttributes.AccessMask) | MemberAttributes.FamilyOrAssembly;
            }
            if (method.IsFamily)
            {
                methodDom.Attributes = (methodDom.Attributes & ~MemberAttributes.AccessMask) | MemberAttributes.Family;
            }
            if (method.IsPublic)
            {
                methodDom.Attributes = (methodDom.Attributes & ~MemberAttributes.AccessMask) | MemberAttributes.Public;
            }

            if(method.IsVirtual)
            {
                methodDom.Attributes = (methodDom.Attributes & ~MemberAttributes.ScopeMask) & ~MemberAttributes.Final;
            }

            methodDom.ReturnType = new CodeTypeReference(method.ReturnType.FullName);

            var parameterNames = new List<CodeExpression>();
            if (!method.IsStatic)
            {
                parameterNames.Add(new CodeThisReferenceExpression());
            }

            parameterNames.AddRange(method.Parameters.Select(x => new CodeVariableReferenceExpression(x.Name)));
            
            if (method.HasBody)
            {
                var variables = method.Body.Variables.ToDictionary(x => x.Index,
                                                               x =>
                                                               new CodeVariableDeclarationStatement(x.VariableType.FullName, x.Name));
                methodDom.Statements.AddRange(ToAST(method.Body.Instructions[0], parameterNames, variables).ToArray());
            }

            return methodDom;
        }

        public static CodeTypeDeclaration ToAST(TypeDefinition typeDefinition, params string[] excludeMethods)
        {
            var typeDeclaration = new CodeTypeDeclaration {Name = typeDefinition.Name};
            typeDeclaration.Members.AddRange(typeDefinition.Methods.Where(x => !excludeMethods.Contains(x.Name)) .Select(ToAST).ToArray());

            return typeDeclaration;
        }


        private static IEnumerable<CodeStatement> ToAST(Instruction method, IList<CodeExpression> parameters, IDictionary<int, CodeVariableDeclarationStatement> variables)
        {
            var stack = new Stack<CodeExpression>();
            while(method != null)
            {
                switch(method.OpCode.Code)
                {
                    #region Loading
                    case Code.Ldarg_0:
                        {
                            stack.Push(parameters[0]);
                            method = method.Next;
                            break;
                        }
                    case Code.Ldarg_1:
                        {
                            stack.Push(parameters[1]);
                            method = method.Next;
                            break;
                        }
                    case Code.Ldarg_2:
                        {
                            stack.Push(parameters[2]);
                            method = method.Next;
                            break;
                        }
                    case Code.Ldarg_3:
                        {
                            stack.Push(parameters[3]);
                            method = method.Next;
                            break;
                        }
                    case Code.Ldfld:
                        {
                            var x = stack.Pop();
                            var member = (MemberReference)method.Operand;
                            var expr = new CodeFieldReferenceExpression(x, member.Name);
                            stack.Push(expr);
                            method = method.Next;
                            break;
                        }
                    case Code.Ldc_I4_0:
                        {
                            stack.Push(new CodePrimitiveExpression(0));
                            method = method.Next;
                            break;
                        }
                    case Code.Ldc_I4_1:
                        {
                            stack.Push(new CodePrimitiveExpression(1));
                            method = method.Next;
                            break;
                        }
                    case Code.Ldc_I4_2:
                        {
                            stack.Push(new CodePrimitiveExpression(2));
                            method = method.Next;
                            break;
                        }
                    case Code.Ldc_I4_S:
                        {
                            stack.Push(new CodePrimitiveExpression(Convert.ToInt32(method.Operand)));
                            method = method.Next;
                            break;
                        }
                    case Code.Stloc_0:
                        {
                            var x = stack.Pop();
                            variables[0].InitExpression = x;
                            method = method.Next;
                            break;
                        }
                    case Code.Stloc_1:
                        {
                            var x = stack.Pop();
                            variables[1].InitExpression = x;
                            method = method.Next;
                            break;
                        }
                    case Code.Stloc_2:
                        {
                            var x = stack.Pop();
                            variables[2].InitExpression = x;
                            method = method.Next;
                            break;
                        }
                    case Code.Stloc_3:
                        {
                            var x = stack.Pop();
                            variables[3].InitExpression = x;
                            method = method.Next;
                            break;
                        }
                    case Code.Ldloc_0:
                        {
                            var x = variables[0].InitExpression;
                            stack.Push(x);
                            method = method.Next;
                            break;
                        }
                    case Code.Ldloc_1:
                        {
                            var x = variables[1].InitExpression;
                            stack.Push(x);
                            method = method.Next;
                            break;
                        }
                    case Code.Ldloc_2:
                        {
                            var x = variables[2].InitExpression;
                            stack.Push(x);
                            method = method.Next;
                            break;
                        }
                    case Code.Ldloc_3:
                        {
                            var x = variables[3].InitExpression;
                            stack.Push(x);
                            method = method.Next;
                            break;
                        }
                    case Code.Ldstr:
                        {
                            var expr = new CodePrimitiveExpression(method.Operand.ToString());
                            stack.Push(expr);
                            method = method.Next;
                            break;
                        }
                    case Code.Newobj:
                        {
                            var ctorMethod = (MethodDefinition)method.Operand;
                            var methodParams = ctorMethod.Parameters.Select(t => stack.Pop()).ToArray();
                            var expr = new CodeObjectCreateExpression(ctorMethod.DeclaringType.FullName, methodParams);
                            stack.Push(expr);
                            method = method.Next;
                            break;
                        }
                    case Code.Newarr:
                        {
                            var size = stack.Pop();
                            var type = (TypeDefinition) method.Operand;
                            var expr = new CodeArrayCreateExpression(type.FullName,size);
                            stack.Push(expr);
                            method = method.Next;
                            break;
                        }
                    case Code.Stelem_Ref:
                        {
                            var value = stack.Pop();
                            var index = stack.Pop();
                            var arr = stack.Pop();
                            yield return new CodeAssignStatement(new CodeArrayIndexerExpression(arr, index), value);
                            method = method.Next;
                            break;
                        }
                    #endregion

                    #region branching
                    case Code.Brfalse_S:
                        {
                            var x = stack.Pop();
                            var expr = new CodeBinaryOperatorExpression(x, CodeBinaryOperatorType.ValueEquality, new CodePrimitiveExpression(null));
                            var statement = new CodeConditionStatement(expr);
                            var falseMethod = method.Next;
                            var trueMethod = (Instruction) method.Operand;
                            statement.TrueStatements.AddRange(ToAST(trueMethod, parameters, variables).ToArray());
                            statement.FalseStatements.AddRange(ToAST(falseMethod, parameters, variables).ToArray());
                            yield return statement;
                            yield break;
                        }
                    case Code.Brtrue_S:
                        {
                            var x = stack.Pop();
                            var expr = new CodeBinaryOperatorExpression(x, CodeBinaryOperatorType.ValueEquality, new CodePrimitiveExpression(null));
                            var falseMethod = (Instruction) method.Operand;
                            var trueMethod = method.Next;
                            var statement = new CodeConditionStatement{Condition = expr};
                            statement.FalseStatements.AddRange(ToAST(falseMethod, parameters, variables).ToArray());
                            statement.TrueStatements.AddRange(ToAST(trueMethod, parameters, variables).ToArray());
                            yield return statement;
                            yield break;
                        }
                    case Code.Bne_Un_S:
                        {
                            var x = stack.Pop();
                            var y = stack.Pop();
                            var expr = new CodeBinaryOperatorExpression(x, CodeBinaryOperatorType.ValueEquality, y);
                            var falseMethod = (Instruction) method.Operand;
                            var trueMethod = method.Next;
                            var statement = new CodeConditionStatement(
                                condition: expr,
                                trueStatements: ToAST(trueMethod, parameters, variables).ToArray(),
                                falseStatements: ToAST(falseMethod, parameters, variables).ToArray());
                            yield return statement;
                            yield break;
                        }
                    case Code.Br_S:
                        {
                            method = (Instruction)method.Operand;
                            break;
                        }

                    #endregion
                    #region calling
                    case Code.Call:
                    case Code.Callvirt:
                        {
                            var member = (MethodDefinition)method.Operand;
                            var methodParams = member.Parameters.Select(t => stack.Pop()).ToList();
                            methodParams.Reverse();
                            if (member.IsStatic)
                            {
                                var type = new CodeTypeReferenceExpression(member.DeclaringType.FullName);
                                var expr = new CodeMethodInvokeExpression(type, member.Name, methodParams.ToArray());
                                stack.Push(expr);
                            }
                            else
                            {
                                var subject = stack.Pop();
                                var expr = new CodeMethodInvokeExpression(subject, member.Name, methodParams.ToArray());
                                stack.Push(expr);
                                //Debug.WriteLine(expr.ToText());
                            }
                            method = method.Next;
                            break;
                        }
                    #endregion
                    #region misc
                    case Code.Xor:
                        {
                            var right = stack.Pop();
                            var left = stack.Pop();
                            var expr = Xor(left, right);
                            stack.Push(expr);
                            method = method.Next;
                            break;
                        }
                    case Code.Ret:
                        {
                            if (stack.Count > 0)
                            {
                                var x = stack.Pop();
                                yield return new CodeMethodReturnStatement(x);
                            }
                            else
                            {
                                yield return new CodeMethodReturnStatement();
                            }
                            yield break;
                        }

                    case Code.Throw:
                        {
                            var x = stack.Pop();
                            var expr = new CodeThrowExceptionStatement(x);
                            yield return expr;
                            yield break;
                        }
                    case Code.Stind_Ref:
                        {
                            var x = stack.Pop();
                            var address = (CodeVariableReferenceExpression)stack.Pop();
                            var expr = new CodeAssignStatement(address, x);
                            yield return expr;
                            yield break;
                        }
                    #endregion

                    case Code.Ceq:
                        {
                            var x = stack.Pop();
                            var y = stack.Pop();
                            var expr = new CodeBinaryOperatorExpression(x, CodeBinaryOperatorType.ValueEquality, y);
                            stack.Push(expr);
                            method = method.Next;
                            break;
                        }
                    case Code.Pop:
                        {
                            yield return new CodeExpressionStatement(stack.Pop());
                            method = method.Next;
                            break;
                        }
                    default:
                        throw new NotImplementedException(method.OpCode.Code.ToString());
                }
            }
        }
    }
#endif
}
