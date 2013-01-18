using Flood.Editor;
using NUnit.Framework;
using NSubstitute;

namespace Editor.Tests
{
    [TestFixture]
    public class ProjectClientTests
    {
        Project project;
        ProjectManager projects;
        bool eventFired = false;
        const string ProjectName = "TestProject";

        [SetUp]
        public void Init()
        {
            project = new Project(ProjectType.Game, ProjectName);
            projects = new ProjectManager();
            eventFired = false;
        }

        [Test]
        public void ProjectProperties()
        {
            Assert.That(project.Name, Is.EqualTo(ProjectName));
            Assert.That(project.Type, Is.EqualTo(ProjectType.Game));
        }

        [Test]
        public void AddProject()
        {
            projects.ProjectAdded += (sender, project1) => eventFired = true;
            projects.AddProject(project);

            Assert.That(projects.Projects, Has.Exactly(1).EqualTo(project));
            Assert.IsTrue(eventFired);
        }

        [Test]
        public void RemoveProject()
        {
            projects.ProjectRemoved += (sender, project1) => eventFired = true;
            projects.AddProject(project);
            projects.RemoveProject(project);

            Assert.That(projects.Projects, Is.Empty);
            Assert.IsTrue(eventFired);
        }

        [Test]
        public void CreateProject()
        {
            var proj = projects.CreateProject(ProjectType.CSharp, ProjectName);
            Assert.That(proj.Name, Is.EqualTo(ProjectName));
            Assert.That(proj.Type, Is.EqualTo(ProjectType.CSharp));
        }
    }

    [TestFixture]
    public class ProjectServerTests
    {
        [Test]
        public void TestAdd()
        {
            var projectManager = Substitute.For<ProjectManager>();
        }
    }
}