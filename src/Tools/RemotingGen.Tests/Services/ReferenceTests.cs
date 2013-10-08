using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.Remoting;
using Flood.Remoting.Metadata;
using NUnit.Framework;

namespace RemotingGen.Tests.Services
{
    [TestFixture]
    public class ReferenceTests
    {
        #region Service
        
        [DataObject(1)]
        public class Data
        {
            [Id(1)] 
            public int Number;
        }

        [Service]
        public interface IService
        {
            [Id(1)]
            Task<Data> GetData();

        }

        public class Service : IService
        {
            public Data Data = new Data();

            public async Task<Data> GetData()
            {
                ReferenceManager.Publish(Data);
                return Data;
            }
        }

        #endregion

        [Test]
        public void TestReference()
        {
            var impl = new Service();
            var proxy = Helper.GetProxy<IService>(impl);
            
            impl.Data.Number = 1;

            var t = proxy.GetData();
            Assert.IsTrue(t.Wait(100));

            var data = t.Result;
            Assert.NotNull(data);
            Assert.AreEqual(1, data.Number);

            Assert.IsTrue(ReferenceManager.Subscribe(data));

            impl.Data.Number = 1337;

            ((ServiceProxy) proxy).RemotingManager.ReferenceManager.DispatchChanges();

            Assert.AreEqual(1337, data.Number);
        }
    }
}
