using System;
using System.Globalization;
using Flood;
using TextAddin.Document;

namespace TextAddin.Controls
{

    public class CarretLayer : Layer
    {

        private TextAnchor textAnchor;
        private int oldColumn;

        private TextDocument Document { 
            get { return TextView.TextLayer.Document; }
        }

        public CarretLayer(TextView parent) : base(parent)
        {
            KeyboardInputEnabled = true;

            SetTextAnchor(1);
        }

        private void SetTextAnchor(int line, int column)
        {
            SetTextAnchor(Document.GetOffset(line,column));
        }

        private void SetTextAnchor(TextLocation textLocation)
        {
            SetTextAnchor(Document.GetOffset(textLocation));
        }

        private void SetTextAnchor(int offset)
        {
            if(textAnchor != null)
                textAnchor.Deleted -= OnAnchorDeleted;
            textAnchor = Document.CreateAnchor(offset);
            textAnchor.Deleted += OnAnchorDeleted;
        }

        private void OnAnchorDeleted(object obj, EventArgs args)
        {
            var line = Document.GetLineByNumber(textAnchor.Line);
            SetTextAnchor(line.LineNumber,1);
        }

        protected override void Render(Flood.GUI.Skins.Skin skin)
        {
            if (DateTime.Now.Millisecond < 500)
                return;

            var location = new TextLocation(textAnchor.Line,textAnchor.Column);
            var charPos = TextView.TextLayer.GetTextLocationPosition(location);

            var lineHeight = Skin.DefaultFont.Size;

            skin.Renderer.DrawColor = new Color(0,0,0,255);
            skin.Renderer.DrawFilledRect(new Rect((int)charPos.X,(int)charPos.Y,1,lineHeight));
        }

        private void SnapCarretToPosition(int x, int y)
        {
            var localPosToCanvas = CanvasPosToLocal(new Vector2i(x, y));
            var location = TextView.TextLayer.GetTextLocation(localPosToCanvas);
            SetTextAnchor(location);
            oldColumn = 0;
        }

        protected void InsertText(string text)
        {
            Document.Insert(textAnchor.Offset, text);

            oldColumn = 0;
        }

        protected void RemoveText(int offset,int length)
        {
            offset = textAnchor.Offset + offset;
            offset = Math.Max(0, offset);
            offset = Math.Min(offset, Document.TextLength);
            
            var maxLength = Document.TextLength - offset;
            length = Math.Min(length, maxLength);

            Document.Remove(offset, length);

            oldColumn = 0;
        }


        private void UpdateCarretOffset(int dOffset)
        {
            var line = Document.GetLineByNumber(textAnchor.Line);
            var maxOffset = Document.TextLength;
            var sign = Math.Sign(dOffset);
            while (textAnchor.Offset + dOffset > line.EndOffset || textAnchor.Offset + dOffset < line.Offset)
            {
                if(textAnchor.Offset + dOffset < 0 || textAnchor.Offset + dOffset > maxOffset)
                    break;

                var oldOffset = textAnchor.Offset;
                UpdateCarretLine(sign);
                line = Document.GetLineByNumber(textAnchor.Line);
                
                if(sign < 0)
                    SetTextAnchor(line.EndOffset);

                dOffset -= textAnchor.Offset - oldOffset;
            }
            
            var offset = textAnchor.Offset + dOffset;
            
            offset = Math.Max(0, offset);
            offset = Math.Min(offset, maxOffset);
            
            SetTextAnchor(offset);
            oldColumn = 0;
        }

        private void UpdateCarretLine(int lineOffset)
        {
            var column = textAnchor.Column;

            var i = Math.Abs(lineOffset);
            var lineNum = textAnchor.Line;
            var maxLineNum = Document.LineCount;
            while (i > 0)
            {
                lineNum += Math.Sign(lineOffset);

                if(lineNum < 1 || lineNum > maxLineNum)
                    break;

                if (!TextView.TextLayer.GetIsCollapsed(lineNum))
                    i--;
            }

            if(oldColumn == 0)
                oldColumn = column;

            if (lineNum < 1)
            {
                lineNum = 1;
                column = 1;
            }
            else if (lineNum > maxLineNum)
            {
                lineNum = maxLineNum;
                var line = Document.GetLineByNumber(lineNum);
                column = line.Length+1;
            }
            else
            {
                var line = Document.GetLineByNumber(lineNum);
                column = Math.Min(oldColumn, line.Length+1);
            }

            SetTextAnchor(lineNum,column);
        }

        #region Input

        private bool mouseDown = false;

        protected override void OnMouseClickedLeft(int x, int y, bool down)
        {
            mouseDown = down;
            SnapCarretToPosition(x, y);
        }

        protected override void OnMouseMoved(int x, int y, int dx, int dy)
        {
            if (mouseDown)
                SnapCarretToPosition(x, y);
        }
 
        protected override bool OnChar(char chr)
        {
            InsertText(chr.ToString(CultureInfo.InvariantCulture));
            return true;
        }

        protected override bool OnKeyBackspace(bool down)
        {
            if(!down)
                return false;

            if(textAnchor.Offset == 0)
                return true;

            UpdateCarretOffset(-1);
            RemoveText(0, 1);

            return true;
        }

        protected override bool OnKeyDelete(bool down)
        {
            if(!down)
                return false;
            
            RemoveText(0, 1);
            return true;
        }

        protected override bool OnKeyLeft(bool down)
        {
            if(!down)
                return false;

            UpdateCarretOffset(-1);
            return true;
        }

        protected override bool OnKeyRight(bool down)
        {
            if(!down)
                return false;

            UpdateCarretOffset(1);
            return true;
        }

        protected override bool OnKeyUp(bool down)
        {
            if(!down)
                return false;

            UpdateCarretLine(-1);
            return true;
        }

        protected override bool OnKeyDown(bool down)
        {
            if(!down)
                return false;

            UpdateCarretLine(1);
            return true;
        }

        protected override bool OnKeyReturn(bool down)
        {
            if(!down)
                return false;

            InsertText("\n");
            return true;
        }

        #endregion

    }
}
