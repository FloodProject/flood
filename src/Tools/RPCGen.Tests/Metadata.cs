using System.Collections.Generic;
using Flood.RPC.Metadata;
using NUnit.Framework;

namespace RPCGen.Tests
{
    [Service]
    interface IService
    {
        [Throws(6, typeof(Exception))]
        [Encrypted]
        [Compressed]
        [Signed]
        bool Ping();
    }

    [DataObject("7D96CE1A-B266-44AF-9C77-85DB8479B5FB")]
    struct DataObject
    {
        [Required, Id(1337)] 
        public int Field;
    }

    [Exception]
    class Exception{}

    [TestFixture]
    internal class Metadata
    {
        [Test]
        public void Service()
        {
            Assert.IsTrue(RPCGen.Metadata.IsService(typeof (IService)), "Is not a service");
        }

        [Test]
        public void DataObject()
        {
            Assert.IsTrue(RPCGen.Metadata.IsDataObject(typeof(DataObject)), "Is not a DataObject");
        }

        [Test]
        public void Exception()
        {
            Assert.IsTrue(RPCGen.Metadata.IsException(typeof (Exception)), "Is not an exception");
        }

        [Test]
        public void Flags()
        {
            var method = typeof (IService).GetMethod("Ping");
            Assert.IsNotNull(method);

            Assert.IsTrue(RPCGen.Metadata.HasEncrypted(method), "Flag Encrypted not found");
            Assert.IsTrue(RPCGen.Metadata.HasCompressed(method), "Flag Compressed not found");
            Assert.IsTrue(RPCGen.Metadata.HasSigned(method), "Flag Signed not found");
        }

        [Test]
        public void Throws()
        {
            var method = typeof (IService).GetMethod("Ping");
            Assert.IsNotNull(method);

            Assert.IsTrue(RPCGen.Metadata.HasThrows(method), "Throws not found");

            List<ExceptionInfo> exInfo;
            RPCGen.Metadata.TryGetThrows(method, out exInfo);
            Assert.IsNotNull(exInfo);
            Assert.AreEqual(1, exInfo.Count);
            Assert.AreEqual(6, exInfo[0].Id);
            Assert.AreEqual(typeof (Exception), exInfo[0].Type);
        }

        [Test]
        public void Required()
        {
            var field = typeof(DataObject).GetField("Field");
            Assert.IsNotNull(field);

            Assert.IsTrue(RPCGen.Metadata.IsRequired(field), "Field is not required");
        }

        [Test]
        public void Id()
        {
            var field = typeof(DataObject).GetField("Field");
            Assert.IsNotNull(field);

            Assert.IsTrue(RPCGen.Metadata.HasId(field), "Field has not an id");

            int id;
            RPCGen.Metadata.TryGetId(field, out id);
            Assert.AreEqual(1337, id, "Can not retrieve field id.");
        }
    }

}
