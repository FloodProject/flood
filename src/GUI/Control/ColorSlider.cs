﻿using System;
using System.Drawing;
using Gwen.Input;

namespace Gwen.Control
{
    /// <summary>
    /// HSV hue selector.
    /// </summary>
    public class ColorSlider : Base
    {
        private int m_SelectedDist;
        private bool m_Depressed;
        private Texture m_Texture;

        /// <summary>
        /// Invoked when the selected color has been changed.
        /// </summary>
        public event GwenEventHandler ColorChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="ColorSlider"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ColorSlider(Base parent)
            : base(parent)
        {
            SetSize(32, 128);
            MouseInputEnabled = true;
            m_Depressed = false;
        }

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        public override void Dispose()
        {
            if (m_Texture != null)
                m_Texture.Dispose();
            base.Dispose();
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            //Is there any way to move this into skin? Not for now, no idea how we'll "actually" render these

            if (m_Texture == null)
            {
                byte[] pixelData = new byte[Width * Height * 4];

                for (int y = 0; y < Height; y++)
                {
                    Color c = GetColorAtHeight(y);
                    for (int x = 0; x < Width; x++)
                    {
                        pixelData[4 * (x + y * Width)] = c.R;
                        pixelData[4 * (x + y * Width) + 1] = c.G;
                        pixelData[4 * (x + y * Width) + 2] = c.B;
                        pixelData[4 * (x + y * Width) + 3] = c.A;
                    }
                }

                m_Texture = new Texture(skin.Renderer);
                m_Texture.Width = Width;
                m_Texture.Height = Height;
                m_Texture.LoadRaw(Width, Height, pixelData);
            }

            skin.Renderer.DrawColor = Color.White;
            skin.Renderer.DrawTexturedRect(m_Texture, new Rectangle(5, 0, Width-10, Height));
            
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
        protected override void OnMouseClickedLeft(int x, int y, bool down)
        {
            m_Depressed = down;
            if (down)
                InputHandler.Instance.MouseFocus = this;
            else
                InputHandler.Instance.MouseFocus = null;

            OnMouseMoved(x, y, 0, 0);
        }

        /// <summary>
        /// Handler invoked on mouse moved event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="dx">X change.</param>
        /// <param name="dy">Y change.</param>
        protected override void OnMouseMoved(int x, int y, int dx, int dy)
        {
            if (m_Depressed)
            {
                Point cursorPos = CanvasPosToLocal(new Point(x, y));

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
