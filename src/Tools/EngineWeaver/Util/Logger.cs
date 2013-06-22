using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EngineWeaver.Util
{
    internal class Logger
    {
        public enum Level {
            None,
            Info,
            Warning,
            Error
        }

        public Level level { get; set;}

        public int Tabs { get; set; }

        public Logger(Level level){
            this.level = level;
            Tabs = 0;
        }

        public void Info(string msg){
            Log(Level.Info,msg);
        }

        public void Warning(string msg){
            Log(Level.Warning,msg);
        }

         public void Error(string msg){
            Log(Level.Error,msg);
        }

        private void Log(Level lvl, string msg){
             if(lvl<level)
                 return;
            var sb = new StringBuilder();
            sb.Append(' ',3*Tabs);
            sb.Append(msg);
            Console.WriteLine(sb.ToString());
        }

    }
}
