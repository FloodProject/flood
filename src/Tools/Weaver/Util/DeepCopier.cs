using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace Weaver.Util
{
    public abstract class CopyKey
    {
        protected CopyKey()
        {
        }

        public object Value { get; set; }

        public object Copier { get; set; }
    }

    public class CopyNode
    {
        public CopyNode Previous { get; private set; }
        public object Value { get; private set; }

        public CopyNode(object value)
        {
            Value = value;
        }

        public CopyNode(object value, CopyNode previous)
        {
            Value = value;
            Previous = previous;
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class MemoizeCopyAttribute : Attribute
    {
    }

    public abstract class DeepCopier<K>
        where K : CopyKey, new()
    {
        private Dictionary<Type, FastInvoke.FastInvokeHandler> mergeMethodInfos;
        private Dictionary<Type, FastInvoke.FastInvokeHandler> copyMethodInfos;
        private HashSet<Type> typesWithMemoization;

        private readonly Dictionary<K,object> CopyMap;

        private CopyNode CopyCurrent;

        private readonly List<IDelayedCopy> delayedCopies;

        private readonly Logger log = new Logger(Logger.LogLevel.Warning);
        private readonly Logger publicLog = new Logger(Logger.LogLevel.Warning);

        public bool IsMemoizationEnabled;

        public DeepCopier()
        {
            mergeMethodInfos = new Dictionary<Type, FastInvoke.FastInvokeHandler>();
            copyMethodInfos = new Dictionary<Type, FastInvoke.FastInvokeHandler>();
            typesWithMemoization = new HashSet<Type>();

            CopyMap = new Dictionary<K,object>();
            CopyCurrent = null;

            delayedCopies = new List<IDelayedCopy>();

            IsMemoizationEnabled = true;

            var type = this.GetType();
            foreach (var m in type.GetMethods(BindingFlags.Public|BindingFlags.Static|BindingFlags.Instance))
            {
                if (m.Name.StartsWith("Merge") &&
                    m.GetParameters().Count() == 2 &&
                    m.GetParameters()[0].ParameterType == m.GetParameters()[1].ParameterType &&
                    !m.IsGenericMethod)
                {
                    var mergeInvoke = FastInvoke.GetMethodInvoker(m);
                    mergeMethodInfos.Add(m.GetParameters()[1].ParameterType, mergeInvoke);
                }

                if (m.Name.Equals("Copy") &&
                    m.GetParameters().Count() == 1 &&
                    m.GetParameters()[0].ParameterType == m.ReturnType &&
                    !m.IsGenericMethod)
                {
                    var copyInvoke = FastInvoke.GetMethodInvoker(m);
                    var paramType = m.GetParameters()[0].ParameterType;
                    copyMethodInfos.Add(paramType, copyInvoke);

                    if (m.GetCustomAttribute<MemoizeCopyAttribute>() != null)
                        typesWithMemoization.Add(paramType);
                }
            }
        }

        public void Log(string msg){
            publicLog.Tabs = log.Tabs;
            publicLog.Info(msg);
        }

        public void Warn(string msg){
            publicLog.Tabs = log.Tabs;
            publicLog.Warning(msg);
        }

        protected void AddDelayedCopy<T>(T from, T to, DelayedCopy<T>.CopyDelegate action)
        {
            var delayedCopy = new DelayedCopy<T>(from, to, action);
            delayedCopy.CopyParent = CopyCurrent;
            delayedCopies.Add(delayedCopy);
        }

        protected void ProcessDelayed()
        {
            while (delayedCopies.Any())
            {
                var array = delayedCopies.ToArray();
                delayedCopies.Clear();
                foreach (var copy in array)
                {
                    CopyCurrent = copy.CopyParent;
                    copy.Copy();
                }
            }
        }

        public virtual void Clear()
        {
            CopyMap.Clear();
        }

        public T GetCopy<T>(T key)
        {
            if(!IsValidCopyKey(key))
                throw new ArgumentException("key");

            T copy;
            if(!TryGetCopy<T>(key, out copy))
                throw new Exception("Key not found.");

            return copy;
        }

        public bool TryGetCopy<T>(T key, out T copy)
        {
            if(!IsValidCopyKey(key))
                throw new ArgumentException("key");

            copy = default(T);
            var found = false;

            object objKey = key;
            object objValue;
            while (CopyMap.TryGetValue(new K {Value = objKey, Copier = this}, out objValue))
            {
                found = true;
                copy = (T) objValue;

                if (objKey == objValue) 
                    break;

                //Find copies of copies
                objKey = objValue; 
            }

            return found;
        }

        public void SetCopy<T>(T key, T copy)
        {
            if(!IsValidCopyKey(key))
                throw new ArgumentException("key");

            if(!IsValidCopyValue(copy))
                throw new ArgumentException("copy");

            CopyMap.Add(new K {Value = key, Copier = this}, copy);
        }

        public bool ContainsCopy(object key)
        {
            if(!IsValidCopyKey(key))
                throw new ArgumentException("key");

            return CopyMap.ContainsKey(new K {Value = key, Copier = this});
        }

        public object GetCurrentCopy(params Type[] filterTypes)
        {
            if (!filterTypes.Any())
                return CopyCurrent.Value;

            var node = CopyCurrent;
            while (node != null)
            {
                var nodeType = node.Value.GetType();
                if (filterTypes.Any(t => t.IsAssignableFrom(nodeType)))
                    return node.Value;

                node = node.Previous;
            }

            return null;
        }

        protected void PushCopy(object copy)
        {
            CopyCurrent = new CopyNode(copy, CopyCurrent);
        }

        protected void PopCopy()
        {
            CopyCurrent = CopyCurrent.Previous;
        }

        protected virtual bool IsValidCopyKey(object key)
        {
            return true;
        }

        protected virtual bool IsValidCopyValue(object value)
        {
            return true;
        }

        protected virtual bool SkipCopy(object from, out object copy)
        {
            return TryGetCopy(from, out copy);
        }

        public void Merge(object obj1, object obj2)
        {
            if(obj1 == null)
                return;

            var obj1Key = new K {Value = obj1, Copier = this};

            if(CopyMap.ContainsKey(obj1Key))
                return;

            PushCopy(obj1);

            var type = obj1.GetType();
            while(!type.IsPublic)
                type = type.BaseType;

            if(!mergeMethodInfos.ContainsKey(type))
                throw new NotImplementedException("Merge<"+type.FullName+">");

            mergeMethodInfos[type].Invoke(this, new []{obj1, obj2});

            PopCopy();

            if(!CopyMap.ContainsKey(obj1Key))
                CopyMap.Add(obj1Key, obj2);
        }

        public T Copy<T>(T value)
        {
            if (value == null)
                return default(T);

            return (T) Copy(value, typeof(T), null);
        }

        private object Copy(object value, Type type, MemberInfo source)
        {
            if(value == null)
                return null;

            if (value.GetType() != type)
            {
                var sourceMsg = (source == null) ? "" : "\nCheck " + FullName(source);
                var msg = string.Format("Cannot copy {0} as {1}{2}", 
                    FullName(value.GetType()), FullName(type),sourceMsg);
                throw new Exception(msg);
            }

            object copy;
            if (SkipCopy(value, out copy))
                return copy;

            PushCopy(value);

            if(!copyMethodInfos.ContainsKey(type))
                throw new NotImplementedException("Copy<"+type.FullName+">");

            var method = copyMethodInfos[type];
            copy = method.Invoke(this, new object[]{value});

            PopCopy();

            if (IsMemoizationEnabled &&
                typesWithMemoization.Contains(type) &&
                !ContainsCopy(value))
                SetCopy(value, copy);

            return copy;
        }

        //TODO Order list instead of multiple iterations
        private List<FieldInfo> GetFields(Type type, params string[] ignores){
            var allFields= type.GetFields(BindingFlags.Public| BindingFlags.Instance);
            return allFields.Where( f =>{
                if(!ignores.Contains(f.Name)) return true;
                log.Info("Ignoring field " + f.Name);
                return false;
            }).ToList();
        }

        private List<PropertyInfo> GetProperties(Type type, params string[] ignores){
            var allProperties= type.GetProperties(BindingFlags.Public| BindingFlags.Instance);
            return allProperties.Where( p => {
                if(!ignores.Contains(p.Name)) return true; 
                log.Info("Ignoring property " + p.Name);
                return false;
            }).ToList();
        }

        public void MergeAll<T>(T from, T to, params string[] ignores)
        {
            var pushed = false;
            if (CopyCurrent == null || !CopyCurrent.Value.Equals(from))
            {
                PushCopy(from);
                pushed = true;
            }
            if (IsMemoizationEnabled &&
                typesWithMemoization.Contains(typeof(T)) &&
                !ContainsCopy(from))
                SetCopy(from, to);

            log.Info("> Merge "+ typeof(T)+ " "+ from);
            log.Tabs++;
            var fields = GetFields(typeof(T), ignores);
            var properties = GetProperties(typeof(T), ignores);

            //ValueTypes
            foreach(FieldInfo field in fields.ToArray()) {
                if(field.FieldType.IsValueType || field.FieldType.FullName.StartsWith("System.String")){
                    var vF = field.GetValue(from);
                    var vT = field.GetValue(to);
                    if(!Object.Equals(vF, vT)){
                        log.Info("Merge field "+ field.Name+" ignored origin:"+vF);
                        //field.SetValue(to, vF);
                    }
                    fields.Remove(field);
                }
            }
            foreach(PropertyInfo property in properties.ToArray()) {
                if((property.PropertyType.IsValueType || property.PropertyType.FullName.StartsWith("System.String"))){
                    if(property.CanWrite){ 
                        var vF = property.GetValue(from);
                        var vT = property.GetValue(to);
                        if(!Object.Equals(vF, vT)){
                            log.Info("Merge property "+ property.Name+" ignored origin:"+vF);
                            //property.SetValue(to, vF);
                        }
                    }
                    properties.Remove(property);
                } 
            }

            //Class types
            foreach(FieldInfo field in fields.ToArray()) {
                log.Info("Merge field "+ field.Name);
                Merge(field.GetValue(from), field.GetValue(to));
                fields.Remove(field);
            }
            foreach(PropertyInfo property in properties.ToArray()) {
                log.Info("Merge property "+ property.Name);
                Merge(property.GetValue(from), property.GetValue(to));
                properties.Remove(property);
            }

            //Misses
            foreach(FieldInfo field in fields.ToArray()) {
                 log.Info("Cant merge field "+ field.FieldType +" "+ field.Name);
             }
             foreach(PropertyInfo property in properties) {
                 log.Info("Cant merge property "+ property.Name);
             }
            log.Tabs--;

            if(pushed)
                PopCopy();
        }

        public void CopyAll<T>(T from, T to, params string[] ignores)
        {
            var pushed = false;
            if (CopyCurrent == null || !CopyCurrent.Value.Equals(from))
            {
                PushCopy(from);
                pushed = true;
            }
            if(typesWithMemoization.Contains(typeof(T)) &&
               !ContainsCopy(from))
                SetCopy(from, to);

            log.Info("> Copy "+ typeof(T)+ " "+ from);
            log.Tabs++;

            var fields = GetFields(typeof(T), ignores);
            var properties = GetProperties(typeof(T), ignores);

            //ValueTypes
            foreach(FieldInfo field in fields.ToArray()) {
                if(field.FieldType.IsValueType || field.FieldType.FullName.StartsWith("System.String")){
                    var vF = field.GetValue(from);
                    var vT = field.GetValue(to);
                    if(!Object.Equals(vF, vT)){
                        log.Info("Copy field "+ field.Name+"  value:"+vF);
                        field.SetValue(to, vF);
                    }
                    fields.Remove(field);
                }
            }
            foreach(PropertyInfo property in properties.ToArray()) {
                if((property.PropertyType.IsValueType || property.PropertyType.FullName.StartsWith("System.String"))){
                    if(property.CanWrite){ 
                        var vF = property.GetValue(from);
                        var vT = property.GetValue(to);
                        if(!Object.Equals(vF, vT)){
                            log.Info("Copy property "+ property.Name+"  value:"+vF);
                            property.SetValue(to, vF);
                        }
                    }
                    properties.Remove(property);
                } 
            }

            //Class types
            foreach(FieldInfo field in fields.ToArray()) {
                log.Info("Copy field "+ field.Name);
                field.SetValue(to, Copy(field.GetValue(from), field.FieldType, field));
                fields.Remove(field);
            }
            foreach(PropertyInfo property in properties.ToArray()) {
                if(property.CanWrite)  {
                    var value = property.GetValue(from);
                    if(value != null){
                        log.Info("Copy property "+ property.Name);
                        property.SetValue(to, Copy(value, property.PropertyType, property));
                    }
                    properties.Remove(property);
                }
            }
           
            //Lists
            foreach(PropertyInfo property in properties.ToArray()) {
                if(property.PropertyType.FullName.StartsWith("Mono.Collections.Generic.Collection")){
                    var fromList = ((IList)property.GetValue(from));
                    var toList = ((IList)property.GetValue(to));
                    log.Info("> List "+ property.Name);
                    log.Tabs++;
                    foreach (var el in fromList)
                    {
                        log.Info(el.ToString());
                        toList.Add(Copy(el, property.PropertyType.GetGenericArguments()[0], property));
                    }
                    log.Tabs--;
                    properties.Remove(property);
                }
            }

            //Misses
            foreach(FieldInfo field in fields.ToArray()) {
                log.Info("Cant copy field "+ field.FieldType +" "+ field.Name);
            }
            foreach(PropertyInfo property in properties) {
                log.Info("Cant copy property "+ property.Name);
            }
            log.Tabs--;

            if(pushed)
                PopCopy();
        }

        private string FullName(MemberInfo member)
        {
            if (member == null)
                return "";

            var name = member.Name;

            var typeInfo = member as TypeInfo;
            if (typeInfo != null && !string.IsNullOrEmpty(typeInfo.Namespace))
                return ((TypeInfo) member).Namespace +"."+ name;

            if (member.DeclaringType == null)
                return name;

            return string.Join(".", FullName(member.DeclaringType), member.Name);
        }
    }
}
