﻿using System;
using System.Collections.Generic;
using System.Drawing;

namespace Gwen.Control
{
    /// <summary>
    /// Multiline label with text chunks having different color/font.
    /// </summary>
    public class RichLabel : Base
    {
        protected struct TextBlock
        {
            public BlockType Type;
            public String Text;
            public Color Color;
            public Font Font;
        }

        protected enum BlockType
        {
            Text,
            NewLine
        }

        private bool m_NeedsRebuild;
        private readonly List<TextBlock> m_TextBlocks;
        private readonly String[] newline;

        /// <summary>
        /// Initializes a new instance of the <see cref="RichLabel"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public RichLabel(Base parent)
            : base(parent)
        {
            newline = new string[] { Environment.NewLine };
            m_TextBlocks = new List<TextBlock>();
        }

        /// <summary>
        /// Adds a line break to the control.
        /// </summary>
        public void AddLineBreak()
        {
            TextBlock block = new TextBlock { Type = BlockType.NewLine };
            m_TextBlocks.Add(block);
        }

        /// <summary>
        /// Adds text to the control.
        /// </summary>
        /// <param name="text">Text to add.</param>
        /// <param name="color">Text color.</param>
        /// <param name="font">Font to use.</param>
        public void AddText(String text, Color color, Font font = null)
        {
            if (String.IsNullOrEmpty(text))
                return;

            var lines = text.Split(newline, StringSplitOptions.None);
            for (int i = 0; i < lines.Length; i++)
            {
                if (i > 0)
                    AddLineBreak();

                TextBlock block = new TextBlock { Type = BlockType.Text, Text = lines[i], Color = color, Font = font };

                m_TextBlocks.Add(block);
                m_NeedsRebuild = true;
                Invalidate();
            }
        }

        /// <summary>
        /// Resizes the control to fit its children.
        /// </summary>
        /// <param name="width">Determines whether to change control's width.</param>
        /// <param name="height">Determines whether to change control's height.</param>
        /// <returns>
        /// True if bounds changed.
        /// </returns>
        public override bool SizeToChildren(bool width = true, bool height = true)
        {
            Rebuild();
            return base.SizeToChildren(width, height);
        }

        protected void SplitLabel(String text, Font font, TextBlock block, ref int x, ref int y, ref int lineHeight)
        {
            var spaced = Util.SplitAndKeep(text, " ");
            if (spaced.Length == 0)
                return;

            int spaceLeft = Width - x;
            String leftOver;

            // Does the whole word fit in?
            Point stringSize = Skin.Renderer.MeasureText(font, text);
            if (spaceLeft > stringSize.X)
            {
                CreateLabel(text, block, ref x, ref y, ref lineHeight, true);
                return;
            }

            // If the first word is bigger than the line, just give up.
            Point wordSize = Skin.Renderer.MeasureText(font, spaced[0]);
            if (wordSize.X >= spaceLeft)
            {
                CreateLabel(spaced[0], block, ref x, ref y, ref lineHeight, true);
                if (spaced[0].Length >= text.Length)
                    return;

                leftOver = text.Substring(spaced[0].Length + 1);
                SplitLabel(leftOver, font, block, ref x, ref y, ref lineHeight);
                return;
            }

            String newString = String.Empty;
            for (int i = 0; i < spaced.Length; i++)
            {
                wordSize = Skin.Renderer.MeasureText(font, newString + spaced[i]);
                if (wordSize.X > spaceLeft)
                {
                    CreateLabel(newString, block, ref x, ref y, ref lineHeight, true);
                    x = 0;
                    y += lineHeight;
                    break;
                }

                newString += spaced[i];
            }

            int newstr_len = newString.Length;
            if (newstr_len < text.Length)
            {
                leftOver = text.Substring(newstr_len + 1);
                SplitLabel(leftOver, font, block, ref x, ref y, ref lineHeight);
            }
        }

        protected void CreateLabel(String text, TextBlock block, ref int x, ref int y, ref int lineHeight, bool noSplit)
        {
            // Use default font or is one set?
            Font font = Skin.DefaultFont;
            if (block.Font != null)
                font = block.Font;

            // This string is too long for us, split it up.
            Point p = Skin.Renderer.MeasureText(font, text);

            if (lineHeight == -1)
            {
                lineHeight = p.Y;
            }

            if (!noSplit)
            {
                if (x + p.X > Width)
                {
                    SplitLabel(text, font, block, ref x, ref y, ref lineHeight);
                    return;
                }
            }

            // Wrap
            if (x + p.X >= Width)
            {
                CreateNewline(ref x, ref y, lineHeight);
            }

            Label label = new Label(this);
            label.SetText(x == 0 ? text.TrimStart(' ') : text);
            label.TextColor = block.Color;
            label.Font = font;
            label.SizeToContents();
            label.SetPosition(x, y);

            //lineheight = (lineheight + pLabel.Height()) / 2;			

            x += label.Width;

            if (x >= Width)
            {
                CreateNewline(ref x, ref y, lineHeight);
            }
        }

        protected void CreateNewline(ref int x, ref int y, int lineHeight)
        {
            x = 0;
            y += lineHeight;
        }

        protected void Rebuild()
        {
            DeleteAllChildren();

            int x = 0;
            int y = 0;
            int lineHeight = -1;


            foreach (var block in m_TextBlocks)
            {
                if (block.Type == BlockType.NewLine)
                {
                    CreateNewline(ref x, ref y, lineHeight);
                    continue;
                }

                if (block.Type == BlockType.Text)
                {
                    CreateLabel(block.Text, block, ref x, ref y, ref lineHeight, false);
                    continue;
                }
            }

            m_NeedsRebuild = false;
        }

        /// <summary>
        /// Handler invoked when control's bounds change.
        /// </summary>
        /// <param name="oldBounds">Old bounds.</param>
        protected override void OnBoundsChanged(Rectangle oldBounds)
        {
            base.OnBoundsChanged(oldBounds);
            Rebuild();
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skin.Base skin)
        {
            base.Layout(skin);
            if (m_NeedsRebuild)
                Rebuild();

            // align bottoms. this is still not ideal, need to take font metrics into account.
            Base prev = null;
            foreach (Base child in Children)
            {
                if (prev != null && child.Y == prev.Y)
                {
                    Align.PlaceRightBottom(child, prev);
                }
                prev = child;
            }
        }
    }
}
