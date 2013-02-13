//using Flood.Editor.Controls;

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
        [Import]
        public EditorWindow Window { get; private set; }

        [Import]
        public ToolManager ToolManager { get; private set; }

        [Import]
        public DocumentManager DocumentManager { get; private set; }

        [Import]
        public ProjectManager ProjectManager { get; private set; }

        public ServerManager ServerManager { get; private set; }

        private ProjectPane projectPane;

        private AggregateCatalog catalog;

        [Export]
        private CompositionContainer container;

        public Editor()
        {
            InitializeServer();
            InitializeContainer();

            Window.GUIInitiated += InitializeGUI;
        }

        public void Dispose()
        {
            Window.Dispose();
        }

        private void InitializeGUI()
        {
            

            var client = InitializeClient();
            Console.WriteLine("Connected to the server.");

            Console.WriteLine("Creating a sample project via RPC...");
            client.CreateProject("Sample");

            ProjectManager.CreateProject(ProjectType.Game, "Sample");
        }

        private void InitializeServer()
        {
            ServerManager = new ServerManager();
            ServerManager.CreateBuiltinServer();
        }

        private static IProjectManagerImpl.Client InitializeClient()
        {
            var transport = new TSocket("localhost", Settings.RPCPort);
            var protocol = new BinaryProtocol(transport);
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

            try
            {
                var assemblyCatalog = new AssemblyCatalog(typeof(Editor).Assembly);
                catalog.Catalogs.Add(assemblyCatalog);
                container.ComposeParts(this);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }

            catalog.Changed += catalog_Changed;

        }

        void catalog_Changed(object sender, ComposablePartCatalogChangeEventArgs e)
        {
            Console.WriteLine(e);
        }
    }
}
