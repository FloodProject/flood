using System;
using System.Linq;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Base for controls whose interior can be scrolled.
    /// </summary>
    public class ScrollControl : Control
    {
        private bool m_CanScrollH;
        private bool m_CanScrollV;
        private bool m_AutoHideBars;

        private readonly ScrollBar m_VerticalScrollBar;
        private readonly ScrollBar m_HorizontalScrollBar;

        /// <summary>
        /// Indicates whether the control can be scrolled horizontally.
        /// </summary>
        public bool CanScrollH { get { return m_CanScrollH; } }

        /// <summary>
        /// Indicates whether the control can be scrolled vertically.
        /// </summary>
        public bool CanScrollV { get { return m_CanScrollV; } }

        /// <summary>
        /// Determines whether the scroll bars should be hidden if not needed.
        /// </summary>
        public bool AutoHideBars { get { return m_AutoHideBars; } set { m_AutoHideBars = value; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="ScrollControl"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ScrollControl(Control parent)
        {
            throw new NotImplementedException();
            //MouseInputEnabled = false;

            //m_VerticalScrollBar = new VerticalScrollBar(this);
            //m_VerticalScrollBar.Dock = Pos.Right;
            //m_VerticalScrollBar.BarMoved += VBarMoved;
            //m_CanScrollV = true;
            //m_VerticalScrollBar.NudgeAmount = 30;

            //m_HorizontalScrollBar = new HorizontalScrollBar(this);
            //m_HorizontalScrollBar.Dock = Pos.Bottom;
            //m_HorizontalScrollBar.BarMoved += HBarMoved;
            //m_CanScrollH = true;
            //m_HorizontalScrollBar.NudgeAmount = 30;

            //m_InnerPanel = new Control(this);
            //m_InnerPanel.SetPosition(0, 0);
            //m_InnerPanel.Margin = Margin.Five;
            //m_InnerPanel.SendToBack();
            //m_InnerPanel.MouseInputEnabled = false;

            //m_AutoHideBars = false;
        }

        protected bool HScrollRequired
        {
            set
            {
                if (value)
                {
                    m_HorizontalScrollBar.SetScrollAmount(0, true);
                    m_HorizontalScrollBar.IsDisabled = true;
                    if (m_AutoHideBars)
                        m_HorizontalScrollBar.IsHidden = true;
                }
                else
                {
                    m_HorizontalScrollBar.IsHidden = false;
                    m_HorizontalScrollBar.IsDisabled = false;
                }
            }
        }

        protected bool VScrollRequired
        {
            set
            {
                if (value)
                {
                    m_VerticalScrollBar.SetScrollAmount(0, true);
                    m_VerticalScrollBar.IsDisabled = true;
                    if (m_AutoHideBars)
                        m_VerticalScrollBar.IsHidden = true;
                }
                else
                {
                    m_VerticalScrollBar.IsHidden = false;
                    m_VerticalScrollBar.IsDisabled = false;
                }
            }
        }

        /// <summary>
        /// Enables or disables inner scrollbars.
        /// </summary>
        /// <param name="horizontal">Determines whether the horizontal scrollbar should be enabled.</param>
        /// <param name="vertical">Determines whether the vertical scrollbar should be enabled.</param>
        public virtual void EnableScroll(bool horizontal, bool vertical)
        {
            m_CanScrollV = vertical;
            m_CanScrollH = horizontal;
            m_VerticalScrollBar.IsHidden = !m_CanScrollV;
            m_HorizontalScrollBar.IsHidden = !m_CanScrollH;
        }

        public virtual void SetInnerSize(int width, int height)
        {
            throw new NotImplementedException();
            //m_InnerPanel.SetSize(width, height);
        }

        protected virtual void VBarMoved(Control control)
        {
            Invalidate();
        }

        protected virtual void HBarMoved(Control control)
        {
            Invalidate();
        }

        /// <summary>
        /// Handler invoked when control children's bounds change.
        /// </summary>
        /// <param name="oldChildBounds"></param>
        /// <param name="child"></param>
        public override void OnChildBoundsChanged(Rectangle oldChildBounds, IControl child)
        {
            throw new NotImplementedException();

            UpdateScrollBars();
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Layout(Skins.Skin skin)
        {
            throw new NotImplementedException();

            UpdateScrollBars();
            base.Layout(skin);
        }

        public override bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Handler invoked on mouse wheel event.
        /// </summary>
        /// <param name="delta">Scroll delta.</param>
        /// <returns></returns>
        public override bool OnMouseWheeled(int delta)
        {
            throw new NotImplementedException();

            if (CanScrollV && m_VerticalScrollBar.IsVisible)
            {
                if (m_VerticalScrollBar.SetScrollAmount(
                    m_VerticalScrollBar.ScrollAmount - m_VerticalScrollBar.NudgeAmount * (delta / 60.0f), true))
                    return true;
            }

            if (CanScrollH && m_HorizontalScrollBar.IsVisible)
            {
                if (m_HorizontalScrollBar.SetScrollAmount(
                    m_HorizontalScrollBar.ScrollAmount - m_HorizontalScrollBar.NudgeAmount * (delta / 60.0f), true))
                    return true;
            }

            return false;
        }

        public virtual void UpdateScrollBars()
        {
            throw new NotImplementedException();
            //if (null == m_InnerPanel)
            //    return;

            ////Get the max size of all our children together
            //int childrenWidth = Children.Count > 0 ? Children.Max(x => x.Right) : 0;
            //int childrenHeight = Children.Count > 0 ? Children.Max(x => x.Bottom) : 0;

            //if (m_CanScrollH)
            //{
            //    m_InnerPanel.SetSize(Math.Max(Width, childrenWidth), Math.Max(Height, childrenHeight));  
            //}
            //else
            //{
            //    m_InnerPanel.SetSize(Width - (m_VerticalScrollBar.IsHidden ? 0 : m_VerticalScrollBar.Width),
            //                         Math.Max(Height, childrenHeight));
            //}

            //float wPercent = Width/
            //                 (float) (childrenWidth + (m_VerticalScrollBar.IsHidden ? 0 : m_VerticalScrollBar.Width));
            //float hPercent = Height/
            //                 (float)
            //                 (childrenHeight + (m_HorizontalScrollBar.IsHidden ? 0 : m_HorizontalScrollBar.Height));

            //if (m_CanScrollV)
            //    VScrollRequired = hPercent >= 1;
            //else
            //    m_VerticalScrollBar.IsHidden = true;

            //if (m_CanScrollH)
            //    HScrollRequired = wPercent >= 1;
            //else
            //    m_HorizontalScrollBar.IsHidden = true;


            //m_VerticalScrollBar.ContentSize = m_InnerPanel.Height;
            //m_VerticalScrollBar.ViewableContentSize = Height - (m_HorizontalScrollBar.IsHidden ? 0 : m_HorizontalScrollBar.Height);


            //m_HorizontalScrollBar.ContentSize = m_InnerPanel.Width;
            //m_HorizontalScrollBar.ViewableContentSize = Width - (m_VerticalScrollBar.IsHidden ? 0 : m_VerticalScrollBar.Width);

            //int newInnerPanelPosX = 0;
            //int newInnerPanelPosY = 0;

            //if (CanScrollV && !m_VerticalScrollBar.IsHidden)
            //{
            //    newInnerPanelPosY =
            //        (int)(
            //            -((m_InnerPanel.Height) - Height + (m_HorizontalScrollBar.IsHidden ? 0 : m_HorizontalScrollBar.Height))*
            //            m_VerticalScrollBar.ScrollAmount);
            //}
            //if (CanScrollH && !m_HorizontalScrollBar.IsHidden)
            //{
            //    newInnerPanelPosX =
            //        (int)(
            //            -((m_InnerPanel.Width) - Width + (m_VerticalScrollBar.IsHidden ? 0 : m_VerticalScrollBar.Width))*
            //            m_HorizontalScrollBar.ScrollAmount);
            //}

            //m_InnerPanel.SetPosition(newInnerPanelPosX, newInnerPanelPosY);
        }

        public virtual void ScrollToBottom()
        {
            if (!CanScrollV)
                return;

            UpdateScrollBars();
            m_VerticalScrollBar.ScrollToBottom();
        }

        public virtual void ScrollToTop()
        {
            if (CanScrollV)
            {
                UpdateScrollBars();
                m_VerticalScrollBar.ScrollToTop();
            }
        }

        public virtual void ScrollToLeft()
        {
            if (CanScrollH)
            {
                UpdateScrollBars();
                m_VerticalScrollBar.ScrollToLeft();
            }
        }

        public virtual void ScrollToRight()
        {
            if (CanScrollH)
            {
                UpdateScrollBars();
                m_VerticalScrollBar.ScrollToRight();
            }
        }

        public virtual void DeleteAll()
        {
            throw new NotImplementedException();
            //m_InnerPanel.DeleteAllChildren();
        }
    }
}
