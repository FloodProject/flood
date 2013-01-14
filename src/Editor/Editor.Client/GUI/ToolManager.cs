using Flood.Editor.GUI.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Editor.GUI
{
    class ToolManager : IPartImportsSatisfiedNotification
    {
        [ImportMany(AllowRecomposition = true)]
        Lazy<EditorTool>[] editorTools;

        [ImportMany(AllowRecomposition = true)]
        Lazy<DocumentTool>[] documentTools;

        [Import]
        EditorGUI editorGUI;

        [Import]
        DocumentManager documentManager;

        bool initiated = false;

        public void OnImportsSatisfied()
        {
            if (!initiated)
            {
                documentManager.DocumentSelected += OnDocumentSelect;
                initiated = true;
            }

            PopulateToolBar();
        }

        public void PopulateToolBar()
        {
            var toolBar = editorGUI.ToolBar;
            toolBar.Clear();
            if (toolBar != null)
            {
                foreach (var toolLazy in editorTools)
                {
                    var tool = toolLazy.Value;
                    if (tool is BarTool)
                    {
                        BarTool t = (BarTool)tool;
                        toolBar.AddTool(t.Text, t.OnSelect);
                    }

                    if (tool is DockableTool)
                    {
                        DockableTool t = (DockableTool)tool;
                        t.InitControl(editorGUI.Canvas);
                    }
                }
            }
        }

        public void OnDocumentSelect(Document doc)
        {

        }
    }
}
