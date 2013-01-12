using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Tools.RPCGen
{
    class MSBuildUpdater
    {
        private const string bof = "<!--RPCGen BOF-->\n";
        private const string eof = "<!--RPCGen EOF-->\n";

        public static void UpdateMSBuild(string msbuildPath, List<string> addedFiles)
        {
            string msbuild = File.ReadAllText(msbuildPath);

            int begin,end;
            
            begin = msbuild.IndexOf(bof);
            if(begin>0){
                end = msbuild.IndexOf(eof);
                end += eof.Count();
            }else{
                begin = end = msbuild.IndexOf("<ItemGroup>");
            }

            if (begin < 0)
                throw new Exception("Empty MSBuild Project File");

            var adds = bof;
            adds += "<ItemGroup>\n";
            Uri msbuildURI = new Uri(msbuildPath);
            foreach(var file in addedFiles)
            {
                Uri fileURI = new Uri(file);
                var relativePath = msbuildURI.MakeRelative(fileURI);
                relativePath = relativePath.Replace('/',Path.DirectorySeparatorChar);

                adds += "    <Compile Include=\""+relativePath+"\">\n";
                adds += string.Format("      <Link>{0}</Link>\n",
                    Path.Combine("Generated",Path.GetFileName(relativePath)));
                adds += "    </Compile>";
            }
            adds += "  </ItemGroup>\n";
            adds += eof;

            if(begin == end || adds != msbuild.Substring(begin, end-begin)){
                var newMSBuild = msbuild.Substring(0, begin) + adds + msbuild.Substring(end, msbuild.Length-end);
                File.WriteAllText(msbuildPath,newMSBuild);
            }

        }
    }
}
