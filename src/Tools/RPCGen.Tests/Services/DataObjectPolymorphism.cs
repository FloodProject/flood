
using System.Threading.Tasks;
using Flood.RPC;
using Flood.RPC.Metadata;
using NUnit.Framework;

namespace RPCGen.Tests.Services
{
    [TestFixture]
    public class DataObjectPolymorphism
    {
        #region Service
        [DataObject(16)]
        public class DataObjectBase
        {
            public  virtual string GetText()
            {
                return "Base";
            }
        }

        [DataObject(17)]
        public class DataObject : DataObjectBase
        {
            public  override string GetText()
            {
                return "Override";
            }
        }
        
        [Service]
        public interface IService
        {
            [Id(0)]
            Task<DataObjectBase> GetDataObject();
        }

        public class Service : IService
        {
            public DataObject Data = new DataObject(); 

            public async Task<DataObjectBase> GetDataObject()
            {
                return Data;
            }
        }
        #endregion

        [Test]
        public void TestDataObjectPolymorphism()
        {
            var impl = new Service();
            var proxy = Helper.GetProxy<IService>(impl);

            var rpcProxy = (RPCProxy) proxy;
            rpcProxy.RPCManager.ContextManager.RequestContext(rpcProxy.Peer, typeof(IService).Assembly);

            var task = proxy.GetDataObject();

            Assert.IsTrue(task.Wait(100));

            var data = task.Result;

            Assert.True(data is DataObject);

            Assert.AreEqual("Override", data.GetText());
        }
    }
}
