using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUI;
using Flood.GUI.Controls;

namespace Editor.Client.GUI
{
    /// <summary>
    /// Display multiple Controls side by side in an axis.
    /// </summary>
    public class Container : Control
    {
        private Control splitter;

        public bool IsHorizontal
        {
            get { return splitter is HorizontalSplitter; }
            set
            {
                Control newSplitter = null;
                if(value){
                    if (!(splitter is HorizontalSplitter))
                        newSplitter = new HorizontalSplitter(this);
                }else{
                    if (!(splitter is VerticalSplitter))
                        newSplitter = new HorizontalSplitter(this);
                }

                if(newSplitter == null)
                    return;

                newSplitter.Dock = Pos.Fill;
                if(splitter != null)
                {
                    foreach (var child in splitter.Children)
                        child.Parent = newSplitter;
                }
                splitter = newSplitter;
            }
        }

        public Container(Control parent) : base(parent)
        {
            IsHorizontal = true;
            var p1 = new PaneGroup(splitter);
            p1.AddPane(new Pane());
            p1.AddPane(new Pane());

            var b2 = new Button(splitter);

            var sH = splitter as HorizontalSplitter;
            if(sH!=null)
            {
                sH.SetPanel(0, p1);
                sH.SetPanel(1, b2);
            }
        }

    }
}
