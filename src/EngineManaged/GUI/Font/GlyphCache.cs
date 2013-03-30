
using System.Collections.Generic;
using Flood;

namespace Flood.GUI
{

    public class GlyphCache
    {
        struct GlyphInfo
        {
            public Glyph Glyph;
            public SubTexture SubTexture;
        }

        private Dictionary<int, GlyphInfo> glyphs = new Dictionary<int, GlyphInfo>();

        public void AddGlyph(Font font, char c, Glyph glyph, SubTexture subTexture)
        {
            var glyphHash = GlyphHash(font, c);
            var glyphInfo = new GlyphInfo() {Glyph = glyph, SubTexture = subTexture};
            glyphs.Add(glyphHash,glyphInfo);
        }

        public bool TryGetGlyph(Font font, char c, out Glyph glyph, out SubTexture subTexture)
        {
            var glyphHash = GlyphHash(font, c);
            GlyphInfo glyphInfo;
            if (glyphs.TryGetValue(glyphHash, out glyphInfo))
            {
                glyph = glyphInfo.Glyph;
                subTexture = glyphInfo.SubTexture;
                return true;
            }

            subTexture = default(SubTexture);
            glyph = default(Glyph);
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
