using System;
using System.Collections.Generic;
using Flood.Editor.Shared;
using System.Timers;
using Flood.Server;

namespace Flood.Editor.Server
{
    public class ProjectManager : IProjectManager
    {
        private readonly Dictionary<Guid, Project> _projects;
        readonly IDatabaseManager database;
        private Timer _timer;
        private bool _databaseModified;
        private const int TimerInterval = 5000;


        public ProjectManager(IDatabaseManager db)
        {
            projects = new Dictionary<Guid, Project>();
            database = db;
            LoadProjectsFromDatabase();
            _databaseModified = false;
            StartTimer();
        }

        /// <summary>
        /// Timer callback to saves changes to the database permanently
        /// </summary>
        private void TimerEvent(object sender, ElapsedEventArgs e)
        {
            try
            {
                if (databaseModified)
                    database.SaveChanges();
                databaseModified = false;
            }
            catch(SessionNotOpenException sessionNotOpenException)
            {
            }
        }

        /// <summary>
        /// Store modified project in the database
        /// </summary>
        private void UpdateDatabase(Project proj)
        {
            try
            {
                database.Store(proj, proj.Id);
                databaseModified = true;
            }
            catch (SessionNotOpenException sessionNotOpenException)
            {
            }
        }
        /// <summary>
        /// Start timer function
        /// </summary>
        private void StartTimer()
        {
            _timer = new Timer(TimerInterval);
            _timer.Elapsed += new ElapsedEventHandler(TimerEvent);
            _timer.AutoReset = true;
            _timer.Enabled = true;
        }

        /// <summary>
        /// Loads all projects stored in the databse
        /// </summary>
        private void LoadProjectsFromDatabase()
        {
            database.StartSession();
            List<Project> projs = database.LoadAll<Project>();
            foreach (var project in projs)
                _projects.Add(project.Id, project);
        }

        /// <summary>
        /// Adds user to project
        /// </summary>
        public bool AddUserToProject(Guid projectId, ProjectUser user)
        {
            if (!_projects.ContainsKey(projectId))
                return false;
            
            bool ret = _projects[projectId].AddUser(user);
            if(ret)
                UpdateDatabase(_projects[projectId]);
            
            return ret;
        }

        /// <summary>
        /// Removes user from project
        /// </summary>
        public bool RemoveUserFromProject(Guid projectId, ProjectUser user)
        {
            if (!_projects.ContainsKey(projectId))
                return false;
            bool ret = _projects[projectId].RemoveUser(user);
            if (ret)
                UpdateDatabase(_projects[projectId]);

            return ret;
        }

        /// <summary>
        /// Modifies user permissions 
        /// </summary>
        /// <remarks /> New permissions are passed insed the ProjectUser
        public bool ModifyUserPermissions(Guid projectId, ProjectUser user)
        {
            if (!_projects.ContainsKey(projectId))
                return false;
            bool ret = _projects[projectId].ModifyUser(user);
            if (ret)
                UpdateDatabase(_projects[projectId]);

            return ret;
 
        }

        /// <summary>
        /// Returns all project users 
        /// </summary>
        public ICollection<ProjectUser> GetUsers(Guid projectId)
        {
            if (!_projects.ContainsKey(projectId))
                return null;
            return _projects[projectId].GetUsers();
        }

        /// <summary>
        /// Returns project 
        /// </summary>
        public Project GetProject(Guid projectId)
        {
            if (!_projects.ContainsKey(projectId))
                return null;
            return _projects[projectId];
        }

        /// <summary>
        /// Read-only dictionary of all the projects.
        /// </summary>
        public Dictionary<Guid, Project> Projects
        {
            get { return _projects; }
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

            _projects.Add(project.Id, project);
            ProjectAdded(this, project);
            UpdateDatabase(project);
        }

        /// <summary>
        /// Removes an existing project.
        /// </summary>
        /// <remarks>Invokes the ProjectRemoved event.</remarks>
        /// <returns>True if the project was removed, false otherwise.</returns>
        public bool RemoveProject(Guid id)
        {
            var project = _projects[id];
            var didRemove = _projects.Remove(id);

            if (didRemove)
            {
                database.Delete<Project>(id);
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
        public Project CreateProject(ProjectType type, string name, Owner founder)
        {
            var project = new Project(type, name, System.Guid.NewGuid(), founder);

            AddProject(project);

            return project;
        }
    }
}
