﻿using System;
using System.Windows.Forms;
using Gwen.ControlInternal;

namespace Gwen.Control
{
    /// <summary>
    /// Splitter control.
    /// </summary>
    public class CrossSplitter : Base
    {
        private readonly SplitterBar m_VSplitter;
        private readonly SplitterBar m_HSplitter;
        private readonly SplitterBar m_CSplitter;

        private readonly Base[] m_Sections;

        private float m_HVal; // 0-1
        private float m_VVal; // 0-1
        private int m_BarSize; // pixels

        private int m_ZoomedSection; // 0-3

        /// <summary>
        /// Invoked when one of the panels has been zoomed (maximized).
        /// </summary>
        public event GwenEventHandler PanelZoomed;

        /// <summary>
        /// Invoked when one of the panels has been unzoomed (restored).
        /// </summary>
        public event GwenEventHandler PanelUnZoomed;

        /// <summary>
        /// Invoked when the zoomed panel has been changed.
        /// </summary>
        public event GwenEventHandler ZoomChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="CrossSplitter"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public CrossSplitter(Base parent)
            : base(parent)
        {
            m_Sections = new Base[4];

            m_VSplitter = new SplitterBar(this);
            m_VSplitter.SetPosition(0, 128);
            m_VSplitter.Dragged += OnVerticalMoved;
            m_VSplitter.Cursor = Cursors.SizeNS;

            m_HSplitter = new SplitterBar(this);
            m_HSplitter.SetPosition(128, 0);
            m_HSplitter.Dragged += OnHorizontalMoved;
            m_HSplitter.Cursor = Cursors.SizeWE;

            m_CSplitter = new SplitterBar(this);
            m_CSplitter.SetPosition(128, 128);
            m_CSplitter.Dragged += OnCenterMoved;
            m_CSplitter.Cursor = Cursors.SizeAll;

            m_HVal = 0.5f;
            m_VVal = 0.5f;

            SetPanel(0, null);
            SetPanel(1, null);
            SetPanel(2, null);
            SetPanel(3, null);

            SplitterSize = 5;
            SplittersVisible = false;

            m_ZoomedSection = -1;
        }

        /// <summary>
        /// Centers the panels so that they take even amount of space.
        /// </summary>
        public void CenterPanels()
        {
            m_HVal = 0.5f;
            m_VVal = 0.5f;
            Invalidate();
        }

        /// <summary>
        /// Indicates whether any of the panels is zoomed.
        /// </summary>
        public bool IsZoomed { get { return m_ZoomedSection != -1; } }

        /// <summary>
        /// Gets or sets a value indicating whether splitters should be visible.
        /// </summary>
        public bool SplittersVisible
        {
            get { return m_CSplitter.ShouldDrawBackground; }
            set 
            {
                m_CSplitter.ShouldDrawBackground = value;
                m_VSplitter.ShouldDrawBackground = value;
                m_HSplitter.ShouldDrawBackground = value;
            }
        }

        /// <summary>
        /// Gets or sets the size of the splitter.
        /// </summary>
        public int SplitterSize { get { return m_BarSize; } set { m_BarSize = value; } }

        private void UpdateVSplitter()
        {
            m_VSplitter.MoveTo(m_VSplitter.X, (Height - m_VSplitter.Height) * (m_VVal));
        }

        private void UpdateHSplitter()
        {
            m_HSplitter.MoveTo( ( Width - m_HSplitter.Width ) * ( m_HVal ), m_HSplitter.Y );
        }

        private void UpdateCSplitter()
        {
            m_CSplitter.MoveTo((Width - m_CSplitter.Width) * (m_HVal), (Height - m_CSplitter.Height) * (m_VVal));
        }

        protected void OnCenterMoved(Base control)
        {
            CalculateValueCenter();
            Invalidate();
        }

        protected void OnVerticalMoved(Base control)
        {
            m_VVal = CalculateValueVertical();
            Invalidate();
        }

