﻿using System;

namespace Gwen.Control
{
    /// <summary>
    /// Numeric text box - accepts only float numbers.
    /// </summary>
    public class TextBoxNumeric : TextBox
    {
        /// <summary>
        /// Current numeric value.
        /// </summary>
        protected float m_Value;

        /// <summary>
        /// Initializes a new instance of the <see cref="TextBoxNumeric"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public TextBoxNumeric(Base parent)
            : base(parent)
        {
            SetText("0", false);
        }

        protected virtual bool IsTextAllowed(String str)
        {
            if (str == "" || str == "-")
                return true; // annoying if single - is not allowed
            float d;
            return float.TryParse(str, out d);
        }

        /// <summary>
        /// Determines whether the control can insert text at a given cursor position.
        /// </summary>
        /// <param name="text">Text to check.</param>
        /// <param name="position">Cursor position.</param>
        /// <returns>True if allowed.</returns>
        protected override bool IsTextAllowed(String text, int position)
        {
            String newText = Text.Insert(position, text);
            return IsTextAllowed(newText);
        }

        /// <summary>
        /// Current numerical value.
        /// </summary>
        public virtual float Value
        {
            get { return m_Value; }
            set
            {
                m_Value = value;
                Text = value.ToString();
            }
        }
        
        // text -> value
        /// <summary>
        /// Handler for text changed event.
        /// </summary>
        protected override void OnTextChanged()
        {
            if (String.IsNullOrEmpty(Text) || Text == "-")
            {
                m_Value = 0;
                //SetText("0");
            }
            else
                m_Value = float.Parse(Text);
            base.OnTextChanged();
        }

        /// <summary>
        /// Sets the control text.
        /// </summary>
        /// <param name="str">Text to set.</param>
        /// <param name="doEvents">Determines whether to invoke "text changed" event.</param>
        public override void SetText(string str, bool doEvents = true)
        {
            if (IsTextAllowed(str))
                base.SetText(str, doEvents);
        }
    }
}
