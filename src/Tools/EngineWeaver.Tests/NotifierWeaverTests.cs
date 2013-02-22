using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Mono.Cecil;
using NUnit.Framework;
using Flood;
using DSLToolkit.Weaver;
using System.IO;

namespace EngineWeaver.Tests
{
    [TestFixture]
    public class NotifierWeaverTests
    {
        class PublicNotifierWeaver : NotifierWeaver
        {
            public void PublicWeaveTypeInterface(TypeDefinition type)
            {
                WeaveTypeInterface(type);
            }

            public void PublicWeaveTypeProperties(TypeDefinition type)
            {
                WeaveTypeProperties(type);
            }
        }

        private object CreateWeavedInstance(Type type)
        {
            var weaver = new PublicNotifierWeaver();
            var assembly = AssemblyDefinition.ReadAssembly(typeof(NotifierWeaverTests).Assembly.Location);
            var typeDefinition = assembly.MainModule.Import(type).Resolve();
            weaver.PublicWeaveTypeInterface(typeDefinition);
            for(var t = typeDefinition; t != null; t = t.BaseType.Resolve())
            {
                weaver.PublicWeaveTypeProperties(t);
                if(t.BaseType == null)
                    break;
            }

            using (MemoryStream mStream = new MemoryStream()) 
                { 
                    // Write the edited data to the memory stream.. 
                    typeDefinition.Module.Assembly.Write(mStream); 
                
                    // Load the new assembly from the memory stream buffer.. 
                    var newAssembly = Assembly.Load(mStream.GetBuffer());
                    var newType = newAssembly.GetType(type.FullName);
                    return newType.GetConstructor(Type.EmptyTypes).Invoke(null);
                } 
        }

        public class TestClass : IPropertyNotifier
        {
            public event PropertyChanged PropertyChanged;

            [Flood.Property]
            public int Integer { get; set; }
        }

        public class InheritedTestClass : TestClass
        {
            [Flood.Property]
            public int Integer2 { get; set; }
        }

        [Test]
        public void ValueTypeProperty()
        {
            var instance = CreateWeavedInstance(typeof (TestClass));

            var eventInfo = instance.GetType().GetEvent("PropertyChanged");
            var integerProperty = instance.GetType().GetProperty("Integer");

            TestProperty(instance, integerProperty, eventInfo, 0, 1);
        }

        [Test]
        public void SuperClassWithProperty()
        {
            var instance = CreateWeavedInstance(typeof (InheritedTestClass));

            var eventInfo = instance.GetType().GetEvent("PropertyChanged");
            var integerProperty1 = instance.GetType().GetProperty("Integer");
            var integerProperty2 = instance.GetType().GetProperty("Integer2");

            TestProperty(instance, integerProperty1, eventInfo, 0, 1); //test base type
            TestProperty(instance, integerProperty2, eventInfo, 0, 1); //test super type
        }

        
        public class TestNames : IPropertyNotifier
        {
            public event PropertyChanged PropertyChanged;

            [Flood.Property]
            public int Name1 { get; set; }
        }

        [Test]
        public void PropertyName()
        {
            var instance = CreateWeavedInstance(typeof (TestNames));

            var eventInfo = instance.GetType().GetEvent("PropertyChanged");
            var property1 = instance.GetType().GetProperty("Name1");

            var propertyName = "";

            var handler = new PropertyChanged(
                (o, name, value, nValue) =>
                    {
                        propertyName = name;
                        return false;
                    });

            eventInfo.AddEventHandler(instance, handler);
            property1.SetValue(instance,1);
            Assert.AreEqual(property1.Name,propertyName);
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="instance"></param>
        /// <param name="property"></param>
        /// <param name="eventInfo"></param>
        /// <param name="value0">Initial value</param>
        /// <param name="value1">Changed value</param>
        private static void TestProperty(object instance,PropertyInfo property, EventInfo eventInfo, object value0, object value1)
        {
            var eventTriggerCount = 0;
            var keepAliveHandler = true;

            object oldValue = null;
            object newValue = null;

            var handler = new PropertyChanged(
                (o, name, value, nValue) =>
                    {
                        eventTriggerCount++;
                        oldValue = value;
                        newValue = nValue;
                        return keepAliveHandler;
                    });

            property.SetValue(instance,value0);
            Assert.AreEqual(value0,property.GetValue(instance));
            Assert.AreEqual(0,eventTriggerCount);

            eventInfo.AddEventHandler(instance, handler);

            property.SetValue(instance,value1);
            Assert.AreEqual(value1,property.GetValue(instance));
            Assert.AreEqual(1,eventTriggerCount);
            Assert.AreEqual(value0, oldValue);
            Assert.AreEqual(value1, newValue);

            property.SetValue(instance,value1); //same value should not be triggered
            Assert.AreEqual(value1,property.GetValue(instance));
            Assert.AreEqual(1,eventTriggerCount);

            eventInfo.RemoveEventHandler(instance, handler);

            property.SetValue(instance,value0); //handler should not be triggered
            Assert.AreEqual(value0,property.GetValue(instance));
            Assert.AreEqual(1,eventTriggerCount);

            eventInfo.AddEventHandler(instance, handler);

            keepAliveHandler = false; //event should be removed after next trigger

            property.SetValue(instance,value1); //test event self remove from return false
            Assert.AreEqual(value1,property.GetValue(instance));
            Assert.AreEqual(2,eventTriggerCount);
            Assert.AreEqual(value0, oldValue);
            Assert.AreEqual(value1, newValue);

            property.SetValue(instance,value0); //should not trigger handler
            Assert.AreEqual(value0,property.GetValue(instance));
            Assert.AreEqual(2,eventTriggerCount);
        }

    }
}
