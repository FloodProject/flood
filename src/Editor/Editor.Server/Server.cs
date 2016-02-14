using System;
using Flood.Networking;
using Flood.Remoting;

namespace Flood.Editor.Server
{
    /// <summary>
    /// Represents the editor server.
    /// </summary>
    public class EditorServer : Networking.Server
    {
        public UserManager UserManager { get; set; }
        public ProjectManager ProjectManager { get; set; }

        public EditorServer(RemotingManager remotingManager, HostEndPoint endPoint)
            : base(remotingManager, endPoint)
        {
            UserManager = new UserManager();
            //serviceManager.AddImplementation<IUserManager>(UserManager);
            //var serviceManager = remotingManager.ServiceManager;
            //serviceManager.

            ProjectManager = new ProjectManager();
            //serviceManager.AddImplementation<IProjectManager>(ProjectManager);
        }

        public void Serve()
        {
            Console.WriteLine("Server is starting...");
        }
    }
}