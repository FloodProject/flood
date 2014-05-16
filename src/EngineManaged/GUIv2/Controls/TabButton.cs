using System;
using Flood.GUIv2.Input;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Tab header.
    /// </summary>
    public class TabButton : Button
    {
        private Control m_Page;
        private TabControl m_Control;

        /// <summary>
        /// Indicates whether the tab is active.
        /// </summary>
        public bool IsActive { get { return m_Page != null && m_Page.IsVisible; } }

        // todo: remove public access
        public TabControl TabControl
        {
            get { return m_Control; }
            set
            {
                if (value == m_Control) return;
                if (m_Control != null)
                    m_Control.OnLoseTab(this);
                m_Control = value;
            }
        }

        /// <summary>
        /// Interior of the tab.
        /// </summary>
        public Control Page { get { return m_Page; } set { m_Page = value; } }

        /// <summary>
        /// Determines whether the control should be clipped to its bounds while rendering.
        /// </summary>
        public override bool ShouldClip
        {
            get { return false; }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="TabButton"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public TabButton(Control parent)
            : base(parent)
        {
            DragAndDrop_SetPackage(true, "TabButtonMove");
            TextAlignment = AlignmentFlags.Top | AlignmentFlags.Left;
            TextMargin = new Margin(5, 3, 3, 3);
            Margin = Margin.Two;
            KeyboardInputEnabled = true;
        }

        public override void DragAndDrop_StartDragging(DragDrop.Package package, int x, int y)
        {
            throw new NotImplementedException();

            IsHidden = true;
        }

        public override void DragAndDrop_EndDragging(bool success, int x, int y)
        {
            throw new NotImplementedException();

            IsHidden = false;
            IsDepressed = false;
        }

        public override bool DragAndDrop_ShouldStartDrag()
        {
            throw new NotImplementedException();

            return m_Control.AllowReorder;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            throw new NotImplementedException();

            skin.DrawTabButton(this, IsActive, m_Control.TabStrip.Alignment);
        }

        /// <summary>
        /// Handler for Down Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        public override bool OnKeyDown(bool down)
        {
            throw new NotImplementedException();

            OnKeyRight(down);
            return true;
        }

        /// <summary>
        /// Handler for Up Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        public override bool OnKeyUp(bool down)
        {
            throw new NotImplementedException();

            OnKeyLeft(down);
            return true;
        }

        /// <summary>
        /// Handler for Right Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        public override bool OnKeyRight(bool down)
        {
            throw new NotImplementedException();

            //if (down)
            //{
            //    var count = Parent.Children.Count;
            //    int me = Parent.Children.IndexOf(this);
            //    if (me + 1 < count)
            //    {
            //        var nextTab = Parent.Children[me + 1];
            //        TabControl.OnTabPressed(nextTab);
            //        InputHandler.KeyboardFocus = nextTab;
            //    }
            //}

            //return true;
        }

        /// <summary>
        /// Handler for Left Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        public override bool OnKeyLeft(bool down)
        {
            throw new NotImplementedException();

            //if (down)
            //{
            //    var count = Parent.Children.Count;
            //    int me = Parent.Children.IndexOf(this);
            //    if (me - 1 >= 0)
            //    {
            //        var prevTab = Parent.Children[me - 1];
            //        TabControl.OnTabPressed(prevTab);
            //        InputHandler.KeyboardFocus = prevTab;
            //    }
            //}

            //return true;
        }

        /// <summary>
        /// Updates control colors.
        /// </summary>
        public override void UpdateColors()
        {
            if (IsActive)
            {
                if (IsDisabled)
                {
                    TextColor = Skin.Colors.Tab.Active.Disabled;
                    return;
                }
                if (IsDepressed)
                {
                    TextColor = Skin.Colors.Tab.Active.Down;
                    return;
                }
                if (IsHovered)
                {
                    TextColor = Skin.Colors.Tab.Active.Hover;
                    return;
                }

                TextColor = Skin.Colors.Tab.Active.Normal;
            }

            if (IsDisabled)
            {
                TextColor = Skin.Colors.Tab.Inactive.Disabled;
                return;
            }
            if (IsDepressed)
            {
                TextColor = Skin.Colors.Tab.Inactive.Down;
                return;
            }
            if (IsHovered)
            {
                TextColor = Skin.Colors.Tab.Inactive.Hover;
                return;
            }

            TextColor = Skin.Colors.Tab.Inactive.Normal;
        }
    }
}
