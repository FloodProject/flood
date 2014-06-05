using Flood.GUIv2.ControlInternal;
using Flood.GUIv2.DragDrop;
using System;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Tab strip - groups TabButtons and allows reordering.
    /// </summary>
    public class TabStrip : Control
    {
        private Control m_TabDragControl;
        private bool m_AllowReorder;

        /// <summary>
        /// Determines whether it is possible to reorder tabs by mouse dragging.
        /// </summary>
        public bool AllowReorder { get { return m_AllowReorder; } set { m_AllowReorder = value; } }

        /// <summary>
        /// Determines whether the control should be clipped to its bounds while rendering.
        /// </summary>
        public override bool ShouldClip
        {
            get { return false; }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="TabStrip"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public TabStrip(Control parent)
        {
            throw new NotImplementedException();

            m_AllowReorder = false;
        }

        /// <summary>
        /// Strip position (top/left/right/bottom).
        /// </summary>
        public AlignmentFlags StripPosition
        {
            get { return Alignment; }
            set
            {
                Alignment = value;
                if (Alignment == AlignmentFlags.Top)
                    Margin = new Margin(5, 0, 0, 0);
                if (Alignment == AlignmentFlags.Left)
                    Margin = new Margin(0, 5, 0, 0);
                if (Alignment == AlignmentFlags.Bottom)
                    Margin = new Margin(5, 0, 0, 0);
                if (Alignment == AlignmentFlags.Right)
                    Margin = new Margin(0, 5, 0, 0);
            }
        }

        public override bool DragAndDrop_HandleDrop(Package p, int x, int y)
        {
            Vector2i LocalPos = CanvasPosToLocal(new Vector2i(x, y));

            TabButton button = DragAndDrop.SourceControl as TabButton;
            TabControl tabControl = Parent as TabControl;
            if (tabControl != null && button != null)
            {
                if (button.TabControl != tabControl)
                {
                    // We've moved tab controls!
                    tabControl.AddPage(button);
                }
            }

            var droppedOn = GetControlAt(LocalPos.X, LocalPos.Y);
            if (droppedOn != null && droppedOn != this)
            {
                Vector2i dropPos = droppedOn.CanvasPosToLocal(new Vector2i(x, y));
                DragAndDrop.SourceControl.BringNextToControl(droppedOn, dropPos.X > droppedOn.Width/2);
            }
            else
            {
                DragAndDrop.SourceControl.BringToFront();
            }
            return true;
        }

        public override bool DragAndDrop_CanAcceptPackage(DragDrop.Package p)
        {
            if (!m_AllowReorder)
                return false;

            if (p.Name == "TabButtonMove")
                return true;

            return false;
        }

        ///// <summary>
        ///// Lays out the control's interior according to alignment, padding, dock etc.
        ///// </summary>
        ///// <param name="skin">Skin to use.</param>
        //public override void Layout(Skins.Skin skin)
        //{
        //    throw new NotImplementedException();

        //    //Vector2i largestTab = new Vector2i(5, 5);

        //    //int num = 0;
        //    //foreach (var child in Children)
        //    //{
        //    //    TabButton button = child as TabButton;
        //    //    if (null == button) continue;

        //    //    button.SizeToContents();

        //    //    Margin m = new Margin();
        //    //    int notFirst = num > 0 ? -1 : 0;

        //    //    if (Dock == Pos.Top)
        //    //    {
        //    //        m.Left = notFirst;
        //    //        button.Dock = Pos.Left;
        //    //    }

        //    //    if (Dock == Pos.Left)
        //    //    {
        //    //        m.Top = notFirst;
        //    //        button.Dock = Pos.Top;
        //    //    }

        //    //    if (Dock == Pos.Right)
        //    //    {
        //    //        m.Top = notFirst;
        //    //        button.Dock = Pos.Top;
        //    //    }

        //    //    if (Dock == Pos.Bottom)
        //    //    {
        //    //        m.Left = notFirst;
        //    //        button.Dock = Pos.Left;
        //    //    }

        //    //    largestTab.X = Math.Max(largestTab.X, button.Width);
        //    //    largestTab.Y = Math.Max(largestTab.Y, button.Height);

        //    //    button.Margin = m;
        //    //    num++;
        //    //}

        //    //if (Dock == Pos.Top || Dock == Pos.Bottom)
        //    //    SetSize(Width, largestTab.Y);

        //    //if (Dock == Pos.Left || Dock == Pos.Right)
        //    //    SetSize(largestTab.X, Height);

        //    //base.Layout(skin);
        //}

        public override void DragAndDrop_HoverEnter(DragDrop.Package p, int x, int y)
        {
            if (m_TabDragControl != null)
            {
                throw new InvalidOperationException("ERROR! TabStrip::DragAndDrop_HoverEnter");
            }

            m_TabDragControl = new Highlight(this);
            m_TabDragControl.MouseInputEnabled = false;
            m_TabDragControl.SetSize(3, Height);
        }

        public override void DragAndDrop_HoverLeave(DragDrop.Package p)
        {
            throw new NotImplementedException();
            //if (m_TabDragControl != null)
            //{
            //    RemoveChild(m_TabDragControl, false); // [omeg] need to do that explicitely
            //    m_TabDragControl.Dispose();
            //}
            //m_TabDragControl = null;
        }

        public override void DragAndDrop_Hover(DragDrop.Package p, int x, int y)
        {
            Vector2i localPos = CanvasPosToLocal(new Vector2i(x, y));

            IControl droppedOn = GetControlAt(localPos.X, localPos.Y);
            if (droppedOn != null && droppedOn != this)
            {
                Vector2i dropPos = droppedOn.CanvasPosToLocal(new Vector2i(x, y));
                m_TabDragControl.SetBounds(new Rectangle(0, 0, 3, Height));
                m_TabDragControl.BringToFront();
                m_TabDragControl.SetPosition(droppedOn.X - 1, 0);

                if (dropPos.X > droppedOn.Width/2)
                {
                    m_TabDragControl.MoveBy(droppedOn.Width - 1, 0);
                }
                m_TabDragControl.Alignment = AlignmentFlags.NotSet;
            }
            else
            {
                m_TabDragControl.Alignment = AlignmentFlags.Left;
                m_TabDragControl.BringToFront();
            }
        }
    }
}
