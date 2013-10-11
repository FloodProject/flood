
using Editor.Services;
using Flood.Modules;
using TextAddin.Controls;
using TextAddin.Document;

namespace TextAddin
{
    class Module : IModule
    {
        public void OnLoad(Flood.Remoting.ServiceManager serviceManager)
        {
            var paneManager = serviceManager.GetGlobalService<IPaneManager>();

            var textDocument = new TextDocument();
            var textView = new TextView(null, textDocument);

            var pane = new Pane() {Title = "CodeEdit", control = textView};
            paneManager.AddPane(pane);
        }

        public void Update()
        {
            throw new System.NotImplementedException();
        }
    }
}
