using Flood.Editor.Controls;
using Flood.RPC.Protocol;
using Flood.RPC.Transport;
using System;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;

namespace Flood.Editor
{
    public class Editor : IDisposable
    {
        [Export]
        public EditorWindow Window { get; private set; }

        [Export]
        public ToolManager ToolManager { get; private set; }

        [Export]
        public ServerManager ServerManager { get; private set; }

        [Export]
        public DocumentManager DocumentManager { get; private set; }

        [Export]
        public ProjectManager ProjectManager { get; private set; }

        private AggregateCatalog catalog;

        [Export]
        private CompositionContainer container;

        public Editor()
        {
            InitializeGUI();
            InitializeServer();
            InitializeContainer();

            ProjectManager = new ProjectManager();

            var client = InitializeClient();
            Console.WriteLine("Connected to the server.");

            Console.WriteLine("Creating a sample project via RPC...");
            client.CreateProject("Sample");

            ProjectManager.CreateProject(ProjectType.Game, "Sample");
        }

        public void Dispose()
        {
            Window.Dispose();
        }

        private void InitializeGUI()
        {
            Window = new EditorWindow();
            Window.GUIInitiated += OnGUIInit;
        }

        private void InitializeServer()
        {
            ServerManager = new ServerManager();
            ServerManager.CreateBuiltinServer();
        }

        private void OnGUIInit()
        {
            DocumentManager = new DocumentManager();
            ToolManager = new ToolManager();

            container.ComposeParts(this, ToolManager, DocumentManager);
        }

        private static IProjectManagerImpl.Client InitializeClient()
        {
            TTransport transport = new TSocket("localhost", Settings.RPCPort);
            Serializer protocol = new BinaryProtocol(transport);
            var client = new IProjectManagerImpl.Client(protocol);

            for (var i = 0; i < Settings.MaxRetries; i++)
            {
                transport.Open();
                if (transport.Peek()) break;
            }

            return client;
        }

        public void InitializeContainer()
        {
            catalog = new AggregateCatalog();
            container = new CompositionContainer(catalog);
            var batch = new CompositionBatch();

            var assemblyCatalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            catalog.Catalogs.Add(assemblyCatalog);

            // this is the new part
            catalog.Changed += catalog_Changed;
        }

        void catalog_Changed(object sender, ComposablePartCatalogChangeEventArgs e)
        {
            Console.WriteLine(e);
        }
    }
}
