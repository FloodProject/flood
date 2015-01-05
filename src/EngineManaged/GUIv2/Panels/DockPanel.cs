using System;
using System.Collections.Generic;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Panels
{
    public class DockPanel : Panel
    {
        public DockPanel(ExpansionFlags expansion = ExpansionFlags.Fit) 
            : base(expansion)
        {
            new DockSizer(this);
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
                layoutMinSize = UpdateSize(layoutMinSize, child.LayoutMinSize);
                layoutBestSize = UpdateSize(layoutBestSize, child.LayoutBestSize);
                layoutMaxSize = UpdateSize(layoutMaxSize, child.LayoutMaxSize);
            }
        }

        private Vector2i UpdateSize(Vector2i thisSize, Vector2i childSize)
        {
            var x = Math.Max(thisSize.X, childSize.X);
            var y = Math.Max(thisSize.Y, childSize.Y);

            return new Vector2i(x, y);
        }

    }
}
