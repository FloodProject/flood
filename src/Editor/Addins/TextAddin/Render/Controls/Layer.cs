using Flood.GUI;
using Flood.GUI.Controls;

namespace TextAddin.Render
{
    abstract class Layer : Control
    {
        public TextView TextView { get { return (TextView) Parent; }}

        public Layer(TextView parent) : base(parent)
        {
            Dock = Pos.Fill;
        }
    }
}
