using System;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
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
        public ImagePanel()
        {
            m_uv = new float[4];
            SetUV(0, 0, 1, 1);
            MouseInputEnabled = false;
            m_DrawColor = Color.White;
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

        public override bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            var oldWidth= Width;
            var oldHeight= Height;
            SizeToContents();

            return oldWidth > Width || oldHeight > Height;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
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
