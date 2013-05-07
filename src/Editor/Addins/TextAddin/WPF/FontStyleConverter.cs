
using System;

namespace TextAddin
{
    class FontStyleConverter
    {
        public static FontStyle? ConvertFromString(string fontStyle)
        {
            fontStyle = fontStyle.ToLower();
            switch(fontStyle)
            {
                case "normal": return FontStyle.Normal;
                case "italic": return FontStyle.Italic;
                case "oblique": return FontStyle.Oblique;
            }
            throw new NotImplementedException();
        }

        public static string ConvertToString(FontStyle value)
        {
            throw new System.NotImplementedException();
        }
    }
}
