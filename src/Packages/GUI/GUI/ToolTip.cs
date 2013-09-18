using GUI.Controls;

namespace GUI
{
    /// <summary>
    /// Tooltip handling.
    /// </summary>
    public static class ToolTip
    {
        private static Control g_ToolTip;

        /// <summary>
        /// Enables tooltip display for the specified control.
        /// </summary>
        /// <param name="control">Target control.</param>
        public static void Enable(Control control)
        {
            if (null == control.ToolTip)
                return;

            g_ToolTip = control;
        }

        /// <summary>
        /// Disables tooltip display for the specified control.
        /// </summary>
        /// <param name="control">Target control.</param>
        public static void Disable(Control control)
        {
            if (g_ToolTip == control)
            {
                g_ToolTip = null;
            }
        }

        /// <summary>
        /// Disables tooltip display for the specified control.
        /// </summary>
        /// <param name="control">Target control.</param>
        public static void ControlDeleted(Control control)
        {
            Disable(control);
        }

        /// <summary>
        /// Renders the currently visible tooltip.
        /// </summary>
        /// <param name="skin"></param>
        public static void RenderToolTip(Skins.Skin skin)
        {
            if (null == g_ToolTip) return;

            Renderers.Renderer render = skin.Renderer;

            var oldRenderOffset = render.RenderOffset;
            var mousePos = Input.InputHandler.MousePosition;
            var bounds = g_ToolTip.ToolTip.Bounds;

            var offset = Util.FloatRect(mousePos.X - bounds.Width*0.5f, mousePos.Y - bounds.Height - 10,
                                                 bounds.Width, bounds.Height);
            offset = Util.ClampRectToRect(offset, g_ToolTip.GetCanvas().Bounds);

            //Calculate offset on screen bounds
            render.AddRenderOffset(offset);
            render.EndClip();

            skin.DrawToolTip(g_ToolTip.ToolTip);
            g_ToolTip.ToolTip.DoRender(skin);

            render.RenderOffset = oldRenderOffset;
        }
    }
}
