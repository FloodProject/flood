//
// Copyright (C) 2005 Novell, Inc (http://www.novell.com)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//


using System;
using System.Collections;
using System.ComponentModel;
using Mono.Addins;

namespace Flood.Editor.Client.Extensions
{

    public enum IconSize
    {
        Invalid,
        Size16,
        Size32
    }

	[ExtensionNode (Description="A stock icon. It is possible to register several icons with the same 'id' and different sizes.")]
	internal class StockIconCodon : ExtensionNode
	{
		[NodeAttribute ("stockid", true, "Id of the stock icon.")]
		string stockid;
		
		[NodeAttribute ("size", "Size of the icon.")]
		IconSize size = IconSize.Invalid;
		
		[NodeAttribute ("resource", "Name of the resource where the icon is stored.")]
		string resource;
		
		[NodeAttribute ("file", "Name of the file where the icon is stored.")]
		string file;
		
		[NodeAttribute ("icon", "Id of another icon or combination of icons to assign to this stock id.")]
		string iconid;
		
		public string StockId {
			get { return stockid; }
		}
		
		public IconSize IconSize {
			get { return size; }
		}
		
		public string Resource {
			get { return resource; }
		}
		
		public string File {
			get { return file; }
		}
		
		public string IconId {
			get { return iconid; }
		}

        public override int GetHashCode()
        {
            return ComputeHashCode(stockid, size);
        }

        public static int ComputeHashCode(string id, IconSize size)
        {
            var h1 = id.GetHashCode();
            var h2 = size.GetHashCode();
            return (h1 << 5) + h1 ^ h2;
        }
	}
}
