using Flood.GUI.Controls;
using System;

namespace Flood.GUI.DragDrop
{
    public class Package
    {
        public String Name;
        public object UserData;
        public bool IsDraggable;
        public Control DrawControl;
        public Vector2i HoldOffset;
    }
}
