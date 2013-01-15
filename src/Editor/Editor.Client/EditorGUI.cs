using FlushEditor.GUI;
using FlushEditor.GUI.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor
{
     public class EditorGUI : IDisposable{

	    Gwen.Renderer.Base renderer;
	    Gwen.Skin.Base skin;

	    Gwen.Control.Canvas canvas;

        public void Dispose()
        {
            canvas.Dispose();
            renderer.Dispose();
            skin.Dispose();
        }

        public ToolBar ToolBar { get; private set; }

        public DocumentTab DocumentTab { get; private set; }

        public event Action Initiated;

        public void Init(Gwen.Renderer.Base renderer, string textureName)
        {
            this.renderer = renderer;

            skin = new Gwen.Skin.TexturedBase(renderer, textureName);
            //skin = new Gwen.Skin.Simple(renderer);
            //skin.DefaultFont = new Font(renderer, "Courier", 10);
            canvas = new Gwen.Control.Canvas(skin);

            ToolBar = new ToolBar(canvas);

            DocumentTab = new DocumentTab(canvas);

            var label = new Gwen.Control.Label(canvas);
            label.SetText("YOLO!");

            if (Initiated != null)
                Initiated.Invoke();
        }

	    public void Render(){
		    canvas.RenderCanvas();
	    }

        public Gwen.Control.Canvas Canvas
        {
            get { return canvas; }
        }

        
     }
}
