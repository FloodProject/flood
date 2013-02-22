using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUI.Controls;

namespace Flood.Editor
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
        void InitControl(Control parent);
    }
}
