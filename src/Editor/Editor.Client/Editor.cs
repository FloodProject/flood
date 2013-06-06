using Flood.Editor.Client.Gui;
using Flood.Extensions;
using System;
using System.IO;
using System.Reflection;

namespace Flood.Editor.Client
{
    public sealed class Editor : IDisposable
    {
        public MainWindow MainWindow { get; private set; }
        public Window PaneWindow { get; private set; }

        public GwenRenderer GuiRenderer { get; private set; }
        public GwenInput GuiInput { get; private set; }

        public Engine Engine { get; private set; }

        public ServerManager ServerManager { get; private set; }

        public Editor()
        {
            Engine = FloodEngine.GetEngine();

            InitializeAddins();
            InitializeGui();

            ServerManager = new ServerManager();
            ServerManager.CreateBuiltinServer();
        }

        public void Dispose()
        {
            MainWindow.Dispose();
            GuiRenderer.Dispose();
        }

        void InitializeGui()
        {
            GuiRenderer = new GwenRenderer();

            MainWindow = new MainWindow();
            MainWindow.Init(GuiRenderer, "DefaultSkin.png",new Flood.GUI.Font("Vera.ttf",16));

            GuiInput = new GwenInput(Engine.GetInputManager());
            GuiInput.Initialize(MainWindow.Canvas);
        }

        public void Render(RenderBlock rb)
        {
            GuiRenderer.Clear();
            MainWindow.Render();
            GuiRenderer.Render(rb);
        }

        public void Update()
        {
            
        }

        public void SetSize(int x, int y)
        {
            MainWindow.Canvas.SetSize(x, y);
        }

        private void InitializeAddins()
        {
            var libdir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            var addinsPath = Path.Combine(libdir,"Addins");

            ExtensionLoader.LoadAssemblies(addinsPath);
            ExtensionLoader.InitAssembliesExtensions();
        }
    }
}
