using System;
using Flood;
using Flood.GUI.Controls;

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

        public CarretLayer(TextView parent) : base(parent)
        {
            carret = new Carret(parent);
        }


        private bool mouseDown = false;

        private void SnapCarretToPosition(int x, int y)
        {
            var snappedPos = TextView.TextLayer.SnapPositionToText(new Vector2(x, y));
            carret.SetPosition(snappedPos.X,snappedPos.Y);
        }

        protected override void OnMouseClickedLeft(int x, int y, bool down)
        {
            mouseDown = down;
            SnapCarretToPosition(x, y);
        }

        protected override void OnMouseMoved(int x, int y, int dx, int dy)
        {
            if (mouseDown)
                SnapCarretToPosition(x, y);
        }

    }
}
