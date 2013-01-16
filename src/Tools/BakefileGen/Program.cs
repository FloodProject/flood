using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using Flood;

namespace BakefileParser
{
    /// <summary>
    /// The purpose of this tool is to generate the needed Premake files groups
    /// for each platform that we need to compile wxWidgets. These are in the
    /// Bakefile XML format in a file called "files.bkl" in "build\bakefiles".
    /// </summary>
    class BakefileParser : TextGenerator
    {
        public readonly Dictionary<string, List<string>> FileGroups;

        public BakefileParser()
        {
            FileGroups = new Dictionary<string, List<string>>();
        }

        public void Parse(string fileName)
        {
            var doc = XDocument.Load(fileName);

            if (doc == null | doc.Root == null)
                throw new ApplicationException("invalid data");

            foreach (var set in doc.Root.Descendants("set"))
            {
                var @var = set.Attribute("var");

                if (var == null)
                    throw new ApplicationException("no variable in <set>");

                var hints = set.Attribute("hints");

                if (hints != null && hints.Value != "files")
                {
                    Console.WriteLine("Ignoring group '{0}'", @var.Value);
                    continue;
                }

                var filesText = set.Value;
                var files = filesText.Split()
                    .Where(s => !String.IsNullOrWhiteSpace(s)).ToList();

                FileGroups[@var.Value] = files;
            }
        }

        public void GenerateFileGroup(string groupName)
        {
            if (!FileGroups.ContainsKey(groupName))
                return;

            var groupFiles = FileGroups[groupName];
            WriteLine("-- {0}", groupName);

            WriteLine("files");
            WriteStartBraceIndent();
            foreach (var path in groupFiles)
            {
                var file = path;
                if (file.StartsWith("wx/"))
                    file = "include/" + path;

                WriteLine("\"{0}\",", file);
            }
            WriteCloseBraceIndent();
            NewLine();
        }

        public void GenerateConfigBlockStart(string configName)
        {
            WriteLine("cfg = configuration {{ \"{0}\" }}", configName);
            PushIndent();
        }

        public void GenerateConfigBlockEnd()
        {
            PopIndent();
            WriteLine("configuration (cfg)");
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Invalid arguments.");
                Console.WriteLine("Usage:");
                Console.WriteLine("  {0}.exe files.bkl output.lua",
                    Path.GetFileName(Environment.GetCommandLineArgs()[0]));
                return;
            }

            var parser = new BakefileParser();
            parser.Parse(args[0]);

            var options = new {UseAUI = true, UseAdvanced = true,
                UsePropgrid = true};

            // Common files
            parser.GenerateFileGroup("BASE_CMN_SRC");
            parser.GenerateFileGroup("BASE_CMN_HDR");
            parser.GenerateFileGroup("BASE_AND_GUI_CMN_SRC");

            parser.GenerateFileGroup("GUI_CMN_SRC");
            parser.GenerateFileGroup("GUI_CMN_HDR");

            if (options.UseAUI)
            {
                parser.GenerateFileGroup("AUI_SRC");
                parser.GenerateFileGroup("AUI_HDR");
            }

            if (options.UsePropgrid)
            {
                parser.GenerateFileGroup("PROPGRID_SRC");
                parser.GenerateFileGroup("PROPGRID_HDR");
            }

            if (options.UseAdvanced)
            {
                parser.GenerateFileGroup("ADVANCED_CMN_SRC");
                parser.GenerateFileGroup("ADVANCED_CMN_HDR");
            }

            // Unix and Darwin files
            parser.GenerateConfigBlockStart("unix osx");
                parser.GenerateFileGroup("BASE_UNIX_AND_DARWIN_SRC");
                parser.GenerateFileGroup("BASE_UNIX_AND_DARWIN_HDR");
            parser.GenerateConfigBlockEnd();

            // Unix files
            parser.GenerateConfigBlockStart("unix");
                parser.GenerateFileGroup("UNIX_SRC");
            parser.GenerateConfigBlockEnd();

            // Windows files
            parser.GenerateConfigBlockStart("windows");
                parser.GenerateFileGroup("BASE_WIN32_SRC");
                parser.GenerateFileGroup("BASE_AND_GUI_WIN32_SRC");
                parser.GenerateFileGroup("BASE_WIN32_HDR");

                parser.GenerateFileGroup("MSW_LOWLEVEL_SRC");
                parser.GenerateFileGroup("MSW_DESKTOP_LOWLEVEL_SRC");
                parser.GenerateFileGroup("MSW_DESKTOP_LOWLEVEL_HDR");
                parser.GenerateFileGroup("MSW_SRC");
                parser.GenerateFileGroup("MSW_HDR");
                parser.GenerateFileGroup("MSW_DESKTOP_SRC");
                parser.GenerateFileGroup("MSW_DESKTOP_HDR");

                parser.GenerateFileGroup("MSW_DESKTOP_SRC");
                parser.GenerateFileGroup("MSW_DESKTOP_HDR");

                if (options.UseAdvanced)
                {
                    parser.GenerateFileGroup("ADVANCED_MSW_SRC");
                    parser.GenerateFileGroup("ADVANCED_MSW_HDR");

                    parser.GenerateFileGroup("ADVANCED_MSW_NATIVE_SRC");
                    parser.GenerateFileGroup("ADVANCED_MSW_NATIVE_HDR");
                }
            parser.GenerateConfigBlockEnd();

            var generated = parser.ToString();
            File.WriteAllText(args[1], generated);

            Console.WriteLine("Generated {0}...", args[1]);
        }
    }
}
