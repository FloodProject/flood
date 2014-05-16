using System.Collections.Generic;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Panels
{
    public class RelativePanel : Panel
    {
        public Dictionary<IControl, Vector2i> ChildrenPositions { get; set; }

        public RelativePanel(ExpansionFlags expansion = ExpansionFlags.Fit)
            : base(expansion)
        {
            ChildrenPositions = new Dictionary<IControl, Vector2i>();
            new RelativeSizer(this);
        }

        public void AddChild(IControl child, int x, int y)
        {
            if(ChildrenPositions.ContainsKey(child))
                return;

            base.AddChild(child);
            ChildrenPositions[child] = new Vector2i(x, y);
        }
        
        public void AddChild(IControl child, Vector2i position)
        {
            if (ChildrenPositions.ContainsKey(child))
                return;

            base.AddChild(child);
            ChildrenPositions[child] = position;
        }

        public override void AddChild(IControl child)
        {
            if (ChildrenPositions.ContainsKey(child))
                return;

            base.AddChild(child);
            ChildrenPositions[child] = new Vector2i(0, 0);
        }

        public override void RemoveChild(IControl child, bool dispose)
        {
            if (!ChildrenPositions.ContainsKey(child))
                return;

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
