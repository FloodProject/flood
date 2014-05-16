
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Clickable label (for checkboxes etc).
    /// </summary>
    public class LabelClickable : Button
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="LabelClickable"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public LabelClickable(Control parent)
            : base(parent)
        {
            IsToggle = false;
            TextAlignment = AlignmentFlags.Left | AlignmentFlags.CenterVertical;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            // no button look
        }
    }
}
