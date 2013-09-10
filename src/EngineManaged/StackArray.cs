using System;
using System.Runtime.InteropServices;

namespace Flood
{
    public struct StackArray<T,TData>
        where T : struct
        where TData : struct
    {
        [StructLayout(LayoutKind.Sequential)]
        private struct ElementData
        {
            public byte ptr;
            public T data;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct ArrayData
        {
            public byte ptr;
            public TData data;
        }

        private ArrayData data;
        private int length;

        public int Length
        {
            get
            {
                if (length != 0)
                    return length;

                return length = Marshal.SizeOf(data.data)/Marshal.SizeOf(default(T));
            }
        }

        public unsafe T this[int position]
        {
            get
            {
                if(position >= Length )
                    throw new IndexOutOfRangeException();

                var sizeOf = Marshal.SizeOf(typeof(T));
                var offset = sizeOf * position + 1;
                
                var element = new ElementData();

                fixed (byte* pin = &data.ptr)
                    for (var i = 0; i < sizeOf; i++)
                        *(&element.ptr + 1 + i) = *(pin + offset + i);

                return element.data;
            }
            set 
            { 
                if(position >= Length )
                    throw new IndexOutOfRangeException();

                var sizeOf = Marshal.SizeOf(typeof(T));
                var offset = sizeOf * position + 1;

                var element = new ElementData {data = value};

                fixed (byte* pin = &data.ptr)
                    for (var i = 0; i < sizeOf; i++)
                        *(pin + offset + i) = *(&element.ptr + 1 + i);
            }
        }
    }

    public class StackArray<T>
        where T : struct
    {
        protected StackArray(){}

        public static StackArray<T, S002<T>> S002
        {
            get { return new StackArray<T, S002<T>>();}
        }

        public static StackArray<T, S003<T>> S003
        {
            get { return new StackArray<T, S003<T>>();}
        }

        public static StackArray<T, S004<T>> S004
        {
            get { return new StackArray<T, S004<T>>();}
        }

        public static StackArray<T, S005<T>> S005
        {
            get { return new StackArray<T, S005<T>>();}
        }

        public static StackArray<T, S010<T>> S010
        {
            get { return new StackArray<T, S010<T>>();}
        }

        public static StackArray<T, S020<T>> S020
        {
            get { return new StackArray<T, S020<T>>();}
        }

        public static StackArray<T, S040<T>> S040
        {
            get { return new StackArray<T, S040<T>>();}
        }

        public static StackArray<T, S060<T>> S060
        {
            get { return new StackArray<T, S060<T>>();}
        }

        public static StackArray<T, S080<T>> S080
        {
            get { return new StackArray<T, S080<T>>();}
        }

        public static StackArray<T, S100<T>> S100
        {
            get { return new StackArray<T, S100<T>>();}
        }
    }

    public struct S002<T>
    {
        private T _1;
        private T _2;
    }

    public struct S003<T>
    {
        private T _1;
        private S002<T> _2;
    }

    public struct S004<T>
    {
        private S002<T> _1;
        private S002<T> _2;
    }

    public struct S005<T>
    {
        private S002<T> _1;
        private S003<T> _2;
    }

    public struct S010<T>
    {
        private S005<T> _1;
        private S005<T> _2;
    }

    public struct S020<T>
    {
        private S010<T> _1;
        private S010<T> _2;
    }

    public struct S040<T>
    {
        private S020<T> _1;
        private S020<T> _2;
    }

    public struct S060<T>
    {
        private S020<T> _2;
        private S040<T> _1;
    }

    public struct S080<T>
    {
        private S040<T> _1;
        private S040<T> _2;
    }

    public struct S100<T>
    {
        private S040<T> _2;
        private S060<T> _1;
    }
}
