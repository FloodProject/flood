using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Mono.Cecil;

namespace Weaver.Util
{
    public class MemberSignature
    {
        public readonly TypeSignature DeclaringType;
        public readonly string Name;

        protected MemberSignature(TypeSignature declaringType, string name)
        {
            DeclaringType = declaringType;
            Name = name;
        }

        private const string  memberSeparator = "::";
        public virtual string MemberSeparator { get { return memberSeparator; }}

        public override string ToString()
        {
            if (DeclaringType == null)
                return Name;

            return string.Format("{0}{1}{2}", DeclaringType, MemberSeparator, Name);
        }

        public override bool Equals(object obj)
        {
            var member = obj as MemberSignature;
            if (member == null)
                return false;

            return member.Name == Name && member.DeclaringType == DeclaringType;
        }

        public override int GetHashCode()
        {
            if (DeclaringType == null)
                return Name.GetHashCode();

            return Name.GetHashCode() ^ DeclaringType.GetHashCode();
        }

        public static bool operator ==(MemberSignature m1, MemberSignature m2)
        {
            if (ReferenceEquals(m1, m2))
                return true;

            if (((object) m1 == null) || ((object) m2 == null))
                return false;

            return m1.Equals(m2);
        }

        public static bool operator !=(MemberSignature m1, MemberSignature m2)
        {
            return !(m1 == m2);
        }

        public static MemberSignature Parse(string s, string separator = memberSeparator)
        {
            var parts = s.Split(new []{separator},1,StringSplitOptions.None);
            
            if(parts.Length == 1)
                return new MemberSignature(null, parts[0]);

            if(parts.Length == 2)
                return new MemberSignature(TypeSignature.Parse(parts[0]), parts[1]);

            throw new Exception();
        }
    }

    public class TypeSignatureCollection
    {
        public readonly TypeSignature[] Types;

        public TypeSignatureCollection(IList<Type> types)
        {
            Types = new TypeSignature[types.Count()];
            for (int i = 0; i < types.Count(); i++)
                Types[i] = new TypeSignature(types[i]);
        }

        public TypeSignatureCollection(IList<TypeReference> types)
        {
            Types = new TypeSignature[types.Count()];
            for (int i = 0; i < types.Count(); i++)
                Types[i] = new TypeSignature(types[i]);
        }

        public override string ToString()
        {
            return string.Join(",", Types.Select(t => t.ToString()));
        }

        public override bool Equals(object obj)
        {
            var types = obj as TypeSignatureCollection;
            if (types == null)
                return false;

            if (types.Types.Count() != Types.Count())
                return false;

            for (var i = 0; i < Types.Count(); i++)
                if (types.Types[i] != Types[i])
                    return false;

            return true;
        }

        public override int GetHashCode()
        {
            var hash = 0;
            var i = 0;

            foreach (var type in Types)
                hash ^= i++.GetHashCode() ^ type.GetHashCode();

            return hash;
        }

        public static bool operator ==(TypeSignatureCollection m1, TypeSignatureCollection m2)
        {
            if (ReferenceEquals(m1, m2))
                return true;

            if (((object) m1 == null) || ((object) m2 == null))
                return false;

            return m1.Equals(m2);
        }

        public static bool operator !=(TypeSignatureCollection m1, TypeSignatureCollection m2)
        {
            return !(m1==m2);
        }
    }

    public class TypeSignature : MemberSignature
    {
        public readonly string Namespace;
        public readonly TypeSignatureCollection GenericTypeArguments;

        public TypeSignature(TypeSignature declaringType, string name)
            : base(declaringType, name)
        {
            Namespace = null;
        }

        public TypeSignature(string @namespace, string name)
            : base(null, name)
        {
            Namespace = @namespace;
        }

        public TypeSignature(TypeReference type)
            : base((type.DeclaringType == null)? null : new TypeSignature(type.DeclaringType), type.Name)
        {
            Namespace = type.Namespace;

            var typeGen = type as GenericInstanceType;
            if (typeGen != null)
                GenericTypeArguments = new TypeSignatureCollection(typeGen.GenericArguments);
        }

        public TypeSignature(Type type)
            : base((type.DeclaringType == null)? null : new TypeSignature(type.DeclaringType), type.Name)
        {
            Namespace = type.Namespace;

            if(type.GenericTypeArguments.Any())
                GenericTypeArguments = new TypeSignatureCollection(type.GenericTypeArguments);
        }

        public static readonly string  typeSeparator = "/";
        public override string MemberSeparator { get { return typeSeparator; }}

        public override string ToString()
        {
            if (DeclaringType != null)
                return base.ToString();

            var genericArgs = "";
            if (GenericTypeArguments != null && GenericTypeArguments.Types.Count() > 0)
                genericArgs = string.Format("<{0}>", GenericTypeArguments);

            return string.Format("{0}.{1}{2}", Namespace, Name, genericArgs);
        }

