// Copyright (c) AlphaSierraPapa for the SharpDevelop Team (for details please see \doc\copyright.txt)
// This code is distributed under the GNU LGPL (for details please see \doc\license.txt)

using System.Collections.Generic;
using TextAddin.Controls;

namespace TextAddin.Rendering
{
	/// <summary>
	/// Allows transforming visual line elements.
	/// </summary>
	public interface IVisualLineTransformer
	{
		/// <summary>
		/// Applies the transformation to the specified list of visual line elements.
		/// </summary>
		void Transform(VisualLine line, IList<VisualLineElement> elements);
	}
}
