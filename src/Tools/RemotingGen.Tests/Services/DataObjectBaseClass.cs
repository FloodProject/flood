using System.Threading.Tasks;
using Flood.Remoting;
using Flood.Remoting.Metadata;
using NUnit.Framework;

namespace RemotingGen.Tests.Services
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
                ReferenceManager.Publish(Data);
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

        [Test]
        public void TestDataObjectBaseRef()
        {
            var impl = new Service();
            var proxy = Helper.GetProxy<IService>(impl);

            var task = proxy.GetDataObject(8453, 1337);

            var result = task.Result;
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(1337, result.Number);
            Assert.AreEqual(8453, result.BaseNumber);

            Assert.IsTrue(ReferenceManager.Subscribe(result));

            impl.Data.BaseNumber = 1;

            ((ServiceProxy)proxy).RemotingManager.ReferenceManager.DispatchChanges();

            Assert.AreEqual(1, result.BaseNumber);
        }
    }
}
