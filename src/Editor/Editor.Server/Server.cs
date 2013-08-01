using System;

namespace Flood.Editor.Server
{
    /// <summary>
    /// Represents the editor server.
    /// </summary>
    public class EditorServer : Flood.Server.Server
    {
        public UserManager UserManager { get; set; }

        public EditorServer()
        {
            var handler = new ProjectManager(Database);

        }

        public void Serve()
        {
            Console.WriteLine("Server is starting...");
        }
    }
}