using System;
using System.Drawing;
using System.Linq;
using System.Text;
using Gwen.Control;
using Gwen.DragDrop;
using Gwen.Containers;

namespace Gwen.Input
{
    /// <summary>
    /// Input handling.
    /// </summary>
    public class InputHandler
    {
        
        private readonly KeyData m_KeyData;


        private readonly float[] m_LastClickTime;
        private Point m_LastClickPos;

        //Singleton
        private static InputHandler instance;
        public static InputHandler Instance { 
            get { 
                if (instance == null) 
                    instance = new InputHandler(); 
                return instance; 
            } 
        }
        private InputHandler() {
            m_KeyData = new KeyData();
            m_LastClickTime = new float[MaxMouseButtons];
        }


        /// <summary>
        /// Control currently hovered by mouse.
        /// </summary>
        public Base HoveredControl;

        /// <summary>
        /// Control that corrently has keyboard focus.
        /// </summary>
        public Base KeyboardFocus;

        /// <summary>
        /// Control that currently has mouse focus.
        /// </summary>
        public Base MouseFocus;

        /// <summary>
        /// Maximum number of mouse buttons supported.
        /// </summary>
        public int MaxMouseButtons { get { return 5; } }

        /// <summary>
        /// Maximum time in seconds between mouse clicks to be recognized as double click.
        /// </summary>
        public float DoubleClickSpeed { get { return 0.5f; } }

        /// <summary>
        /// Time in seconds between autorepeating of keys.
        /// </summary>
        public float KeyRepeatRate { get { return 0.03f; } }

        /// <summary>
        /// Time in seconds before key starts to autorepeat.
        /// </summary>
        public float KeyRepeatDelay { get { return 0.5f; } }

        /// <summary>
        /// Indicates whether the left mouse button is down.
        /// </summary>
        public bool IsLeftMouseDown { get { return m_KeyData.LeftMouseDown; } }

        /// <summary>
        /// Indicates whether the right mouse button is down.
        /// </summary>
        public bool IsRightMouseDown { get { return m_KeyData.RightMouseDown; } }
        
        /// <summary>
        /// Current mouse position.
        /// </summary>
        public PointImmutable MousePosition { get; private set; }

        /// <summary>
        /// Indicates whether the shift key is down.
        /// </summary>
        public bool IsShiftDown { get { return IsKeyDown(Key.Shift); } }

        /// <summary>
        /// Indicates whether the control key is down.
        /// </summary>
        public bool IsControlDown { get { return IsKeyDown(Key.Control); } }

        /// <summary>
        /// Checks if the given key is pressed.
        /// </summary>
        /// <param name="key">Key to check.</param>
        /// <returns>True if the key is down.</returns>
        public bool IsKeyDown(Key key)
        {
            return m_KeyData.KeyState[(int)key];
        }

        /// <summary>
        /// Handles copy, paste etc.
        /// </summary>
        /// <param name="canvas">Canvas.</param>
        /// <param name="chr">Input character.</param>
        /// <returns>True if the key was handled.</returns>
        public bool DoSpecialKeys(Base canvas, char chr)
        {
            if (null == KeyboardFocus) return false;
            if (KeyboardFocus.GetCanvas() != canvas) return false;
            if (!KeyboardFocus.IsVisible) return false;
            if (!IsControlDown) return false;

            if (chr == 'C' || chr == 'c')
            {
                KeyboardFocus.InputCopy(null);
                return true;
            }

            if (chr == 'V' || chr == 'v')
            {
                KeyboardFocus.InputPaste(null);
                return true;
            }

            if (chr == 'X' || chr == 'x')
            {
                KeyboardFocus.InputCut(null);
                return true;
            }

            if (chr == 'A' || chr == 'a')
            {
                KeyboardFocus.InputSelectAll(null);
                return true;
            }

            return false;
        }

