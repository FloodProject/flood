﻿using System;

namespace Gwen.Control
{
    /// <summary>
    /// Tree control.
    /// </summary>
    public class TreeControl : TreeNode
    {
        private readonly ScrollControl m_ScrollControl;
        private bool m_MultiSelect;

        /// <summary>
        /// Determines if multiple nodes can be selected at the same time.
        /// </summary>
        public bool AllowMultiSelect { get { return m_MultiSelect; } set { m_MultiSelect = value; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="TreeControl"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public TreeControl(Base parent)
            : base(parent)
        {
            m_TreeControl = this;

            RemoveChild(m_ToggleButton, true);
            m_ToggleButton = null;
            RemoveChild(m_Title, true);
            m_Title = null;
            RemoveChild(m_InnerPanel, true);
            m_InnerPanel = null;

            m_MultiSelect = false;

            m_ScrollControl = new ScrollControl(this);
            m_ScrollControl.Dock = Pos.Fill;
            m_ScrollControl.EnableScroll(false, true);
            m_ScrollControl.AutoHideBars = true;
            m_ScrollControl.Margin = Margin.One;

            m_InnerPanel = m_ScrollControl;

            m_ScrollControl.SetInnerSize(1000, 1000); // todo: why such arbitrary numbers?
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            if (ShouldDrawBackground)
                skin.DrawTreeControl(this);
        }

        /// <summary>
        /// Handler invoked when control children's bounds change.
        /// </summary>
        /// <param name="oldChildBounds"></param>
        /// <param name="child"></param>
        protected override void OnChildBoundsChanged(System.Drawing.Rectangle oldChildBounds, Base child)
        {
            if (m_ScrollControl != null)
                m_ScrollControl.UpdateScrollBars();
        }

        /// <summary>
        /// Removes all child nodes.
        /// </summary>
        public virtual void RemoveAll()
        {
            m_ScrollControl.DeleteAll();
        }

        /// <summary>
        /// Handler for node added event.
        /// </summary>
        /// <param name="node">Node added.</param>
        public virtual void OnNodeAdded(TreeNode node)
        {
            node.LabelPressed += OnNodeSelected;
        }

        /// <summary>
        /// Handler for node selected event.
        /// </summary>
        /// <param name="Control">Node selected.</param>
        protected virtual void OnNodeSelected(Base Control)
        {
            if (!m_MultiSelect /*|| InputHandler.Instance.InputHandler.Instance.IsKeyDown(Key.Control)*/)
                UnselectAll();
        }
    }
}
