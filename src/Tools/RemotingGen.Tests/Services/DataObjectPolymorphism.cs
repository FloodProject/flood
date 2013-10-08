
using System.Threading.Tasks;
using Flood.Remoting;
using Flood.Remoting.Metadata;
using NUnit.Framework;

namespace RemotingGen.Tests.Services
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
            [Id(0)]
            public int Number;

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

            impl.Data.Number = 1337;

            var ServiceProxy = (ServiceProxy) proxy;
            ServiceProxy.RemotingManager.ContextManager.RequestContext(ServiceProxy.Peer, typeof(IService).Assembly);

            var task = proxy.GetDataObject();

            Assert.IsTrue(task.Wait(100));

            var data = task.Result;

            Assert.True(data is DataObject);

            Assert.AreEqual("Override", data.GetText(), "Failed to call overrride.");

            Assert.AreEqual(1337, ((DataObject) data).Number);
        }
    }
}
