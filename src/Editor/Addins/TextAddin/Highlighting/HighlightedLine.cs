﻿// Copyright (c) AlphaSierraPapa for the SharpDevelop Team (for details please see \doc\copyright.txt)
// This code is distributed under the GNU LGPL (for details please see \doc\license.txt)

using System;
using System.Collections.Generic;
using TextAddin.Document;
using TextAddin.Utils;

namespace TextAddin.Highlighting
{
	/// <summary>
	/// Represents a highlighted document line.
	/// </summary>
	public class HighlightedLine
	{
		/// <summary>
		/// Creates a new HighlightedLine instance.
		/// </summary>
		public HighlightedLine(TextDocument document, DocumentLine documentLine)
		{
			if (document == null)
				throw new ArgumentNullException("document");
			if (!document.Lines.Contains(documentLine))
				throw new ArgumentException("Line is null or not part of document");
			this.Document = document;
			this.DocumentLine = documentLine;
			this.Sections = new NullSafeCollection<HighlightedSection>();
		}
		
		/// <summary>
		/// Gets the document associated with this HighlightedLine.
		/// </summary>
		public TextDocument Document { get; private set; }
		
		/// <summary>
		/// Gets the document line associated with this HighlightedLine.
		/// </summary>
		public DocumentLine DocumentLine { get; private set; }
		
		/// <summary>
		/// Gets the highlighted sections.
		/// The sections are not overlapping, but they may be nested.
		/// In that case, outer sections come in the list before inner sections.
		/// The sections are sorted by start offset.
		/// </summary>
		public IList<HighlightedSection> Sections { get; private set; }
		
		/// <summary>
		/// Gets the default color of all text outside a <see cref="HighlightedSection"/>.
		/// </summary>
		public HighlightingColor DefaultTextColor { get; set; }
		
		sealed class HtmlElement : IComparable<HtmlElement>
		{
			internal readonly int Offset;
			internal readonly int Nesting;
			internal readonly bool IsEnd;
			internal readonly HighlightingColor Color;
			
			public HtmlElement(int offset, int nesting, bool isEnd, HighlightingColor color)
			{
				this.Offset = offset;
				this.Nesting = nesting;
				this.IsEnd = isEnd;
				this.Color = color;
			}
			
			public int CompareTo(HtmlElement other)
			{
				int r = Offset.CompareTo(other.Offset);
				if (r != 0)
					return r;
				if (IsEnd != other.IsEnd) {
					if (IsEnd)
						return -1;
					else
						return 1;
				} else {
					if (IsEnd)
						return other.Nesting.CompareTo(Nesting);
					else
						return Nesting.CompareTo(other.Nesting);
				}
			}
		}
	}
}
