using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.GUI.Controls.Containers
{
    public class RelativePositionPanel : Panel
    {
        public Dictionary<Control, Vector2i> ChildrenPositions { get; set; }

        public RelativePositionPanel(Canvas parent, ExpansionFlags expansion = ExpansionFlags.Fit) : base(parent, expansion)
        {
            Init();
        }

        public RelativePositionPanel(Panel parent, ExpansionFlags expansion = ExpansionFlags.Fit) : base(parent, expansion)
        {
            Init();
        }

        private void Init()
        {
            ChildrenPositions = new Dictionary<Control, Vector2i>();
            new RelativePositionSizer(this);
        }

        public override void AddChild(Control child)
        {
            base.AddChild(child);
            ChildrenPositions[child] = new Vector2i(0, 0);
        }

        public override void RemoveChild(Control child, bool dispose)
        {
            if (!ChildrenPositions.ContainsKey(child)) return;
            ChildrenPositions.Remove(child);
            base.RemoveChild(child, dispose);
        }

        public void SetPosition(Control child, int x, int y)
        {
            SetPosition(child, new Vector2i(x, y));
        }

        public void SetPosition(Control child, Vector2i position)
        {
            if (!ChildrenPositions.ContainsKey(child))
                return;
            ChildrenPositions[child] = position;
        }

    }
}
