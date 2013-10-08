
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Security;

namespace Flood
{
    class Application
    {
        [SuppressUnmanagedCodeSecurity]
        [DllImport("Editor.Native.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateWxPlatformManager();

        public Engine Engine { get; set; }

        public ResourceManager ResourceManager { get; set; }
        public ArchiveVirtual Archive { get; set; }

        public PlatformManager PlatformManager { get; set; }
        public WindowManager WindowManager { get; set; }

        public RenderDevice RenderDevice;
        public RenderView MainView;

        public Application()
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

        public void Run()
        {
            while (true)
            {
                PlatformManager.Update();
                System.Threading.Thread.Sleep(1);
            }
        }
    }
}
