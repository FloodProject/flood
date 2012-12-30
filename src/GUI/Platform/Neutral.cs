using System;
using System.Threading;
using System.Windows.Forms;

namespace Gwen.Platform
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
            // code from http://forums.getpaint.net/index.php?/topic/13712-trouble-accessing-the-clipboard/page__view__findpost__p__226140
            String ret = String.Empty;
            Thread staThread = new Thread(
                () =>
                {
                    try
                    {
                        if (!Clipboard.ContainsText())
                            return;
                        ret = Clipboard.GetText();
                    }
                    catch (Exception)
                    {
                        return;
                    }
                });
            staThread.SetApartmentState(ApartmentState.STA);
            staThread.Start();
            staThread.Join();
            // at this point either you have clipboard data or an exception
            return ret;
        }

        /// <summary>
        /// Sets the clipboard text.
        /// </summary>
        /// <param name="text">Text to set.</param>
        /// <returns>True if succeeded.</returns>
        public static bool SetClipboardText(String text)
        {
            bool ret = false;
            Thread staThread = new Thread(
                () =>
                {
                    try
                    {
                        Clipboard.SetText(text);
                        ret = true;
                    }
                    catch (Exception)
                    {
                        return;
                    }
                });
            staThread.SetApartmentState(ApartmentState.STA);
            staThread.Start();
            staThread.Join();
            // at this point either you have clipboard data or an exception
            return ret;
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
            var dialog = new OpenFileDialog
                             {
                                 Title = title,
                                 InitialDirectory = startPath,
                                 DefaultExt = @"*.*",
                                 Filter = extension,
                                 CheckPathExists = true,
                                 Multiselect = false
                             };
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                if (callback != null)
                {
                    callback(dialog.FileName);
                }
            }
            else
            {
                if (callback != null)
                {
                    callback(String.Empty);
                }
                return false;
            }

            return true;
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
            var dialog = new SaveFileDialog
            {
                Title = title,
                InitialDirectory = startPath,
                DefaultExt = @"*.*",
                Filter = extension,
                CheckPathExists = true,
                OverwritePrompt = true
            };
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                if (callback != null)
                {
                    callback(dialog.FileName);
                }
            }
            else
            {
                if (callback != null)
                {
                    callback(String.Empty);
                }
                return false;
            }

            return true;
        }
    }
}
