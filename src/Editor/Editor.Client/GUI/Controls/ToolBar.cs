
using System;
namespace Flood.Editor.GUI.Controls
{
    public class ToolBar
    {
        Gwen.Control.Layout.Tile pTileLayout;

        public ToolBar(Gwen.Control.Base parent)
        {

            pTileLayout = new Gwen.Control.Layout.Tile(parent);
            pTileLayout.SetTileSize(22, 22);
            pTileLayout.Height = 24;
            pTileLayout.Dock = Gwen.Pos.Top;

        }

        public void AddTool(string name /* Image image */, Action onSelect)
        {
            var pButton = new Gwen.Control.Button(pTileLayout);
            pButton.SetSize(20, 20);
            pButton.SetToolTipText(name);
            pButton.ShouldDrawBackground = true;
            pButton.Clicked += new Gwen.Control.Base.GwenEventHandler(delegate(Gwen.Control.Base b)
            {
                onSelect.Invoke();
            });
        }

        public void Clear()
        {
            pTileLayout.DeleteAllChildren();
        }

       
    }
}