        public override bool Equals(object obj)
        {
            var type = obj as TypeSignature;
            if (type == null)
                return false;

            if (type.GenericTypeArguments != GenericTypeArguments)
                return false;

            if (DeclaringType != null)
                return base.Equals(obj);

            return type.Namespace == Namespace && base.Equals(obj);
        }

        public override int GetHashCode()
        {
            var hash = 0;

            if (GenericTypeArguments != null)
                hash = GenericTypeArguments.GetHashCode();

            if (DeclaringType != null || Namespace == null)
                return hash ^ base.GetHashCode();

            return hash ^ Namespace.GetHashCode() ^ base.GetHashCode();
        }

        public static bool operator ==(TypeSignature m1, TypeSignature m2)
        {
            if (ReferenceEquals(m1, m2))
                return true;

            if (((object) m1 == null) || ((object) m2 == null))
                return false;

            return m1.Equals(m2);
        }

        public static bool operator !=(TypeSignature m1, TypeSignature m2)
        {
            return !(m1==m2);
        }

        public static TypeSignature Parse(string s)
        {
            if (s.Contains(typeSeparator))
            {
                var memberSig = MemberSignature.Parse(s, typeSeparator);
                return new TypeSignature(memberSig.DeclaringType, memberSig.Name);
            }

            var dotIndex = s.LastIndexOf('.');
            var @namespace = s.Substring(0, dotIndex);
            var name = s.Substring(dotIndex+1, s.Length - dotIndex -1);

            return new TypeSignature(@namespace, name);
        }
    }

    class MemberWithReturnSignature : MemberSignature
    {
        public readonly TypeSignature ReturnType;

        public MemberWithReturnSignature(TypeSignature returnType, TypeSignature declaringType, string name)
            : base(declaringType, name)
        {
            ReturnType = returnType;
        }

        public override string ToString()
        {
            return string.Format("{0} {1}", ReturnType, base.ToString());
        }

        public static MemberWithReturnSignature Parse(string s)
        {
            var parts = s.Split(new []{' '}, 1);

            var typeSignature = TypeSignature.Parse(parts[0]);
            var memberSignature = MemberSignature.Parse(parts[1]);

            return new MemberWithReturnSignature(typeSignature, memberSignature.DeclaringType, memberSignature.Name);
        }
    }

    public class FieldSignature : MemberSignature
    {
        public readonly TypeSignature FieldType;

        public FieldSignature(TypeSignature fieldType, TypeSignature declaringType, string name)
            : base(declaringType, name)
        {
            FieldType = fieldType;
        }

        public FieldSignature(FieldReference field)
            :this(new TypeSignature(field.DeclaringType), new TypeSignature(field.FieldType), field.Name)
        {}

        public FieldSignature(FieldInfo field)
            :this(new TypeSignature(field.DeclaringType), new TypeSignature(field.FieldType), field.Name)
        {}

        public override string ToString()
        {
            return string.Format("{0} {1}", FieldType, base.ToString());
        }

        public override bool Equals(object obj)
        {
            var field = obj as FieldSignature;
            if (field == null)
                return false;

            return field.FieldType == FieldType && base.Equals(obj);
        }

        public static bool operator ==(FieldSignature m1, FieldSignature m2)
        {
            if (ReferenceEquals(m1, m2))
                return true;

            if (((object) m1 == null) || ((object) m2 == null))
                return false;

            return m1.Equals(m2);
        }

        public static bool operator !=(FieldSignature m1, FieldSignature m2)
        {
            return !(m1==m2);
        }

        public override int GetHashCode()
        {
            return FieldType.GetHashCode() ^ base.GetHashCode();
        }

        public static FieldSignature Parse(string s)
        {
            var member = MemberWithReturnSignature.Parse(s);
            return new FieldSignature(member.ReturnType, member.DeclaringType, member.Name);
        }
    }

    public class PropertySignature : MemberSignature
    {
        public readonly TypeSignature PropertyType;

        public PropertySignature(TypeSignature propertyType, TypeSignature declaringType, string name)
            : base(declaringType, name)
        {
            PropertyType = propertyType;
        }

        public PropertySignature(PropertyReference property)
            :this(new TypeSignature(property.PropertyType), new TypeSignature(property.DeclaringType), property.Name)
        {}

        public PropertySignature(PropertyInfo property)
            :this(new TypeSignature(property.PropertyType), new TypeSignature(property.DeclaringType), property.Name)
        {}

        public override string ToString()
        {
            return string.Format("{0} {1}", PropertyType, base.ToString());
        }

        public override bool Equals(object obj)
        {
            var property = obj as PropertySignature;
            if (property == null)
                return false;

            return property.PropertyType == PropertyType && base.Equals(obj);
        }

        public static bool operator ==(PropertySignature m1, PropertySignature m2)
        {
            if (ReferenceEquals(m1, m2))
                return true;

            if (((object) m1 == null) || ((object) m2 == null))
                return false;

            return m1.Equals(m2);
        }

