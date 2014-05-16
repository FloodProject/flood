using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;

// todo: compile/run only on windows

namespace Flood.GUIv2.Platform
{
    /// <summary>
    /// Windows-specific utility functions.
    /// </summary>
    public static class Windows
    {
        private const String FontRegKey = @"SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts";

        private static Dictionary<String, String> m_FontPaths;

        /// <summary>
        /// Gets a font file path from font name.
        /// </summary>
        /// <param name="fontName">Font name.</param>
        /// <returns>Font file path.</returns>
        public static String GetFontPath(String fontName)
        {
            // is this reliable? we rely on lazy jitting to not run win32 code on linux
            if (Environment.OSVersion.Platform != PlatformID.Win32NT)
                return null;

            if (m_FontPaths == null)
                InitFontPaths();

            if (!m_FontPaths.ContainsKey(fontName))
                return null;

            return m_FontPaths[fontName];
        }

        private static void InitFontPaths()
        {
            // very hacky but better than nothing
            m_FontPaths = new Dictionary<String, String>();
            String fontsDir = Environment.GetFolderPath(Environment.SpecialFolder.Fonts);

            RegistryKey key = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Default);
            RegistryKey subkey = key.OpenSubKey(FontRegKey);
            foreach (String fontName in subkey.GetValueNames())
            {
                String fontFile = (String)subkey.GetValue(fontName);
                if (!fontName.EndsWith(" (TrueType)"))
                    continue;
                String font = fontName.Replace(" (TrueType)", "");
                m_FontPaths[font] = Path.Combine(fontsDir, fontFile);
            }
            key.Dispose();
        }
    }
}
