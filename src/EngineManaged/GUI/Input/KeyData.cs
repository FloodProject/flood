using System;
using System.Collections.Generic;
using Flood.GUI.Controls;

namespace Flood.GUI.Input
{
    /// <summary>
    /// Keyboard state.
    /// </summary>
    public class KeyData
    {
        public readonly bool [] KeyState;
        public readonly float [] NextRepeat;
        public Control Target;
        public bool LeftMouseDown;
        public bool RightMouseDown;

        public KeyData()
        {
            KeyState = new bool[(int)Keys.MAX];
            NextRepeat = new float[(int)Keys.MAX];
            // everything is initialized to 0 by default
        }
    }
}
