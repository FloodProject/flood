using System;
using System.Collections.Generic;
using System.Threading;
using Flood;
using GUI.Controls;
using GUI.Input;
using GUI.Renderers;

namespace GUI
{
    class ZCounter
    {
        float zcount;
        
        public void Reset()
        {
            zcount = -99;
        }

        public void Increment()
        {
            zcount += 0.001f;
        }

        public float Value
        {
            get { return zcount; }
        }
    }

    class GUIGeometryBuffer : ManagedGeometryBuffer
    {
        struct Vertex
        {
            public Vector3 Position;
            public Vector2 UV;
            public Color Color;
            public float shift;
        };

        private ZCounter zCounter;

        public GUIGeometryBuffer(ZCounter zCounter)
        {
            this.zCounter = zCounter;
        }

        protected override GeometryBuffer CreateGeometryBuffer()
        {
            var gb = new GeometryBuffer();
            gb.Declarations.Add(new VertexElement(VertexAttribute.Position, VertexDataType.Float, 3));
            gb.Declarations.Add(new VertexElement(VertexAttribute.TexCoord0, VertexDataType.Float, 2));
            gb.Declarations.Add(new VertexElement(VertexAttribute.Color, VertexDataType.Byte, 4));
            gb.Declarations.Add(new VertexElement(VertexAttribute.Normal, VertexDataType.Float, 1));
            gb.Declarations.CalculateStrides();
            return gb;
        }

        protected override ResourceHandle<Material> CreateMaterial(ResourceHandle<Image> imageHandle)
        {
            var materialHandle = Material.Create(Allocator.GetHeap(), "GwenGui");
            var mat = materialHandle.Resolve();
            mat.BackfaceCulling = false;
            mat.SetBlending(BlendSource.SourceAlpha, BlendDestination.InverseSourceAlpha);

            if (imageHandle.Id == ResourceHandle<Image>.Invalid)
            {
                mat.SetShader("VertexColor");
            }
            else
            {
                mat.SetShader("TexColor");
                mat.SetTexture(0, imageHandle);
                mat.GetTextureUnit(0).WrapMode = TextureWrapMode.Clamp;
            }

            return materialHandle;
        }

        public void AddRectangle(RectangleF rect, Color color)
        {
            ResourceHandle<Image> imageHandle;
            imageHandle.Id = ResourceHandle<Image>.Invalid;

            AddRectangle(rect,Vector2.Zero,Vector2.Zero,Vector2.Zero,Vector2.Zero, imageHandle, color);
        }

        public void AddRectangle(RectangleF rect, 
            Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomRightUV, Vector2 bottomLeftUV,
            ResourceHandle<Image> imageHandle, Color color)
        {
            Vertex v1, v2, v3, v4;

            InitVetexes(out v1, out v2, out v3, out v4,
                        rect, color,
                        topLeftUV, topRightUV, bottomRightUV, bottomLeftUV);

             unsafe
             {
                 AddQuad(new IntPtr(&v1),new IntPtr(&v2),new IntPtr(&v3),new IntPtr(&v4), (uint)sizeof(Vertex), imageHandle);
             }
        }

        public void AddRectangle(RectangleF rect, 
            Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomRightUV, Vector2 bottomLeftUV,
            ResourceHandle<Material> materialHandle, Color color)
        {
            Vertex v1, v2, v3, v4;

            InitVetexes(out v1, out v2, out v3, out v4,
                        rect, color,
                        topLeftUV, topRightUV, bottomRightUV, bottomLeftUV);

             unsafe
             {
                 AddQuad(new IntPtr(&v1),new IntPtr(&v2),new IntPtr(&v3),new IntPtr(&v4), (uint)sizeof(Vertex), materialHandle);
             }
        }

        private void InitVetexes(out Vertex v1, out Vertex v2, out Vertex v3, out Vertex v4,
            RectangleF rect , Color color,
            Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomRightUV, Vector2 bottomLeftUV)
        {

            var top = Math.Max(rect.GetBottom(), rect.GetTop());
            var bottom = Math.Min(rect.GetBottom(), rect.GetTop());
            var left = rect.GetLeft();
            var right = rect.GetRight();

            v1.Position = new Vector3(left, bottom, zCounter.Value);
            v2.Position = new Vector3(right, bottom, zCounter.Value);
            v3.Position = new Vector3(right, top, zCounter.Value);
            v4.Position = new Vector3(left, top, zCounter.Value);

            zCounter.Increment();

            v1.shift = left  - ((int)left);
            v2.shift = right - ((int)right);
            v3.shift = right - ((int)right);
            v4.shift = left  - ((int)left);

            v1.Color = color;
            v2.Color = color;
            v3.Color = color;
            v4.Color = color;

            v1.UV = topLeftUV;
            v2.UV = topRightUV;
            v3.UV = bottomRightUV;
            v4.UV = bottomLeftUV;
        }
    }

