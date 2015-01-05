using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using Flood.GUIv2;
using Flood.GUIv2.Controls;

namespace Flood.Tests
{
    class GUIv2 : IDisposable
    {
        const string TestsDirectory = "../../../../tests/GUIv2";
        readonly string BaseDirectory;

        static Application app;
        public Windows.Window window;
        public Windows.Window nativeWindow;
        public GuiRenderable guiRenderable;

        public GUIv2()
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

        public void ResetRenderable()
        {
            window.RemoveRenderable(guiRenderable);
            guiRenderable.Dispose();
            guiRenderable = new GuiRenderable();
            window.AddRenderable(guiRenderable);
        }

        #region Common testing logic

        public void Test(IControl control, string folder, string assertId, string assertMessage = "")
        {
            if (!Directory.Exists(Path.Combine(BaseDirectory, folder)))
                Directory.CreateDirectory(Path.Combine(BaseDirectory, folder));

            var renderChanged = HasRenderChanged(control, folder + Path.DirectorySeparatorChar + assertId);
            var serializationChanged = HasSerializationChanged(control, folder + Path.DirectorySeparatorChar + assertId);

            NUnit.Framework.Assert.IsFalse(renderChanged,
                assertId + " render changed. " + assertMessage);
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

        bool HasRenderChanged(IControl id, string assertId)
        {
            return Assert(assertId, "png", Render, CompareImages);
        }

        void Render(string outputPath)
        {
            int width, height;

            app.RunStep();

            guiRenderable.Canvas.GetRenderBounds(out width, out height);

            var imageSettings = new Settings { Height = (ushort)height, Width = (ushort)width };
            var renderBuffer = app.RenderDevice.Backend.CreateRenderBuffer(imageSettings);
            renderBuffer.CreateRenderBuffer(RenderBufferType.Color);
            renderBuffer.CreateRenderBuffer(RenderBufferType.Depth);

            renderBuffer.Context = window.RenderContext;
            window.view.ClearColor = Color.White;
            window.view.RenderTarget = renderBuffer;
            window.view.Size = new Vector2i(width, height);
            //guiRenderable.Canvas.SetSize(width, height);

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

        bool HasSerializationChanged(IControl control, string assertId)
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
            public int BestWidth;
            public int BestHeight;
            public int MinWidth;
            public int MinHeight;
            public int MaxWidth;
            public int MaxHeight;
            public int MarginRight;
            public int MarginTop;
            public int MarginLeft;
            public int MarginBottom;
            public bool IsHidden;
            public bool IsExpandHorizontal;
            public bool IsExpandVertical;
            public bool IsFillHorizontal;
            public bool IsFillVertical;
            public bool IsFit;
            public bool Focused;
            public List<GUIDumpData> Children;
        }

        static GUIDumpData DumpLayout(IControl control)
        {
            // Find the root control in the hierarchy.
            var rootControl = control;
            while (rootControl.Parent != null)
                rootControl = rootControl.Parent;

            return BuildDumpTree(rootControl);
        }

        static GUIDumpData BuildDumpTree(IControl rootControl)
        {
            var data = new GUIDumpData
                           {
                               Control = rootControl.GetType().Name,
                               Name = rootControl.Name,
                               X = rootControl.X,
                               Y = rootControl.Y,
                               Width = rootControl.Width,
                               Height = rootControl.Height,
                               BestWidth = rootControl.BestSize.X,
                               BestHeight = rootControl.BestSize.Y,
                               MinWidth = rootControl.MinimumSize.X,
                               MinHeight = rootControl.MinimumSize.Y,
                               MaxWidth = rootControl.MaximumSize.X,
                               MaxHeight = rootControl.MaximumSize.Y,
                               MarginRight = rootControl.Margin.Right,
                               MarginTop = rootControl.Margin.Top,
                               MarginLeft = rootControl.Margin.Left,
                               MarginBottom = rootControl.Margin.Bottom,
                               IsHidden = rootControl.IsHidden,
                               IsExpandHorizontal = rootControl.IsExpandHorizontal,
                               IsExpandVertical = rootControl.IsExpandVertical,
                               IsFillHorizontal = rootControl.IsFillHorizontal,
                               IsFillVertical = rootControl.IsFillVertical,
                               IsFit = rootControl.IsFit,
                               Focused = rootControl.HasFocus,
                           };

            var control = rootControl as Panel;
            if (control != null)
            {
                if (control.Children.Count != 0)
                    data.Children = new List<GUIDumpData>();
         
                // Recursively go through each child to build the dump tree.
                foreach (var child in control.Children)
                    data.Children.Add(BuildDumpTree(child));
            }

            var compositeControl = rootControl as CompositeControl;
            if (compositeControl != null)
            {
                if (compositeControl.Children.Count != 0)
                    data.Children = new List<GUIDumpData>();
         
                // Recursively go through each child to build the dump tree.
                foreach (var child in compositeControl.Children)
                    data.Children.Add(BuildDumpTree(child));
            }

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