        /// <summary>
        /// Handles accelerator input.
        /// </summary>
        /// <param name="canvas">Canvas.</param>
        /// <param name="chr">Input character.</param>
        /// <returns>True if the key was handled.</returns>
        public bool HandleAccelerator(Base canvas, char chr)
        {
            //Build the accelerator search string
            StringBuilder accelString = new StringBuilder();
            if (IsControlDown)
                accelString.Append("CTRL+");
            if (IsShiftDown)
                accelString.Append("SHIFT+");
            // [omeg] todo: alt?

            accelString.Append(chr);
            String acc = accelString.ToString();

            //Debug::Msg("Accelerator string :%S\n", accelString.c_str());)

            if (KeyboardFocus != null && KeyboardFocus.HandleAccelerator(acc))
                return true;

            if (MouseFocus != null && MouseFocus.HandleAccelerator(acc))
                return true;

            if (canvas.HandleAccelerator(acc))
                return true;

            return false;
        }

        /// <summary>
        /// Mouse moved handler.
        /// </summary>
        /// <param name="canvas">Canvas.</param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="dx"></param>
        /// <param name="dy"></param>
        public void OnMouseMoved(Base canvas, int x, int y, int dx, int dy)
        {
            // Send input to canvas for study		
            MousePosition = new PointImmutable(x,y);

            var container = canvas as Container;
            if(container!=null)
                UpdateHoveredControl(container);
        }

        /// <summary>
        /// Handles focus updating and key autorepeats.
        /// </summary>
        /// <param name="control">Unused.</param>
        public void OnCanvasThink(Base control)
        {
            if (MouseFocus != null && !MouseFocus.IsVisible)
                MouseFocus = null;

            if (KeyboardFocus != null && (!KeyboardFocus.IsVisible || !KeyboardFocus.KeyboardInputEnabled))
                KeyboardFocus = null;

            if (null == KeyboardFocus) return;
            if (KeyboardFocus.GetCanvas() != control) return;

            float time = Platform.Neutral.GetTimeInSeconds();

            //
            // Simulate Key-Repeats
            //
            for (int i = 0; i < (int)Key.Count; i++)
            {
                if (m_KeyData.KeyState[i] && m_KeyData.Target != KeyboardFocus)
                {
                    m_KeyData.KeyState[i] = false;
                    continue;
                }

                if (m_KeyData.KeyState[i] && time > m_KeyData.NextRepeat[i])
                {
                    m_KeyData.NextRepeat[i] = Platform.Neutral.GetTimeInSeconds() + KeyRepeatRate;

                    if (KeyboardFocus != null)
                    {
                        KeyboardFocus.InputKeyPressed((Key)i);
                    }
                }
            }
        }


        //TODO use property changed instead of this hack
        public event EventHandler MouseClicked;

        /// <summary>
        /// Mouse click handler.
        /// </summary>
        /// <param name="canvas">Canvas.</param>
        /// <param name="mouseButton">Mouse button number.</param>
        /// <param name="down">Specifies if the button is down.</param>
        /// <returns>True if handled.</returns>
        public bool OnMouseClicked(Base canvas, int mouseButton, bool down)
        {
            

            // If we click on a control that isn't a menu we want to close
            // all the open menus. Menus are children of the canvas.
            if (down && (null == HoveredControl || !HoveredControl.IsMenuComponent))
            {
                canvas.CloseMenus();
            }

            if (null == HoveredControl) return false;
            if (HoveredControl.GetCanvas() != canvas) return false;
            if (!HoveredControl.IsVisible) return false;
            if (HoveredControl == canvas) return false;

            if (mouseButton > MaxMouseButtons)
                return false;

            if (mouseButton == 0) 
                m_KeyData.LeftMouseDown = down;
            else if (mouseButton == 1) 
                m_KeyData.RightMouseDown = down;

            // Double click.
            // Todo: Shouldn't double click if mouse has moved significantly
            bool isDoubleClick = false;

            if (down &&
                m_LastClickPos.X == MousePosition.X &&
                m_LastClickPos.Y == MousePosition.Y &&
                (Platform.Neutral.GetTimeInSeconds() - m_LastClickTime[mouseButton]) < DoubleClickSpeed)
            {
                isDoubleClick = true;
            }

            if (down && !isDoubleClick)
            {
                m_LastClickTime[mouseButton] = Platform.Neutral.GetTimeInSeconds();
                m_LastClickPos = MousePosition;
            }

            if (down)
            {
                FindKeyboardFocus(HoveredControl);
            }

            HoveredControl.UpdateCursor();

            // This tells the child it has been touched, which
            // in turn tells its parents, who tell their parents.
            // This is basically so that Windows can pop themselves
            // to the top when one of their children have been clicked.
            if (down)
                HoveredControl.Touch();

            if (MouseClicked!=null)
                MouseClicked.Invoke(this, new EventArgs());

#if GWEN_HOOKSYSTEM
            if (bDown)
            {
                if (Hook::CallHook(&Hook::BaseHook::OnControlClicked, HoveredControl, MousePosition.x,
                                   MousePosition.y))
                    return true;
            }
#endif

            switch (mouseButton)
            {
                case 0:
                    {
                        if (DragAndDrop.OnMouseButton(HoveredControl, MousePosition.X, MousePosition.Y, down))
                            return true;

                        if (isDoubleClick)
                            HoveredControl.InputMouseDoubleClickedLeft(MousePosition.X, MousePosition.Y);
                        else
                            HoveredControl.InputMouseClickedLeft(MousePosition.X, MousePosition.Y, down);
                        return true;
                    }

                case 1:
                    {
                        if (isDoubleClick)
                            HoveredControl.InputMouseDoubleClickedRight(MousePosition.X, MousePosition.Y);
                        else
                            HoveredControl.InputMouseClickedRight(MousePosition.X, MousePosition.Y, down);
                        return true;
                    }
            }

            return false;
        }

