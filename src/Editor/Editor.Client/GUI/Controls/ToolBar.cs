using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Editor.Client.Extensions;
using Flood.GUI;
using Flood.GUI.Controls;
using Flood.GUI.Skins;
using Mono.Addins;

namespace Editor.Client.GUI.Controls
{
    class ToolBar : Control
    {
        private string extensionPoint;

        private const int lineHeight = 20;

        public ToolBar(Control parent, string extensionPoint) : base(parent)
        {
            this.extensionPoint = extensionPoint;

            RebuildToolbar();

            AddinManager.AddExtensionNodeHandler(extensionPoint, (s,a) => RebuildToolbar());
        }

        private void RebuildToolbar()
        {
            DeleteAllChildren();
            foreach (var tool in AddinManager.GetExtensionNodes(extensionPoint).Cast<ToolNode>())
            {
                var control = tool.CreateControl(this);
            }
            SizeToChildren(false,true);
        }

        protected override void PostLayout(Skin skin)
        {
            var  bounds = InnerBounds;
            var pos = new Point( bounds.X, bounds.Y );

            var right = 0;
            var line = 0;

            foreach (var pChild in Children)
            {
                if ( pChild.Dock != Pos.None ) continue;

                if(right + pChild.Width > Width)
                {
                    line++;
                    right = 0;
                }
                    
                pChild.SetPosition(right, line * lineHeight);

                right += pChild.Width;
            }
        }
    }
}
