
namespace Flood.GUI.Skins.Texturing
{
    public struct SubRect
    {
        public float[] uv;
    }

    /// <summary>
    /// 3x3 texture grid.
    /// </summary>
    public struct Bordered
    {
        private ResourceHandle<Image> imageHandle;

        private readonly SubRect[] m_Rects;

        private Margin m_Margin;

        private float m_Width;
        private float m_Height;

        public Bordered(ResourceHandle<Image> imageHandle, float x, float y, float w, float h, Margin inMargin, float drawMarginScale = 1.0f)
            : this()
        {
            m_Rects = new SubRect[9];
            for (int i = 0; i < m_Rects.Length; i++)
            {
                m_Rects[i].uv = new float[4];
            }

            Init(imageHandle, x, y, w, h, inMargin, drawMarginScale);
        }

        void DrawRect(Renderers.Renderer render, int i, int x, int y, int w, int h)
        {
            render.DrawTexturedRect(imageHandle,
                                    new Rectangle(x, y, w, h),
                                    m_Rects[i].uv[0], m_Rects[i].uv[1], m_Rects[i].uv[2], m_Rects[i].uv[3]);
        }

        void SetRect(int num, float x, float y, float w, float h)
        {
            float texw = imageHandle.Resolve().Width;
            float texh = imageHandle.Resolve().Height;

            //x -= 1.0f;
            //y -= 1.0f;

            m_Rects[num].uv[0] = x / texw;
            m_Rects[num].uv[1] = y / texh;

            m_Rects[num].uv[2] = (x + w) / texw;
            m_Rects[num].uv[3] = (y + h) / texh;

            //	rects[num].uv[0] += 1.0f / material->width;
            //	rects[num].uv[1] += 1.0f / material->width;
        }

        private void Init(ResourceHandle<Image> imageHandle, float x, float y, float w, float h, Margin inMargin, float drawMarginScale = 1.0f)
        {
            this.imageHandle = imageHandle;

            m_Margin = inMargin;

            SetRect(0, x, y, m_Margin.Left, m_Margin.Top);
            SetRect(1, x + m_Margin.Left, y, w - m_Margin.Left - m_Margin.Right, m_Margin.Top);
            SetRect(2, (x + w) - m_Margin.Right, y, m_Margin.Right, m_Margin.Top);

            SetRect(3, x, y + m_Margin.Top, m_Margin.Left, h - m_Margin.Top - m_Margin.Bottom);
            SetRect(4, x + m_Margin.Left, y + m_Margin.Top, w - m_Margin.Left - m_Margin.Right,
                    h - m_Margin.Top - m_Margin.Bottom);
            SetRect(5, (x + w) - m_Margin.Right, y + m_Margin.Top, m_Margin.Right, h - m_Margin.Top - m_Margin.Bottom - 1);

            SetRect(6, x, (y + h) - m_Margin.Bottom, m_Margin.Left, m_Margin.Bottom);
            SetRect(7, x + m_Margin.Left, (y + h) - m_Margin.Bottom, w - m_Margin.Left - m_Margin.Right, m_Margin.Bottom);
            SetRect(8, (x + w) - m_Margin.Right, (y + h) - m_Margin.Bottom, m_Margin.Right, m_Margin.Bottom);

            m_Margin.Left = (int)(m_Margin.Left*drawMarginScale);
            m_Margin.Right = (int)(m_Margin.Right*drawMarginScale);
            m_Margin.Top = (int)(m_Margin.Top*drawMarginScale);
            m_Margin.Bottom = (int)(m_Margin.Bottom*drawMarginScale);

            m_Width = w - x;
            m_Height = h - y;
        }

        // can't have this as default param
        public void Draw(Renderers.Renderer render, Rectangle r)
        {
            Draw(render, r, Color.White);
        }

        public void Draw(Renderers.Renderer render, Rectangle r, Color col)
        {
            render.DrawColor = col;

            if (r.Width < m_Width && r.Height < m_Height)
            {
                render.DrawTexturedRect(imageHandle, r, m_Rects[0].uv[0], m_Rects[0].uv[1], m_Rects[8].uv[2], m_Rects[8].uv[3]);
                return;
            }

            DrawRect(render, 0, r.X, r.Y, m_Margin.Left, m_Margin.Top);
            DrawRect(render, 1, r.X + m_Margin.Left, r.Y, r.Width - m_Margin.Left - m_Margin.Right, m_Margin.Top);
            DrawRect(render, 2, (r.X + r.Width) - m_Margin.Right, r.Y, m_Margin.Right, m_Margin.Top);

            DrawRect(render, 3, r.X, r.Y + m_Margin.Top, m_Margin.Left, r.Height - m_Margin.Top - m_Margin.Bottom);
            DrawRect(render, 4, r.X + m_Margin.Left, r.Y + m_Margin.Top, r.Width - m_Margin.Left - m_Margin.Right,
                     r.Height - m_Margin.Top - m_Margin.Bottom);
            DrawRect(render, 5, (r.X + r.Width) - m_Margin.Right, r.Y + m_Margin.Top, m_Margin.Right,
                     r.Height - m_Margin.Top - m_Margin.Bottom);

            DrawRect(render, 6, r.X, (r.Y + r.Height) - m_Margin.Bottom, m_Margin.Left, m_Margin.Bottom);
            DrawRect(render, 7, r.X + m_Margin.Left, (r.Y + r.Height) - m_Margin.Bottom,
                     r.Width - m_Margin.Left - m_Margin.Right, m_Margin.Bottom);
            DrawRect(render, 8, (r.X + r.Width) - m_Margin.Right, (r.Y + r.Height) - m_Margin.Bottom, m_Margin.Right,
                     m_Margin.Bottom);
        }
    }
}
