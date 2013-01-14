using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Editor.GUI
{

    [InheritedExport]
    public abstract class EditorTool
    {

    }

    [InheritedExport]
    public abstract class DocumentTool
    {
        
    }

    interface BarTool
    {
        void OnSelect();
        string Text { get; }
        string Image { get; }
    }

    interface ToggableTool : BarTool
    {
        void OnDeselect();
    }

    interface DockableTool
    {
        void InitControl(Gwen.Control.Base parent);
    }
}
