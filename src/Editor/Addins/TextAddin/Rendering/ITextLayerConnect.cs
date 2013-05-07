// Copyright (c) AlphaSierraPapa for the SharpDevelop Team (for details please see \doc\copyright.txt)
// This code is distributed under the GNU LGPL (for details please see \doc\license.txt)

using TextAddin.Controls;

namespace TextAddin.Rendering
{
	/// <summary>
	/// Allows <see cref="VisualLineElementGenerator"/>s, <see cref="IVisualLineTransformer"/>s and
	/// <see cref="IBackgroundRenderer"/>s to be notified when they are added or removed from a text layer.
	/// </summary>
	public interface ITextLayerConnect
	{
		/// <summary>
		/// Called when added to a text view.
		/// </summary>
		void AddToTextLayer(TextLayer textLayer);
		
		/// <summary>
		/// Called when removed from a text layer.
		/// </summary>
		void RemoveFromTextLayer(TextLayer textLayer);
	}
}
