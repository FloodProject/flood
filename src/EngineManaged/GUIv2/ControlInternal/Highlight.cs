using System;
using Flood.GUIv2.Controls;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.ControlInternal
{
    /// <summary>
    /// Drag&drop highlight.
    /// </summary>
    public class Highlight : Control
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Highlight"/> class.
        /// </summary>
        public Highlight(Control toHighLight)
        {
            //todo: this does nothing without a parent, it seems 
            throw new NotImplementedException();
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            skin.DrawHighlight(this);
        }
    }
}
