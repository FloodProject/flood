using NUnit.Framework;

namespace Flood.Editor.Tests
{
    [TestFixture]
    public class ServerTests
    {
        [Test]
        public void TestAdd()
        {
            Assert.That(2, Is.EqualTo(2));
        }
    }

    [TestFixture]
    public class ProjectManagerTests
    {
        [Test]
        public void TestCreateProject()
        {
            Assert.That(2, Is.EqualTo(2));
        }
    }
}