    abstract class ManagedGeometryBuffer
    {
        struct BatchInfo
        {
            public RenderBatch Batch;
            public List<int> Ranges;
        };

        readonly GeometryBuffer gb;
        readonly Dictionary<uint, BatchInfo> batches;
        readonly Dictionary<uint, ResourceHandle<Material>> materials;

        protected ManagedGeometryBuffer()
        {
            gb = CreateGeometryBuffer();

            batches = new Dictionary<uint, BatchInfo>();
            materials = new Dictionary<uint, ResourceHandle<Material>>();
        }

        protected abstract GeometryBuffer CreateGeometryBuffer();
        protected abstract ResourceHandle<Material> CreateMaterial(ResourceHandle<Image> imageHandle);

        protected void AddQuad(IntPtr v1, IntPtr v2, IntPtr v3, IntPtr v4, uint structSize, ResourceHandle<Image> imageHandle)
        {
            var materialHandle = GetCreateMaterial(imageHandle);
            AddQuad(v1, v2, v3, v4, structSize, materialHandle);
        }

        protected void AddQuad(IntPtr v1, IntPtr v2, IntPtr v3, IntPtr v4, uint structSize, ResourceHandle<Material> materialHandle)
        {
            var batchInfo = GetCreateBatchInfo(materialHandle);
            batchInfo.Ranges.Add((int) gb.NumVertices);

            gb.Add(v1, structSize);
            gb.Add(v2, structSize);
            gb.Add(v3, structSize);
            gb.Add(v4, structSize);
        }

        public void Render(RenderBlock rb)
        {
            foreach (var batch in batches.Values)
            {
                if (batch.Ranges.Count > 0)
                {
                    RenderBatchRange newRange;
                    newRange.Start = (ushort)gb.NumIndices;

                    foreach (var range in batch.Ranges)
                    {
                        var vertexIndex = (ushort)range;
                        gb.AddIndex(vertexIndex++);
                        gb.AddIndex(vertexIndex++);
                        gb.AddIndex(vertexIndex++);
                        gb.AddIndex(vertexIndex);
                    }

                    newRange.End = (ushort)gb.NumIndices;
                    batch.Batch.Range = newRange;

                    var state = new RenderState(batch.Batch);
                    rb.AddState(state);
                }
            }
        }

        public void Clear()
        {
            foreach(var batch in batches.Values)
            {
                batch.Ranges.Clear();
            }

            gb.Clear();
            gb.ClearIndexes();
        }

        ResourceHandle<Material> GetCreateMaterial(ResourceHandle<Image> imageHandle)
        {
            var id = imageHandle.Id;

            if (!materials.ContainsKey(id))
            {
                var materialHandle = CreateMaterial(imageHandle);

                materials.Add(id, materialHandle);
            }

            return materials[id];
        }

        BatchInfo GetCreateBatchInfo(ResourceHandle<Material> materialHandle)
        {
            if (!batches.ContainsKey(materialHandle.Id))
            {
                var batch = new RenderBatch
                {
                    GeometryBuffer = gb,
                    RenderLayer = RenderLayer.Overlays,
                    PrimitiveType = PrimitiveType.Quads,
                    Material = materialHandle
                };

                var batchInfo = new BatchInfo {Batch = batch, Ranges = new List<int>()};
                batches[materialHandle.Id] = batchInfo;
            }

            return batches[materialHandle.Id];
        }

    };


    public class TextRenderer
    {
        static readonly Flood.TextureAtlas textureAtlas;
        static readonly GlyphCache glyphCache;

        private static readonly ResourceHandle<Material> textMaterial; 

        static TextRenderer()
        {
            textureAtlas = new TextureAtlas(512,PixelFormat.R8G8B8);
            glyphCache = new GlyphCache();

            textMaterial = Material.Create(Allocator.GetHeap(), "TextMaterial");
            var mat = textMaterial.Resolve();
            mat.BackfaceCulling = false;
            mat.SetBlending(BlendSource.SourceAlpha, BlendDestination.InverseSourceAlpha);
            mat.SetShader("Text");
            mat.SetTexture(0, textureAtlas.AtlasImageHandle);
            mat.GetTextureUnit(0).WrapMode = TextureWrapMode.ClampToEdge;
        }

        public static Vector2 MeasureText(System.String text, Font font)
        {
            float curX = 0;
            var ttfont = font.EngineFont.Resolve();
            for(var i = 0; i < text.Length; i++)
            {
                var c = text[i];

                Glyph glyph;
                var foundGlyph = ttfont.GetGlyphInfo(c, font.Size, out glyph);
                if (!foundGlyph)
                {
                    Log.Warn("Glyph not found for character " + c);
                    continue;
                }

                curX += glyph.Advance;
                if(i < text.Length - 1) 
                    curX += ttfont.GetKerning(text[i], text[i + 1], font.Size).X;

            }

            return new Vector2(curX,font.Size);;
        }

