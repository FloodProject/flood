using System;
using Flood;
using Flood.GUI.Controls;
using ICSharpCode.AvalonEdit.Document;

namespace TextAddin.Render.Controls
{
    class CarretLayer : Layer
    {

        private DocumentLine line;
        private int column;
        private int oldColumn;

        public CarretLayer(TextView parent) : base(parent)
        {
            KeyboardInputEnabled = true;

            line = TextView.TextDocument.GetLineByNumber(1);
            column = 1;

            line.Deleted += (sender, args) => UpdateCarretOffset(0);
        }

        protected override void Render(Flood.GUI.Skins.Skin skin)
        {
            if (DateTime.Now.Millisecond < 500) 
                return;

            var location = new TextLocation(line.LineNumber,column);
            var charPos = TextView.TextLayer.GetTextLocationPosition(location);

            var lineHeight = Skin.DefaultFont.Size;

            skin.Renderer.DrawColor = new Color(0,0,0,255);
            skin.Renderer.DrawFilledRect(new Rect((int)charPos.X,(int)charPos.Y,1,lineHeight));
        }

        private void SnapCarretToPosition(int x, int y)
        {
            var localPosToCanvas = CanvasPosToLocal(new Vector2i(x, y));
            var location = TextView.TextLayer.GetTextLocation(localPosToCanvas);
            line = TextView.TextDocument.GetLineByNumber(location.Line);
            column = location.Column;
            oldColumn = 0;
        }

        protected void InsertText(string text)
        {
            var carretOffset = TextView.TextDocument.GetOffset(line.LineNumber,column);
            TextView.TextDocument.Insert(carretOffset, text);

            oldColumn = 0;
        }

        protected void RemoveText(int offset,int length)
        {
            var carretOffset = TextView.TextDocument.GetOffset(line.LineNumber, column);
            TextView.TextDocument.Remove(carretOffset+offset, length);

            oldColumn = 0;
        }


        private void UpdateCarretOffset(int dOffset)
        {
            line = TextView.TextDocument.GetLineByNumber(line.LineNumber);

            var offset = line.Offset + column;
            var maxOffset = TextView.TextDocument.TextLength;
            while (offset + dOffset > line.EndOffset || offset + dOffset < line.Offset)
            {
                UpdateCarretLine(Math.Sign(dOffset));
                var oldOffset = offset;
                offset = line.Offset + column;
                dOffset -= offset - oldOffset;

                if(offset + dOffset < 1 || offset + dOffset > maxOffset)
                    break;
            }
            
            offset += dOffset;
            offset = Math.Max(1, offset);
            offset = Math.Min(offset, maxOffset);
            
            column = Math.Max(1,offset - line.Offset);
            oldColumn = 0;
        }

        private void UpdateCarretLine(int lineOffset)
        {
            var i = Math.Abs(lineOffset);
            var lineNum = line.LineNumber;
            var maxLineNum = TextView.TextDocument.LineCount;
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
                line = TextView.TextDocument.GetLineByNumber(1);;
                column = 1;
            }
            else if (lineNum > maxLineNum)
            {
                line = TextView.TextDocument.GetLineByNumber(maxLineNum);
                column = line.Length;
            }
            else
            {
                line = TextView.TextDocument.GetLineByNumber(lineNum);
                column = Math.Min(oldColumn, line.Length);
            }
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
            InsertText(""+chr);
            UpdateCarretOffset(1);
            return true;
        }

        protected override bool OnKeyBackspace(bool down)
        {
            if(!down)
                return false;

            RemoveText(-1, 1);
            UpdateCarretOffset(-1);

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
