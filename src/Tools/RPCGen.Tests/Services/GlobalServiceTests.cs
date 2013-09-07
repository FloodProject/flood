
using System.Threading.Tasks;
using Flood.RPC.Metadata;
using NUnit.Framework;

namespace RPCGen.Tests.Services
{
    [TestFixture]
    public class GlobalServiceTests
    {
        #region Service
        [GlobalService(1)]
        public interface IService
        {
            [Id(1)]
            Task<int> Test([Id(0)]int param);
        }

        public class Service : IService
        {
            public async Task<int> Test(int param)
            {
                return param;
            }
        }

        #endregion

        [Test]
        public void TestGlobalService()
        {
            var rpcManager = Helper.CreateRPCManager();
            var remotePeer = Helper.CreateRemotePeer(rpcManager);

            var globalServiceImpl = new Service();
            rpcManager.ServiceManager.GetCreateImplementation<IService>(globalServiceImpl);

            var proxy = rpcManager.GetGlobalService<IService>(remotePeer);

            var i = 1337;

            var t1 = proxy.Test(i);

            Assert.IsTrue(t1.Wait(100));
            Assert.AreEqual(i, t1.Result);
        }
    }
}
