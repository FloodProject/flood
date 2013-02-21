﻿using System;
using System.Drawing;
using Gwen.Input;

namespace Gwen.Control
{
    /// <summary>
    /// Text box (editable).
    /// </summary>
    public class TextBox : Label
    {
        private bool m_SelectAll;

        private int m_CursorPos;
        private int m_CursorEnd;

        private Rectangle m_SelectionBounds;
        private Rectangle m_CaretBounds;

        private float m_LastInputTime;

        protected override bool AccelOnlyFocus { get { return true; } }
        protected override bool NeedsInputChars { get { return true; } }

        /// <summary>
        /// Determines whether text should be selected when the control is focused.
        /// </summary>
        public bool SelectAllOnFocus { get { return m_SelectAll; } set { m_SelectAll = value; if (value) OnSelectAll(this); } }

        /// <summary>
        /// Indicates whether the text has active selection.
        /// </summary>
        public bool HasSelection { get { return m_CursorPos != m_CursorEnd; } }

        /// <summary>
        /// Invoked when the text has changed.
        /// </summary>
        public event GwenEventHandler TextChanged;

        /// <summary>
        /// Invoked when the submit key has been pressed.
        /// </summary>
        public event GwenEventHandler SubmitPressed;

        /// <summary>
        /// Current cursor position (character index).
        /// </summary>
        public int CursorPos
        {
            get { return m_CursorPos; }
            set
            {
                if (m_CursorPos == value) return;

                m_CursorPos = value;
                RefreshCursorBounds();
            }
        }

        public int CursorEnd
        {
            get { return m_CursorEnd; }
            set
            {
                if (m_CursorEnd == value) return;

                m_CursorEnd = value;
                RefreshCursorBounds();
            }
        }

