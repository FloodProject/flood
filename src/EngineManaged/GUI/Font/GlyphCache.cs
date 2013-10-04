
using System.Collections.Generic;
using Flood;

namespace GUI
{

    public class GlyphCache
    {

        private Dictionary<int, SubTexture> glyphs = new Dictionary<int, SubTexture>();

        public void AddGlyph(Font font, char c, SubTexture subTexture)
        {
            var glyphHash = GlyphHash(font, c);
            glyphs.Add(glyphHash,subTexture);
        }

        public bool TryGetGlyph(Font font, char c, out SubTexture subTexture)
        {
            var glyphHash = GlyphHash(font, c);
            if (glyphs.TryGetValue(glyphHash, out subTexture))
                return true;

            subTexture = default(SubTexture);
            return false;
        }

        private static int GlyphHash(Font font, char c)
        {
            return CombineHashCodes(CombineHashCodes(font.FaceName.GetHashCode(), c.GetHashCode()), font.Size.GetHashCode());
        }

        private static int CombineHashCodes(int h1, int h2)
        {
          return (h1 << 5) + h1 ^ h2;
        }

    }
}