        public static bool GetPositionTextIndex(string text, Font font, float x, out int index)
        {
            float curX = 0;

            var ttfont = font.EngineFont.Resolve();
            for(var i = 0; i < text.Length; i++)
            {
                var c = text[i];

                Glyph glyph;
                var foundGlyph = ttfont.GetGlyphInfo(c, font.Size, out glyph);
                if (!foundGlyph)
                {
                    Log.Warn("Glyph not found for character " + c);
                    continue;
                }

                curX += glyph.Advance;
                if(i < text.Length - 1) 
                   curX += ttfont.GetKerning(text[i], text[i + 1], font.Size).X;

                if(curX >= x)
                {
                    index = i;
                    return true;
                }
            }

            index = 0;
            return false;
        }

        internal static void DrawText(GUIGeometryBuffer geometryBuffer, Font font, Vector2 position, String text, Color color)
        {
            var ttfont = font.EngineFont.Resolve();
            for(var i = 0; i < text.Length; i++)
            {
                char c = text[i];

                Glyph glyph;
                var foundGlyph = ttfont.GetGlyphInfo(c, font.Size, out glyph);
                if (!foundGlyph)
                {
                    Log.Warn("Glyph not found for character " + c);
                    continue;
                }

                bool drawSubtexture = true;
                SubTexture subTexture;
                if(!glyphCache.TryGetGlyph(font, c, out subTexture))
                {
                    var imageHandle = ttfont.CreateGlyphImage(c, font.Size);
                    if (imageHandle.Id == ResourceHandle<Resource>.Invalid)
                    {
                        drawSubtexture = false;
                    }
                    else
                    {
                        var subTextureFound = textureAtlas.GetImageSubTexture(imageHandle, out subTexture);
                        if (!subTextureFound)
                        {
                            textureAtlas.AddImage(imageHandle);
                            subTextureFound = textureAtlas.GetImageSubTexture(imageHandle, out subTexture);
                            if (subTextureFound)
                            {
                                glyphCache.AddGlyph(font, c, subTexture);
                            }
                            else
                            {
                                Log.Warn("subTexture not Found\n");
                                continue;
                            }
                        }
                    }
                }

                if (drawSubtexture)
                {

                    var renderRect = new RectangleF(
                        position.X + glyph.XOffset, 
                        position.Y + glyph.BaseLineOffset, 
                        glyph.Width,
                        glyph.Height);

                    geometryBuffer.AddRectangle(renderRect,subTexture.LeftTopUV,subTexture.RightTopUV,subTexture.RightBottomUV,subTexture.LeftBottomUV,textMaterial,color);
                }

                if (i < text.Length-1){
                    var kern = ttfont.GetKerning(text[i], text[i+1], font.Size);
                    position.X += glyph.Advance + kern.X;
                    position.Y += kern.Y;
                }
            }
        }
    };

    public class GwenRenderer : Renderer
    {
        private readonly ZCounter zCounter;
        private readonly GUIGeometryBuffer _guiBuffer;

        public GwenRenderer()
        {
            zCounter = new ZCounter();
            _guiBuffer = new GUIGeometryBuffer(zCounter);
        }

        public void Render(RenderBlock rb)
        {
            _guiBuffer.Render(rb);
        }

        public void Clear()
        {
            _guiBuffer.Clear();
            zCounter.Reset();
        }

        public override void DrawFilledRect(Rectangle rect) 
        {
            rect = Translate(rect);
            _guiBuffer.AddRectangle(new RectangleF(rect.X,rect.Y,rect.Width,rect.Height), DrawColor);
        }

        public override Color DrawColor { get; set; }

        public override void DrawTexturedRect(ResourceHandle<Image> imageHandle, RectangleF rect, float u1, float v1, float u2, float v2)
        {
            rect = Translate(rect);
            _guiBuffer.AddRectangle(rect, new Vector2(u1,v1), new Vector2(u2,v1), new Vector2(u2,v2), new Vector2(u1,v2), imageHandle, DrawColor);
        }

        public void DrawTexturedRect(ResourceHandle<Image> imageHandle, RectangleF rect, Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomRightUV, Vector2 bottomLeftUV)
        {
            rect = Translate(rect);
            _guiBuffer.AddRectangle(rect,topLeftUV,topRightUV,bottomRightUV,bottomLeftUV, imageHandle, DrawColor);
        }

        public override Vector2 MeasureText(Font font, string text) 
        {
            return TextRenderer.MeasureText(text,font);
        }

