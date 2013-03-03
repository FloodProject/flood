using System;
using Editor.Client.GUI;

namespace Editor.Client.Gui
{
    /// <summary>
    /// Window composed exclusively by a Container.
    /// </summary>
    public class PaneWindow : GuiWindow
    {
        public Container Container { get; private set; }

        public PaneWindow()
        {
            throw new NotImplementedException();
        }

        protected override void Init()
        {
            throw new NotImplementedException();
        }
    }
}
