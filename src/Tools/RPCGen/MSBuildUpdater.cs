using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Flood.Tools.RPCGen
{
    /// <summary>
    /// This class is used to update the MSBuild project so that the generated
    /// files appear in the IDE right when they are generated. This will most
    /// likely be triggered by the build system in the post-build events.
    /// </summary>
    class MSBuildUpdater
    {
        static readonly string NewLine = Environment.NewLine;
        static readonly string bof = "<!--RPCGen BOF-->" + NewLine;
        static readonly string eof = "<!--RPCGen EOF-->" + NewLine;

        public static void UpdateMSBuild(string msbuildPath, List<string> addedFiles)
        {
            string msbuild = File.ReadAllText(msbuildPath);

            // Check if there is an item group.
            var itemIndex = msbuild.IndexOf("<ItemGroup>", StringComparison.Ordinal);

            if (itemIndex < 0)
                throw new Exception("Empty MSBuild Project File");

            // Check if there are the BOF and EOF XML markers.
            int bofIndex = msbuild.IndexOf(bof, StringComparison.Ordinal);
            int eofIndex = msbuild.IndexOf(eof, StringComparison.Ordinal);

            var itemGroup = GenerateItemGroup(msbuildPath, addedFiles);
            string newMSBuild = null;

            // If we have the markers, then replace the contents between them.
            if (bofIndex > 0 && eofIndex > 0)
            {
                newMSBuild = msbuild.Substring(0, bofIndex) + itemGroup
                    + msbuild.Substring(eofIndex + eof.Count());
            }
            else
            {
                newMSBuild = msbuild.Insert(itemIndex, itemGroup);
            }

            // Do not rewrite the file if the generated contents have not changed.
            if (msbuild == newMSBuild)
                return;

            File.WriteAllText(msbuildPath, newMSBuild);
        }

        private static string GenerateItemGroup(string msbuildPath, List<string> addedFiles)
        {
            var str = bof;
            str += "<ItemGroup>" + NewLine;
            Uri msbuildURI = new Uri(msbuildPath);
            foreach (var file in addedFiles)
            {
                Uri fileURI = new Uri(file);
                var relativePath = msbuildURI.MakeRelativeUri(fileURI).ToString();
                relativePath = relativePath.Replace('/', Path.DirectorySeparatorChar);

                str += "    <Compile Include=\"" + relativePath + "\">" + NewLine;
                str += string.Format("      <Link>{0}</Link>{1}",
                    Path.Combine("Generated", Path.GetFileName(relativePath)), NewLine);
                str += "    </Compile>" + NewLine;
            }
            str += "  </ItemGroup>" + NewLine;
            str += eof;
            return str;
        }
    }
}
