using Flood;
using Flood.RPC;
using Flood.RPC.Metadata;
using NUnit.Framework;
using System.Threading.Tasks;

namespace RPCGen.Tests.Services
{
    [TestFixture]
    public class ProcedureOverloadTests
    {
        #region Service
        [Service]
        public interface IService
        {
            [Id(1)]
            Task Test();

            [Id(2)]
            Task<int> Test([Id(0)]int param);

            [Id(3)]
            Task<double> Test([Id(0)]double param);

            [Id(4)]
            Task<int> Test([Id(0)]int param1, [Id(1)]int param2);
        }

        public class Service : IService
        {

            public async Task Test()
            {
            }

            public async Task<int> Test(int param)
            {
                return param;
            }

            public async Task<double> Test(double param)
            {
                return param+1;
            }

            public async Task<int> Test(int param1, int param2)
            {
                return param1 + param2;
            }
        }

        #endregion

        [Test]
        public void TestOverload()
        {
            var proxy = Helper.GetProxy<IService>(new Service());
            proxy.Test();

            var i = 1337;

            var t1 = proxy.Test(i);
            var t2 = proxy.Test((double)i);
            var t3 = proxy.Test(i, i);

            Assert.IsTrue(t1.Wait(1000));
            Assert.AreEqual(i, t1.Result);

            Assert.IsTrue(t2.Wait(1000));
            Assert.AreEqual(i + 1, t2.Result);

            Assert.IsTrue(t3.Wait(1000));
            Assert.AreEqual(i + i, t3.Result);
        }
    }
}
