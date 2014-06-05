using System;
using Flood.GUIv2.ControlInternal;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Base class for scrollbars.
    /// </summary>
    public class ScrollBar : Control
    {
        protected readonly ScrollBarButton[] m_ScrollButton;
        protected readonly ScrollBarBar m_Bar;

        protected bool m_Depressed;
        protected float m_ScrollAmount;
        protected float m_ContentSize;
        protected float m_ViewableContentSize;
        protected float m_NudgeAmount;

        /// <summary>
        /// Invoked when the bar is moved.
        /// </summary>
        public event GwenEventHandler BarMoved;

        /// <summary>
        /// Bar size (in pixels).
        /// </summary>
        public virtual int BarSize { get; set; }

        /// <summary>
        /// Bar position (in pixels).
        /// </summary>
        public virtual int BarPos { get { return 0; } }
        
        /// <summary>
        /// Button size (in pixels).
        /// </summary>
        public virtual int ButtonSize { get { return 0; } }

        public virtual float NudgeAmount { get { return m_NudgeAmount / m_ContentSize; } set { m_NudgeAmount = value; } }
        public float ScrollAmount { get { return m_ScrollAmount; } }
        public float ContentSize
        {
            get { return m_ContentSize; }
            set
            {
                //todo: uncomment this when this becomes a composite control

                //if (m_ContentSize != value) 
                //    Invalidate();
                //m_ContentSize = value;
            }
        }
        public float ViewableContentSize
        {
            get { return m_ViewableContentSize; }
            set
            {
                //todo: uncomment this when this becomes a composite control

                //if (m_ViewableContentSize != value) 
                //    Invalidate();
                //m_ViewableContentSize = value;
            }
        }

        /// <summary>
        /// Indicates whether the bar is horizontal.
        /// </summary>
        public virtual bool IsHorizontal { get { return false; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="ScrollBar"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        protected ScrollBar(Control parent)
        {
            m_ScrollButton = new ScrollBarButton[2];
            m_ScrollButton[0] = new ScrollBarButton(this);
            m_ScrollButton[1] = new ScrollBarButton(this);

            m_Bar = new ScrollBarBar(this);

            SetBounds(0, 0, 15, 15);
            m_Depressed = false;

            m_ScrollAmount = 0;
            m_ContentSize = 0;
            m_ViewableContentSize = 0;

            NudgeAmount = 20;
        }

        /// <summary>
        /// Sets the scroll amount (0-1).
        /// </summary>
        /// <param name="value">Scroll amount.</param>
        /// <param name="forceUpdate">Determines whether the control should be updated.</param>
        /// <returns>True if control state changed.</returns>
        public virtual bool SetScrollAmount(float value, bool forceUpdate = false)
        {
            throw new NotImplementedException();
            //todo: uncomment this when this becomes a composite control
            //if (m_ScrollAmount == value && !forceUpdate)
            //    return false;
            //m_ScrollAmount = value;
            //Invalidate();
            //OnBarMoved(this);
            //return true;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            throw new NotImplementedException();

            skin.DrawScrollBar(this, IsHorizontal, m_Depressed);
        }

        /// <summary>
        /// Handler for the BarMoved event.
        /// </summary>
        /// <param name="control">The control.</param>
        protected virtual void OnBarMoved(IControl control)
        {
            if (BarMoved != null)
                BarMoved.Invoke(this);
        }

        protected virtual float CalculateScrolledAmount()
        {
            return 0;
        }

        protected virtual int CalculateBarSize()
        {
            return 0;
        } 

        public virtual void ScrollToLeft() { } 
        public virtual void ScrollToRight() { }
        public virtual void ScrollToTop() { }
        public virtual void ScrollToBottom() { }
    }
}
