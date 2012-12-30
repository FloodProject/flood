using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor.GUI
{
    public interface ToolMetadata
    {
        string Text { get; }
        string Image { get; }
        Type[] TargetDocuments { get; }
    }

    [MetadataAttribute]
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false)]
    public class ToolAttribute : ExportAttribute, ToolMetadata
    {
        public ToolAttribute() : base(typeof(Tool)) { }
        public string Text { get; set; }
        public string Image { get; set; }
        public Type[] TargetDocuments { get; set; }

    }


    interface Tool
    {

    }

    interface SelectableTool : Tool
    {
        void OnSelect();
    }

    interface ToggableTool : SelectableTool
    {
        void OnDeselect();
    }

    interface DockableTool : Tool
    {
        void InitControl(Gwen.Control.Base parent);
    }
}
