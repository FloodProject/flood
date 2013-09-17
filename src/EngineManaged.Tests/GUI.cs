using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Security;
using Flood.GUI;
using Flood.GUI.Controls;
using Flood.GUI.Skins;
using NUnit.Framework;

namespace Flood.Tests
{
    class GUI : IDisposable
    {
        private const string StoredValuesDirectory = "../../GUITests";
        private const string AssetsDirectory = "bin/Assets/";

        [SuppressUnmanagedCodeSecurity]
        [DllImport("Editor.Native.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr CreateWxPlatformManager();

        private Engine Engine { get; set; }

        private PlatformManager PlatformManager { get; set; }
        private WindowManager WindowManager { get; set; }
        private Window MainWindow { get; set; }
        private ArchiveVirtual Archive { get; set; }
        private ResourceManager ResourceManager { get; set; }

        private RenderDevice RenderDevice;
        private RenderView MainView;

        private GwenRenderer Renderer;

        public Skin Skin;

        public GUI(string textureName, string defaultFontName, int defaultFontSize)
        {
            PlatformManager = new PlatformManager(CreateWxPlatformManager());

            Engine = new Engine(PlatformManager);
            Engine.Init();

            // Search for the location of the main assets folder.
            string assetsPath;
            if (!SearchForAssetsDirectory(out assetsPath))
                throw new Exception("Editor assets were not found");

            Archive = new ArchiveVirtual();
            Archive.MountDirectories(assetsPath, Allocator.GetHeap());

            ResourceManager = Engine.ResourceManager;
            ResourceManager.Archive = Archive;

            WindowManager = Engine.WindowManager;

            MainWindow = CreateWindow();
            var context = MainWindow.CreateContext(new RenderContextSettings());

            MainView = MainWindow.CreateView();
            MainView.ClearColor = Color.Red;

            MainWindow.MakeCurrent();
            context.Init();

            RenderDevice = Engine.RenderDevice;
            RenderDevice.ActiveView = MainView;

            var options = new ResourceLoadOptions {Name = textureName, AsynchronousLoad = false};
            var imageHandle = ResourceManager.LoadResource<Image>(options);
            if (imageHandle.Id == 0)
                return;

            Renderer = new GwenRenderer();
            Skin = new TexturedSkin(Renderer, imageHandle, new Flood.GUI.Font(defaultFontName, defaultFontSize));

            if (!Directory.Exists(StoredValuesDirectory))
                Directory.CreateDirectory(StoredValuesDirectory);
        }

        private bool SearchForAssetsDirectory(out string path)
        {
            var currentDirectory = Directory.GetCurrentDirectory();

            while (true)
            {
                var fullPath = Path.Combine(currentDirectory, AssetsDirectory);

                if (Directory.Exists(fullPath))
                {
                    path = fullPath;
                    return true;
                }

                var parentDirectory = Directory.GetParent(currentDirectory);
                if (parentDirectory == null) break;

                currentDirectory = parentDirectory.FullName;
            }

            path = null;
            return false;
        }

        private Window CreateWindow()
        {
            var settings = new WindowSettings(1, 1, "GUI", WindowStyles.TopLevel);

            return WindowManager.CreateWindow(settings);
        }

        private void Render(Canvas canvas, string outputPath)
        {
            canvas.RenderCanvas();

            var width = 0;
            var height = 0;
            foreach (var control in canvas.Children)
                RenderBounds(control, ref width, ref height);

            var imageSettings = new Settings { Height = (ushort)height, Width = (ushort)width };
            var renderBuffer = RenderDevice.Backend.CreateRenderBuffer(imageSettings);
            renderBuffer.CreateRenderBuffer(RenderBufferType.Color);
            renderBuffer.CreateRenderBuffer(RenderBufferType.Depth);

            renderBuffer.Context = MainWindow.Context;
            MainView.RenderTarget = renderBuffer;
            MainView.Size = new Vector2i(width, height);
            canvas.SetSize(width, height);

            RenderDevice.ActiveView = MainView;

            if (!renderBuffer.Check())
                throw new Exception("Error creating render buffer");

            renderBuffer.Bind();

            var rb = new RenderBlock();

            Renderer.Render(rb);

            RenderDevice.ClearView();

            RenderDevice.Render(rb);
            Engine.StepFrame();

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

        public void AssertUnchanged(Canvas canvas, string assertId, string assertMessage = "")
        {
            var renderChanged = HasRenderChanged(canvas, assertId);
            var serializationChanged = HasSerializationChanged(canvas, assertId);

            Assert.IsFalse(renderChanged, assertId+" render changed. "+assertMessage);
            Assert.IsFalse(serializationChanged, assertId+" serialization changed. "+assertMessage);
        }

        #region Assert Render
        private bool HasRenderChanged(Canvas canvas, string assertId)
        {
            var baseDirectory = Path.GetFullPath(StoredValuesDirectory);
            var resultImagePath = Path.Combine(baseDirectory, assertId + "_result.png");
            var expectedImagePath = Path.Combine(baseDirectory, assertId + "_expected.png");
            var diffImagePath = Path.Combine(baseDirectory, assertId + "_diff.png");

            Render(canvas, resultImagePath);

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
        private static bool HasSerializationChanged(Control control, string assertId)
        {
            return false;
        }
        #endregion

        public void Dispose()
        {
            Skin.Dispose();
            Renderer.Dispose();
        }
    }
}
