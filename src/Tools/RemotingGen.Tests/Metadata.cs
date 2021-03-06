﻿using System.Collections.Generic;
using Flood.Remoting.Metadata;
using NUnit.Framework;

namespace RemotingGen.Tests
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

    [GlobalService(1337)]
    interface IGlobalService
    {
        
    }

    [DataObject(1337)]
    class DataObject
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
            Assert.IsTrue(RemotingGen.Metadata.IsService(typeof (IService)), "Is not a service");
            Assert.IsTrue(RemotingGen.Metadata.IsService(typeof (IGlobalService)), "Is not a service");

            ushort id;
            RemotingGen.Metadata.TryGetGlobalServiceId(typeof(IGlobalService), out id);
            Assert.AreEqual(1337, id, "Can not retrieve GlobalService id.");
        }

        [Test]
        public void DataObject()
        {
            Assert.IsTrue(RemotingGen.Metadata.IsDataObject(typeof(DataObject)), "Is not a DataObject");

            ushort id;
            RemotingGen.Metadata.TryGetDataObjectId(typeof(DataObject), out id);
            Assert.AreEqual(1337, id, "Can not retrieve DataObject id.");
        }

        [Test]
        public void Exception()
        {
            Assert.IsTrue(RemotingGen.Metadata.IsException(typeof (Exception)), "Is not an exception");
        }

        [Test]
        public void Flags()
        {
            var method = typeof (IService).GetMethod("Ping");
            Assert.IsNotNull(method);

            Assert.IsTrue(RemotingGen.Metadata.HasEncrypted(method), "Flag Encrypted not found");
            Assert.IsTrue(RemotingGen.Metadata.HasCompressed(method), "Flag Compressed not found");
            Assert.IsTrue(RemotingGen.Metadata.HasSigned(method), "Flag Signed not found");
        }

        [Test]
        public void Throws()
        {
            var method = typeof (IService).GetMethod("Ping");
            Assert.IsNotNull(method);

            Assert.IsTrue(RemotingGen.Metadata.HasThrows(method), "Throws not found");

            List<ExceptionInfo> exInfo;
            RemotingGen.Metadata.TryGetThrows(method, out exInfo);
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

            Assert.IsTrue(RemotingGen.Metadata.IsRequired(field), "Field is not required");
        }

        [Test]
        public void Id()
        {
            var field = typeof(DataObject).GetField("Field");
            Assert.IsNotNull(field);

            Assert.IsTrue(RemotingGen.Metadata.HasId(field), "Field has not an id");

            int id;
            RemotingGen.Metadata.TryGetId(field, out id);
            Assert.AreEqual(1337, id, "Can not retrieve field id.");
        }
    }

}
