
namespace Weaver.Util
{
    interface IDelayedCopy
    {
        void Copy();

        CopyNode CopyParent { get; set; }
    }

    public class DelayedCopy<T> : IDelayedCopy
    {
        private readonly T originObject;
        private readonly T destObject;
        private readonly CopyDelegate action;

        public CopyNode CopyParent { get; set; }

        public delegate void CopyDelegate(T originObject, T destObject);

        public DelayedCopy(T originObject, T destObject, CopyDelegate action)
        {
            this.originObject = originObject;
            this.destObject = destObject;
            this.action = action;
        }

        public void Copy()
        {
            action.Invoke(originObject, destObject);
        }
    }
}
