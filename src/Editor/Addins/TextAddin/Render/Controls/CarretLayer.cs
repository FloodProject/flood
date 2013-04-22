using System;
using Flood;
using Flood.GUI.Controls;
using ICSharpCode.AvalonEdit.Document;

namespace TextAddin.Render.Controls
{
    class CarretLayer : Layer
    {
        class Carret : Control
        {
            public int LineHeight { get; set; }

            public Carret(Control parent) : base(parent)
            {
                LineHeight = Skin.DefaultFont.Size;
                MouseInputEnabled = false;
            }

            protected override void Render(Flood.GUI.Skins.Skin skin)
            {
                if (DateTime.Now.Millisecond < 500) 
                    return;

                skin.Renderer.DrawColor = new Color(0,0,0,255);
                skin.Renderer.DrawFilledRect(new Rect(0,0,1,LineHeight));
            }
        }

        private Carret carret;

        private TextLocation location;
        public TextLocation Location
        {
            get { return location; }
            private set
            {
                location = value;
                var charPos = TextView.TextLayer.GetTextLocationPosition(location);
                carret.SetPosition(charPos.X,charPos.Y);
            }
        }

        public CarretLayer(TextView parent) : base(parent)
        {
            carret = new Carret(parent);

            KeyboardInputEnabled = true;
            Location = new TextLocation(1,1);
        }


        private bool mouseDown = false;

        private void SnapCarretToPosition(int x, int y)
        {
            Location = TextView.TextLayer.GetTextLocation(new Vector2(x, y));
        }

        protected void InsertText(string text)
        {
            var offset = TextView.TextDocument.GetOffset(Location);
            TextView.TextDocument.Insert(offset, text);
        }

        protected void RemoveText(int offset,int length)
        {
            var carretOffset = TextView.TextDocument.GetOffset(Location);
            TextView.TextDocument.Remove(carretOffset+offset, length);
        }

        #region Input

        protected override void OnMouseClickedLeft(int x, int y, bool down)
        {
            mouseDown = down;
            Console.WriteLine("Down");
            SnapCarretToPosition(x, y);
        }

        protected override void OnMouseMoved(int x, int y, int dx, int dy)
        {
            if (mouseDown)
                SnapCarretToPosition(x, y);
        }
 
        protected override bool OnChar(char chr)
        {
            InsertText(""+chr);
            Location = new TextLocation(Location.Line, Location.Column+1);
            return true;
        }

        protected override bool OnKeyBackspace(bool down)
        {
            if(!down)
                return false;

            RemoveText(-1, 1);
            Location = new TextLocation(Location.Line, Location.Column-1);

            return true;
        }

        protected override bool OnKeyDelete(bool down)
        {
            if(!down)
                return false;

            RemoveText(0, 1);
            return true;
        }

        protected override bool OnKeyLeft(bool down)
        {
            if(!down)
                return false;

            Location = new TextLocation(Location.Line, Location.Column-1);
            return true;
        }

        protected override bool OnKeyRight(bool down)
        {
            if(!down)
                return false;

            Location = new TextLocation(Location.Line, Location.Column+1);
            return true;
        }

        #endregion

    }
}
