
using System;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls.Layout
{
    /// <summary>
    /// Helper control that positions its children in a specific way.
    /// </summary>
    public class Positioner : Control
    {
        private Pos m_Pos;

        /// <summary>
        /// Children position.
        /// </summary>
        public Pos Pos { get { return m_Pos; } set { m_Pos = value; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="Positioner"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public Positioner(Control parent)
        {
            Pos = Pos.Left | Pos.Top;
        }

        /// <summary>
        /// Function invoked after layout.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void PostLayout(Skins.Skin skin)
        {
            throw new NotImplementedException();
            //foreach (Control child in Children) // ok?
            //{
            //    child.Position(m_Pos);
            //}
        }
    }

    /// <summary>
    /// Helper class that centers all its children.
    /// </summary>
    public class Center : Positioner
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Center"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public Center(Control parent) : base(parent)
        {
            Pos = Pos.Center;
        }
    }
}
