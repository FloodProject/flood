using System;
using System.Collections.Generic;

namespace Flood.Editor
{
    public enum ProjectType
    {
        CPlusPlus,
        CSharp,
        Game
    }

    public class Project
    {
        /// <summary>
        /// Type of the project.
        /// </summary>
        public ProjectType Type { get; private set; }

        /// <summary>
        /// Name of the project.
        /// </summary>
        public string Name { get; private set; }

        public Project(ProjectType type, string name)
        {
            Type = type;
            Name = name;
        }
    }

    public class ProjectManager
    {
        private List<Project> projects;

        /// <summary>
        /// List of all the projects.
        /// </summary>
        public IReadOnlyList<Project> Projects
        {
            get { return projects; }
        }

        /// <summary>
        /// Event fired when a project is added.
        /// </summary>
        public event EventHandler<Project> ProjectAdded = delegate { };

        /// <summary>
        /// Event fired when a project is removed.
        /// </summary>
        public event EventHandler<Project> ProjectRemoved = delegate { };

        public ProjectManager()
        {
            projects = new List<Project>();
        }

        /// <summary>
        /// Adds a new project.
        /// </summary>
        /// <remarks>Invokes the ProjectAdded event.</remarks>
        public void AddProject(Project project)
        {
            projects.Add(project);
            ProjectAdded(this, project);
        }

        /// <summary>
        /// Removes an existing project.
        /// </summary>
        /// <remarks>Invokes the ProjectRemoved event.</remarks>
        /// <returns>True if the</returns>
        public bool RemoveProject(Project project)
        {
            var didRemove = projects.Remove(project);

            if (didRemove)
                ProjectRemoved(this, project);

            return didRemove;
        }

        /// <summary>
        /// Creates a new project of the given type and name and adds
        /// it to the list.
        /// </summary>
        /// <returns>Returns a new project of this type.</returns>
        public Project CreateProject(ProjectType type, string name)
        {
            var project = new Project(type, name);

            AddProject(project);

            return project;
        }
    }
}
