using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Security;
using Flood;

namespace FLDEditor
{
    public class Editor
    {
        [SuppressUnmanagedCodeSecurity]
        [DllImport("Editor.Native.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateWxPlatformManager();

        public Engine Engine { get; set; }

        public ResourceManager ResourceManager { get; set; }
        public ArchiveVirtual Archive { get; set; }

        public PlatformManager PlatformManager { get; set; }
        public WindowManager WindowManager { get; set; }
        public Window MainWindow { get; set; }
        public List<Window> ChildWindows { get; set; }

        public RenderDevice RenderDevice;
        public RenderView MainView;

        public EditorWindow EditorWindow { get; set; }

        public Editor()
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

            EditorWindow = new EditorWindow();

            WindowManager = Engine.WindowManager;

            MainWindow = CreateWindow();
            var context = MainWindow.CreateContext(new RenderContextSettings());

            MainView = MainWindow.CreateView();
            MainView.ClearColor = Color.Red;

            MainWindow.Idle += Update;
            MainWindow.Render += Render;
            MainWindow.TargetResize += Resize;
            MainWindow.Show(true);

            MainWindow.MakeCurrent();
            context.Init();

            RenderDevice = Engine.RenderDevice;
        }

        private const string AssetsDirectory = "bin/Assets/";

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

        public Window CreateWindow()
        {
            var settings = new WindowSettings(1000, 700, "Editor",
                WindowStyles.TopLevel);

            return WindowManager.CreateWindow(settings);
        }

        public void Update()
        {
            EditorWindow.Update();
        }

        public void Render()
        {
            var rb = new RenderBlock();
            EditorWindow.Render(rb);

            RenderDevice.ActiveView = MainView;
            RenderDevice.ClearView();

            RenderDevice.Render(rb);
            Engine.StepFrame();
        }

        public void Resize(Flood.Settings settings)
        {
            EditorWindow.SetSize(settings.Width, settings.Height);
        }

        public void Run()
        {
            PlatformManager.Update();
        }
    }
}