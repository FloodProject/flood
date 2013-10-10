using System.Threading.Tasks;
using Flood.Editor.Shared;
using System;
using System.Collections.Generic;
using System.Timers;

namespace Flood.Editor.Server
{
    public class ProjectManager : IProjectManager
    {
        const int TimerInterval = 5000;

        readonly Dictionary<Guid, Project> projects;

        Timer timer;
        bool databaseModified;

        public ProjectManager()
        {
            projects = new Dictionary<Guid, Project>();

            databaseModified = false;

            LoadProjectsFromDatabase();
            StartTimer();
        }

        /// <summary>
        /// Timer callback to saves changes to the database permanently
        /// </summary>
        private void TimerEvent(object sender, ElapsedEventArgs e)
        {
            
        }

        /// <summary>
        /// Store modified project in the database
        /// </summary>
        private void UpdateDatabase(Project proj)
        {
           
        }

        /// <summary>
        /// Start timer function
        /// </summary>
        private void StartTimer()
        {
            timer = new Timer(TimerInterval);
            timer.Elapsed += new ElapsedEventHandler(TimerEvent);
            timer.AutoReset = true;
            timer.Enabled = true;
        }

        /// <summary>
        /// Loads all projects stored in the databse
        /// </summary>
        private void LoadProjectsFromDatabase()
        {
            
        }

        /// <summary>
        /// Adds user to project
        /// </summary>
        public async Task<bool> AddUserToProject(Guid projectId, ProjectUser user)
        {
            if (!projects.ContainsKey(projectId))
                return false;
            
            bool ret = projects[projectId].AddUser(user);
            if(ret)
                UpdateDatabase(projects[projectId]);
            
            return ret;
        }

        /// <summary>
        /// Removes user from project
        /// </summary>
        public async Task<bool> RemoveUserFromProject(Guid projectId, ProjectUser user)
        {
            if (!projects.ContainsKey(projectId))
                return false;
            bool ret = projects[projectId].RemoveUser(user);
            if (ret)
                UpdateDatabase(projects[projectId]);

            return ret;
        }

        /// <summary>
        /// Modifies user permissions.
        /// </summary>
        /// <remarks /> New permissions are passed insed the ProjectUser
        public async Task<bool> ModifyUserPermissions(Guid projectId, ProjectUser user)
        {
            if (!projects.ContainsKey(projectId))
                return false;

            var ret = projects[projectId].ModifyUser(user);

            if (ret)
                UpdateDatabase(projects[projectId]);

            return ret;
 
        }

        /// <summary>
        /// Returns all project users.
        /// </summary>
        public async Task<ICollection<ProjectUser>> GetUsers(Guid projectId)
        {
            if (!projects.ContainsKey(projectId))
                return null;

            return projects[projectId].GetUsers();
        }

        /// <summary>
        /// Returns project.
        /// </summary>
        public async Task<Project> GetProject(Guid projectId)
        {
            if (!projects.ContainsKey(projectId))
                return null;
            return projects[projectId];
        }


        /// <summary>
        /// Read-only dictionary of all the projects.
        /// </summary>
        public async Task<Dictionary<Guid, Project>> GetProjects()
        {
            return projects;
        }

        /// <summary>
        /// Event fired when a project is added.
        /// </summary>
        public event EventHandler<Project> ProjectAdded = delegate { };

        /// <summary>
        /// Event fired when a project is removed.
        /// </summary>
        public event EventHandler<Project> ProjectRemoved = delegate { };
        
        /// <summary>
        /// Adds a new project.
        /// </summary>
        /// <remarks>Invokes the ProjectAdded event.</remarks>
        private void AddProject(Project project)
        {
            if (project == null)
                return;

            projects.Add(project.Id, project);
            ProjectAdded(this, project);
            UpdateDatabase(project);
        }

        /// <summary>
        /// Removes an existing project.
        /// </summary>
        /// <remarks>Invokes the ProjectRemoved event.</remarks>
        /// <returns>True if the project was removed, false otherwise.</returns>
        public async Task<bool> RemoveProject(Guid id)
        {
            var project = projects[id];
            var didRemove = projects.Remove(id);

            if (didRemove)
            {
                //database.Delete<Project>(id);
                databaseModified = true;
                ProjectRemoved(this, project);
            }

            return didRemove;
        }

        /// <summary>
        /// Creates a new project of the given type and name and adds
        /// it to the list.
        /// </summary>
        /// <returns>Returns a new project of this type.</returns>
        public async Task<Project> CreateProject(ProjectType type, string name, Owner founder)
        {
            var project = new Project(type, name, System.Guid.NewGuid(), founder);

            AddProject(project);

            return project;
        }
    }
}
