using Flood.GUIv2.Input;
using System;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// HSV hue selector.
    /// </summary>
    public class ColorSlider : Control
    {
        private int m_SelectedDist;
        private bool m_Depressed;
        private ResourceHandle<Image> imageHandle;

        /// <summary>
        /// Invoked when the selected color has been changed.
        /// </summary>
        public event GwenEventHandler ColorChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="ColorSlider"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ColorSlider(Control parent)
        {
            SetSize(32, 128);
            MouseInputEnabled = true;
            m_Depressed = false;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            throw new NotImplementedException();

            //Is there any way to move this into skin? Not for now, no idea how we'll "actually" render these

            if (imageHandle.Id == ResourceHandle<Image>.Invalid)
            {
                throw new NotImplementedException();
                /*var bitmap = new Bitmap(Width, Height);

                for (int y = 0; y < Height; y++)
                {
                    Color c = GetColorAtHeight(y);
                    for (int x = 0; x < Width; x++)
                    {
                        bitmap.SetPixel(x,y, new Color(c.R,c.G,c.B,c.A));
                    }
                }

                imageHandle = new MaterialHandle(skin.Renderer);
                imageHandle.LoadBitmap(bitmap);*/
            }

            skin.Renderer.DrawColor = Color.White;
            skin.Renderer.DrawTexturedRect(imageHandle, new Rectangle(5, 0, Width-10, Height));
            
            int drawHeight = m_SelectedDist - 3;

            //Draw our selectors
            skin.Renderer.DrawColor = Color.Black;
            skin.Renderer.DrawFilledRect(new Rectangle(0, drawHeight + 2, Width, 1));
            skin.Renderer.DrawFilledRect(new Rectangle(0, drawHeight, 5, 5));
            skin.Renderer.DrawFilledRect(new Rectangle(Width - 5, drawHeight, 5, 5));
            skin.Renderer.DrawColor = Color.White;
            skin.Renderer.DrawFilledRect(new Rectangle(1, drawHeight + 1, 3, 3));
            skin.Renderer.DrawFilledRect(new Rectangle(Width - 4, drawHeight + 1, 3, 3));

            base.Render(skin);
        }

        /// <summary>
        /// Handler invoked on mouse click (left) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="down">If set to <c>true</c> mouse button is down.</param>
        public override void OnMouseClickedLeft(int x, int y, bool down)
        {
            throw new NotImplementedException();

            m_Depressed = down;
            if (down)
                InputHandler.MouseFocus = this;
            else
                InputHandler.MouseFocus = null;

            OnMouseMoved(x, y, 0, 0);
        }

        /// <summary>
        /// Handler invoked on mouse moved event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="dx">X change.</param>
        /// <param name="dy">Y change.</param>
        public override void OnMouseMoved(int x, int y, int dx, int dy)
        {
            throw new NotImplementedException();

            if (m_Depressed)
            {
                var cursorPos = CanvasPosToLocal(new Vector2i(x, y));

                if (cursorPos.Y < 0)
                    cursorPos.Y = 0;
                if (cursorPos.Y > Height)
                    cursorPos.Y = Height;

                m_SelectedDist = cursorPos.Y;
                if (ColorChanged != null)
                    ColorChanged.Invoke(this);
            }
        }

        private Color GetColorAtHeight(int y)
        {
            float yPercent = y / (float)Height;
            return Util.HSVToColor(yPercent * 360, 1, 1);
        }

        private void SetColor(Color color)
        {
            HSV hsv = color.ToHSV();

            m_SelectedDist = (int)(hsv.h / 360 * Height);

            if (ColorChanged != null)
                ColorChanged.Invoke(this);
        }

        /// <summary>
        /// Selected color.
        /// </summary>
        public Color SelectedColor { get { return GetColorAtHeight(m_SelectedDist); } set { SetColor(value); } }
    }
}
