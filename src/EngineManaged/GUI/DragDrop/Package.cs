using System;
using System.Drawing;
using Flood.GUI.Controls;

namespace Flood.GUI.DragDrop
{
    public class Package
    {
        public String Name;
        public object UserData;
        public bool IsDraggable;
        public Control DrawControl;
        public Point HoldOffset;
    }
}
