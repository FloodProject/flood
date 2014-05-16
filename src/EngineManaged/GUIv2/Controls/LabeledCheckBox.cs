using System;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// CheckBox with label.
    /// </summary>
    public class LabeledCheckBox : Control
    {
        private readonly CheckBox m_CheckBox;
        private readonly LabelClickable m_Label;

        /// <summary>
        /// Invoked when the control has been checked.
        /// </summary>
        public event GwenEventHandler Checked;

        /// <summary>
        /// Invoked when the control has been unchecked.
        /// </summary>
        public event GwenEventHandler UnChecked;

        /// <summary>
        /// Invoked when the control's check has been changed.
        /// </summary>
        public event GwenEventHandler CheckChanged;

        /// <summary>
        /// Indicates whether the control is checked.
        /// </summary>
        public bool IsChecked { get { return m_CheckBox.IsChecked; } set { m_CheckBox.IsChecked = value; } }

        /// <summary>
        /// Label text.
        /// </summary>
        public String Text { get { return m_Label.Text; } set { m_Label.Text = value; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="LabeledCheckBox"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public LabeledCheckBox(Control parent)
        {
            SetSize(200, 19);
            m_CheckBox = new CheckBox(this);
            m_CheckBox.Alignment = AlignmentFlags.Left;
            m_CheckBox.Margin = new Margin(0, 2, 2, 2);
            m_CheckBox.IsTabable = false;
            m_CheckBox.CheckChanged += OnCheckChanged;

            m_Label = new LabelClickable(this);
            //m_Label.Dock = Pos.Fill;
            m_Label.Expansion = ExpansionFlags.Expand;
            m_Label.Clicked += m_CheckBox.Press;
            m_Label.IsTabable = false;

            IsTabable = false;
        }

        /// <summary>
        /// Handler for CheckChanged event.
        /// </summary>
        protected virtual void OnCheckChanged(IControl control)
        {
            if (m_CheckBox.IsChecked)
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

        public override bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Handler for Space keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        public override bool OnKeySpace(bool down)
        {
            throw new NotImplementedException();

            base.OnKeySpace(down);
            if (!down) 
                m_CheckBox.IsChecked = !m_CheckBox.IsChecked; 
            return true;
        }
    }
}
