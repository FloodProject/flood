using System;
using System.Drawing;

namespace Flood.GUI.Controls
{
    /// <summary>
    /// Image container.
    /// </summary>
    public class ImagePanel : Control
    {
        private ResourceHandle<Image> imageHandle;
        private readonly float[] m_uv;
        private Color m_DrawColor;

        /// <summary>
        /// Initializes a new instance of the <see cref="ImagePanel"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ImagePanel(Control parent)
            : base(parent)
        {
            m_uv = new float[4];
            SetUV(0, 0, 1, 1);
            MouseInputEnabled = false;
            m_DrawColor = Color.White;
        }

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        public override void Dispose()
        {
            //imageHandle.Resolve().Dispose();
            base.Dispose();
        }

        /// <summary>
        /// Sets the texture coordinates of the image.
        /// </summary>
        public virtual void SetUV(float u1, float v1, float u2, float v2)
        {
            m_uv[0] = u1;
            m_uv[1] = v1;
            m_uv[2] = u2;
            m_uv[3] = v2;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skins.Skin skin)
        {
            base.Render(skin);
            skin.Renderer.DrawColor = m_DrawColor;
            skin.Renderer.DrawTexturedRect(imageHandle, RenderBounds, m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
        }

        /// <summary>
        /// Sizes the control to its contents.
        /// </summary>
        public virtual void SizeToContents()
        {
            if(imageHandle.Id == ResourceHandle<Image>.Invalid)
                return;

            float texw = imageHandle.Resolve().Width;
            float texh = imageHandle.Resolve().Height;

            SetSize((int)texw, (int)texh);
        }

        public void SetImage(ResourceHandle<Image> imageHandle)
        {
            this.imageHandle = imageHandle;
            SizeToContents();
        }
    }
}
