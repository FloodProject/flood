
using System;
using Flood.GUI;
using Flood.GUI.Controls;
using Flood.GUI.Controls.Layout;

namespace Flood.Editor.Controls
{
    public class ToolBar
    {
        Tile pTileLayout;

        public ToolBar(Control parent)
        {
            pTileLayout = new Tile(parent);
            pTileLayout.SetTileSize(22, 22);
            pTileLayout.Height = 24;
            pTileLayout.Dock = Pos.Top;
        }

        public void AddTool(string name /* Image image */, Action onSelect)
        {
            var pButton = new Button(pTileLayout);
            pButton.SetSize(20, 20);
            pButton.SetToolTipText(name);
            pButton.ShouldDrawBackground = true;
            pButton.Clicked += new Control.GwenEventHandler(
                b => onSelect.Invoke());
        }

        public void Clear()
        {
            pTileLayout.DeleteAllChildren();
        }
    }
}
