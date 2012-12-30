using System;
using System.Drawing;
using Gwen.Control;

namespace Gwen.DragDrop
{
    public class Package
    {
        public String Name;
        public object data;
        public bool IsDraggable;
        public Base DrawControl;
        public Point HoldOffset;
    }
}
