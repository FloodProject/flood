// Copyright (c) AlphaSierraPapa for the SharpDevelop Team (for details please see \doc\copyright.txt)
// This code is distributed under the GNU LGPL (for details please see \doc\license.txt)

using System;
using System.Runtime.Serialization;
using System.Security.Permissions;
using Flood;

namespace TextAddin.Highlighting
{
    /// <summary>
    /// A highlighting color is a set of font properties and foreground and background color.
    /// </summary>
    [Serializable]
    public class HighlightingColor : ISerializable
    {
        /// <summary>
        /// Gets/Sets the name of the color.
        /// </summary>
        public string Name { get; set; }
        
        /// <summary>
        /// Gets/sets the font weight. Null if the highlighting color does not change the font weight.
        /// </summary>
        public FontWeight? FontWeight { get; set; }
        
        /// <summary>
        /// Gets/sets the font style. Null if the highlighting color does not change the font style.
        /// </summary>
        public FontStyle? FontStyle { get; set; }
        
        /// <summary>
        /// Gets/sets the foreground color applied by the highlighting.
        /// </summary>
        public Color? Foreground { get; set; }
        
        /// <summary>
        /// Gets/sets the background color applied by the highlighting.
        /// </summary>
        public Color? Background { get; set; }
        
        /// <summary>
        /// Creates a new HighlightingColor instance.
        /// </summary>
        public HighlightingColor()
        {
        }
        
        /// <summary>
        /// Deserializes a HighlightingColor.
        /// </summary>
        protected HighlightingColor(SerializationInfo info, StreamingContext context)
        {
            if (info == null)
                throw new ArgumentNullException("info");
            this.Name = info.GetString("Name");
            if (info.GetBoolean("HasWeight"))
                this.FontWeight = TextAddin.FontWeight.FromOpenTypeWeight(info.GetInt32("Weight"));
            if (info.GetBoolean("HasStyle"))
                this.FontStyle = FontStyleConverter.ConvertFromString(info.GetString("Style"));
            this.Foreground = (Color)info.GetValue("Foreground", typeof(Color));
            this.Background = (Color)info.GetValue("Background", typeof(Color));
        }

        public TextRunProperties Apply(TextRunProperties textRunProperties)
        {
             if (Foreground != null)
                textRunProperties.Foreground = Foreground.Value;

            if (Background != null)
                textRunProperties.Background = Background.Value;

            if (FontWeight != null)
                textRunProperties.FontWeight = FontWeight.Value;

            if (FontStyle != null)
                textRunProperties.FontStyle = FontStyle.Value;

            return textRunProperties;
        }
        
        /// <summary>
        /// Serializes this HighlightingColor instance.
        /// </summary>
        #if DOTNET4
        [System.Security.SecurityCritical]
        #else
        [SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.SerializationFormatter)]
        #endif
        public virtual void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            if (info == null)
                throw new ArgumentNullException("info");
            info.AddValue("Name", this.Name);
            info.AddValue("HasWeight", this.FontWeight.HasValue);
            if (this.FontWeight.HasValue)
                info.AddValue("Weight", this.FontWeight.Value.ToOpenTypeWeight());
            info.AddValue("HasStyle", this.FontStyle.HasValue);
            if (this.FontStyle.HasValue)
                info.AddValue("Style", this.FontStyle.Value.ToString());
            info.AddValue("Foreground", this.Foreground);
            info.AddValue("Background", this.Background);
        }
    }
}
