using EngineWeaver.Util;
using Mono.Cecil;
using Mono.Cecil.Cil;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace EngineWeaver
{
    public class AssemblyWeaver
    {
        private string destAssemblyPath;
        private AssemblyDefinition destAssembly;

        private List<CecilCopier> copiers;

        public AssemblyWeaver(string destAssemblyPath)
        {
            this.destAssemblyPath = destAssemblyPath; 
            destAssembly = CecilUtils.GetAssemblyDef(destAssemblyPath);
        }

        public void AddAssembly(string origAssemblyPath)
        {
            var origAssembly = CecilUtils.GetAssemblyDef(origAssemblyPath);
            var copier = new CecilCopier(origAssembly.MainModule, destAssembly.MainModule);

            foreach (var origType in origAssembly.MainModule.Types)
                if(origType.BaseType != null)
                    copier.Copy(origType);

            copier.Process();

        }

        public void Write()
        {
            var writerParameters = new WriterParameters();
            writerParameters.WriteSymbols = destAssembly.MainModule.HasSymbols;
            writerParameters.SymbolWriterProvider = new Mono.Cecil.Pdb.PdbWriterProvider();
            destAssembly.Write(destAssemblyPath, writerParameters);
        }

    }
}
