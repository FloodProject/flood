using System;
using Flood.GUIv2.ControlInternal;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Base resizable control.
    /// </summary>
    public class ResizableControl : Control
    {
        private bool m_ClampMovement;
        private readonly Resizer[] m_Resizer;

        /// <summary>
        /// Determines whether control's position should be restricted to its parent bounds.
        /// </summary>
        public bool ClampMovement { get { return m_ClampMovement; } set { m_ClampMovement = value; } }

        /// <summary>
        /// Invoked when the control has been resized.
        /// </summary>
        public event GwenEventHandler Resized;

        /// <summary>
        /// Initializes a new instance of the <see cref="ResizableControl"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ResizableControl(Control parent)
        {
            SetSize(100, 30);
            m_Resizer = new Resizer[10];
            MinimumSize = new Vector2i(5, 5);
            m_ClampMovement = false;

            m_Resizer[2] = new Resizer(this);
            m_Resizer[2].Alignment = AlignmentFlags.Bottom;
            m_Resizer[2].ResizeDir = AlignmentFlags.Bottom;
            m_Resizer[2].Resized += OnResized;
            m_Resizer[2].Target = this;

            m_Resizer[1] = new Resizer(m_Resizer[2]);
            m_Resizer[1].Alignment = AlignmentFlags.Left;
            m_Resizer[1].ResizeDir = AlignmentFlags.Bottom | AlignmentFlags.Left;
            m_Resizer[1].Resized += OnResized;
            m_Resizer[1].Target = this;

            m_Resizer[3] = new Resizer(m_Resizer[2]);
            m_Resizer[3].Alignment = AlignmentFlags.Right;
            m_Resizer[3].ResizeDir = AlignmentFlags.Bottom | AlignmentFlags.Right;
            m_Resizer[3].Resized += OnResized;
            m_Resizer[3].Target = this;

            m_Resizer[8] = new Resizer(this);
            m_Resizer[8].Alignment = AlignmentFlags.Top;
            m_Resizer[8].ResizeDir = AlignmentFlags.Top;
            m_Resizer[8].Resized += OnResized;
            m_Resizer[8].Target = this;

            m_Resizer[7] = new Resizer(m_Resizer[8]);
            m_Resizer[7].Alignment = AlignmentFlags.Left;
            m_Resizer[7].ResizeDir = AlignmentFlags.Left | AlignmentFlags.Top;
            m_Resizer[7].Resized += OnResized;
            m_Resizer[7].Target = this;

            m_Resizer[9] = new Resizer(m_Resizer[8]);
            m_Resizer[9].Alignment = AlignmentFlags.Right;
            m_Resizer[9].ResizeDir = AlignmentFlags.Right | AlignmentFlags.Top;
            m_Resizer[9].Resized += OnResized;
            m_Resizer[9].Target = this;

            m_Resizer[4] = new Resizer(this);
            m_Resizer[4].Alignment = AlignmentFlags.Left;
            m_Resizer[4].ResizeDir = AlignmentFlags.Left;
            m_Resizer[4].Resized += OnResized;
            m_Resizer[4].Target = this;

            m_Resizer[6] = new Resizer(this);
            m_Resizer[6].Alignment = AlignmentFlags.Right;
            m_Resizer[6].ResizeDir = AlignmentFlags.Right;
            m_Resizer[6].Resized += OnResized;
            m_Resizer[6].Target = this;
        }

        /// <summary>
        /// Handler for the resized event.
        /// </summary>
        /// <param name="control">Event source.</param>
        protected virtual void OnResized(IControl control)
        {
            if (Resized != null)
                Resized.Invoke(this);
        }

        protected Resizer GetResizer(int i)
        {
            return m_Resizer[i];
        }

        /// <summary>
        /// Disables resizing.
        /// </summary>
        public void DisableResizing()
        {
            for (int i = 0; i < 10; i++)
            {
                if (m_Resizer[i] == null)
                    continue;
                m_Resizer[i].MouseInputEnabled = false;
                m_Resizer[i].IsHidden = true;
                Margin = new Margin(m_Resizer[i].Width, m_Resizer[i].Width, m_Resizer[i].Width, m_Resizer[i].Width);
            }
        }

        /// <summary>
        /// Enables resizing.
        /// </summary>
        public void EnableResizing()
        {
            for (int i = 0; i < 10; i++)
            {
                if (m_Resizer[i] == null)
                    continue;
                m_Resizer[i].MouseInputEnabled = true;
                m_Resizer[i].IsHidden = false;
                Margin = Margin.Zero; // todo: check if ok
            }
        }

        /// <summary>
        /// Sets the control bounds.
        /// </summary>
        /// <param name="x">X position.</param>
        /// <param name="y">Y position.</param>
        /// <param name="width">Width.</param>
        /// <param name="height">Height.</param>
        /// <returns>
        /// True if bounds changed.
        /// </returns>
        public override bool SetBounds(int x, int y, int width, int height)
        {
            throw new NotImplementedException();

            var minSize = MinimumSize;
            // Clamp Minimum Size
            if (width < minSize.X) width = minSize.X;
            if (height < minSize.Y) height = minSize.Y;

            // Clamp to parent's window
            IControl parent = Parent;
            if (parent != null && m_ClampMovement)
            {
                if (x + width > parent.Width) x = parent.Width - width;
                if (x < 0) x = 0;
                if (y + height > parent.Height) y = parent.Height - height;
                if (y < 0) y = 0;
            }

            return base.SetBounds(x, y, width, height);
        }

        public override bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            throw new System.NotImplementedException();
        }
    }
}
