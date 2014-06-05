
using System;

namespace Flood
{
    public static class RectExtensions
    {
        public static int GetLeft(this Rectangle rect)
        {
            return rect.X;
        }

        public static int GetRight(this Rectangle rect)
        {
            return rect.X + rect.Width;
        }

        public static int GetTop(this Rectangle rect)
        {
            return rect.Y;
        }

        public static int GetBottom(this Rectangle rect)
        {
            return rect.Y + rect.Height;
        }

        public static float GetLeft(this RectangleF rect)
        {
            return rect.X;
        }

        public static float GetRight(this RectangleF rect)
        {
            return rect.X + rect.Width;
        }

        public static float GetTop(this RectangleF rect)
        {
            return rect.Y;
        }

        public static float GetBottom(this RectangleF rect)
        {
            return rect.Y + rect.Height;
        }
    }
    
    public static class VectorExtensions
    {
        public static Vector2i TryToAdd(this Vector2i v1, Vector2i v2)
        {
            var x = (v1.X == int.MaxValue) ? v1.X : v1.X + v2.X;
            var y = (v1.Y == int.MaxValue) ? v1.Y : v1.Y + v2.Y;
            return new Vector2i(x, y);
        }

        public static Vector2i TryToAddAssign(this Vector2i v1, Vector2i v2)
        {
            v1.X = (v1.X == int.MaxValue) ? v1.X : v1.X + v2.X;
            v1.Y = (v1.Y == int.MaxValue) ? v1.Y : v1.Y + v2.Y;
            return v1;
        }
        
        public static Vector2i TryToSubtract(this Vector2i v1, Vector2i v2)
        {
            var x = (v1.X == int.MinValue) ? v1.X : v1.X - v2.X;
            var y = (v1.Y == int.MinValue) ? v1.Y : v1.Y - v2.Y;
            return new Vector2i(x, y);
        }

        public static Vector2i TryToSubtractAssign(this Vector2i v1, Vector2i v2)
        {
            v1.X = (v1.X == int.MinValue) ? v1.X : v1.X - v2.X;
            v1.Y = (v1.Y == int.MinValue) ? v1.Y : v1.Y - v2.Y;
            return v1;
        }

        public static bool EQ(this Vector2i size1, Vector2i size2)
        {
            return size1.X == size2.X && size1.Y == size2.Y;
        }

        public static bool LE(this Vector2i size1, Vector2i size2)
        {
            return size1.EQ(size2) || size1.LT(size2);
        }

        public static bool LT(this Vector2i size1, Vector2i size2)
        {
            return size1.X < size2.X && size1.Y < size2.Y;
        }

        public static bool GE(this Vector2i size1, Vector2i size2)
        {
            return size1.EQ(size2) || size1.GT(size2);
        }

        public static bool GT(this Vector2i size1, Vector2i size2)
        {
            return size1.X > size2.X && size1.Y > size2.Y;
        }
    }

    internal static class StreamExtensions
    {
        /// <summary>
        /// Reads the contents of the stream into a byte array.
        /// data is returned as a byte array. An IOException is
        /// thrown if any of the underlying IO calls fail.
        /// </summary>
        /// <param name="stream">The stream to read.</param>
        /// <returns>A byte array containing the contents of the stream.</returns>
        /// <exception cref="NotSupportedException">The stream does not support reading.</exception>
        /// <exception cref="ObjectDisposedException">Methods were called after the stream was closed.</exception>
        /// <exception cref="System.IO.IOException">An I/O error occurs.</exception>
        public static byte[] ReadAllBytes(this System.IO.Stream source)
        {
            long originalPosition = source.Position;
            source.Position = 0;

            try
            {
                byte[] readBuffer = new byte[4096];

                int totalBytesRead = 0;
                int bytesRead;

                while ((bytesRead = source.Read(readBuffer, totalBytesRead, readBuffer.Length - totalBytesRead)) > 0)
                {
                    totalBytesRead += bytesRead;

                    if (totalBytesRead == readBuffer.Length)
                    {
                        int nextByte = source.ReadByte();
                        if (nextByte != -1)
                        {
                            byte[] temp = new byte[readBuffer.Length * 2];
                            System.Buffer.BlockCopy(readBuffer, 0, temp, 0, readBuffer.Length);
                            System.Buffer.SetByte(temp, totalBytesRead, (byte)nextByte);
                            readBuffer = temp;
                            totalBytesRead++;
                        }
                    }
                }

                byte[] buffer = readBuffer;
                if (readBuffer.Length != totalBytesRead)
                {
                    buffer = new byte[totalBytesRead];
                    System.Buffer.BlockCopy(readBuffer, 0, buffer, 0, totalBytesRead);
                }
                return buffer;
            }
            finally
            {
                source.Position = originalPosition;
            }
        }
    }

}
