using System;

namespace Flood.GUIv2.Renderers
{
    /// <summary>
    /// Base renderer.
    /// </summary>
    public class Renderer : IDisposable
    {
        //public Random rnd;
        private Vector2i m_RenderOffset;
        private Rectangle m_ClipRegion;
        //protected ICacheToTexture m_RTT;

        public float Scale { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Renderer"/> class.
        /// </summary>
        protected Renderer()
        {
            //rnd = new Random();
            m_RenderOffset = new Vector2i(0,0);
            Scale = 1.0f;
            if (CTT != null)
                CTT.Initialize();
        }
        
        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        /// <filterpriority>2</filterpriority>
        public virtual void Dispose()
        {
            if (CTT != null)
                CTT.ShutDown();
            GC.SuppressFinalize(this);
        }

#if DEBUG
        ~Renderer()
        {
            throw new InvalidOperationException(String.Format("IDisposable object finalized: {0}", GetType()));
            //Debug.Print(String.Format("IDisposable object finalized: {0}", GetType()));
        }
#endif

        /// <summary>
        /// Starts rendering.
        /// </summary>
        public virtual void Begin() 
        {}

        /// <summary>
        /// Stops rendering.
        /// </summary>
        public virtual void End() 
        {}

        /// <summary>
        /// Gets or sets the current drawing color.
        /// </summary>
        public virtual Color DrawColor { get; set; }

        /// <summary>
        /// Rendering offset. No need to touch it usually.
        /// </summary>
        public Vector2i RenderOffset { get { return m_RenderOffset; } set { m_RenderOffset = value; } }

        /// <summary>
        /// Clipping rectangle.
        /// </summary>
        public Rectangle ClipRegion { get { return m_ClipRegion; } set { m_ClipRegion = value; } }

        /// <summary>
        /// Indicates whether the clip region is visible.
        /// </summary>
        public bool ClipRegionVisible
        {
            get
            {
                if (m_ClipRegion.Width <= 0 || m_ClipRegion.Height <= 0)
                    return false;

                return true;
            }
        }

        /// <summary>
        /// Draws a line.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="a"></param>
        /// <param name="b"></param>
        public virtual void DrawLine(int x, int y, int a, int b)
        {}

        /// <summary>
        /// Draws a solid filled rectangle.
        /// </summary>
        /// <param name="rect"></param>
        public virtual void DrawFilledRect(Rectangle rect)
        {}

        /// <summary>
        /// Starts clipping to the current clipping rectangle.
        /// </summary>
        public virtual void StartClip()
        {}

        /// <summary>
        /// Stops clipping.
        /// </summary>
        public virtual void EndClip()
        {}

        /// <summary>
        /// Draws textured rectangle.
        /// </summary>
        /// <param name="material">Material to use.</param>
        /// <param name="materialHandle">Rectangle bounds.</param>
        /// <param name="u1">MaterialHandle coordinate u1.</param>
        /// <param name="v1">MaterialHandle coordinate v1.</param>
        /// <param name="u2">MaterialHandle coordinate u2.</param>
        /// <param name="v2">MaterialHandle coordinate v2.</param>
        public virtual void DrawTexturedRect(ResourceHandle<Image> imageHandle, RectangleF rect, float u1=0, float v1=0, float u2=1, float v2=1)
        {}

        /// <summary>
        /// Draws textured rectangle.
        /// </summary>
        /// <param name="t">MaterialHandle to use.</param>
        /// <param name="materialHandle">Material to use.</param>
        /// <param name="u1">MaterialHandle coordinate u1.</param>
        /// <param name="v1">MaterialHandle coordinate v1.</param>
        /// <param name="u2">MaterialHandle coordinate u2.</param>
        /// <param name="v2">MaterialHandle coordinate v2.</param>
        public void DrawTexturedRect(ResourceHandle<Image> imageHandle, Rectangle rect, float u1=0, float v1=0, float u2=1, float v2=1)
        {
            DrawTexturedRect(imageHandle, new RectangleF(rect.X, rect.Y, rect.Width, rect.Height), u1, v1, u2, v2);
        }

        /// <summary>
        /// Draws "missing image" default texture.
        /// </summary>
        /// <param name="rect">Target rectangle.</param>
        public virtual void DrawMissingImage(Rectangle rect)
        {
            //DrawColor = Color.FromArgb(255, rnd.Next(0,255), rnd.Next(0,255), rnd.Next(0, 255));
            DrawColor = Color.Red;
            DrawFilledRect(rect);
        }

        /// <summary>
        /// Cache to texture provider.
        /// </summary>
        public virtual ICacheToTexture CTT { get { return null; } }

        /// <summary>
        /// Loads the specified font.
        /// </summary>
        /// <param name="font">Font to load.</param>
        /// <returns>True if succeeded.</returns>
        public virtual bool LoadFont(Font font)
        {
            return false;
        }

        /// <summary>
        /// Frees the specified font.
        /// </summary>
        /// <param name="font">Font to free.</param>
        public virtual void FreeFont(Font font)
        {}

        /// <summary>
        /// Returns dimensions of the text using specified font.
        /// </summary>
        /// <param name="font">Font to use.</param>
        /// <param name="text">Text to measure.</param>
        /// <returns>Width and height of the rendered text.</returns>
        public virtual Vector2 MeasureText(Font font, String text)
        {
            var p = new Vector2(font.Size * Scale * text.Length * 0.4f, font.Size * Scale);
            return p;
        }

        /// <summary>
        /// Renders text using specified font.
        /// </summary>
        /// <param name="font">Font to use.</param>
        /// <param name="position">Top-left corner of the text.</param>
        /// <param name="text">Text to render.</param>
        public virtual void RenderText(Font font, Vector2i position, String text)
        {
            float size = font.Size * Scale;

            for ( int i=0; i<text.Length; i++ )
            {
                char chr = text[i];

                if ( chr == ' ' ) 
                    continue;

                Rectangle r = Util.FloatRect(position.X + i * size * 0.4f, position.Y, size * 0.4f - 1, size);

                /*
                    This isn't important, it's just me messing around changing the
                    shape of the rect based on the letter.. just for fun.
                */
                if ( chr == 'l' || chr == 'i' || chr == '!' || chr == 't' )
                {
                    r.Width = 1;
                }
                else if ( chr >= 'a' && chr <= 'z' )
                {
                    r.Y = (int)(r.Y + size * 0.5f);
                    r.Height = (int)(r.Height - size * 0.4f);
                }
                else if ( chr == '.' || chr == ',' )
                {
                    r.X += 2;
                    r.Y += r.Height - 2;
                    r.Width = 2;
                    r.Height = 2;
                }
                else if ( chr == '\'' || chr == '`'  || chr == '"' )
                {
                    r.X += 3;
                    r.Width = 2;
                    r.Height = 2;
                }

                if ( chr == 'o' || chr == 'O' || chr == '0' )
                    DrawLinedRect( r );	
                else
                    DrawFilledRect( r );
            }
        }

        //
        // No need to implement these functions in your derived class, but if 
        // you can do them faster than the default implementation it's a good idea to.
        //

        /// <summary>
        /// Draws a lined rectangle. Used for keyboard focus overlay.
        /// </summary>
        /// <param name="rect">Target rectangle.</param>
        public virtual void DrawLinedRect(Rectangle rect)
        {
            DrawFilledRect(new Rectangle(rect.X, rect.Y, rect.Width, 1));
            DrawFilledRect(new Rectangle(rect.X, rect.Y + rect.Height - 1, rect.Width, 1));

            DrawFilledRect(new Rectangle(rect.X, rect.Y, 1, rect.Height));
            DrawFilledRect(new Rectangle(rect.X + rect.Width - 1, rect.Y, 1, rect.Height));
        }

        /// <summary>
        /// Draws a single pixel. Very slow, do not use. :P
        /// </summary>
        /// <param name="x">X.</param>
        /// <param name="y">Y.</param>
        public virtual void DrawPixel(int x, int y)
        {
            // [omeg] amazing ;)
            DrawFilledRect(new Rectangle(x, y, 1, 1));
        }

        /// <summary>
        /// Gets pixel color of a specified texture. Slow.
        /// </summary>
        /// <param name="materialHandle"> </param>
        /// <param name="x">X.</param>
        /// <param name="y">Y.</param>
        /// <returns>Pixel color.</returns>
        public virtual Color PixelColor(ResourceHandle<Image> imagelHandle, uint x, uint y)
        {
            return PixelColor(imagelHandle, x, y, Color.White);
        }

        /// <summary>
        /// Gets pixel color of a specified texture, returning default if otherwise failed. Slow.
        /// </summary>
        /// <param name="materialHandle"> </param>
        /// <param name="x">X.</param>
        /// <param name="y">Y.</param>
        /// <param name="defaultColor">Color to return on failure.</param>
        /// <returns>Pixel color.</returns>
        public virtual Color PixelColor(ResourceHandle<Image> imagelHandle, uint x, uint y, Color defaultColor)
        {
            return defaultColor;
        }

        /// <summary>
        /// Draws a round-corner rectangle.
        /// </summary>
        /// <param name="rect">Target rectangle.</param>
        /// <param name="slight"></param>
        public virtual void DrawShavedCornerRect(Rectangle rect, bool slight = false)
        {
            // Draw INSIDE the w/h.
            rect.Width -= 1;
            rect.Height -= 1;

            if (slight)
            {
                DrawFilledRect(new Rectangle(rect.X + 1, rect.Y, rect.Width - 1, 1));
                DrawFilledRect(new Rectangle(rect.X + 1, rect.Y + rect.Height, rect.Width - 1, 1));

                DrawFilledRect(new Rectangle(rect.X, rect.Y + 1, 1, rect.Height - 1));
                DrawFilledRect(new Rectangle(rect.X + rect.Width, rect.Y + 1, 1, rect.Height - 1));
                return;
            }

            DrawPixel(rect.X + 1, rect.Y + 1);
            DrawPixel(rect.X + rect.Width - 1, rect.Y + 1);

            DrawPixel(rect.X + 1, rect.Y + rect.Height - 1);
            DrawPixel(rect.X + rect.Width - 1, rect.Y + rect.Height - 1);

            DrawFilledRect(new Rectangle(rect.X + 2, rect.Y, rect.Width - 3, 1));
            DrawFilledRect(new Rectangle(rect.X + 2, rect.Y + rect.Height, rect.Width - 3, 1));

            DrawFilledRect(new Rectangle(rect.X, rect.Y + 2, 1, rect.Height - 3));
            DrawFilledRect(new Rectangle(rect.X + rect.Width, rect.Y + 2, 1, rect.Height - 3));
        }

        private float TranslateX(float x)
        {
            var x1 = x + m_RenderOffset.X;
            return x1 * Scale;
        }

        private float TranslateY(float y)
        {
            var y1 = y + m_RenderOffset.Y;
            return y1 * Scale;
        }

        /// <summary>
        /// Translates a panel's local drawing coordinate into view space, taking offsets into account.
        /// </summary>
        public Vector2i Translate(Vector2i p)
        {
            return new Vector2i(Util.Ceil(TranslateX(p.X)), Util.Ceil(TranslateY(p.Y)));
        }

        /// <summary>
        /// Translates a panel's local drawing coordinate into view space, taking offsets into account.
        /// </summary>
        protected Rectangle Translate(Rectangle rect)
        {
            return new Rectangle(Util.Ceil(TranslateX(rect.X)), Util.Ceil(TranslateY(rect.Y)), Util.Ceil(rect.Width * Scale), Util.Ceil(rect.Height * Scale));
        }

        /// <summary>
        /// Translates a panel's local drawing coordinate into view space, taking offsets into account.
        /// </summary>
        protected RectangleF Translate(RectangleF rect)
        {
            return new RectangleF(TranslateX(rect.X), TranslateY(rect.Y), rect.Width * Scale, rect.Height * Scale);
        }

        /// <summary>
        /// Adds a point to the render offset.
        /// </summary>
        /// <param name="offset">Point to add.</param>
        public void AddRenderOffset(Rectangle offset)
        {
            m_RenderOffset = new Vector2i(m_RenderOffset.X + offset.X, m_RenderOffset.Y + offset.Y);
        }

        /// <summary>
        /// Adds a rectangle to the clipping region.
        /// </summary>
        /// <param name="rect">Rectangle to add.</param>
        public void AddClipRegion(Rectangle rect)
        {

            rect.X = m_RenderOffset.X;
            rect.Y = m_RenderOffset.Y;

            Rectangle r = rect;
            if (rect.X < m_ClipRegion.X)
            {
                r.Width -= (m_ClipRegion.X - r.X);
                r.X = m_ClipRegion.X;
            }

            if (rect.Y < m_ClipRegion.Y)
            {
                r.Height -= (m_ClipRegion.Y - r.Y);
                r.Y = m_ClipRegion.Y;
            }

            if (rect.GetRight() > m_ClipRegion.GetRight())
            {
                r.Width = m_ClipRegion.GetRight() - r.X;
            }

            if (rect.GetBottom() > m_ClipRegion.GetBottom())
            {
                r.Height = m_ClipRegion.GetBottom() - r.Y;
            }

            m_ClipRegion = r;
        }
    }
}
