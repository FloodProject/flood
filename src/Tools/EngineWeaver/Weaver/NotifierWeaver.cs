using Mono.Cecil;
using System;
using System.Collections.Generic;

using System.Linq;
using Flood;

namespace DSLToolkit.Weaver
{
    
    public class NotifierWeaver
    {

        class Template{
            private Dictionary<FieldChanged, FieldChanged> handlers = new Dictionary<FieldChanged,FieldChanged>();
            
            public Template() {

            }

            public Template(int i) 
            :this(){

            }


            public event FieldChanged MyEvent {
                add {
                    var handler = value;
                    if (handlers.ContainsKey(handler))
                    {
                        Console.WriteLine("Event already added");
                        return;
                    }
                
                    FieldChanged safeHandler = (ob, p, o, n) =>
                    {
                        try{
                            if(handler(ob, p, o, n))
                                return true;
                        }catch{
                   
                        }
                        MyEvent -= handler;
                        return false;
                    };
                    handlers.Add(handler, safeHandler);
                }
                remove {
                    handlers.Remove(value);
                }
            }

            private void EventInvoke(object obj, System.Reflection.PropertyInfo propertyInfo, object oldValue, object newValue){
                foreach (var handler in handlers.Values)
                {
                    handler.Invoke(obj, propertyInfo, oldValue, newValue);
                }
            }

            private static System.Reflection.PropertyInfo pInfo;
            private object pValue;
            public object Property {
                get {
                    return pValue;
                }
                set {
                    if(value != pValue){
                        EventInvoke(this, pInfo, pValue, value);
                        pValue = value;
                    }

                }
            }
        }

        private static EventDefinition tNotifierEvent;
        private static MethodDefinition tNotifierMethod;
        private static FieldDefinition tNotifierField;

        private static PropertyDefinition tPropertyProperty;
        private static FieldDefinition tPropertyField;
        private static FieldDefinition tPropertyInfoField;

        private static TypeDefinition originType;

        static NotifierWeaver()
        {
            var type = typeof(Template);
            var assemblyPath = type.Assembly.Location;
            var assembly = AssemblyDefinition.ReadAssembly(assemblyPath);
            var module = assembly.MainModule;
            originType = module.Import(type).Resolve();

            tNotifierEvent  = originType.Events.Single(e => e.Name == "MyEvent");
            tNotifierMethod = originType.Methods.Single(e => e.Name == "EventInvoke");
            tNotifierField  = originType.Fields.Single(e => e.Name == "handlers");
            
            tPropertyProperty = originType.Properties.Single(e => e.Name == "Property");
            tPropertyField    = originType.Fields.Single(e => e.Name == "pValue");
            tPropertyInfoField= originType.Fields.Single(e => e.Name == "pInfo");
        }


       
        public void WeaveModule(ModuleDefinition module)
        {

            var types = module.Types.Where(
                t => t.Interfaces.Any( 
                    i  => i.FullName == typeof(IPropertyNotifier).FullName));

            foreach(var type in types)
            {
                var weaver = new TypeWeaver(originType,type);

                var destinationEvent = type.Events.Single(e => e.Name == "MyEvent");

                //TODO delete event generated field
                weaver.Merge(tNotifierEvent, destinationEvent);

                var desNotifierMethod = weaver.AddDefinition(tNotifierMethod);
                weaver.AddDefinition(tNotifierField);
    
                weaver.ProcessInstructions();

                var properties = type.Properties.Where(
                    p => p.CustomAttributes.Any( 
                        a => a.AttributeType.FullName == typeof(FieldAttributes).FullName));

                foreach(var property in properties){
            
                    weaver.NamePrefix = "<$"+property.Name+"$>";

                    weaver.Map(tNotifierMethod, desNotifierMethod);

                    var propField = weaver.AddDefinition(tPropertyField);
                    propField.FieldType = property.PropertyType;

                    weaver.AddDefinition(tPropertyInfoField);

                    //TODO delete property generated field
                    weaver.Merge(tPropertyProperty, property);
           
                    weaver.ProcessInstructions();
                }
            }

        }
    }
}
