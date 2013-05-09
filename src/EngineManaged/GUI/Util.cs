using System;
using System.Drawing;
using System.Text.RegularExpressions;

namespace Flood.GUI
{
    /// <summary>
    /// Misc utility functions.
    /// </summary>
    public static class Util
    {
        public static int Round(float x)
        {
            return (int)Math.Round(x, MidpointRounding.AwayFromZero);
        }
        /*
        public static int Trunc(float x)
        {
            return (int)Math.Truncate(x);
        }
        */
        public static int Ceil(float x)
        {
            return (int)Math.Ceiling(x);
        }

        public static Rectangle FloatRect(float x, float y, float w, float h)
        {
            return new Rectangle((int)x, (int)y, (int)w, (int)h);
        }

        public static int Clamp(int x, int min, int max)
        {
            if (x < min)
                return min;
            if (x > max)
                return max;
            return x;
        }

        public static float Clamp(float x, float min, float max)
        {
            if (x < min)
                return min;
            if (x > max)
                return max;
            return x;
        }

        public static Rectangle ClampRectToRect(Rectangle inside, Rectangle outside, bool clampSize = false)
        {
            if (inside.X < outside.X)
                inside.X = outside.X;

            if (inside.Y < outside.Y)
                inside.Y = outside.Y;

            if (inside.GetRight() > outside.GetRight())
            {
                if (clampSize)
                    inside.Width = outside.Width;
                else
                    inside.X = outside.GetRight() - inside.Width;
            }
            if (inside.GetBottom() > outside.GetBottom())
            {
                if (clampSize)
                    inside.Height = outside.Height;
                else
                    inside.Y = outside.GetBottom() - inside.Height;
            }

            return inside;
        }

        // from http://stackoverflow.com/questions/359612/how-to-change-rgb-color-to-hsv
        public static HSV ToHSV(this Color color)
        {
            throw new NotImplementedException();

            /*HSV hsv = new HSV();
            int max = Math.Max(color.R, Math.Max(color.G, color.B));
            int min = Math.Min(color.R, Math.Min(color.G, color.B));

            hsv.h = color.GetHue();
            hsv.s = (max == 0) ? 0 : 1f - (1f * min / max);
            hsv.v = max / 255f;

            return hsv;*/
        }

        public static Color HSVToColor(float h, float s, float v)
        {
            int hi = Convert.ToInt32(Math.Floor(h / 60)) % 6;
            float f = h / 60 - (float)Math.Floor(h / 60);

            v = v * 255;
            var va = Convert.ToByte(v);
            var p = Convert.ToByte(v * (1 - s));
            var q = Convert.ToByte(v * (1 - f * s));
            var t = Convert.ToByte(v * (1 - (1 - f) * s));

            if (hi == 0)
                return new Color(va, t, p, 255);
            if (hi == 1)
                return new Color(q, va, p, 255);
            if (hi == 2)
                return new Color(p, va, t, 255);
            if (hi == 3)
                return new Color(p, q, va, 255);
            if (hi == 4)
                return new Color(t, p, va, 255);
            return new Color(va, p, q, 255);
        }

        // can't create extension operators
        public static Color Subtract(this Color color, Color other)
        {
            var nR = (byte)Math.Max(0, color.R - other.R);
            var nG = (byte)Math.Max(0, color.G - other.G);
            var nB = (byte)Math.Max(0, color.B - other.B);
            var nA = (byte)Math.Max(0, color.A - other.A);
            return new Color(nR, nG, nB, nA);
        }

        public static Color Add(this Color color, Color other)
        {
            var nR = (byte)Math.Min(255, color.R + other.R);
            var nG = (byte)Math.Min(255, color.G + other.G);
            var nB = (byte)Math.Min(255, color.B + other.B);
            var nA = (byte)Math.Min(255, color.A + other.A);
            return new Color(nR, nG, nB, nA);
        }

        public static Color Multiply(this Color color, float amount)
        {
            if(amount < 0)
                throw new ArgumentException("amount must be positive");

            var nR = (byte)Math.Min(255, color.R * amount);
            var nG = (byte)Math.Min(255, color.G * amount);
            var nB = (byte)Math.Min(255, color.B * amount);
            return new Color(nR, nG, nB, color.A);
        }

        public static Rectangle Add(this Rectangle r, Rectangle other)
        {
            return new Rectangle(r.X + other.X, r.Y + other.Y, r.Width + other.Width, r.Height + other.Height);
        }

        /// <summary>
        /// Splits a string but keeps the separators intact (at the end of split parts).
        /// </summary>
        /// <param name="text">String to split.</param>
        /// <param name="separators">Separator characters.</param>
        /// <returns>Split strings.</returns>
        public static String[] SplitAndKeep(String text, String separators)
        {
            return Regex.Split(text, @"(?=[" + separators + "])");
        }
    }
}
