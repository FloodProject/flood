using Flood.GUI.ControlInternal;
using System;

namespace Flood.GUI.Controls
{
    /// <summary>
    /// Single property row.
    /// </summary>
    public class PropertyRow : Control
    {
        private readonly Label m_Label;
        private readonly Property.Control m_Property;
        private bool m_LastEditing;
        private bool m_LastHover;

        /// <summary>
        /// Invoked when the property value has changed.
        /// </summary>
        public event GwenEventHandler ValueChanged;

        /// <summary>
        /// Indicates whether the property value is being edited.
        /// </summary>
        public bool IsEditing { get { return m_Property != null && m_Property.IsEditing; } }

        /// <summary>
        /// Property value.
        /// </summary>
        public String Value { get { return m_Property.Value; } set { m_Property.Value = value; } }

        /// <summary>
        /// Indicates whether the control is hovered by mouse pointer.
        /// </summary>
        public override bool IsHovered
        {
            get
            {
                return base.IsHovered || (m_Property != null && m_Property.IsHovered);
            }
        }

        /// <summary>
        /// Property name.
        /// </summary>
        public String Label { get { return m_Label.Text; } set { m_Label.Text = value; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="PropertyRow"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        /// <param name="prop">Property control associated with this row.</param>
        public PropertyRow(Control parent, Property.Control prop)
            : base(parent)
        {
            PropertyRowLabel label = new PropertyRowLabel(this);
            label.Dock = Pos.Left;
            label.TextAlignment = Pos.Left | Pos.Top;
            label.Margin = new Margin(2, 2, 0, 0);
            m_Label = label;

            m_Property = prop;
            m_Property.Parent = this;
            m_Property.Dock = Pos.Fill;
            m_Property.ValueChanged += OnValueChanged;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skins.Skin skin)
        {
            /* SORRY */
            if (IsEditing != m_LastEditing)
            {
                OnEditingChanged();
                m_LastEditing = IsEditing;
            }

            if (IsHovered != m_LastHover)
            {
                OnHoverChanged();
                m_LastHover = IsHovered;
            }
            /* SORRY */

            skin.DrawPropertyRow(this, m_Label.Right, IsEditing, IsHovered | m_Property.IsHovered);
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skins.Skin skin)
        {
            Properties parent = Parent as Properties;
            if (null == parent) return;

            m_Label.Width = parent.SplitWidth;

            if (m_Property != null)
            {
                Height = m_Property.Height;
            }
        }

        protected virtual void OnValueChanged(Control control)
        {
            if (ValueChanged != null)
                ValueChanged.Invoke(this);
        }

        private void OnEditingChanged()
        {
            m_Label.Redraw();
        }

        private void OnHoverChanged()
        {
            m_Label.Redraw();
        }
    }
}
