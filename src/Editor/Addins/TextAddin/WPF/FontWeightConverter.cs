
using System;

namespace TextAddin
{
    class FontWeightConverter
    {
        public static FontWeight? ConvertFromString(string fontWeight)
        {
            fontWeight = fontWeight.ToLower();
            switch(fontWeight)
            {
                case "thin": return new FontWeight(100);
                case "extralight": return new FontWeight(200);
                case "ultralight": return new FontWeight(200);
                case "light": return new FontWeight(300);
                case "normal": return new FontWeight(400);
                case "regular": return new FontWeight(400);
                case "medium": return new FontWeight(500);
                case "demibold": return new FontWeight(600);
                case "semibold": return new FontWeight(600);
                case "bold": return new FontWeight(700);
                case "extrabold": return new FontWeight(800);
                case "ultrabold": return new FontWeight(800);
                case "black": return new FontWeight(900);
                case "heavy": return new FontWeight(900);
                case "extrablack": return new FontWeight(950);
                case "ultrablack": return new FontWeight(950);
            }
            throw new NotImplementedException();
        }

        public static string ConvertToString(FontWeight value)
        {
            throw new System.NotImplementedException();
        }
    }
}
