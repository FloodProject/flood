using System;
using System.Text;

namespace RemotingGen
{
    internal class Logger
    {
        public enum LogLevel
        {
            None,
            Debug,
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

        public void Info(string msg, params object[] args)
        {
            Log(LogLevel.Info, msg, args);
        }

        public void Debug(string msg, params object[] args)
        {
            Log(LogLevel.Debug, msg, args);
        }

        public void Warning(string msg, params object[] args)
        {
            Log(LogLevel.Warning, msg, args);
        }

        public void Error(string msg, params object[] args)
        {
            Log(LogLevel.Error, msg, args);
        }

        private void Log(LogLevel lvl, string msg, params object[] args)
        {
            if (lvl < Level)
                return;

            var sb = new StringBuilder();
            sb.Append(' ', 3 * Tabs);
            sb.Append(string.Format(msg, args));
            Console.WriteLine(sb.ToString());
        }
    }
}