        protected void OnHorizontalMoved(Base control)
        {
            m_HVal = CalculateValueHorizontal();
            Invalidate();
        }

        private void CalculateValueCenter()
        {
            m_HVal = m_CSplitter.X / (float)(Width - m_CSplitter.Width);
            m_VVal = m_CSplitter.Y / (float)(Height - m_CSplitter.Height);
        }

        private float CalculateValueVertical()
        {
            return m_VSplitter.Y / (float)(Height - m_VSplitter.Height);
        }

        private float CalculateValueHorizontal()
        {
            return m_HSplitter.X / (float)(Width - m_HSplitter.Width);
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skin.Base skin)
        {
            m_VSplitter.SetSize(Width, m_BarSize);
            m_HSplitter.SetSize(m_BarSize, Height);
            m_CSplitter.SetSize(m_BarSize, m_BarSize);

            UpdateVSplitter();
            UpdateHSplitter();
            UpdateCSplitter();

            if (m_ZoomedSection == -1)
            {
                if (m_Sections[0] != null)
                    m_Sections[0].SetBounds(0, 0, m_HSplitter.X, m_VSplitter.Y);

                if (m_Sections[1] != null)
                    m_Sections[1].SetBounds(m_HSplitter.X + m_BarSize, 0, Width - (m_HSplitter.X + m_BarSize), m_VSplitter.Y);

                if (m_Sections[2] != null)
                    m_Sections[2].SetBounds(0, m_VSplitter.Y + m_BarSize, m_HSplitter.X, Height - (m_VSplitter.Y + m_BarSize));

                if (m_Sections[3] != null)
                    m_Sections[3].SetBounds(m_HSplitter.X + m_BarSize, m_VSplitter.Y + m_BarSize, Width - (m_HSplitter.X + m_BarSize), Height - (m_VSplitter.Y + m_BarSize));
            }
            else
            {
                //This should probably use Fill docking instead
                m_Sections[m_ZoomedSection].SetBounds(0, 0, Width, Height);
            }
        }

        /// <summary>
        /// Assigns a control to the specific inner section.
        /// </summary>
        /// <param name="index">Section index (0-3).</param>
        /// <param name="panel">Control to assign.</param>
        public void SetPanel(int index, Base panel)
        {
            m_Sections[index] = panel;

            if (panel != null)
            {
                panel.Dock = Pos.None;
                panel.Parent = this;
            }

            Invalidate();
        }

        /// <summary>
        /// Gets the specific inner section.
        /// </summary>
        /// <param name="index">Section index (0-3).</param>
        /// <returns>Specified section.</returns>
        public Base GetPanel(int index)
        {
            return m_Sections[index];
        }

        /// <summary>
        /// Internal handler for the zoom changed event.
        /// </summary>
        protected void OnZoomChanged()
        {
            if (ZoomChanged != null)
                ZoomChanged.Invoke(this);
         
            if (m_ZoomedSection == -1)
            {
                if (PanelUnZoomed != null)
                    PanelUnZoomed.Invoke(this);
            }
            else
            {
                if (PanelZoomed != null)
                    PanelZoomed.Invoke(this);
            }
        }

        /// <summary>
        /// Maximizes the specified panel so it fills the entire control.
        /// </summary>
        /// <param name="section">Panel index (0-3).</param>
        public void Zoom(int section)
        {
            UnZoom();

            if (m_Sections[section] != null)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (i != section && m_Sections[i] != null)
                        m_Sections[i].IsHidden = true;
                }
                m_ZoomedSection = section;

                Invalidate();
            }
            OnZoomChanged();
        }

        /// <summary>
        /// Restores the control so all panels are visible.
        /// </summary>
        public void UnZoom()
        {
            m_ZoomedSection = -1;

            for (int i = 0; i < 4; i++)
            {
                if (m_Sections[i] != null)
                    m_Sections[i].IsHidden = false;
            }

            Invalidate();
            OnZoomChanged();
        }
    }
}