        public override void RenderText(Font font, Vector2i position, string text)
        {
            position = Translate(position);
            TextRenderer.DrawText(_guiBuffer, font, new Vector2(position.X,position.Y), text, DrawColor);
        }

        public override Color PixelColor(ResourceHandle<Image> imageHandle, uint x, uint y, Color defaultColor)
        {
            var image = imageHandle.Resolve();
            if(image == null || image.Width == 0)
                return defaultColor;

            var offset = (int)(4 * (x + y * image.Width));
            var data = image.Buffer;

            var pixel = new Color(data[offset + 0],
                data[offset + 1], data[offset + 2], data[offset + 3]);
        
            return pixel;
        }

    };

    public sealed class GwenInput : IDisposable
    {
        readonly InputManager inputManager;
        readonly Mouse mouse;
        readonly Keyboard keyboard;

        Canvas canvas;

        int mouseX;
        int mouseY;

        bool m_AltGr;

        public GwenInput(InputManager inputManager)
        {
            this.inputManager = inputManager;
            
            canvas = null;
            mouseX = 0;
            mouseY = 0;
            m_AltGr = false;

            mouse = inputManager.Mouse;
            if (mouse != null)
            {
                mouse.MouseMove += ProcessMouseMove;
                mouse.MouseDrag += ProcessMouseDrag;
                mouse.MouseButtonPress += ProcessMouseButtonPressed;
                mouse.MouseButtonRelease += ProcessMouseButtonReleased;
                mouse.MouseWheelMove += ProcessMouseWheel;
            }

            keyboard = inputManager.Keyboard;
            if (keyboard != null)
            {
                keyboard.KeyPress += ProcessKeyDown;
                keyboard.KeyRelease += ProcessKeyUp;
                keyboard.KeyText += ProcessText;
            }
        }

        ~GwenInput()
        {
            
        }

        public void Dispose()
        {
            Log.Info("Disposing GwenInput");

            if (mouse != null)
            {
                mouse.MouseMove -= ProcessMouseMove;
                mouse.MouseDrag -= ProcessMouseDrag;
                mouse.MouseButtonPress -= ProcessMouseButtonPressed;
                mouse.MouseButtonRelease -= ProcessMouseButtonReleased;
                mouse.MouseWheelMove -= ProcessMouseWheel;
            }

            if (keyboard != null)
            {
                keyboard.KeyPress -= ProcessKeyDown;
                keyboard.KeyRelease -= ProcessKeyUp;
                keyboard.KeyText -= ProcessText;
            }

            GC.SuppressFinalize(this);
        }

        public void Initialize(Canvas c)
        {
            canvas = c;
        }

        static char TranslateChar(Keys key)
        {
            if (key >= Keys.A && key <= Keys.Z)
                return (char)('a' + ((int)key - (int) Keys.A));
            return ' ';
        }

        void ProcessMouseMove(MouseMoveEvent mouseMoveEvent)
        {
            var dx = mouseMoveEvent.X - mouseX;
            var dy = mouseMoveEvent.Y - mouseY;

            mouseX = mouseMoveEvent.X;
            mouseY = mouseMoveEvent.Y;

            canvas.Input_MouseMoved(mouseX, mouseY, dx, dy);
        }

        void ProcessMouseDrag(MouseDragEvent mouseDragEvent)
        {
            int dx = mouseDragEvent.X - mouseX;
            int dy = mouseDragEvent.Y - mouseY;

            mouseX = mouseDragEvent.X;
            mouseY = mouseDragEvent.Y;

            canvas.Input_MouseMoved(mouseX, mouseY, dx, dy);
        }

        void ProcessMouseButtonPressed(MouseButtonEvent mouseButtonEvent)
        {
            canvas.Input_MouseButton((int) mouseButtonEvent.Button, true);
        }

        void ProcessMouseButtonReleased(MouseButtonEvent mouseButtonEvent)
        {
            canvas.Input_MouseButton((int) mouseButtonEvent.Button, false);
        }

        void ProcessMouseWheel(MouseWheelEvent mouseWheelEvent)
        {
            canvas.Input_MouseWheel(mouseWheelEvent.Delta*60);
        }

        void ProcessKeyDown(KeyEvent keyEvent)
        {
            var ch = TranslateChar(keyEvent.KeyCode);

            if (InputHandler.DoSpecialKeys(canvas, ch))
                return;
        
            canvas.Input_Key(keyEvent.KeyCode, true);
        }

        void ProcessKeyUp(KeyEvent keyEvent)
        {
            canvas.Input_Key(keyEvent.KeyCode, false);
        }

        void ProcessText(KeyEvent keyEvent)
        {
            var ch =  (char)keyEvent.Unicode;
            canvas.Input_Character(ch);
        }
    }
}