        /// <summary>
        /// Key handler.
        /// </summary>
        /// <param name="canvas">Canvas.</param>
        /// <param name="key">Key.</param>
        /// <param name="down">True if the key is down.</param>
        /// <returns>True if handled.</returns>
        public bool OnKeyEvent(Base canvas, Key key, bool down)
        {
            if (null == KeyboardFocus) return false;
            if (KeyboardFocus.GetCanvas() != canvas) return false;
            if (!KeyboardFocus.IsVisible) return false;

            int iKey = (int)key;
            if (down)
            {
                if (!m_KeyData.KeyState[iKey])
                {
                    m_KeyData.KeyState[iKey] = true;
                    m_KeyData.NextRepeat[iKey] = Platform.Neutral.GetTimeInSeconds() + KeyRepeatDelay;
                    m_KeyData.Target = KeyboardFocus;

                    return KeyboardFocus.InputKeyPressed(key);
                }
            }
            else
            {
                if (m_KeyData.KeyState[iKey])
                {
                    m_KeyData.KeyState[iKey] = false;

                    // BUG BUG. This causes shift left arrow in textboxes
                    // to not work. What is disabling it here breaking?
                    //m_KeyData.Target = NULL;

                    return KeyboardFocus.InputKeyPressed(key, false);
                }
            }

            return false;
        }

        private void UpdateHoveredControl(Container inCanvas)
        {
            Base hovered = inCanvas.GetControlAt(MousePosition.X, MousePosition.Y);

            if (hovered != HoveredControl)
            {
                if (HoveredControl != null)
                {
                    var oldHover = HoveredControl;
                    HoveredControl = null;
                    oldHover.InputMouseLeft();
                }

                HoveredControl = hovered;

                if (HoveredControl != null)
                {
                    HoveredControl.InputMouseEntered();
                }
            }

            if (MouseFocus != null && MouseFocus.GetCanvas() == inCanvas)
            {
                if (HoveredControl != null)
                {
                    var oldHover = HoveredControl;
                    HoveredControl = null;
                    oldHover.Redraw();
                }
                HoveredControl = MouseFocus;
            }
        }

        private void FindKeyboardFocus(Base control)
        {
            if (null == control) return;
            if (control.KeyboardInputEnabled)
            {
                //Make sure none of our children have keyboard focus first - todo recursive
                if(control is Container){
                    if (((Container)control).Children.Any(child => child == KeyboardFocus))
                    {
                        return;
                    }
                }

                control.Focus();
                return;
            }

            FindKeyboardFocus(control.Parent);
            return;
        }
    }
}
