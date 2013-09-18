using System;
using Flood;
using GUI.Controls;

namespace GUI.DragDrop
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
