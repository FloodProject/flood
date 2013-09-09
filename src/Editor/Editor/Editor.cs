using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Security;

namespace Flood.Editor
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

        public Client.Editor Client { get; set; }

        public Editor()
        {
            PlatformManager = new PlatformManager(CreateWxPlatformManager());

            Engine = new Engine(PlatformManager);
            Engine.Init();

            // TODO: Get the mount paths from the editor preferences.
            Archive = new ArchiveVirtual();
            Archive.ArchiveMountDirectories("Assets/", Allocator.GetHeap());

            ResourceManager = Engine.ResourceManager;
            ResourceManager.Archive = Archive;

            Client = new Client.Editor();

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

        public Window CreateWindow()
        {
            var settings = new WindowSettings(1000, 700, "Editor",
                WindowStyles.TopLevel);

            return WindowManager.CreateWindow(settings);
        }

        public void Update()
        {
            Client.Update();
        }

        public void Render()
        {
            var rb = new RenderBlock();
            Client.Render(rb);

            RenderDevice.ActiveView = MainView;
            RenderDevice.ClearView();

            RenderDevice.Render(rb);
            Engine.StepFrame();
        }

        public void Resize(Flood.Settings settings)
        {
            Client.SetSize(settings.Width, settings.Height);
        }

        public void Run()
        {
            PlatformManager.Update();
        }

        public static void Main(string[] args)
        {
            var editor = new Editor();
            editor.Run();
        }
    }
}