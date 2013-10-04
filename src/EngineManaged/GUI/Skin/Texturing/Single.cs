
using Flood;

namespace GUI.Skins.Texturing
{
    /// <summary>
    /// Single textured element.
    /// </summary>
    public struct Single
    {
        private readonly ResourceHandle<Image> _imageHandle;
        private readonly float[] m_uv;
        private readonly int m_Width;
        private readonly int m_Height;

        public Single(ResourceHandle<Image> imageHandle, float x, float y, float w, float h )
        {
            this._imageHandle = imageHandle;

            float texw = imageHandle.Resolve().Width;
            float texh = imageHandle.Resolve().Height;

            m_uv = new float[4];
            m_uv[0] = x / texw;
            m_uv[1] = y / texh;
            m_uv[2] = (x + w) / texw;
            m_uv[3] = (y + h) / texh;

            m_Width = (int) w;
            m_Height = (int) h;
        }

        // can't have this as default param
        public void Draw(Renderers.Renderer render, Rectangle r)
        {
            Draw(render, r, Color.White);
        }

        public void Draw(Renderers.Renderer render, Rectangle r, Color col)
        {
            render.DrawColor = col;
            render.DrawTexturedRect(_imageHandle, r, m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
        }

        public void DrawCenter(Renderers.Renderer render, Rectangle r)
        {
            DrawCenter(render, r, Color.White);
        }

        public void DrawCenter(Renderers.Renderer render, Rectangle r, Color col)
        {
            r.X += (int)((r.Width - m_Width) * 0.5);
            r.Y += (int)((r.Height - m_Height) * 0.5);
            r.Width = m_Width;
            r.Height = m_Height;

            Draw(render, r, col);
        }
    }
}
