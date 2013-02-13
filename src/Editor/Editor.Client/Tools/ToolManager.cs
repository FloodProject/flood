using System;
using System.ComponentModel.Composition;

namespace Flood.Editor
{
    [Export]
    public class ToolManager : IPartImportsSatisfiedNotification
    {
        [ImportMany(AllowRecomposition = true)]
        Lazy<EditorTool>[] editorTools;

        [ImportMany(AllowRecomposition = true)]
        Lazy<DocumentTool>[] documentTools;

        [Import]
        EditorWindow editorWindow;

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

            if(editorWindow.ToolBar != null)
            {
                PopulateToolBar();
            } else
            {
                editorWindow.GUIInitiated += PopulateToolBar;
            }
        }

        public void PopulateToolBar()
        { 
            var toolBar = editorWindow.ToolBar;
            toolBar.Clear();

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
                    t.InitControl(editorWindow.Canvas);
                }
            }
        }

        public void OnDocumentSelect(Document doc)
        {

        }
    }
}
