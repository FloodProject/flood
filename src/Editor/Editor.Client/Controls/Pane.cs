
using EngineManaged.GUI.Control;

namespace Flood.Editor.Controls
{
    /// <summary>
    /// 
    /// </summary>
    public abstract class Pane : EditorTool, DockableTool 
    {
        public abstract void InitControl(Base parent);
    }
}