using System;
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

        //this should only be called if control is Fit
        public override void LayoutSizes()
        {
            layoutMinSize = new Vector2i(MinNotSet, MinNotSet);
            layoutBestSize = new Vector2i(MinNotSet, MinNotSet);
            layoutMaxSize = new Vector2i(MinNotSet, MinNotSet);
            InheritedExpandHorizontal = false; //todo: this logic to update expand of panel must be done in all panels
            InheritedExpandVertical = false; //todo: this logic to update expand of panel must be done in all panels
            foreach (var child in Children)
            {
                if (child.IsHidden) continue;
                if (child.IsExpandHorizontal) InheritedExpandHorizontal = true;
                if (child.IsExpandVertical) InheritedExpandVertical = true;
                layoutMinSize = UpdateSize(layoutMinSize, child.LayoutMinSize, child);
                layoutBestSize = UpdateSize(layoutBestSize, child.LayoutBestSize, child);
                layoutMaxSize = UpdateSize(layoutMaxSize, child.LayoutMaxSize, child);
            }
        }

        private Vector2i UpdateSize(Vector2i thisSize, Vector2i childSize, IControl child)
        {
            var x = Math.Max(thisSize.X, childSize.X + ChildrenPositions[child].X - child.Margin.Left);
            var y = Math.Max(thisSize.Y, childSize.Y + ChildrenPositions[child].Y - child.Margin.Top);

            return new Vector2i(x, y);
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
