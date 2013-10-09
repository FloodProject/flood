
using System.IO;
using Flood.Modules;

namespace Flood.Editor
{
    public class Runtime
    {
        public static void Main(string[] args)
        {
            var app = new Application();
            
            app.ModuleManager.ModuleLibrary.AddDirectory(Directory.GetCurrentDirectory());
            app.ModuleManager.LoadModule(new ModuleId("Editor.Client", 0));

            app.Run();
        }
    }
}