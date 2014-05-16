using System;
using Flood.GUIv2.DragDrop;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Titlebar for DockedTabControl.
    /// </summary>
    public class TabTitleBar : Label
    {
        public TabTitleBar(Control parent)
        {
            MouseInputEnabled = true;
            TextMargin = new Margin(5, 2, 5, 2);
            Margin = new Margin(1, 2, 1, 2);

            DragAndDrop_SetPackage(true, "TabWindowMove");
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            throw new NotImplementedException();

            skin.DrawTabTitleBar(this);
        }

        public override void DragAndDrop_StartDragging(DragDrop.Package package, int x, int y)
        {
            DragAndDrop.SourceControl = Parent;
            DragAndDrop.SourceControl.DragAndDrop_StartDragging(package, x, y);
        }

        public void UpdateFromTab(TabButton button)
        {
            Text = button.Text;
            SizeToContents();
        }
    }
}
