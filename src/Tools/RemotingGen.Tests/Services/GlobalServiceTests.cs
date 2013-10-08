
using System.Threading.Tasks;
using Flood.Remoting.Metadata;
using NUnit.Framework;

namespace RemotingGen.Tests.Services
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
            var messageProcessor = Helper.CreateMessageProcessor();
            var remotePeer = Helper.CreatePeer(messageProcessor);

            var globalServiceImpl = new Service();
            messageProcessor.ServiceManager.GetCreateImplementation<IService>(globalServiceImpl);

            var proxy = messageProcessor.ServiceManager.GetGlobalService<IService>(remotePeer);

            var i = 1337;

            var t1 = proxy.Test(i);

            Assert.IsTrue(t1.Wait(100));
            Assert.AreEqual(i, t1.Result);
        }
    }
}
