using System;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Docked tab control.
    /// </summary>
    public class DockedTabControl : TabControl
    {
        private readonly TabTitleBar m_TitleBar;

        /// <summary>
        /// Determines whether the title bar is visible.
        /// </summary>
        public bool TitleBarVisible { get { return !m_TitleBar.IsHidden; } set { m_TitleBar.IsHidden = !value; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="DockedTabControl"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public DockedTabControl(Control parent)
            : base(parent)
        {
            Expansion = ExpansionFlags.Expand;

            m_TitleBar = new TabTitleBar(this);
            m_TitleBar.Alignment = AlignmentFlags.Top;
            m_TitleBar.IsHidden = true;
        }

        ///// <summary>
        ///// Lays out the control's interior according to alignment, padding, dock etc.
        ///// </summary>
        ///// <param name="skin">Skin to use.</param>
        //public override void Layout(Skins.Skin skin)
        //{
        //    throw new NotImplementedException();

        //    TabStrip.IsHidden = (TabCount <= 1);
        //    UpdateTitleBar();
        //    base.Layout(skin);
        //}

        private void UpdateTitleBar()
        {
            if (CurrentButton == null)
                return;

            m_TitleBar.UpdateFromTab(CurrentButton);
        }

        public override void DragAndDrop_StartDragging(DragDrop.Package package, int x, int y)
        {
            base.DragAndDrop_StartDragging(package, x, y);

            IsHidden = true;
            // This hiding our parent thing is kind of lousy.
            Parent.IsHidden = true;
        }

        public override void DragAndDrop_EndDragging(bool success, int x, int y)
        {
            IsHidden = false;
            if (!success)
            {
                Parent.IsHidden = false;
            }
        }

        public void MoveTabsTo(DockedTabControl target)
        {
            throw new NotImplementedException();

            //var children = TabStrip.Children.ToArray(); // copy because collection will be modified
            //foreach (Control child in children)
            //{
            //    TabButton button = child as TabButton;
            //    if (button == null)
            //        continue;
            //    target.AddPage(button);
            //}
            //Invalidate();
        }
    }
}
