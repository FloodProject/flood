using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DSLToolkit.Util
{
    public class DeepCopier
    {
        private object objectMethod;
        private Dictionary<Type, MethodInfo> mergeMethodInfos;
        private Dictionary<Type, MethodInfo> copyMethodInfos;

        public readonly Dictionary<object,object> CopyMap;

        private readonly Logger log = new Logger(Logger.Level.Info);
        private readonly Logger publicLog = new Logger(Logger.Level.Info);

        public DeepCopier(object obj){
            mergeMethodInfos = new Dictionary<Type, MethodInfo>();
            copyMethodInfos = new Dictionary<Type, MethodInfo>();
            CopyMap = new Dictionary<object,object>();

            objectMethod = obj;

            var type = obj.GetType();
            foreach (var m in type.GetMethods(BindingFlags.NonPublic|BindingFlags.Public|BindingFlags.Static|BindingFlags.Instance)){
                if (m.Name.StartsWith("Merge") && 
                    m.GetParameters().Count() >= 3 &&
                    m.GetParameters()[0].ParameterType == this.GetType() &&
                    m.GetParameters()[1].ParameterType == m.GetParameters()[2].ParameterType &&
                    !m.IsGenericMethod)
                    mergeMethodInfos.Add(m.GetParameters()[1].ParameterType,m);
                 if (m.Name.StartsWith("Copy") && 
                    m.GetParameters().Count() >= 2 &&
                     m.GetParameters()[0].ParameterType == this.GetType() &&
                    !m.IsGenericMethod)
                    copyMethodInfos.Add(m.GetParameters()[1].ParameterType,m);
            }
          
        }

        public void Log(string msg){
            publicLog.Tabs = log.Tabs;
            publicLog.Info(msg);
        }
        

        public void Merge(object obj1, object obj2, object data = null){

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

            var m = mergeMethodInfos[type];
            //try {
                if(m.GetParameters().Count() == 3){
                    m.Invoke(objectMethod,new object[]{this, obj1, obj2});
                } else {
                    m.Invoke(objectMethod,new object[]{this, obj1, obj2, data});
                }
           // } catch(Exception e){
           //     log.Error(e.ToString());
           // }

            CopyMap.Add(obj1,obj2);
        }

        public T Copy<T>(T value, object data = null)
        {
            if(value == null)
                return default(T);

            if(CopyMap.ContainsValue(value)){
                return value;
            }

            if(CopyMap.ContainsKey(value)){
                return (T)CopyMap[value];
            }

            var type = value.GetType();
            while(!type.IsPublic)
                type = type.BaseType;

            if(!copyMethodInfos.ContainsKey(type))
                throw new NotImplementedException("Copy<"+type.FullName+">");
            

            var m = copyMethodInfos[type];
            object copy = null;
            if(m.GetParameters().Count() == 2){
                copy = m.Invoke(objectMethod,new object[]{this, value});
            } else {
                copy = m.Invoke(objectMethod,new object[]{this, value, data});
            }
            if(copy == null){
                throw new NullReferenceException();
            }

            CopyMap.Add(value,copy);
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



        public void MergeAll<T>(T from, T to, object data, params string[] ignores)
        {
             if(data is string)
                throw new ArgumentException("data cannot be string");

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
                Merge(field.GetValue(from), field.GetValue(to), data);
                fields.Remove(field);
            }
            foreach(PropertyInfo property in properties.ToArray()) {
                log.Info("Merge property "+ property.Name);
                Merge(property.GetValue(from), property.GetValue(to), data);
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



        public void CopyAll<T>(T from, T to, object data, params string[] ignores)
        {
            if(data is string)
                throw new ArgumentException("data cannot be string");

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
                field.SetValue(to, Copy(field.GetValue(from), data));
                fields.Remove(field);
            }
            foreach(PropertyInfo property in properties.ToArray()) {
                if(property.CanWrite)  {
                    var value = property.GetValue(from);
                    if(value != null){
                        log.Info("Copy property "+ property.Name);
                        property.SetValue(to, Copy(value, data));
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
                    foreach(var el in fromList){
                        toList.Add(Copy(el,data));
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
        }


    }
}
