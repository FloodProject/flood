using Flood.Network;
using System;

namespace Flood.Editor.Server
{
    /// <summary>
    /// Represents the editor server.
    /// </summary>
    public class EditorServer : Flood.Server.Server
    {
        public UserManager UserManager { get; set; }
        public ProjectManager ProjectManager { get; set; }

        public EditorServer(HostEndPoint endPoint)
            : base(endPoint)
        {
            var serviceManager = Host.ServiceManager;

            UserManager = new UserManager();
            //serviceManager.AddImplementation<IUserManager>(UserManager);

            ProjectManager = new ProjectManager(Database);
            //serviceManager.AddImplementation<IProjectManager>(ProjectManager);
        }

        public void Serve()
        {
            Console.WriteLine("Server is starting...");
        }
    }
}