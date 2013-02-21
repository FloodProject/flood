
using System;
using NUnit.Framework;
using Mono.Addins;
using System.IO;
using SimpleApp;
using System.Xml.Serialization;

namespace UnitTests
{
	[TestFixture()]
	public class TestLoadUnload: TestBase
	{
		void ResetStatus (bool enable)
		{
			Addin ainfo;
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.HelloWorldExtension");
			ainfo.Enabled = enable;
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.FileContentExtension");
			ainfo.Enabled = enable;
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.SystemInfoExtension");
			ainfo.Enabled = enable;
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.CommandExtension");
			ainfo.Enabled = enable;
		}
		
		[Test()]
		public void TestDisable ()
		{
			Addin ainfo;
			
			Assert.AreEqual (4, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 1");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.HelloWorldExtension");
			Assert.IsNotNull (ainfo, "t1");
			Assert.IsTrue (ainfo.Enabled, "t1.1");
			ainfo.Enabled = false;
			Assert.IsFalse (ainfo.Enabled, "t1.2");
			
			Assert.AreEqual (3, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 2");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.FileContentExtension");
			Assert.IsNotNull (ainfo, "t3");
			Assert.IsTrue (ainfo.Enabled, "t3.1");
			ainfo.Enabled = false;
			Assert.IsFalse (ainfo.Enabled, "t3.2");
			
			Assert.AreEqual (2, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 3");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.SystemInfoExtension");
			Assert.IsNotNull (ainfo, "t5");
			Assert.IsTrue (ainfo.Enabled, "t5.1");
			ainfo.Enabled = false;
			Assert.IsFalse (ainfo.Enabled, "t5.2");
			
			Assert.AreEqual (0, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 4");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.CommandExtension");
			Assert.IsNotNull (ainfo, "t2");
			Assert.IsTrue (ainfo.Enabled, "t2.1");
			ainfo.Enabled = false;
			Assert.IsFalse (ainfo.Enabled, "t2.2");
			
			Assert.AreEqual (0, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 5");
			
			ResetStatus (true);
		}
		
		[Test()]
		public void TestEnable ()
		{
			ResetStatus (false);
			
			Assert.AreEqual (0, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 1");
			
			Assert.IsFalse (AddinManager.Registry.IsAddinEnabled ("SimpleApp.HelloWorldExtension"), "t1");
			AddinManager.Registry.EnableAddin ("SimpleApp.HelloWorldExtension,0.1.0");
			Assert.IsTrue (AddinManager.Registry.IsAddinEnabled ("SimpleApp.HelloWorldExtension"), "t1.1");
			
			Assert.AreEqual (1, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 2");
			
			Assert.IsFalse (AddinManager.Registry.IsAddinEnabled ("SimpleApp.CommandExtension"), "t2");
			AddinManager.Registry.EnableAddin ("SimpleApp.CommandExtension,0.1.0");
			Assert.IsTrue (AddinManager.Registry.IsAddinEnabled ("SimpleApp.CommandExtension"), "t2.1");
			
			Assert.AreEqual (1, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 3");
			
			Assert.IsFalse (AddinManager.Registry.IsAddinEnabled ("SimpleApp.SystemInfoExtension"), "t3");
			AddinManager.Registry.EnableAddin ("SimpleApp.SystemInfoExtension,0.1.0");
			Assert.IsTrue (AddinManager.Registry.IsAddinEnabled ("SimpleApp.SystemInfoExtension"), "t3.1");
			
			Assert.AreEqual (3, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 4");
			
			Assert.IsFalse (AddinManager.Registry.IsAddinEnabled ("SimpleApp.FileContentExtension"), "t4");
			AddinManager.Registry.EnableAddin ("SimpleApp.FileContentExtension,0.1.0");
			Assert.IsTrue (AddinManager.Registry.IsAddinEnabled ("SimpleApp.FileContentExtension"), "t4.1");
			
			Assert.AreEqual (4, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 5");
			
			ResetStatus (true);
		}
		
		[Test()]
		public void TestDisableWithDeps ()
		{
			Addin ainfo;
			
			Assert.AreEqual (4, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 1");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.HelloWorldExtension");
			Assert.IsNotNull (ainfo, "t1");
			Assert.IsTrue (ainfo.Enabled, "t1.1");
			ainfo.Enabled = false;
			Assert.IsFalse (ainfo.Enabled, "t1.2");
			
			Assert.AreEqual (3, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 2");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.CommandExtension");
			Assert.IsNotNull (ainfo, "t2");
			Assert.IsTrue (ainfo.Enabled, "t2.1");
			ainfo.Enabled = false;
			Assert.IsFalse (ainfo.Enabled, "t2.2");
			
			// SystemInfoExtension depends on CommandExtension, so it should be disabled after disabling CommandExtension
			
			Assert.AreEqual (1, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 3");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.SystemInfoExtension");
			Assert.IsNotNull (ainfo, "t4");
			Assert.IsFalse (ainfo.Enabled, "t4.1");
			
			// FileContentExtension depends on SystemInfoExtension, but the dependency is optional
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.FileContentExtension");
			Assert.IsNotNull (ainfo, "t5");
			Assert.IsTrue (ainfo.Enabled, "t5.1");
			ainfo.Enabled = false;
			Assert.IsFalse (ainfo.Enabled, "t5.2");
			
			Assert.AreEqual (0, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 4");
			
			ResetStatus (true);
		}
		
		[Test()]
		public void TestEnableWithDeps ()
		{
			ResetStatus (false);
			
			Addin ainfo;
			
			Assert.AreEqual (0, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 1");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.HelloWorldExtension");
			Assert.IsNotNull (ainfo, "t1");
			Assert.IsFalse (ainfo.Enabled, "t1.1");
			ainfo.Enabled = true;
			Assert.IsTrue (ainfo.Enabled, "t1.2");
			
			Assert.AreEqual (1, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 2");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.FileContentExtension");
			Assert.IsNotNull (ainfo, "t3");
			Assert.IsFalse (ainfo.Enabled, "t3.1");
			ainfo.Enabled = true;
			Assert.IsTrue (ainfo.Enabled, "t3.2");
			
			Assert.AreEqual (2, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 3");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.SystemInfoExtension");
			Assert.IsNotNull (ainfo, "t5");
			Assert.IsFalse (ainfo.Enabled, "t5.1");
			ainfo.Enabled = true;
			Assert.IsTrue (ainfo.Enabled, "t5.2");
			
			Assert.AreEqual (4, AddinManager.GetExtensionNodes ("/SimpleApp/Writers").Count, "count 4");
			
			ainfo = AddinManager.Registry.GetAddin ("SimpleApp.CommandExtension");
			Assert.IsNotNull (ainfo, "t2");
			Assert.IsTrue (ainfo.Enabled, "t2.1");
			
			ResetStatus (true);
		}
		
		[Test()]
		public void TestCurrentAddin ()
		{
			Assert.AreEqual ("SimpleApp.Core,0.1.0", AddinManager.CurrentAddin.ToString ());
			
			InstanceExtensionNode node = (InstanceExtensionNode) AddinManager.GetExtensionNode ("/SimpleApp/Writers/HelloWorldExtension.HelloWorldWriter");
			Assert.IsNotNull (node, "t1");
			
			IWriter w = (IWriter) node.CreateInstance ();
			Assert.AreEqual ("SimpleApp.HelloWorldExtension,0.1.0", w.Test ("currentAddin"));
		}
		
		public class TestClass
		{
			public string Value;
		}
		
		[Test]
		public void TestAddinAssemblyLoading ()
		{
			// Test for bug 672858 - Mono.Addins causes error in XML serializer
			Environment.SetEnvironmentVariable ("MONO_XMLSERIALIZER_THS","0");
			XmlSerializer ser = new XmlSerializer (typeof(TestClass));
			ser.Serialize (new StringWriter (), new TestClass ());
		}
		
		[Test()]
		public void TestLoadLatestVersion ()
		{
			ExtensionNodeList list = AddinManager.GetExtensionNodes ("/SimpleApp/InstallUninstallTest");
			Assert.AreEqual (1, list.Count);
			Assert.AreEqual ("10.2", ((ItemSetNode)list[0]).Label);
		}
	}
}
