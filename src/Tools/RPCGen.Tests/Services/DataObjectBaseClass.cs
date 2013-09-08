using System.Threading.Tasks;
using Flood.RPC;
using Flood.RPC.Metadata;
using NUnit.Framework;

namespace RPCGen.Tests.Services
{
    [TestFixture]
    public class DataObjectBaseClass
    {
        #region Service
        [DataObject(14)]
        public class DataObjectBase
        {
            [Id(0)]
            public int BaseNumber;
        }

        [DataObject(15)]
        public class DataObject : DataObjectBase
        {
            [Id(0)]
            public int Number;
        }
        
        [Service]
        public interface IService
        {
            [Id(0)]
            Task<DataObject> GetDataObject(int baseNumber, int number);
        }

        public class Service : IService
        {
            public DataObject Data = new DataObject(); 

            public async Task<DataObject> GetDataObject(int baseNumber, int number)
            {
                Data.BaseNumber = baseNumber;
                Data.Number = number;
                return Data;
            }
        }
        #endregion

        [Test]
        public void TestDataObjectBase()
        {
            var impl = new Service();
            var proxy = Helper.GetProxy<IService>(impl);

            var task = proxy.GetDataObject(8453, 1337);

            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(task.Result.Number, 1337);
            Assert.AreEqual(task.Result.BaseNumber, 8453);
        }
    }
}
