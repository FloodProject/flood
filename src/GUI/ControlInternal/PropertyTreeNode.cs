﻿using System;
using Gwen.Control;

namespace Gwen.ControlInternal
{
    /// <summary>
    /// Properties node.
    /// </summary>
    public class PropertyTreeNode : TreeNode
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="PropertyTreeNode"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public PropertyTreeNode(Base parent)
            : base(parent)
        {
            m_Title.TextColorOverride = Skin.Colors.Properties.Title;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            skin.DrawPropertyTreeNode(this, m_InnerPanel.X, m_InnerPanel.Y);
        }
    }
}
