using System;
using Flood.RPC.Metadata;

namespace Flood.Editor
{
    [Service]
    public interface IProjectManager
    {
        /// <summary>
        /// Creates a new projects with the given name.
        /// </summary>
        void CreateProject([Id(0)] string name);

        /// <summary>
        /// Event fired when a new project is created.
        /// </summary>
        event EventHandler<string> ProjectCreated;
    }
}