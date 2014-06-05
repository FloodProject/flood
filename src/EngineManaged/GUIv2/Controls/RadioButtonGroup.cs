using System;
using System.Linq;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Radio button group.
    /// </summary>
    public class RadioButtonGroup : GroupBox
    {
        private LabeledRadioButton m_Selected;
        
        /// <summary>
        /// Selected radio button.
        /// </summary>
        public LabeledRadioButton Selected { get { return m_Selected; } }

        /// <summary>
        /// Internal name of the selected radio button.
        /// </summary>
        public String SelectedName { get { return m_Selected.Name; } }

        /// <summary>
        /// Text of the selected radio button.
        /// </summary>
        public String SelectedLabel { get { return m_Selected.Text; } }

        /// <summary>
        /// Index of the selected radio button.
        /// </summary>
        public int SelectedIndex
        {
            get
            {
                throw new NotImplementedException();
                //return Children.IndexOf(m_Selected);
            }
        }

        /// <summary>
        /// Invoked when the selected option has changed.
        /// </summary>
        public event Control.GwenEventHandler SelectionChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="RadioButtonGroup"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        /// <param name="label">Label for the outlining GroupBox.</param>
        public RadioButtonGroup(Control parent, String label)
            : base(parent)
        {
            IsTabable = false;
            KeyboardInputEnabled = true;
            Text = label;
            AutoSizeToContents = true;
        }

        /// <summary>
        /// Adds a new option.
        /// </summary>
        /// <param name="text">Option text.</param>
        /// <returns>Newly created control.</returns>
        public virtual LabeledRadioButton AddOption(String text)
        {
            return AddOption(text, String.Empty);
        }

        /// <summary>
        /// Adds a new option.
        /// </summary>
        /// <param name="text">Option text.</param>
        /// <param name="optionName">Internal name.</param>
        /// <returns>Newly created control.</returns>
        public virtual LabeledRadioButton AddOption(String text, String optionName)
        {
            throw new NotImplementedException();

            //LabeledRadioButton lrb = new LabeledRadioButton(this);
            //lrb.Name = optionName;
            //lrb.Text = text;
            //lrb.RadioButton.Checked += OnRadioClicked;
            //lrb.Alignment = AlignmentFlags.Top;
            //lrb.Margin = new Margin(0, 0, 0, 1); // 1 bottom
            //lrb.KeyboardInputEnabled = false; // todo: true?
            //lrb.IsTabable = true;

            //Invalidate();
            //return lrb;
        }

        /// <summary>
        /// Handler for the option change.
        /// </summary>
        /// <param name="fromPanel">Event source.</param>
        protected virtual void OnRadioClicked(IControl fromPanel)
        {
            throw new NotImplementedException();

            //RadioButton @checked = fromPanel as RadioButton;
            //foreach (LabeledRadioButton rb in Children.OfType<LabeledRadioButton>()) // todo: optimize
            //{
            //    if (rb.RadioButton == @checked)
            //        m_Selected = rb;
            //    else
            //        rb.RadioButton.IsChecked = false;
            //}

            //OnChanged();
        }
        /*
        /// <summary>
        /// Sizes to contents.
        /// </summary>
        public override void SizeToContents()
        {
            Layout(Skin); // options are docked so positions are not updated until layout runs
            //base.SizeToContents();
            int width = 0;
            int height = 0;
            foreach (Base child in Children)
            {
                width = Math.Max(child.Width, width);
                height += child.Height;
            }
            SetSize(width, height);
            InvalidateParent();
        }
        */
        protected virtual void OnChanged()
        {
            if (SelectionChanged != null)
                SelectionChanged.Invoke(this);
        }

        /// <summary>
        /// Selects the specified option.
        /// </summary>
        /// <param name="index">Option to select.</param>
        public void SetSelection(int index)
        {
            throw new NotImplementedException();

            //if (index < 0 || index >= Children.Count)
            //    return;

            //(Children[index] as LabeledRadioButton).RadioButton.Press();
        }
    }
}
