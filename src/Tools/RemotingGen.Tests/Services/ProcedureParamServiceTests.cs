using Flood.Remoting.Metadata;
using NUnit.Framework;
using System;
using System.Threading.Tasks;

namespace RemotingGen.Tests.Services
{
    [TestFixture]
    public class ProcedureParamServiceTests
    {
        #region Service

        [Service]
        public interface IService2
        {
            [Id(1)]
            Task<string> TestString(string value);
        }

        [Service]
        public interface IService
        {
            [Id(1)]
            Task<string> TestString(IService2 service, string value);
        }

        public class Service2 : IService2
        {
            public async Task<string> TestString(string value)
            {
                return value;
            }
        }

        public class Service : IService
        {
            public Task<string> TestString(IService2 service, string value)
            {
                return service.TestString(value);
            }
        }

        #endregion

        Service impl1;
        Service2 impl2;
        IService proxy1;

        [TestFixtureSetUp]
        public void SetUp()
        {
            impl1 = new Service();
            impl2 = new Service2();
            proxy1 = Helper.GetProxy<IService>(impl1);
        }

        [Test]
        public void TestParamService()
        {
            var expected = "123456789";
            var task = proxy1.TestString(impl2, expected);
            Assert.IsTrue(task.Wait(100));
            Assert.AreEqual(expected, task.Result);
        }
    }
}
