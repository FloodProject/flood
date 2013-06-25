using System.Diagnostics;
using Mono.Cecil;
using System;
using System.Collections.Generic;

using System.Linq;

namespace EngineWeaver
{
    
    public class NotifierWeaver
    {
        /*
        class Template{
            private Dictionary<PropertyChanged, PropertyChanged> handlers = new Dictionary<PropertyChanged,PropertyChanged>();
            
            public event PropertyChanged PropertyChanged {
                add {
                    var handler = value;
                    if (handlers.ContainsKey(handler))
                    {
                        Debug.WriteLine("Event already added");
                        return;
                    }
                
                    PropertyChanged safeHandler = (ob, p, o, n) =>
                    {
                        try{
                            if(handler(ob, p, o, n))
                                return true;
                        }catch(Exception e){
                            Debug.WriteLine(e);
                        }
                        handlers.Remove(value);
                        return false;
                    };
                    handlers.Add(handler, safeHandler);
                }
                remove {
                    handlers.Remove(value);
                }
            }

            protected void EventInvoke(object obj, string propertyName, object oldValue, object newValue){
                for (int pos = handlers.Count - 1; pos >= 0; pos--)
                {
                    var handler = handlers.ElementAt(pos).Value;
                    handler.Invoke(obj, propertyName, oldValue, newValue);
                }
            }

            private static string pNameC;
            private object pValue;
            public object Property {
                get {
                    return pValue;
                }
                set {
                    if(value != pValue){
                        EventInvoke(this, pNameC, pValue, value);
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
        private static FieldDefinition tPropertyNameC;

        private static TypeDefinition originType;

        static NotifierWeaver()
        {
            var type = typeof(Template);
            var assemblyPath = type.Assembly.Location;
            var assembly = AssemblyDefinition.ReadAssembly(assemblyPath);
            var module = assembly.MainModule;
            originType = module.Import(type).Resolve();

            tNotifierEvent  = originType.Events.Single(e => e.Name == "PropertyChanged");
            tNotifierMethod = originType.Methods.Single(e => e.Name == "EventInvoke");
            tNotifierField  = originType.Fields.Single(e => e.Name == "handlers");
            
            tPropertyProperty = originType.Properties.Single(e => e.Name == "Property");
            tPropertyField    = originType.Fields.Single(e => e.Name == "pValue");
            tPropertyNameC    = originType.Fields.Single(e => e.Name == "pNameC");
        }

        protected void WeaveTypeInterface(TypeDefinition type)
        {
            var destinationEvent = type.GetEvent("PropertyChanged");

            type = destinationEvent.DeclaringType; //can be base type

            var copier = new CecilCopier(originType.Module,type.Module);
            var weaver = new TypeWeaver(originType, type, copier);

            weaver.ProcessNestedTypes();

            //TODO delete event generated field
            weaver.Merge(tNotifierEvent, destinationEvent);

            weaver.AddDefinition(tNotifierMethod);
            weaver.AddDefinition(tNotifierField);

            weaver.ProcessInstructions();
        }

        protected void WeaveTypeProperties(TypeDefinition type)
        {
            var properties = type.Properties.Where(
                p => p.CustomAttributes.Any(
                    a => a.AttributeType.FullName == typeof (PropertyAttribute).FullName));
                    
            if(!properties.Any())
                return;
               
            if(!type.HasInterface(typeof(IPropertyNotifier).FullName))
                throw new Exception(string.Format("Type {0} has properties but does not implement {1}", type.FullName, typeof(IPropertyNotifier).FullName));

            Console.WriteLine(type.FullName);

            var copier = new CecilCopier(originType.Module,type.Module);
            var weaver = new TypeWeaver(originType, type, copier);
                    
            foreach(var property in properties)
            {

                copier.NamePrefix = "<$" + property.Name + "$>";

                var desNotifierMethod =  type.GetMethod(tNotifierMethod.Name);
                         
                copier.Map(tNotifierMethod, desNotifierMethod);

                var propField = weaver.AddDefinition(tPropertyField);
                propField.FieldType = property.PropertyType;

                weaver.AddConstant(tPropertyNameC,property.Name);

                //TODO delete property generated field
                weaver.Merge(tPropertyProperty, property);

                weaver.ProcessInstructions();
            }
        }
       
        public void WeaveModule(ModuleDefinition module)
        {

            var typesWithInterface = module.Types.Where(
                t => t.Interfaces.Any( 
                    i  => i.FullName == typeof(IPropertyNotifier).FullName));

            foreach(var type in typesWithInterface)
            {
                WeaveTypeInterface(type);
            }

            foreach(var type in module.Types)
            {
                WeaveTypeProperties(type);
            }

        }*/
    }
}
