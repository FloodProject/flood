
using System.Threading;
using Flood.GUI.Controls;
using NUnit.Framework;

namespace Flood.Tests
{
    [TestFixture]
    class GUITests
    {
        private GUI GUI;

        [TestFixtureSetUp]
        public void Setup()
        {
            GUI = new GUI("DefaultSkin.png", "Vera.ttf", 16);
        }

        [TestFixtureTearDown]
        public void TearDown()
        {
            GUI.Dispose();
        }

        [Test]
        public void TestButton()
        {
            using (var canvas = new Canvas(GUI.Skin))
            {
                var control = new Button(canvas);
                control.Text = "Button";

                GUI.AssertUnchanged(canvas, "button1");
            }
        }

    }
}
