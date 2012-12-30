﻿using System;

namespace Gwen.Control.Property
{
    /// <summary>
    /// Base control for property entry.
    /// </summary>
    public class Base : Control.Base
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Base"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public Base(Control.Base parent) : base(parent)
        {
            Height = 17;
        }

        /// <summary>
        /// Invoked when the property value has been changed.
        /// </summary>
        public event GwenEventHandler ValueChanged;

        /// <summary>
        /// Property value (todo: always string, which is ugly. do something about it).
        /// </summary>
        public virtual String Value { get { return null; } set { SetValue(value, false); } }

        /// <summary>
        /// Indicates whether the property value is being edited.
        /// </summary>
        public virtual bool IsEditing { get { return false; } }

        protected virtual void DoChanged()
        {
            if (ValueChanged != null)
                ValueChanged.Invoke(this);
        }

        protected virtual void OnValueChanged(Control.Base control)
        {
            DoChanged();
        }

        /// <summary>
        /// Sets the property value.
        /// </summary>
        /// <param name="value">Value to set.</param>
        /// <param name="fireEvents">Determines whether to fire "value changed" event.</param>
        public virtual void SetValue(String value, bool fireEvents = false)
        {
            
        }
    }
}
