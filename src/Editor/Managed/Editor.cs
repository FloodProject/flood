using FlushEditor.GUI;
using FlushEditor.GUI.Tools;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor
{
   
    public class Editor : IDisposable{

        private AggregateCatalog catalog;

        [Export]
	    public EditorGUI GUI { get; private set; }

        [Export]
        private CompositionContainer container;

        [Export]
        private ToolManager toolManager;

        [Export]
        private DocumentManager documentManager;

        public Editor()
        {
            GUI = new EditorGUI();
           
            InitContainer();

            GUI.Initiated += OnGUIInit;
            
	    }

        public void Dispose()
        {
            GUI.Dispose();
        }

        private void OnGUIInit(){
            documentManager = new DocumentManager();
            toolManager = new ToolManager();

            container.ComposeParts(this, toolManager, documentManager);
        }

        public void InitContainer()
        {
            try
            {
                catalog = new AggregateCatalog();
                container = new CompositionContainer(catalog);
                var batch = new CompositionBatch();

                catalog.Catalogs.Add(new AssemblyCatalog(Assembly.GetExecutingAssembly()));

                // this is the new part
                catalog.Changed += catalog_Changed;
            }
            catch (CompositionException compositionException)
            {
                // testing
                throw;
            }
        }

        void catalog_Changed(object sender, ComposablePartCatalogChangeEventArgs e)
        {
            Console.WriteLine(e);
        }
    }
}
