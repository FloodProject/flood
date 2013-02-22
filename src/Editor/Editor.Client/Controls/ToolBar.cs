
using System;
using EngineManaged.GUI.Controls;

namespace Flood.Editor.Controls
{
    public class ToolBar
    {
        EngineManaged.GUI.Controls.Layout.Tile pTileLayout;

        public ToolBar(EngineManaged.GUI.Controls.Base parent)
        {
            pTileLayout = new EngineManaged.GUI.Controls.Layout.Tile(parent);
            pTileLayout.SetTileSize(22, 22);
            pTileLayout.Height = 24;
            pTileLayout.Dock = EngineManaged.GUI.Pos.Top;
        }

        public void AddTool(string name /* Image image */, Action onSelect)
        {
            var pButton = new EngineManaged.GUI.Controls.Button(pTileLayout);
            pButton.SetSize(20, 20);
            pButton.SetToolTipText(name);
            pButton.ShouldDrawBackground = true;
            pButton.Clicked += new EngineManaged.GUI.Controls.Base.GwenEventHandler(
                b => onSelect.Invoke());
        }

        public void Clear()
        {
            pTileLayout.DeleteAllChildren();
        }

       
    }
}
