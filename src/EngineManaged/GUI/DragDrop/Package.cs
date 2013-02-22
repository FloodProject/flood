using System;
using System.Drawing;
using EngineManaged.GUI.Controls;

namespace EngineManaged.GUI.DragDrop
{
    public class Package
    {
        public String Name;
        public object UserData;
        public bool IsDraggable;
        public Base DrawControl;
        public Point HoldOffset;
    }
}
