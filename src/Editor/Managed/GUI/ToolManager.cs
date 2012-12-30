using FlushEditor.GUI.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor.GUI
{
    class ToolManager : IPartImportsSatisfiedNotification
    {
        [ImportMany(AllowRecomposition = true)]
        Lazy<Tool,ToolMetadata>[] tools;

        [Import]
        EditorGUI editorGUI;

        public void OnImportsSatisfied()
        {
            var toolBar = editorGUI.ToolBar;

            if (toolBar != null)
            {
                foreach (var toolLazy in tools)
                {
                    var tool = toolLazy.Value;
                    if (tool is SelectableTool)
                    {
                        SelectableTool t = (SelectableTool) tool;
                        toolBar.AddTool(toolLazy.Metadata.Text, t.OnSelect);
                    }

                    if (tool is DockableTool)
                    {
                        DockableTool t = (DockableTool) tool;
                        t.InitControl(editorGUI.Canvas);
                    }
                }
            }
        }
    }
}
