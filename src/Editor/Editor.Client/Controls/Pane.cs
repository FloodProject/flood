
using Flood.GUI.Controls;

namespace Flood.Editor.Controls
{
    /// <summary>
    /// 
    /// </summary>
    public abstract class Pane : EditorTool, DockableTool 
    {
        public abstract void InitControl(Control parent);
    }
}