        /// <summary>
        /// Determines whether the control can insert text at a given cursor position.
        /// </summary>
        /// <param name="text">Text to check.</param>
        /// <param name="position">Cursor position.</param>
        /// <returns>True if allowed.</returns>
        protected virtual bool IsTextAllowed(String text, int position)
        {
            return true;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="TextBox"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public TextBox(Base parent)
            : base(parent)
        {
            SetSize(200, 20);

            MouseInputEnabled = true;
            KeyboardInputEnabled = true;

            Alignment = Pos.Left | Pos.CenterV;
            TextPadding = new Padding(4, 2, 4, 2);

            m_CursorPos = 0;
            m_CursorEnd = 0;
            m_SelectAll = false;

            TextColor = Color.FromArgb(255, 50, 50, 50); // TODO: From Skin

            IsTabable = true;

            AddAccelerator("Ctrl + C", OnCopy);
            AddAccelerator("Ctrl + X", OnCut);
            AddAccelerator("Ctrl + V", OnPaste);
            AddAccelerator("Ctrl + A", OnSelectAll);
        }

        /// <summary>
        /// Renders the focus overlay.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void RenderFocus(Skin.Base skin)
        {
            // nothing
        }

        /// <summary>
        /// Handler for text changed event.
        /// </summary>
        protected override void OnTextChanged()
        {
            base.OnTextChanged();

            if (m_CursorPos > TextLength) m_CursorPos = TextLength;
            if (m_CursorEnd > TextLength) m_CursorEnd = TextLength;

            if (TextChanged != null)
                TextChanged.Invoke(this);
        }

        /// <summary>
        /// Handler for character input event.
        /// </summary>
        /// <param name="chr">Character typed.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnChar(char chr)
        {
            base.OnChar(chr);

            if (chr == '\t') return false;

            InsertText(chr.ToString());
            return true;
        }

        /// <summary>
        /// Inserts text at current cursor position, erasing selection if any.
        /// </summary>
        /// <param name="text">Text to insert.</param>
        void InsertText(String text)
        {
            // TODO: Make sure fits (implement maxlength)

            if (HasSelection)
            {
                EraseSelection();
            }

            if (m_CursorPos > TextLength)
                m_CursorPos = TextLength;

            if (!IsTextAllowed(text, m_CursorPos))
                return;

            String str = Text;
            str = str.Insert(m_CursorPos, text);
            SetText(str);

            m_CursorPos += text.Length;
            m_CursorEnd = m_CursorPos;

            RefreshCursorBounds();
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            base.Render(skin);

            if (ShouldDrawBackground)
                skin.DrawTextBox(this);

            if (!HasFocus) return;

            // Draw selection.. if selected..
            if (m_CursorPos != m_CursorEnd)
            {
                skin.Renderer.DrawColor = Color.FromArgb(200, 50, 170, 255);
                skin.Renderer.DrawFilledRect(m_SelectionBounds);
            }

            // Draw caret
            float time = Platform.Neutral.GetTimeInSeconds() - m_LastInputTime;

            if ((time % 1.0f) <= 0.5f)
            {
                skin.Renderer.DrawColor = Color.Black;
                skin.Renderer.DrawFilledRect(m_CaretBounds);
            }
        }

        protected virtual void RefreshCursorBounds()
        {
            m_LastInputTime = Platform.Neutral.GetTimeInSeconds();

            MakeCaretVisible();

            Point pA = GetCharacterPosition(m_CursorPos);
            Point pB = GetCharacterPosition(m_CursorEnd);

            m_SelectionBounds.X = Math.Min(pA.X, pB.X);
            m_SelectionBounds.Y = TextY - 1;
            m_SelectionBounds.Width = Math.Max(pA.X, pB.X) - m_SelectionBounds.X;
            m_SelectionBounds.Height = TextHeight + 2;

            m_CaretBounds.X = pA.X;
            m_CaretBounds.Y = TextY - 1;
            m_CaretBounds.Width = 1;
            m_CaretBounds.Height = TextHeight + 2;

            Redraw();
        }

        /// <summary>
        /// Handler for Paste event.
        /// </summary>
        /// <param name="from">Source control.</param>
        protected override void OnPaste(Base from)
        {
            base.OnPaste(from);
            InsertText(Platform.Neutral.GetClipboardText());
        }

        /// <summary>
        /// Handler for Copy event.
        /// </summary>
        /// <param name="from">Source control.</param>
        protected override void OnCopy(Base from)
        {
            if (!HasSelection) return;
            base.OnCopy(from);

            Platform.Neutral.SetClipboardText(GetSelection());
        }

        /// <summary>
        /// Handler for Cut event.
        /// </summary>
        /// <param name="from">Source control.</param>
        protected override void OnCut(Base from)
        {
            if (!HasSelection) return;
            base.OnCut(from);

            Platform.Neutral.SetClipboardText(GetSelection());
            EraseSelection();
        }

        /// <summary>
        /// Handler for Select All event.
        /// </summary>
        /// <param name="from">Source control.</param>
        protected override void OnSelectAll(Base from)
        {
            //base.OnSelectAll(from);
            m_CursorEnd = 0;
            m_CursorPos = TextLength;

            RefreshCursorBounds();
        }

        /// <summary>
        /// Handler invoked on mouse double click (left) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        protected override void OnMouseDoubleClickedLeft(int x, int y)
        {
            //base.OnMouseDoubleClickedLeft(x, y);
            OnSelectAll(this);
        }

        /// <summary>
        /// Handler for Return keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyReturn(bool down)
        {
            base.OnKeyReturn(down);
            if (down) return true;

            OnReturn();

            // Try to move to the next control, as if tab had been pressed
            OnKeyTab(true);

            // If we still have focus, blur it.
            if (HasFocus)
            {
                Blur();
            }

            return true;
        }

        /// <summary>
        /// Handler for Backspace keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyBackspace(bool down)
        {
            base.OnKeyBackspace(down);

            if (!down) return true;
            if (HasSelection)
            {
                EraseSelection();
                return true;
            }

            if (m_CursorPos == 0) return true;

            DeleteText(m_CursorPos - 1, 1);

            return true;
        }

        /// <summary>
        /// Handler for Delete keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyDelete(bool down)
        {
            base.OnKeyDelete(down);
            if (!down) return true;
            if (HasSelection)
            {
                EraseSelection();
                return true;
            }

            if (m_CursorPos >= TextLength) return true;

            DeleteText(m_CursorPos, 1);

            return true;
        }

        /// <summary>
        /// Handler for Left Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyLeft(bool down)
        {
            base.OnKeyLeft(down);
            if (!down) return true;

            if (m_CursorPos > 0)
                m_CursorPos--;

            if (!Input.InputHandler.IsShiftDown)
            {
                m_CursorEnd = m_CursorPos;
            }

            RefreshCursorBounds();
            return true;
        }

        /// <summary>
        /// Handler for Right Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyRight(bool down)
        {
            base.OnKeyRight(down);
            if (!down) return true;

            if (m_CursorPos < TextLength)
                m_CursorPos++;

            if (!Input.InputHandler.IsShiftDown)
            {
                m_CursorEnd = m_CursorPos;
            }

            RefreshCursorBounds();
            return true;
        }

        /// <summary>
        /// Handler for Home keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyHome(bool down)
        {
            base.OnKeyHome(down);
            if (!down) return true;
            m_CursorPos = 0;

            if (!Input.InputHandler.IsShiftDown)
            {
                m_CursorEnd = m_CursorPos;
            }

            RefreshCursorBounds();
            return true;
        }

        /// <summary>
        /// Handler for End keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyEnd(bool down)
        {
            base.OnKeyEnd(down);
            m_CursorPos = TextLength;

            if (!Input.InputHandler.IsShiftDown)
            {
                m_CursorEnd = m_CursorPos;
            }

            RefreshCursorBounds();
            return true;
        }

        /// <summary>
        /// Returns currently selected text.
        /// </summary>
        /// <returns>Current selection.</returns>
        public String GetSelection()
        {
            if (!HasSelection) return String.Empty;

            int start = Math.Min(m_CursorPos, m_CursorEnd);
            int end = Math.Max(m_CursorPos, m_CursorEnd);

            String str = Text;
            return str.Substring(start, end - start);
        }

        /// <summary>
        /// Deletes text.
        /// </summary>
        /// <param name="startPos">Starting cursor position.</param>
        /// <param name="length">Length in characters.</param>
        public virtual void DeleteText(int startPos, int length)
        {
            String str = Text;
            str = str.Remove(startPos, length);
            SetText(str);

            if (m_CursorPos > startPos)
            {
                CursorPos = m_CursorPos - length;
            }

            CursorEnd = m_CursorPos;
        }

        /// <summary>
        /// Deletes selected text.
        /// </summary>
        public virtual void EraseSelection()
        {
            int start = Math.Min(m_CursorPos, m_CursorEnd);
            int end = Math.Max(m_CursorPos, m_CursorEnd);

            DeleteText(start, end - start);

            // Move the cursor to the start of the selection, 
            // since the end is probably outside of the string now.
            m_CursorPos = start;
            m_CursorEnd = start;
        }

        /// <summary>
        /// Handler invoked on mouse click (left) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="down">If set to <c>true</c> mouse button is down.</param>
        protected override void OnMouseClickedLeft(int x, int y, bool down)
        {
            base.OnMouseClickedLeft(x, y, down);
            if (m_SelectAll)
            {
                OnSelectAll(this);
                //m_SelectAll = false;
                return;
            }

            int c = GetClosestCharacter(x, y);

            if (down)
            {
                CursorPos = c;

                if (!Input.InputHandler.IsShiftDown)
                    CursorEnd = c;

                InputHandler.MouseFocus = this;
            }
            else
            {
                if (InputHandler.MouseFocus == this)
                {
                    CursorPos = c;
                    InputHandler.MouseFocus = null;
                }
            }
        }

        /// <summary>
        /// Handler invoked on mouse moved event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="dx">X change.</param>
        /// <param name="dy">Y change.</param>
        protected override void OnMouseMoved(int x, int y, int dx, int dy)
        {
            base.OnMouseMoved(x, y, dx, dy);
            if (InputHandler.MouseFocus != this) return;

            int c = GetClosestCharacter(x, y);

            CursorPos = c;
        }

        protected virtual void MakeCaretVisible()
        {
            int caretPos = GetCharacterPosition(m_CursorPos).X - TextX;

            // If the caret is already in a semi-good position, leave it.
            {
                int realCaretPos = caretPos + TextX;
                if (realCaretPos > Width*0.1f && realCaretPos < Width*0.9f)
                    return;
            }

            // The ideal position is for the caret to be right in the middle
            int idealx = (int)(-caretPos + Width * 0.5f);

            // Don't show too much whitespace to the right
            if (idealx + TextWidth < Width - TextPadding.Right)
                idealx = -TextWidth + (Width - TextPadding.Right);

            // Or the left
            if (idealx > TextPadding.Left)
                idealx = TextPadding.Left;

            SetTextPosition(idealx, TextY);
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skin.Base skin)
        {
            base.Layout(skin);

            RefreshCursorBounds();
        }

        /// <summary>
        /// Handler for the return key.
        /// </summary>
        protected virtual void OnReturn()
        {
            if (SubmitPressed != null)
                SubmitPressed.Invoke(this);
        }
    }
}
