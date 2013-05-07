using Flood.GUI;
using Flood.GUI.Controls;

namespace TextAddin.Controls
{
    public abstract class Layer : Control
    {
        public TextView TextView { get { return (TextView) Parent; }}

        protected Layer(TextView parent) : base(parent)
        {
            Dock = Pos.Fill;
        }
    }
}
