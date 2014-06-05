using Flood.GUIv2.Controls;

namespace Flood.GUIv2.ControlInternal
{
    /// <summary>
    /// Tree node toggle button (the little plus sign).
    /// </summary>
    public class TreeToggleButton : Button
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="TreeToggleButton"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public TreeToggleButton(Control parent)
        {
            IsToggle = true;
            IsTabable = false;
        }

        /// <summary>
        /// Renders the focus overlay.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void RenderFocus(Skins.Skin skin)
        {

        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        //public override void Render(Skins.Skin skin)
        //{
        //    skin.DrawTreeButton(this, ToggleState);
        //}
    }
}
