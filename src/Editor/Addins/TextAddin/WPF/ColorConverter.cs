

using System;
using System.Globalization;
using Flood;

namespace TextAddin
{
    public class ColorConverter
    {

        public static Color ConvertFromString(string colorString)
        {
            if (colorString.StartsWith("#", StringComparison.Ordinal)) {
                byte a = 255;
                int offset = 0;
                if (colorString.Length > 7) {
                    offset = 2;
                    a = byte.Parse(colorString.Substring(1,2), NumberStyles.HexNumber, CultureInfo.InvariantCulture);
                }
                
                byte r = byte.Parse(colorString.Substring(1 + offset,2), NumberStyles.HexNumber, CultureInfo.InvariantCulture);
                byte g = byte.Parse(colorString.Substring(3 + offset,2), NumberStyles.HexNumber, CultureInfo.InvariantCulture);
                byte b = byte.Parse(colorString.Substring(5 + offset,2), NumberStyles.HexNumber, CultureInfo.InvariantCulture);
                return new Color(r,g,b,a);
            }

            switch(colorString)
            {
                case "AliceBlue": return ConvertFromString("#FFF0F8FF");
                case "AntiqueWhite": return ConvertFromString("#FFFAEBD7");
                case "Aqua": return ConvertFromString("#FF00FFFF");
                case "Aquamarine": return ConvertFromString("#FF7FFFD4");
                case "Azure": return ConvertFromString("#FFF0FFFF");
                case "Beige": return ConvertFromString("#FFF5F5DC");
                case "Bisque": return ConvertFromString("#FFFFE4C4");
                case "Black": return ConvertFromString("#FF000000");
                case "BlanchedAlmond": return ConvertFromString("#FFFFEBCD");
                case "Blue": return ConvertFromString("#FF0000FF");
                case "BlueViolet": return ConvertFromString("#FF8A2BE2");
                case "Brown": return ConvertFromString("#FFA52A2A");
                case "BurlyWood": return ConvertFromString("#FFDEB887");
                case "CadetBlue": return ConvertFromString("#FF5F9EA0");
                case "Chartreuse": return ConvertFromString("#FF7FFF00");
                case "Chocolate": return ConvertFromString("#FFD2691E");
                case "Coral": return ConvertFromString("#FFFF7F50");
                case "CornflowerBlue": return ConvertFromString("#FF6495ED");
                case "Cornsilk": return ConvertFromString("#FFFFF8DC");
                case "Crimson": return ConvertFromString("#FFDC143C");
                case "Cyan": return ConvertFromString("#FF00FFFF");
                case "DarkBlue": return ConvertFromString("#FF00008B");
                case "DarkCyan": return ConvertFromString("#FF008B8B");
                case "DarkGoldenrod": return ConvertFromString("#FFB8860B");
                case "DarkGray": return ConvertFromString("#FFA9A9A9");
                case "DarkGreen": return ConvertFromString("#FF006400");
                case "DarkKhaki": return ConvertFromString("#FFBDB76B");
                case "DarkMagenta": return ConvertFromString("#FF8B008B");
                case "DarkOliveGreen": return ConvertFromString("#FF556B2F");
                case "DarkOrange": return ConvertFromString("#FFFF8C00");
                case "DarkOrchid": return ConvertFromString("#FF9932CC");
                case "DarkRed": return ConvertFromString("#FF8B0000");
                case "DarkSalmon": return ConvertFromString("#FFE9967A");
                case "DarkSeaGreen": return ConvertFromString("#FF8FBC8F");
                case "DarkSlateBlue": return ConvertFromString("#FF483D8B");
                case "DarkSlateGray": return ConvertFromString("#FF2F4F4F");
                case "DarkTurquoise": return ConvertFromString("#FF00CED1");
                case "DarkViolet": return ConvertFromString("#FF9400D3");
                case "DeepPink": return ConvertFromString("#FFFF1493");
                case "DeepSkyBlue": return ConvertFromString("#FF00BFFF");
                case "DimGray": return ConvertFromString("#FF696969");
                case "DodgerBlue": return ConvertFromString("#FF1E90FF");
                case "Firebrick": return ConvertFromString("#FFB22222");
                case "FloralWhite": return ConvertFromString("#FFFFFAF0");
                case "ForestGreen": return ConvertFromString("#FF228B22");
                case "Fuchsia": return ConvertFromString("#FFFF00FF");
                case "Gainsboro": return ConvertFromString("#FFDCDCDC");
                case "GhostWhite": return ConvertFromString("#FFF8F8FF");
                case "Gold": return ConvertFromString("#FFFFD700");
                case "Goldenrod": return ConvertFromString("#FFDAA520");
                case "Gray": return ConvertFromString("#FF808080");
                case "Green": return ConvertFromString("#FF008000");
                case "GreenYellow": return ConvertFromString("#FFADFF2F");
                case "Honeydew": return ConvertFromString("#FFF0FFF0");
                case "HotPink": return ConvertFromString("#FFFF69B4");
                case "IndianRed": return ConvertFromString("#FFCD5C5C");
                case "Indigo": return ConvertFromString("#FF4B0082");
                case "Ivory": return ConvertFromString("#FFFFFFF0");
                case "Khaki": return ConvertFromString("#FFF0E68C");
                case "Lavender": return ConvertFromString("#FFE6E6FA");
                case "LavenderBlush": return ConvertFromString("#FFFFF0F5");
                case "LawnGreen": return ConvertFromString("#FF7CFC00");
                case "LemonChiffon": return ConvertFromString("#FFFFFACD");
                case "LightBlue": return ConvertFromString("#FFADD8E6");
                case "LightCoral": return ConvertFromString("#FFF08080");
                case "LightCyan": return ConvertFromString("#FFE0FFFF");
                case "LightGoldenrodYellow": return ConvertFromString("#FFFAFAD2");
                case "LightGray": return ConvertFromString("#FFD3D3D3");
                case "LightGreen": return ConvertFromString("#FF90EE90");
                case "LightPink": return ConvertFromString("#FFFFB6C1");
                case "LightSalmon": return ConvertFromString("#FFFFA07A");
                case "LightSeaGreen": return ConvertFromString("#FF20B2AA");
                case "LightSkyBlue": return ConvertFromString("#FF87CEFA");
                case "LightSlateGray": return ConvertFromString("#FF778899");
                case "LightSteelBlue": return ConvertFromString("#FFB0C4DE");
                case "LightYellow": return ConvertFromString("#FFFFFFE0");
                case "Lime": return ConvertFromString("#FF00FF00");
                case "LimeGreen": return ConvertFromString("#FF32CD32");
                case "Linen": return ConvertFromString("#FFFAF0E6");
                case "Magenta": return ConvertFromString("#FFFF00FF");
                case "Maroon": return ConvertFromString("#FF800000");
                case "MediumAquamarine": return ConvertFromString("#FF66CDAA");
                case "MediumBlue": return ConvertFromString("#FF0000CD");
                case "MediumOrchid": return ConvertFromString("#FFBA55D3");
                case "MediumPurple": return ConvertFromString("#FF9370DB");
                case "MediumSeaGreen": return ConvertFromString("#FF3CB371");
                case "MediumSlateBlue": return ConvertFromString("#FF7B68EE");
                case "MediumSpringGreen": return ConvertFromString("#FF00FA9A");
                case "MediumTurquoise": return ConvertFromString("#FF48D1CC");
                case "MediumVioletRed": return ConvertFromString("#FFC71585");
                case "MidnightBlue": return ConvertFromString("#FF191970");
                case "MintCream": return ConvertFromString("#FFF5FFFA");
                case "MistyRose": return ConvertFromString("#FFFFE4E1");
                case "Moccasin": return ConvertFromString("#FFFFE4B5");
                case "NavajoWhite": return ConvertFromString("#FFFFDEAD");
                case "Navy": return ConvertFromString("#FF000080");
                case "OldLace": return ConvertFromString("#FFFDF5E6");
                case "Olive": return ConvertFromString("#FF808000");
                case "OliveDrab": return ConvertFromString("#FF6B8E23");
                case "Orange": return ConvertFromString("#FFFFA500");
                case "OrangeRed": return ConvertFromString("#FFFF4500");
                case "Orchid": return ConvertFromString("#FFDA70D6");
                case "PaleGoldenrod": return ConvertFromString("#FFEEE8AA");
                case "PaleGreen": return ConvertFromString("#FF98FB98");
                case "PaleTurquoise": return ConvertFromString("#FFAFEEEE");
                case "PaleVioletRed": return ConvertFromString("#FFDB7093");
                case "PapayaWhip": return ConvertFromString("#FFFFEFD5");
                case "PeachPuff": return ConvertFromString("#FFFFDAB9");
                case "Peru": return ConvertFromString("#FFCD853F");
                case "Pink": return ConvertFromString("#FFFFC0CB");
                case "Plum": return ConvertFromString("#FFDDA0DD");
                case "PowderBlue": return ConvertFromString("#FFB0E0E6");
                case "Purple": return ConvertFromString("#FF800080");
                case "Red": return ConvertFromString("#FFFF0000");
                case "RosyBrown": return ConvertFromString("#FFBC8F8F");
                case "RoyalBlue": return ConvertFromString("#FF4169E1");
                case "SaddleBrown": return ConvertFromString("#FF8B4513");
                case "Salmon": return ConvertFromString("#FFFA8072");
                case "SandyBrown": return ConvertFromString("#FFF4A460");
                case "SeaGreen": return ConvertFromString("#FF2E8B57");
                case "SeaShell": return ConvertFromString("#FFFFF5EE");
                case "Sienna": return ConvertFromString("#FFA0522D");
                case "Silver": return ConvertFromString("#FFC0C0C0");
                case "SkyBlue": return ConvertFromString("#FF87CEEB");
                case "SlateBlue": return ConvertFromString("#FF6A5ACD");
                case "SlateGray": return ConvertFromString("#FF708090");
                case "Snow": return ConvertFromString("#FFFFFAFA");
                case "SpringGreen": return ConvertFromString("#FF00FF7F");
                case "SteelBlue": return ConvertFromString("#FF4682B4");
                case "Tan": return ConvertFromString("#FFD2B48C");
                case "Teal": return ConvertFromString("#FF008080");
                case "Thistle": return ConvertFromString("#FFD8BFD8");
                case "Tomato": return ConvertFromString("#FFFF6347");
                case "Transparent": return ConvertFromString("#00FFFFFF");
                case "Turquoise": return ConvertFromString("#FF40E0D0");
                case "Violet": return ConvertFromString("#FFEE82EE");
                case "Wheat": return ConvertFromString("#FFF5DEB3");
                case "White": return ConvertFromString("#FFFFFFFF");
                case "WhiteSmoke": return ConvertFromString("#FFF5F5F5");
                case "Yellow": return ConvertFromString("#FFFFFF00");
                case "YellowGreen": return ConvertFromString("#FF9ACD32");
            }

            throw new NotImplementedException();
        }
    }
}
