using System;

namespace Flood.GUIv2.Platform
{
    /// <summary>
    /// Platform-agnostic utility functions.
    /// </summary>
    public static class Neutral
    {
        private static DateTime m_LastTime;
        private static float m_CurrentTime;

        /// <summary>
        /// Changes the mouse cursor.
        /// </summary>
        /// <param name="cursor">Cursor type.</param>
        public static void SetCursor(Cursor cursor)
        {
            Cursor.Current = cursor;
        }

        /// <summary>
        /// Gets text from clipboard.
        /// </summary>
        /// <returns>Clipboard text.</returns>
        public static String GetClipboardText()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Sets the clipboard text.
        /// </summary>
        /// <param name="text">Text to set.</param>
        /// <returns>True if succeeded.</returns>
        public static bool SetClipboardText(String text)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Gets time since last measurement.
        /// </summary>
        /// <returns>Time interval in seconds.</returns>
        public static float GetTimeInSeconds()
        {
            var time = DateTime.UtcNow;
            var diff = time - m_LastTime;
            var seconds = diff.TotalSeconds;
            if (seconds > 0.1)
                seconds = 0.1;
            m_CurrentTime += (float)seconds;
            m_LastTime = time;
            return m_CurrentTime;
        }

        /// <summary>
        /// Displays an open file dialog.
        /// </summary>
        /// <param name="title">Dialog title.</param>
        /// <param name="startPath">Initial path.</param>
        /// <param name="extension">File extension filter.</param>
        /// <param name="callback">Callback that is executed after the dialog completes.</param>
        /// <returns>True if succeeded.</returns>
        public static bool FileOpen(String title, String startPath, String extension, Action<String> callback)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Displays a save file dialog.
        /// </summary>
        /// <param name="title">Dialog title.</param>
        /// <param name="startPath">Initial path.</param>
        /// <param name="extension">File extension filter.</param>
        /// <param name="callback">Callback that is executed after the dialog completes.</param>
        /// <returns>True if succeeded.</returns>
        public static bool FileSave(String title, String startPath, String extension, Action<String> callback)
        {
            throw new NotImplementedException();
        }
    }
}