        public static bool operator !=(PropertySignature m1, PropertySignature m2)
        {
            return !(m1==m2);
        }

        public override int GetHashCode()
        {
            return PropertyType.GetHashCode() ^ base.GetHashCode();
        }

        public static PropertySignature Parse(string s)
        {
            var member = MemberWithReturnSignature.Parse(s);
            return new PropertySignature(member.ReturnType, member.DeclaringType, member.Name);
        }
    }

    public class MethodSignature : MemberSignature
    {
        public readonly TypeSignature ReturnType;
        public readonly List<ParameterSignature> Parameters;

        public MethodSignature(TypeSignature returnType, TypeSignature declaringType, string name, List<ParameterSignature> parameters )
            : base(declaringType, name)
        {
            ReturnType = returnType;
            Parameters = parameters;
        }

        public MethodSignature(MethodReference method)
            : base(new TypeSignature(method.DeclaringType), method.Name)
        {
             ReturnType = new TypeSignature(method.ReturnType);

            Parameters = new List<ParameterSignature>();
            foreach (var param in method.Parameters)
                Parameters.Add(new ParameterSignature(param));
        }

        public MethodSignature(MethodInfo method)
            : base(new TypeSignature(method.DeclaringType), method.Name)
        {
             ReturnType = new TypeSignature(method.ReturnType);

            Parameters = new List<ParameterSignature>();
            foreach (var param in method.GetParameters())
                Parameters.Add(new ParameterSignature(param));
        }

        public override string ToString()
        {
            var @params = string.Join(",", Parameters.Select(p => p.ToString()));

            return string.Format("{0} {1}({2})", ReturnType, base.ToString(), @params);
        }

        public override bool Equals(object obj)
        {
            var method = obj as MethodSignature;
            if (method == null)
                return false;

            if(!base.Equals(obj))
                return false;

            if (method.Parameters.Count != Parameters.Count)
                return false;

            for(int i = 0; i < Parameters.Count; i++)
                if (method.Parameters[i] != Parameters[i])
                    return false;

            return method.ReturnType == ReturnType;
        }

        public static bool operator ==(MethodSignature m1, MethodSignature m2)
        {
            if (ReferenceEquals(m1, m2))
                return true;

            if (((object) m1 == null) || ((object) m2 == null))
                return false;

            return m1.Equals(m2);
        }

        public static bool operator !=(MethodSignature m1, MethodSignature m2)
        {
            return !(m1==m2);
        }

        public override int GetHashCode()
        {
            var hash = 0;
            var i = 0;
            foreach (var parameterSignature in Parameters)
                hash ^= i++.GetHashCode() ^ parameterSignature.GetHashCode();

            return hash ^ ReturnType.GetHashCode() ^ base.GetHashCode();
        }

        public static MethodSignature Parse(string s)
        {
            var parStartIndex = s.IndexOf('(');
            var parEndIndex = s.IndexOf('(');

            var memberSignature = s.Substring(0, parStartIndex);
            var parameterSignatures = s.Substring(parStartIndex + 1, parEndIndex).Split(',');

            var paramList = new List<ParameterSignature>();
            foreach (var param in parameterSignatures)
                paramList.Add(ParameterSignature.Parse(param));

            var member = MemberWithReturnSignature.Parse(memberSignature);
            return new MethodSignature(member.ReturnType, member.DeclaringType, member.Name, paramList);
        }
    }

    public class ParameterSignature
    {
        public readonly string Name;
        public readonly TypeSignature ParamType;

        public ParameterSignature(TypeSignature paramType, string name)
        {
            ParamType = paramType;
            Name = name;
        }

        public ParameterSignature(ParameterReference param)
            :this(new TypeSignature(param.ParameterType), param.Name)
        {}

        public ParameterSignature(ParameterInfo param)
            :this(new TypeSignature(param.ParameterType), param.Name)
        {}

        public override string ToString()
        {
            return string.Format("{0} {1}", ParamType, Name);
        }

        public override bool Equals(object obj)
        {
            var parameter = obj as ParameterSignature;
            if (parameter == null)
                return false;

            return parameter.ParamType == ParamType;
        }

        public static bool operator ==(ParameterSignature m1, ParameterSignature m2)
        {
            if (ReferenceEquals(m1, m2))
                return true;

            if (((object) m1 == null) || ((object) m2 == null))
                return false;

            return m1.Equals(m2);
        }

        public static bool operator !=(ParameterSignature m1, ParameterSignature m2)
        {
            return !(m1==m2);
        }

        public override int GetHashCode()
        {
            return ParamType.GetHashCode();
        }

        public static ParameterSignature Parse(string s)
        {
            var parts = s.Split(new []{' '}, 1);

            var typeSignature = TypeSignature.Parse(parts[0]);

            return new ParameterSignature(typeSignature, parts[1]);
        }
    }
}
