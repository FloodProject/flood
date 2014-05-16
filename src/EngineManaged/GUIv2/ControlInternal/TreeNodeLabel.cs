using Flood.GUIv2.Controls;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.ControlInternal
{
    /// <summary>
    /// Tree node label.
    /// </summary>
    public class TreeNodeLabel : Button
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="TreeNodeLabel"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public TreeNodeLabel(Control parent)
            : base(parent)
        {
            TextAlignment = AlignmentFlags.Left | AlignmentFlags.CenterVertical;
            ShouldDrawBackground = false;
            Height = 16;
            TextMargin = new Margin(3, 0);
        }

        /// <summary>
        /// Updates control colors.
        /// </summary>
        public override void UpdateColors()
        {
            if (IsDisabled)
            {
                TextColor = Skin.Colors.Button.Disabled;
                return;
            }

            if (IsDepressed || ToggleState)
            {
                TextColor = Skin.Colors.Tree.Selected;
                return;
            }

            if (IsHovered)
            {
                TextColor = Skin.Colors.Tree.Hover;
                return;
            }

            TextColor = Skin.Colors.Tree.Normal;
        }
    }
}
