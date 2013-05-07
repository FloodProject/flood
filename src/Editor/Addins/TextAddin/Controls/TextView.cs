using Flood.GUI;
using Flood.GUI.Controls;
using TextAddin.Document;

namespace TextAddin.Controls
{
    public class TextView : Control
    {
        public TextLayer TextLayer { get; private set; }
        public CarretLayer CarretLayer { get; private set; }

        public TextView(Control parent, TextDocument textDocument) : base(parent)
        {
            Dock = Pos.Fill;
            TextLayer = new TextLayer(this, textDocument);
            CarretLayer = new CarretLayer(this);
        }

    }
}
