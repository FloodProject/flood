using System;
using Flood.RPC.Metadata;

namespace Flood.Editor
{
    public class ProjectManager : IProjectManager
    {
        public void CreateProject(string name)
        {
            Console.WriteLine("Project created: {0}", name);
        }

        public event EventHandler<string> ProjectCreated;
    }
}