using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.RPC;
using Flood.RPC.Metadata;
using NUnit.Framework;

namespace RPCGen.Tests.Services
{
    [TestFixture]
    public class ReferenceTests
    {
        #region Service
        
        [DataObject("B5865AB2-75E2-4B29-A9C9-07385B71DE8C")]
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

            ((RPCProxy) proxy).RPCManager.ReferenceManager.DispatchChanges();

            Assert.AreEqual(1337, data.Number);
        }
    }
}
