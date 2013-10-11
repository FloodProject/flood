using System;
using System.Collections.Generic;

namespace Flood.GUI
{
    class GlyphManager
    {
        private TextureAtlas textureAtlas;

        private readonly ResourceHandle<Material> textMaterial; 

        struct GlyphId
        {
            public string FontName;
            public int Size;
            public int Codepoint;

            public GlyphId(Font font, int codepoint)
            {
                FontName = font.FaceName;
                Size = font.Size;
                Codepoint = codepoint;
            }
        }

        private readonly Dictionary<GlyphId, SubTexture> glyphSubTextures;
        private readonly Dictionary<GlyphId, Glyph> glyphInfos;

        public GlyphManager()
        {
            glyphSubTextures = new Dictionary<GlyphId, SubTexture>();
            glyphInfos = new Dictionary<GlyphId, Glyph>();

            textureAtlas = new TextureAtlas(512,PixelFormat.R8G8B8);

            textMaterial = Material.Create(Allocator.GetHeap(), "TextMaterial");
            var mat = textMaterial.Resolve();
            mat.BackfaceCulling = false;
            mat.SetBlending(BlendSource.SourceAlpha, BlendDestination.InverseSourceAlpha);
            mat.SetShader("Text");
            mat.SetTexture(0, textureAtlas.AtlasImageHandle);
            mat.GetTextureUnit(0).WrapMode = TextureWrapMode.ClampToEdge;
        }

        public bool TryGetGlyphInfo(Font font,int codepoint, out Glyph glyph)
        {
            var glyphId = new GlyphId(font, codepoint);
            if (glyphInfos.TryGetValue(glyphId, out glyph))
                return true;

            var nativeFont = font.EngineFont.Resolve();

            if (!nativeFont.GetGlyphInfo(codepoint, font.Size, out glyph))
                return false;

            glyphInfos[glyphId] = glyph;
            return true;
        }

        public bool TryGetGlyphImage(Font font, int codepoint, out SubTexture texture, out ResourceHandle<Material> material)
        {
            material = textMaterial;

            var glyphId = new GlyphId(font, codepoint);
            if (glyphSubTextures.TryGetValue(glyphId, out texture))
                return true;

            var nativeFont = font.EngineFont.Resolve();
            var imageHandle = nativeFont.CreateGlyphImage(codepoint, font.Size);

            if (imageHandle.Id == ResourceHandle<Resource>.Invalid)
                return false;
            
            if (!textureAtlas.AddImage(imageHandle))
            {
                throw new Exception("Atlas full");
            }

            if (!textureAtlas.GetImageSubTexture(imageHandle, out texture))
                return false;

            glyphSubTextures.Add(glyphId, texture);

            return true;
        }

        public Vector2 GetKerning(Font font, int c1, int c2)
        {
            var nativeFont = font.EngineFont.Resolve();

            return nativeFont.GetKerning(c1, c2, font.Size);
        }
    }
}
