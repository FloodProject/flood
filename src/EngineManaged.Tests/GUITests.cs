
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
            GUI = new GUI();
        }

        [TestFixtureTearDown]
        public void TearDown()
        {
            GUI.Dispose();
        }

        [Test]
        public void TestButton()
        {
            var canvas = GUI.guiRenderable.Canvas;

            canvas.Children.Clear();

            var control = new Button(canvas);
            control.Text = "Button";

            GUI.AssertUnchanged("button1");
        }
    }
}
