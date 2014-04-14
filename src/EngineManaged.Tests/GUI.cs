using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using Flood.GUI;
using Flood.GUI.Controls;

namespace Flood.Tests
{
    class GUI : IDisposable
    {
        const string TestsDirectory = "../../../../tests/GUI";
        readonly string BaseDirectory;

        static Application app;
        public Windows.Window window;
        public Windows.Window nativeWindow;
        public GuiRenderable guiRenderable;

        public GUI()
        {
            BaseDirectory = Path.GetFullPath(TestsDirectory);
            Directory.CreateDirectory(BaseDirectory);

            // Rendering initialization code
            app = new Application();

            var windowTask = app.WindowManager.GetCreateWindow("Test");
            windowTask.Wait();
            window = windowTask.Result;

            nativeWindow = app.WindowManager.GetWindow("Test");
            nativeWindow.nativeWindow.Show(visible: false);

            guiRenderable = new GuiRenderable();
            window.AddRenderable(guiRenderable);
        }

        #region Common testing logic

        public void Test(Control control, string assertId, string assertMessage = "")
        {
            var renderChanged = HasRenderChanged(control, assertId);
            NUnit.Framework.Assert.IsFalse(renderChanged,
                assertId + " render changed. " + assertMessage);

            var serializationChanged = HasSerializationChanged(control, assertId);
            NUnit.Framework.Assert.IsFalse(serializationChanged,
                assertId + " serialization changed. " + assertMessage);
        }

        delegate void GenerateDataDelegate(string path);
        delegate bool CompareDataDelegate(string expected, string actual, string diff);

        bool Assert(string assertId, string extension, GenerateDataDelegate generator,
            CompareDataDelegate comparer)
        {
            var actualPath = Path.Combine(BaseDirectory, assertId + "_actual." + extension);
            var expectedPath = Path.Combine(BaseDirectory, assertId + "_expected." + extension);
            var diffPath = Path.Combine(BaseDirectory, assertId + "_diff." + extension);

            if (!File.Exists(expectedPath))
            {
                generator(expectedPath);
                return false;
            }

            generator(actualPath);

            if (comparer(expectedPath, actualPath, diffPath))
            {
                File.Delete(actualPath);
                File.Delete(diffPath);
                return false;
            }

            return true;
        }

        #endregion

        #region GUI rendering tests

        bool HasRenderChanged(Control id, string assertId)
        {
            return Assert(assertId, "png", Render, CompareImages);
        }

        void Render(string outputPath)
        {
            var width = 0;
            var height = 0;

            // current layout algorithm is bugged,
            // multiple passes are neccessary to render properly
            for (int i = 0; i < 5; i++)
                guiRenderable.Canvas.Update();

            app.RunStep();

            foreach (var control in guiRenderable.Canvas.Children)
                RenderBounds(control, ref width, ref height);

            var imageSettings = new Settings { Height = (ushort)height, Width = (ushort)width };
            var renderBuffer = app.RenderDevice.Backend.CreateRenderBuffer(imageSettings);
            renderBuffer.CreateRenderBuffer(RenderBufferType.Color);
            renderBuffer.CreateRenderBuffer(RenderBufferType.Depth);

            renderBuffer.Context = window.RenderContext;
            window.view.ClearColor = Color.White;
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

        void RenderBounds(Control control, ref int width, ref int height)
        {
            if (control.Right > width)
                width = control.Right;

            if (control.Bottom > height)
                height = control.Bottom;
        }

        public void Dispose()
        {
            guiRenderable.Dispose();
        }

        static bool CompareImages(string expected, string actual, string diff)
        {
            var startInfo = new ProcessStartInfo
            {
                CreateNoWindow = true,
                UseShellExecute = false,
                FileName = "pdiff.exe",
                WindowStyle = ProcessWindowStyle.Hidden,
                RedirectStandardOutput = true,
                Arguments = string.Format(
                    "\"{0}\" \"{1}\" -output \"{2}\" -verbose", expected,
                    actual, diff)
            };

            using (var exeProcess = Process.Start(startInfo))
            {
                exeProcess.WaitForExit();

                var output = exeProcess.StandardOutput.ReadToEnd();
                return output.Contains("PASS");
            }
        }

        #endregion

        #region GUI serialization tests

        bool HasSerializationChanged(Control control, string assertId)
        {
            var layout = DumpLayout(control);
            var actualText = SerializeDumpData(layout);

            GenerateDataDelegate generateLayout = path =>
                File.WriteAllText(path, actualText);

            CompareDataDelegate compareLayouts = (expected, actual, diff) =>
            {
                var expectedText = File.ReadAllText(expected);
                var diffEngine = new my.utils.Diff();
                var diffs = diffEngine.DiffText(actualText, expectedText);

                if (diffs.Length == 0)
                    return true;

                // TODO: Calculate a text-based diff patch and write it in a file.
                return false;
            };

            return Assert(assertId, "json", generateLayout, compareLayouts);
        }

        sealed class GUIDumpData
        {
            public string Control;
            public string Name;
            public int X;
            public int Y;
            public int Width;
            public int Height;
            public int MinWidth;
            public int MinHeight;
            public int MaxWidth;
            public int MaxHeight;
            public bool IsHidden;
            public bool Focused;
            public List<GUIDumpData> Children;
        }

        static GUIDumpData DumpLayout(Control control)
        {
            // Find the root control in the hierarchy.
            var rootControl = control;
            while (rootControl.Parent != null)
                rootControl = rootControl.Parent;

            return BuildDumpTree(rootControl);
        }

        static GUIDumpData BuildDumpTree(Control rootControl)
        {
            var data = new GUIDumpData
            {
                Control = rootControl.GetType().Name,
                Name = rootControl.Name,
                X = rootControl.X,
                Y = rootControl.Y,
                Width = rootControl.Width,
                Height = rootControl.Height,
                MinWidth = rootControl.MinimumSize.X,
                MinHeight = rootControl.MinimumSize.Y,
                MaxWidth = rootControl.MaximumSize.X,
                MaxHeight = rootControl.MaximumSize.Y,
                IsHidden = rootControl.IsHidden,
                Focused = rootControl.HasFocus,
            };

            if (rootControl.Children.Count != 0)
                data.Children = new List<GUIDumpData>();

            // Recursively go through each child to build the dump tree.
            foreach (var child in rootControl.Children)
                data.Children.Add(BuildDumpTree(child));

            return data;
        }

        static string SerializeDumpData(GUIDumpData data)
        {
            var json = fastJSON.JSON.Instance;
            json.Parameters.UseExtensions = false;
            json.Parameters.SerializeNullValues = false;

            return json.Beautify(json.ToJSON(data));
        }

        #endregion
    }
}
