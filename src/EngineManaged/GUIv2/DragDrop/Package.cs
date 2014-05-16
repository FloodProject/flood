using Flood.GUIv2.Controls;
using System;

namespace Flood.GUIv2.DragDrop
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
