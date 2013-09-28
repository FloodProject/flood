using System;
using System.Text;

namespace Weaver.Util
{
    internal class Logger
    {
        public enum LogLevel
        {
            None,
            Info,
            Warning,
            Error
        }

        public LogLevel Level { get; set; }

        public int Tabs { get; set; }

        public Logger(LogLevel level)
        {
            Level = level;
            Tabs = 0;
        }

        public void Info(string msg)
        {
            Log(LogLevel.Info, msg);
        }

        public void Warning(string msg)
        {
            Log(LogLevel.Warning, msg);
        }

        public void Error(string msg)
        {
            Log(LogLevel.Error, msg);
        }

        private void Log(LogLevel lvl, string msg)
        {
            if (lvl < Level)
                return;

            var sb = new StringBuilder();
            sb.Append(' ', 3*Tabs);
            sb.Append(msg);
            Console.WriteLine(sb.ToString());
        }
    }
}
