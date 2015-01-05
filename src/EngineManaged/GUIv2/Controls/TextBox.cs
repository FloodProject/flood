using Flood.GUIv2.ControlInternal;
using Flood.GUIv2.Input;
using System;
using Flood.GUIv2.Panels;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Text box (editable).
    /// </summary>
    public class TextBox : CompositeControl
    {
        private readonly Text m_Text;
        private readonly TextBoxInternal m_TextBoxInternal;

        /// <summary>
        /// Text alignment.
        /// </summary>
        public AlignmentFlags TextAlignment { get { return m_Text.Alignment; } set { m_Text.Alignment = value; Invalidate(); } }

        /// <summary>
        /// Text.
        /// </summary>
        public String Text { get { return m_Text.String; } set { SetText(value); } }

        /// <summary>
        /// Font.
        /// </summary>
        public Font Font
        {
            get { return m_Text.Font; }
            set
            {
                m_Text.Font = value;
                SizeToContents();
            }
        }

        /// <summary>
        /// Sets the textbox text.
        /// </summary>
        /// <param name="str">Text to set.</param>
        /// <param name="doEvents">Determines whether to invoke "text changed" event.</param>
        public virtual void SetText(String str, bool doEvents = true)
        {
            if (Text == str)
                return;

            m_Text.String = str;
            SizeToContents();

            if (doEvents)
                OnTextChanged();
        }
        /// <summary>
        /// Text color.
        /// </summary>
        public Color TextColor { get { return m_Text.TextColor; } set { m_Text.TextColor = value; } }

        /// <summary>
        /// Override text color (used by tooltips).
        /// </summary>
        public Color TextColorOverride { get { return m_Text.TextColorOverride; } set { m_Text.TextColorOverride = value; } }

        /// <summary>
        /// Text override - used to display different string.
        /// </summary>
        public String TextOverride { get { return m_Text.TextOverride; } set { m_Text.TextOverride = value; } }

        /// <summary>
        /// Width of the text (in pixels).
        /// </summary>
        public int TextWidth { get { return m_Text.Width; } }

        /// <summary>
        /// Height of the text (in pixels).
        /// </summary>
        public int TextHeight { get { return m_Text.Height; } }

        public int TextX { get { return m_Text.X; } }
        public int TextY { get { return m_Text.Y; } }

        /// <summary>
        /// Text length (in characters).
        /// </summary>
        public int TextLength { get { return m_Text.Length; } }
        public int TextRight { get { return m_Text.Right; } }
        public virtual void MakeColorNormal() { TextColor = Skin.Colors.Label.Default; }
        public virtual void MakeColorBright() { TextColor = Skin.Colors.Label.Bright; }
        public virtual void MakeColorDark() { TextColor = Skin.Colors.Label.Dark; }
        public virtual void MakeColorHighlight() { TextColor = Skin.Colors.Label.Highlight; }

        /// <summary>
        /// Gets the coordinates of specified character.
        /// </summary>
        /// <param name="index">Character index.</param>
        /// <returns>Character coordinates (local).</returns>
        public virtual Vector2 GetCharacterPosition(int index)
        {
            var p = m_Text.GetCharacterPosition(index);
            return new Vector2(p.X + m_Text.X, p.Y + m_Text.Y);
        }

        /// <summary>
        /// Returns index of the character closest to specified point (in canvas coordinates).
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        protected int GetClosestCharacter(int x, int y)
        {
            return m_Text.GetClosestCharacter(m_Text.CanvasPosToLocal(new Vector2i(x, y)));
        }

        /// <summary>
        /// Text margin.
        /// </summary>
        public Margin TextMargin { get { return m_Text.Margin; } set { m_Text.Margin = value; Invalidate(); } }

        public override bool AccelOnlyFocus { get { return true; } }
        public override bool NeedsInputChars { get { return true; } }

        /// <summary>
        /// Determines whether text should be selected when the control is focused.
        /// </summary>
        public bool SelectAllOnFocus { get { return m_TextBoxInternal.SelectAll; } set { m_TextBoxInternal.SelectAll = value; if (value) OnSelectAll(this); } }

        /// <summary>
        /// Indicates whether the text has active selection.
        /// </summary>
        public bool HasSelection { get { return m_TextBoxInternal.CursorPos != m_TextBoxInternal.CursorEnd; } }

        /// <summary>
        /// Invoked when the text has changed.
        /// </summary>
        public event Control.GwenEventHandler TextChanged;

        /// <summary>
        /// Invoked when the submit key has been pressed.
        /// </summary>
        public event Control.GwenEventHandler SubmitPressed;

        /// <summary>
        /// Current cursor position (character index).
        /// </summary>
        public int CursorPos
        {
            get { return m_TextBoxInternal.CursorPos; }
            set
            {
                if (m_TextBoxInternal.CursorPos == value) return;

                m_TextBoxInternal.CursorPos = value;
                RefreshCursorBounds();
            }
        }

        public int CursorEnd
        {
            get { return m_TextBoxInternal.CursorEnd; }
            set
            {
                if (m_TextBoxInternal.CursorEnd == value) return;

                m_TextBoxInternal.CursorEnd = value;
                RefreshCursorBounds();
            }
        }

        public virtual void SizeToContents()
        {
            if (Skin == null)
                return;
            PreLayout(Skin);
            InvalidateParent();
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
        public TextBox() : base(new DockPanel())
        {
            m_Text = new Text();
            m_TextBoxInternal = new TextBoxInternal();

            Panel.AddChild(m_Text);
            Panel.AddChild(m_TextBoxInternal);

            MouseInputEnabled = true;
            KeyboardInputEnabled = true;

            TextAlignment = AlignmentFlags.Left | AlignmentFlags.CenterVertical;
            TextMargin = new Margin(4, 2);



            TextColor = new Color(50, 50, 50, 255); // TODO: From Skin

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
        public override void RenderFocus(Skins.Skin skin)
        {
            // nothing
        }

        /// <summary>
        /// Handler for text changed event.
        /// </summary>
        protected void OnTextChanged()
        {
            if (m_TextBoxInternal.CursorPos > TextLength) m_TextBoxInternal.CursorPos = TextLength;
            if (m_TextBoxInternal.CursorEnd > TextLength) m_TextBoxInternal.CursorEnd = TextLength;

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
        public override bool OnChar(char chr)
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

            if (m_TextBoxInternal.CursorPos > TextLength)
                m_TextBoxInternal.CursorPos = TextLength;

            if (!IsTextAllowed(text, m_TextBoxInternal.CursorPos))
                return;

            String str = Text;
            str = str.Insert(m_TextBoxInternal.CursorPos, text);
            SetText(str);

            m_TextBoxInternal.CursorPos += text.Length;
            m_TextBoxInternal.CursorEnd = m_TextBoxInternal.CursorPos;

            RefreshCursorBounds();
        }



        protected virtual void RefreshCursorBounds()
        {
            m_TextBoxInternal.LastInputTime = Platform.Neutral.GetTimeInSeconds();

            MakeCaretVisible();

            var pA = GetCharacterPosition(m_TextBoxInternal.CursorPos);
            var pB = GetCharacterPosition(m_TextBoxInternal.CursorEnd);

            var x = (int)Math.Min(pA.X, pB.X);
            var y = TextY - 1;
            var w = (int)(Math.Max(pA.X, pB.X) - m_TextBoxInternal.SelectionBounds.X + 0.5);
            var h = TextHeight + 2;
            m_TextBoxInternal.SelectionBounds = new Rectangle(x, y, w, h);
           
            x = (int)pA.X;
            y = TextY - 1;
            w = 1;
            h = TextHeight + 2;

            m_TextBoxInternal.CaretBounds = new Rectangle(x, y, w, h);

            Redraw();
        }

        /// <summary>
        /// Handler for Paste event.
        /// </summary>
        /// <param name="from">Source control.</param>
        public override void OnPaste(IControl from)
        {
            base.OnPaste(from);
            InsertText(Platform.Neutral.GetClipboardText());
        }

        /// <summary>
        /// Handler for Copy event.
        /// </summary>
        /// <param name="from">Source control.</param>
        public override void OnCopy(IControl from)
        {
            if (!HasSelection) return;
            base.OnCopy(from);

            Platform.Neutral.SetClipboardText(GetSelection());
        }

        /// <summary>
        /// Handler for Cut event.
        /// </summary>
        /// <param name="from">Source control.</param>
        public override void OnCut(IControl from)
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
        public override void OnSelectAll(IControl from)
        {

            base.OnSelectAll(from);
            m_TextBoxInternal.CursorEnd = 0;
            m_TextBoxInternal.CursorPos = TextLength;

            RefreshCursorBounds();
        }

        /// <summary>
        /// Handler invoked on mouse double click (left) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        public override void OnMouseDoubleClickedLeft(int x, int y)
        {

            base.OnMouseDoubleClickedLeft(x, y);
            OnSelectAll(this);
        }

        /// <summary>
        /// Handler for Return keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        public override bool OnKeyReturn(bool down)
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
        public override bool OnKeyBackspace(bool down)
        {

            base.OnKeyBackspace(down);

            if (!down) return true;
            if (HasSelection)
            {
                EraseSelection();
                return true;
            }

            if (m_TextBoxInternal.CursorPos == 0) return true;

            DeleteText(m_TextBoxInternal.CursorPos - 1, 1);

            return true;
        }

        /// <summary>
        /// Handler for Delete keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        public override bool OnKeyDelete(bool down)
        {

            base.OnKeyDelete(down);
            if (!down) return true;
            if (HasSelection)
            {
                EraseSelection();
                return true;
            }

            if (m_TextBoxInternal.CursorPos >= TextLength) return true;

            DeleteText(m_TextBoxInternal.CursorPos, 1);

            return true;
        }

        /// <summary>
        /// Handler for Left Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        public override bool OnKeyLeft(bool down)
        {

            base.OnKeyLeft(down);
            if (!down) return true;

            if (m_TextBoxInternal.CursorPos > 0)
                m_TextBoxInternal.CursorPos--;

            if (!Input.InputHandler.IsShiftDown)
            {
                m_TextBoxInternal.CursorEnd = m_TextBoxInternal.CursorPos;
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
        public override bool OnKeyRight(bool down)
        {

            base.OnKeyRight(down);
            if (!down) return true;

            if (m_TextBoxInternal.CursorPos < TextLength)
                m_TextBoxInternal.CursorPos++;

            if (!Input.InputHandler.IsShiftDown)
            {
                m_TextBoxInternal.CursorEnd = m_TextBoxInternal.CursorPos;
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
        public override bool OnKeyHome(bool down)
        {

            base.OnKeyHome(down);
            if (!down) return true;
            m_TextBoxInternal.CursorPos = 0;

            if (!Input.InputHandler.IsShiftDown)
            {
                m_TextBoxInternal.CursorEnd = m_TextBoxInternal.CursorPos;
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
        public override bool OnKeyEnd(bool down)
        {
            base.OnKeyEnd(down);
            m_TextBoxInternal.CursorPos = TextLength;

            if (!Input.InputHandler.IsShiftDown)
            {
                m_TextBoxInternal.CursorEnd = m_TextBoxInternal.CursorPos;
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

            int start = Math.Min(m_TextBoxInternal.CursorPos, m_TextBoxInternal.CursorEnd);
            int end = Math.Max(m_TextBoxInternal.CursorPos, m_TextBoxInternal.CursorEnd);

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

            if (m_TextBoxInternal.CursorPos > startPos)
            {
                CursorPos = m_TextBoxInternal.CursorPos - length;
            }

            CursorEnd = m_TextBoxInternal.CursorPos;
        }

        /// <summary>
        /// Deletes selected text.
        /// </summary>
        public virtual void EraseSelection()
        {
            int start = Math.Min(m_TextBoxInternal.CursorPos, m_TextBoxInternal.CursorEnd);
            int end = Math.Max(m_TextBoxInternal.CursorPos, m_TextBoxInternal.CursorEnd);

            DeleteText(start, end - start);

            // Move the cursor to the start of the selection, 
            // since the end is probably outside of the string now.
            m_TextBoxInternal.CursorPos = start;
            m_TextBoxInternal.CursorEnd = start;
        }

        /// <summary>
        /// Handler invoked on mouse click (left) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="down">If set to <c>true</c> mouse button is down.</param>
        public override void OnMouseClickedLeft(int x, int y, bool down)
        {

            base.OnMouseClickedLeft(x, y, down);
            if (m_TextBoxInternal.SelectAll)
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
        public override void OnMouseMoved(int x, int y, int dx, int dy)
        {

            base.OnMouseMoved(x, y, dx, dy);
            if (InputHandler.MouseFocus != this) return;

            int c = GetClosestCharacter(x, y);

            CursorPos = c;
        }

        protected virtual void MakeCaretVisible()
        {
            //todo: figure out what this is supposed to do and fix it
            //int caretPos = (int)GetCharacterPosition(m_TextBoxInternal.CursorPos).X - TextX;

            //// If the caret is already in a semi-good position, leave it.
            //{
            //    int realCaretPos = caretPos + TextX;
            //    if (realCaretPos > Width * 0.1f && realCaretPos < Width * 0.9f)
            //        return;
            //}

            //// The ideal position is for the caret to be right in the middle
            //int idealx = (int)(-caretPos + Width * 0.5f);

            //// Don't show too much whitespace to the right
            //if (idealx + TextWidth < Width - TextMargin.Right)
            //    idealx = -TextWidth + (Width - TextMargin.Right);

            //// Or the left
            //if (idealx > TextMargin.Left)
            //    idealx = TextMargin.Left;

            //SetTextPosition(idealx, TextY);
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void PreLayout(Skins.Skin skin)
        {

            base.PreLayout(skin);

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
