using System;
using System.Diagnostics;
using System.IO;
using Flood.GUI;
using Flood.GUI.Controls;
using NUnit.Framework;

namespace Flood.Tests
{
    class GUI : IDisposable
    {
        private const string StoredValuesDirectory = "../../GUITests";

        private static Application app;

        public Windows.Window window;
        public GuiRenderable guiRenderable;

        public GUI()
        {
            app = new Application();

            var windowTask = app.WindowManager.GetCreateWindow("Test");
            windowTask.Wait();
            window = windowTask.Result;

            guiRenderable = new GuiRenderable();
            window.AddRenderable(guiRenderable);
        }

        private void Render(string outputPath)
        {
            var width = 0;
            var height = 0;
            foreach (var control in guiRenderable.Canvas.Children)
                RenderBounds(control, ref width, ref height);

            app.RunStep();

            var imageSettings = new Settings { Height = (ushort)height, Width = (ushort)width };
            var renderBuffer = app.RenderDevice.Backend.CreateRenderBuffer(imageSettings);
            renderBuffer.CreateRenderBuffer(RenderBufferType.Color);
            renderBuffer.CreateRenderBuffer(RenderBufferType.Depth);

            renderBuffer.Context = window.RenderContext;
            window.view.RenderTarget = renderBuffer;
            window.view.Size = new Vector2i(width, height);
            guiRenderable.Canvas.SetSize(width, height);

            window.device.ActiveView = window.view;

            if (!renderBuffer.Check())
                throw new Exception("Error creating render buffer");

            renderBuffer.Bind();

            window.Render();

            var image = renderBuffer.ReadImage(1);

            var imageWriter = new ImageWriter();
            imageWriter.Save(image, outputPath);

            renderBuffer.Unbind();
        }

        private void RenderBounds(Control control, ref int width, ref int height)
        {
            if (control.Right > width)
                width = control.Right;

            if (control.Bottom > height)
                height = control.Bottom;
        }

        public void AssertUnchanged(string assertId, string assertMessage = "")
        {
            var renderChanged = HasRenderChanged(assertId);
            var serializationChanged = HasSerializationChanged(assertId);

            Assert.IsFalse(renderChanged, assertId+" render changed. "+assertMessage);
            Assert.IsFalse(serializationChanged, assertId+" serialization changed. "+assertMessage);
        }

        #region Assert Render
        private bool HasRenderChanged(string assertId)
        {
            var baseDirectory = Path.GetFullPath(StoredValuesDirectory);
            var resultImagePath = Path.Combine(baseDirectory, assertId + "_result.png");
            var expectedImagePath = Path.Combine(baseDirectory, assertId + "_expected.png");
            var diffImagePath = Path.Combine(baseDirectory, assertId + "_diff.png");

            Directory.CreateDirectory(baseDirectory);

            Render(resultImagePath);

            if (!File.Exists(expectedImagePath))
            {
                File.Copy(resultImagePath, expectedImagePath);
                File.Delete(resultImagePath);
                return false;
            }

            if (AreImagesEqual(expectedImagePath, resultImagePath, diffImagePath))
            {
                File.Delete(resultImagePath);
                File.Delete(diffImagePath);
                return false;
            }

            return true;
        }

        private bool AreImagesEqual(string expectedImagePath, string resultImagePath, string diffImagePath)
        {
            var startInfo = new ProcessStartInfo();
            startInfo.CreateNoWindow = false;
            startInfo.UseShellExecute = false;
            startInfo.FileName = "pdiff.exe";
            startInfo.WindowStyle = ProcessWindowStyle.Hidden;
            startInfo.RedirectStandardOutput = true;
            startInfo.Arguments = string.Format(
                "\"{0}\" \"{1}\" -output \"{2}\" -verbose", expectedImagePath, resultImagePath, diffImagePath);

            using (Process exeProcess = Process.Start(startInfo))
            {
                exeProcess.WaitForExit();

                var output = exeProcess.StandardOutput.ReadToEnd();
                if (output.Contains("PASS"))
                    return true;

                return false;
            }
        }
        #endregion

        #region Assert Serialization
        private static bool HasSerializationChanged(string assertId)
        {
            return false;
        }
        #endregion

        public void Dispose()
        {
            guiRenderable.Dispose();
        }
    }
}
