 using Flood.GUI;
 using Flood.GUI.Controls;
 using ICSharpCode.AvalonEdit.Document;
 using ICSharpCode.AvalonEdit.Rendering;
 using TextAddin.Render.Controls;

namespace TextAddin.Render
{
    class TextView : Control
    {
        public TextLayer TextLayer { get; private set; }
        public CarretLayer CarretLayer { get; private set; }

        public TextDocument TextDocument  { get; private set; }

        public TextView(Control parent, TextDocument textDocument) : base(parent)
        {
            Dock = Pos.Fill;
            TextDocument = textDocument;
            TextLayer = new TextLayer(this);
            CarretLayer = new CarretLayer(this);
        }
    }
}
