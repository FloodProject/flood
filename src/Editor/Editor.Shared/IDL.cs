using Flood.RPC.Metadata;
using System;


namespace CsharpServer
{
    [Message]
    public struct SharedStruct
    {
        [Id(1)]
        public int Key;
        [Id(2)]
        public string Value;
    }

    [Service]
    public interface ISharedService
    {
        SharedStruct GetStruct([Id(0)] int key);
    }

    public enum Operation : byte
    {
        Add = 1,
        Substract,
        Multiply,
        Divide
    }

    [Message]
    public struct Work
    {
        [Id(1), Default(0)]
        public int Num1;
        [Id(2)]
        public int Num2;
        [Id(3)]
        public Operation Op;
        [Id(4), Optional]
        public string Comment;
    }

    [Exception]
    public class InvalidOperation : Exception
    {
        [Id(1)]
        public int What;
        [Id(2)]
        public string Why;
    }

    /**
     * Ahh, now onto the cool part, defining a service. Services just need a name
     * and can optionally inherit from another service using the extends keyword.
     */

    [Service]
    public interface ICalculator : ISharedService
    {
        void Ping();

        int Add([Id(1)] int num1, [Id(2)] int num2);

        [Throws(1, typeof(InvalidOperation))]
        int Calculate([Id(1)] int logid, [Id(2)] Work work);

        [Oneway]
        void Zip();
    }
}
