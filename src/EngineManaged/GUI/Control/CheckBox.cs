
using System;

namespace Flood.GUI.Controls
{
    /// <summary>
    /// CheckBox control.
    /// </summary>
    public class CheckBox : Button
    {
        private bool m_Checked;

        /// <summary>
        /// Indicates whether the checkbox is checked.
        /// </summary>
        public bool IsChecked
        {
            get { return m_Checked; } 
            set
            {
                if (m_Checked == value) return;
                m_Checked = value;
                OnCheckChanged();
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="CheckBox"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public CheckBox(Control parent)
            : base(parent)
        {
            AutoSizeToContents = false;
            SetSize(15, 15);
            //m_Checked = true; // [omeg] why?!
            //Toggle();
        }

        /// <summary>
        /// Toggles the checkbox.
        /// </summary>
        public override void Toggle()
        {
            base.Toggle();
            IsChecked = !IsChecked;
        }

        /// <summary>
        /// Invoked when the checkbox has been checked.
        /// </summary>
        public event GwenEventHandler Checked;

        /// <summary>
        /// Invoked when the checkbox has been unchecked.
        /// </summary>
        public event GwenEventHandler UnChecked;

        /// <summary>
        /// Invoked when the checkbox state has been changed.
        /// </summary>
        public event GwenEventHandler CheckChanged;

        /// <summary>
        /// Determines whether unchecking is allowed.
        /// </summary>
        protected virtual bool AllowUncheck { get { return true; } }

        /// <summary>
        /// Handler for CheckChanged event.
        /// </summary>
        protected virtual void OnCheckChanged()
        {
            if (IsChecked)
            { 
                if (Checked != null)
                    Checked.Invoke(this);
            }
            else
            {
                if (UnChecked != null)
                    UnChecked.Invoke(this);
            }

            if (CheckChanged != null)
                CheckChanged.Invoke(this);
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skins.Skin skin)
        {
            base.Render(skin);
            skin.DrawCheckBox(this, m_Checked, IsDepressed);
        }

        public override void SetText(string str, bool doEvents = true)
        {
            throw new InvalidOperationException("This control cannot have text");
        }

        /// <summary>
        /// Internal OnPressed implementation.
        /// </summary>
        protected override void OnClicked()
        {
            if (IsDisabled)
                return;
            
            if (IsChecked && !AllowUncheck)
            {
                return;
            }

            Toggle();
        }
    }
}
