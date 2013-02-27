//using Flood.Editor.Controls;

using System.IO;
using System.Reflection;
using Editor.Client;
using Editor.Client.Gui;
using Flood.GUI.Renderers;
using Flood.RPC.Protocol;
using Flood.RPC.Transport;
using System;
using Mono.Addins;

namespace Flood.Editor
{
    public class Editor : IDisposable
    {

        public MainWindow MainWindow { get; private set; }


        public Editor(Renderer renderer, string textureName)
        {
            InitAddins();

            MainWindow = new MainWindow();
            MainWindow.Init(renderer,textureName);
        }

        public void Dispose()
        {
        }

        private void InitAddins()
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
