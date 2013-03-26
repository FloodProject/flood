using System.IO;
using System.Reflection;
using Flood.Editor.Client;
using Flood.Editor.Client.Gui;
using Flood.GUI.Renderers;
using System;
using Mono.Addins;

namespace Flood.Editor
{
    public sealed class Editor : IDisposable
    {
        public GwenRenderer GuiRenderer { get; private set; }
        public MainWindow MainWindow { get; private set; }
        public Window PaneWindow  { get; private set; }

        public Editor()
        {
            InitializeAddins();
            InitializeGui();
        }

        public void Dispose()
        {
            MainWindow.Dispose();
        }

        private void InitializeGui()
        {
            GuiRenderer = new GwenRenderer();

            MainWindow = new MainWindow();
            MainWindow.Init(GuiRenderer, "DefaultSkin.png");
        }

        public void Update()
        {
            if (PaneWindow == null)
            {
                //CreatePaneWindow();
                //MainWindow.NativeWindow.MakeCurrent();
            }
        }

        public void SetSize(int x, int y)
        {
            MainWindow.Canvas.SetSize(x, y);
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
            Log.Error("Error loading add-in: " + args.Message);
        }

        static void OnLoad (object s, AddinEventArgs args)
        {
            Log.Info("Add-in loaded: " + args.AddinId);
        }

        static void OnUnload (object s, AddinEventArgs args)
        {
            Log.Info("Add-in unloaded: " + args.AddinId);
        }

        static void OnExtensionChange (object s, ExtensionEventArgs args)
        {
            Log.Info("Extension changed: " + args.Path);
        }
    }
}
