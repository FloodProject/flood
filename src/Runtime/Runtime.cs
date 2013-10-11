
using System.IO;
using Flood.Modules;

namespace Flood.Editor
{
    public class Runtime
    {
        public static void Main(string[] args)
        {
            var app = new Application();

            var moduleDir = Path.Combine(Directory.GetCurrentDirectory(), "Modules");
            app.ModuleManager.ModuleLibrary.AddDirectory(moduleDir);

            app.ModuleManager.LoadModule(new ModuleId("Editor.Client", 0));

            app.Run();
        }
    }
}