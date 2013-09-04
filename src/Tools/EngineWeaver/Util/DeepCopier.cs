using Mono.Cecil;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace EngineWeaver.Util
{
    public abstract class DeepCopier
    {
        private Dictionary<Type, FastInvoke.FastInvokeHandler> mergeMethodInfos;
        private Dictionary<Type, FastInvoke.FastInvokeHandler> copyMethodInfos;

        public readonly Dictionary<object,object> CopyMap;

        private readonly Logger log = new Logger(Logger.LogLevel.Warning);
        private readonly Logger publicLog = new Logger(Logger.LogLevel.Warning);

        public DeepCopier(){
            mergeMethodInfos = new Dictionary<Type, FastInvoke.FastInvokeHandler>();
            copyMethodInfos = new Dictionary<Type, FastInvoke.FastInvokeHandler>();
            CopyMap = new Dictionary<object,object>();

            var type = this.GetType();
            foreach (var m in type.GetMethods(BindingFlags.NonPublic|BindingFlags.Public|BindingFlags.Static|BindingFlags.Instance))
            {


                if (m.Name.StartsWith("Merge") &&
                    m.GetParameters().Count() == 2 &&
                    m.GetParameters()[0].ParameterType == m.GetParameters()[1].ParameterType &&
                    !m.IsGenericMethod)
                {
                    var mergeInvoke = FastInvoke.GetMethodInvoker(m);
                    mergeMethodInfos.Add(m.GetParameters()[1].ParameterType, mergeInvoke);
                }

                if (m.Name.StartsWith("Copy") &&
                    m.GetParameters().Count() == 1 &&
                    m.GetParameters()[0].ParameterType == m.ReturnType &&
                    !m.IsGenericMethod)
                {
                    var copyInvoke = FastInvoke.GetMethodInvoker(m);
                    copyMethodInfos.Add(m.GetParameters()[0].ParameterType, copyInvoke);
                }
                    
            }
        }

        public void Log(string msg){
            publicLog.Tabs = log.Tabs;
            publicLog.Info(msg);
        }

        public void Merge(object obj1, object obj2){

            if(obj1 == null)
                return;

            if(CopyMap.ContainsKey(obj1) || 
               CopyMap.ContainsKey(obj2))
                return;
            
            var type = obj1.GetType();
            while(!type.IsPublic)
                type = type.BaseType;

            if(!mergeMethodInfos.ContainsKey(type))
                throw new NotImplementedException("Merge<"+type.FullName+">");

            mergeMethodInfos[type].Invoke(this, new []{obj1, obj2});

            if(!CopyMap.ContainsKey(obj1))
                CopyMap.Add(obj1, obj2);
        }

        public T Copy<T>(T value)
        {
            if(value == null)
                return default(T);

            if(CopyMap.ContainsKey(value)){
                return (T)CopyMap[value];
            }

            var type = value.GetType();
            while(!type.IsPublic)
                type = type.BaseType;

            if(!copyMethodInfos.ContainsKey(type))
                throw new NotImplementedException("Copy<"+type.FullName+">");

            var copy = copyMethodInfos[type].Invoke(this, new object[]{value});

            if(!CopyMap.ContainsKey(value))
                CopyMap.Add(value, copy);

            return (T)copy;
        }

        //TODO Order list instead of multiple iterations
        private List<FieldInfo> GetFields(Type type, params string[] ignores){
            var allFields= type.GetFields(BindingFlags.Public| BindingFlags.Instance);
            return allFields.Where( f => {
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
            if(CopyMap.ContainsKey(from))
                throw new Exception("Cannot merge object twice");

            CopyMap.Add(from, to);

            log.Info("> Merge "+ typeof(T));
            log.Tabs++;
            var fields = GetFields(typeof(T),ignores);
            var properties = GetProperties(typeof(T),ignores);

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
        }



        public void CopyAll<T>(T from, T to, params string[] ignores)
        {
            if(!CopyMap.ContainsKey(from))
                CopyMap.Add(from, to);

            log.Info("> Copy "+ typeof(T));
            log.Tabs++;

            var fields = GetFields(typeof(T),ignores);
            var properties = GetProperties(typeof(T),ignores);

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
                field.SetValue(to, Copy(field.GetValue(from)));
                fields.Remove(field);
            }
            foreach(PropertyInfo property in properties.ToArray()) {
                if(property.CanWrite)  {
                    var value = property.GetValue(from);
                    if(value != null){
                        log.Info("Copy property "+ property.Name);
                        property.SetValue(to, Copy(value));
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
                    foreach(var el in fromList)
                        toList.Add(Copy(el));
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
        }


    }
}
