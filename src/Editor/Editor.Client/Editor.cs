using System.IO;
using System.Reflection;
using Flood.Editor.Client.Gui;
using Flood.GUI.Renderers;
using System;
using Mono.Addins;

namespace Flood.Editor
{
    public sealed class Editor : IDisposable
    {
        public MainWindow MainWindow { get; private set; }
        public Window PaneWindow  { get; private set; }

        public Editor(Renderer renderer, string textureName)
        {
            InitializeAddins();

            MainWindow = new MainWindow();
            MainWindow.Init(renderer, textureName);
        }

        public void Dispose()
        {
            MainWindow.Dispose();
        }

        public void Update()
        {
            if (PaneWindow == null)
            {
                CreatePaneWindow();
                MainWindow.NativeWindow.MakeCurrent();
            }
        }

        private void CreatePaneWindow()
        {
            var engine = FloodEngine.GetEngine();
            var windowManager = engine.GetWindowManager();

            var settings = new WindowSettings
                {
                    Width = 640,
                    Height = 480,
                    Title = "Pane",
                    Styles = WindowStyles.TopLevel
                };

            PaneWindow = windowManager.CreateWindow(settings);
            PaneWindow.WindowClose += () => Log.Info("Window closed!");
            PaneWindow.WindowFocusChange += hasFocus => Log.Info("Window focus: " + hasFocus);
            PaneWindow.TargetResize += s => Log.Info("Size: " + s.Width);
            PaneWindow.Show(visible: true);

            PaneWindow.SetContext(MainWindow.NativeWindow.GetContext());
            PaneWindow.MakeCurrent();
        }

        private void InitializeAddins()
        {
            AddinManager.AddinLoadError += OnLoadError;
            AddinManager.AddinLoaded += OnLoad;
            AddinManager.AddinUnloaded += OnUnload;

            var libdir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            //var addinsPath = Path.Combine(libdir,"Addins");

            AddinManager.Initialize (libdir,libdir);
            AddinManager.Registry.Update (null);
            AddinManager.ExtensionChanged += OnExtensionChange;
        }

        static void OnLoadError (object s, AddinErrorEventArgs args)
        {
            Console.WriteLine ("Add-in error: " + args.Message);
            Console.WriteLine (args.AddinId);
            Console.WriteLine (args.Exception);
        }

        static void OnLoad (object s, AddinEventArgs args)
        {
            Console.WriteLine ("Add-in loaded: " + args.AddinId);
        }

        static void OnUnload (object s, AddinEventArgs args)
        {
            Console.WriteLine ("Add-in unloaded: " + args.AddinId);
        }

        static void OnExtensionChange (object s, ExtensionEventArgs args)
        {
            Console.WriteLine ("Extension changed: " + args.Path);
        }
    }
}
