﻿using System;
using Gwen.Control.Layout;
using Gwen.ControlInternal;

namespace Gwen.Control
{
    /// <summary>
    /// Numeric up/down.
    /// </summary>
    public class NumericUpDown : TextBoxNumeric
    {
        private int m_Max;
        private int m_Min;

        private readonly Splitter m_Splitter;
        private readonly UpDownButton_Up m_Up;
        private readonly UpDownButton_Down m_Down;

        /// <summary>
        /// Minimum value.
        /// </summary>
        public int Min { get { return m_Min; } set { m_Min = value; } }

        /// <summary>
        /// Maximum value.
        /// </summary>
        public int Max { get { return m_Max; } set { m_Max = value; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="NumericUpDown"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public NumericUpDown(Base parent)
            : base(parent)
        {
            SetSize(100, 20);

            m_Splitter = new Splitter(this);
            m_Splitter.Dock = Pos.Right;
            m_Splitter.SetSize(13, 13);

            m_Up = new UpDownButton_Up(m_Splitter);
            m_Up.Clicked += OnButtonUp;
            m_Up.IsTabable = false;
            m_Splitter.SetPanel(0, m_Up, false);

            m_Down = new UpDownButton_Down(m_Splitter);
            m_Down.Clicked += OnButtonDown;
            m_Down.IsTabable = false;
            m_Down.Padding = new Padding(0, 1, 1, 0);
            m_Splitter.SetPanel(1, m_Down, false);

            m_Max = 100;
            m_Min = 0;
            m_Value = 0f;
            Text = "0";
        }

        /// <summary>
        /// Invoked when the value has been changed.
        /// </summary>
        public event GwenEventHandler ValueChanged;

        /// <summary>
        /// Handler for Up Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyUp(bool down)
        {
            if (down) OnButtonUp(null);
            return true;
        }

        /// <summary>
        /// Handler for Down Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyDown(bool down)
        {
            if (down) OnButtonDown(null);
            return true;
        }

        /// <summary>
        /// Handler for the button up event.
        /// </summary>
        /// <param name="control">Event source.</param>
        protected virtual void OnButtonUp(Base control)
        {
            Value = m_Value + 1;
        }

        /// <summary>
        /// Handler for the button down event.
        /// </summary>
        /// <param name="control">Event source.</param>
        protected virtual void OnButtonDown(Base control)
        {
            Value = m_Value - 1;
        }

        /// <summary>
        /// Determines whether the text can be assighed to the control.
        /// </summary>
        /// <param name="str">Text to evaluate.</param>
        /// <returns>True if the text is allowed.</returns>
        protected override bool IsTextAllowed(string str)
        {
            float d;
            if (!float.TryParse(str, out d))
                return false;
            if (d < m_Min) return false;
            if (d > m_Max) return false;
            return true;
        }

        /// <summary>
        /// Numeric value of the control.
        /// </summary>
        public override float Value
        {
            get
            {
                return base.Value;
            }
            set
            {
                if (value < m_Min) value = m_Min;
                if (value > m_Max) value = m_Max;
                if (value == m_Value) return;

                base.Value = value;
            }
        }

        /// <summary>
        /// Handler for the text changed event.
        /// </summary>
        protected override void OnTextChanged()
        {
            base.OnTextChanged();
            if (ValueChanged != null)
                ValueChanged.Invoke(this);
        }
    }
}
