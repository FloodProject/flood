using System;
using System.Collections.Generic;
using Flood.GUI.ControlInternal;

namespace Flood.GUI.Controls
{
    public class MultiSplitter : Control
    {
        
        public class Splitter : Dragger
        {
            public int Space { get; set; }

            public Splitter(MultiSplitter parent) : base(parent)
            {
                Space = 10;
                Target = this;
                RestrictToParent = true;

                Dragged += control => UpdateRelativePosition();
            }

            public float RelativePosition { get; set; }

            private void UpdateRelativePosition()
            {
                var parent = Parent as MultiSplitter;
                if(parent == null)
                    throw new Exception("Invalid parent");

                if(parent.IsHorizontal)
                {
                    RelativePosition = ((float)X)/parent.Width;
                }
                else
                {
                    RelativePosition = ((float)Y)/parent.Height;
                }
                Invalidate();
                InvalidateParent();
            }

            protected override void Render(Skins.Skin skin)
            {
                if (ShouldDrawBackground)
                    skin.DrawButton(this, true, false, IsDisabled);
            }

            protected override void Layout(Skins.Skin skin)
            {
                var parent = Parent as MultiSplitter;
                if(parent == null)
                    throw new Exception("Invalid parent");

                if (parent.IsHorizontal)
                {
                    SetSize(Space,parent.Height);
                    MoveTo(parent.Width*RelativePosition, 0);
                }
                else
                {
                    SetSize(parent.Width, Space);
                    MoveTo(0, parent.Height*RelativePosition);
                }
            }
        }

        private readonly List<Control> panels;
        private readonly List<Splitter> splitters;
        private readonly IReadOnlyList<Control> readOnlyPanels;
        private readonly IReadOnlyList<Splitter> readOnlySplitter;

        public IReadOnlyList<Control> Panels
        {
            get { return readOnlyPanels; }
        }

        public IReadOnlyList<Splitter> Splitters
        {
            get { return readOnlySplitter; }
        }

        public bool IsHorizontal { get; set; }

        protected MultiSplitter(Control parent) : base(parent)
        {
            panels = new List<Control>();
            splitters = new List<Splitter>();
            readOnlyPanels = panels.AsReadOnly();
            readOnlySplitter = splitters.AsReadOnly();
            IsHorizontal = true;
        }

        public void InsertPanel(Control panel)
        {
            InsertPanelAt(panel,panels.Count, splitters.Count);
        }

        public void InsertPanel(Control panel, Control target, bool before)
        {
            var panelIndex = panels.IndexOf(target);
            var rSIndex = panelIndex;
            if (!before)
                 panelIndex++;
               
            InsertPanelAt(panel, panelIndex, rSIndex);
        }

        private void InsertPanelAt(Control panel, int panelIndex, int rSIndex)
        {
            panel.Parent = this;
            panel.IsHidden = false;

            if (panels.Count > 0)
            {
                float right = 1;
                if (rSIndex < splitters.Count)
                    right = splitters[rSIndex].RelativePosition;

                float left = 0;
                if (rSIndex-1 >= 0)
                    left = splitters[rSIndex-1].RelativePosition;

                var newSplitter = new Splitter(this);
                newSplitter.RelativePosition = (right + left)/2;
                splitters.Insert(rSIndex,newSplitter);
            }
            
            panels.Insert(panelIndex,panel);
            Invalidate();
        }

        public void RemovePanel(Control panel)
        {
            if (!panels.Contains(panel)) 
                return;

            var index = panels.IndexOf(panel);
            panels.RemoveAt(index);

            if(panels.Count == 0)
                return;

            var sRIndex = Math.Max(0, Math.Min(index, splitters.Count - 1));
            var sLIndex = Math.Max(index - 1, 0);
            var sRight = splitters[sRIndex];
            var sLeft = splitters[sLIndex];
            sLeft.RelativePosition = (sRight.RelativePosition + sLeft.RelativePosition)/2;
            sLeft.Invalidate();
            splitters.RemoveAt(sRIndex);
            base.RemoveChild(sRight,true);
            panel.IsHidden = true;
        }

        public void ReplacePanel(Control oldPanel, Control newPanel, bool disposeOldPanel)
        {
            var index = panels.IndexOf(oldPanel);
            if (index >= 0)
            {
                newPanel.Parent = this;

                panels.RemoveAt(index);
                panels.Insert(index,newPanel);
                RemoveChild(oldPanel,disposeOldPanel);
                Invalidate();
            }
        }

        public override void Invalidate()
        {
            base.Invalidate();
            foreach (var child in Children)
            {
                child.Invalidate();
            }
        }

        public override void RemoveChild(Control child, bool dispose)
        {
            RemovePanel(child);
            base.RemoveChild(child,dispose);
            Invalidate();
        }

        protected override void Layout(Skins.Skin skin)
        {
            base.Layout(skin);

            //using variables names as if split direction is horizontal
            //but works also for vertical
            int size = (IsHorizontal) ? Width : Height;
            for (var i = 0; i < panels.Count ; i++)
            {
                float right = size;
                if (i < splitters.Count)
                    right = splitters[i].RelativePosition*size;

                float left = 0;
                if (i > 0)
                    left = splitters[i-1].RelativePosition*size + splitters[i-1].Space;

                var panel = panels[i];
                if (IsHorizontal)
                {
                    panel.SetBounds(left, 0, right - left, Height);
                }
                else
                {
                    panel.SetBounds(0, left, Width, right - left);
                }
                panel.Invalidate();
            }
        }
    }
}
