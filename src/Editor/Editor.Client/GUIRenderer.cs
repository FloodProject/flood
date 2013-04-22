using System;
using System.Collections.Generic;
using Flood.GUI;
using Flood.GUI.Controls;
using Flood.GUI.Input;

namespace Flood.Editor.Client
{
    class ManagedGeometryBuffer
    {
        struct BatchInfo
        {
            public RenderBatch Batch;
            public List<int> Ranges;
        };

        struct Vertex
        {
            public Vector3 Position;
            public Vector2 UV;
            public Color Color;
        };

        readonly GeometryBuffer gb;
        readonly Dictionary<uint, BatchInfo> batches;
        readonly Dictionary<uint, ResourceHandle<Material>> materials;

        float zcount;

        public ManagedGeometryBuffer()
        {
            gb = new GeometryBuffer();

            batches = new Dictionary<uint, BatchInfo>();
            materials = new Dictionary<uint, ResourceHandle<Material>>();

            gb.Declarations.Add(new VertexElement(VertexAttribute.Position, VertexDataType.Float, 3));
            gb.Declarations.Add(new VertexElement(VertexAttribute.TexCoord0, VertexDataType.Float, 2));
            gb.Declarations.Add(new VertexElement(VertexAttribute.Color, VertexDataType.Byte, 4));
            gb.Declarations.CalculateStrides();

            zcount = -99;
        }

        ~ManagedGeometryBuffer()
        {
            //gb.Dispose();
        }

        public void AddRectangle(Rect rect, Color color)
        {
            var imageHandle = new ResourceHandle<Image>(0);
            AddRectangle(rect,Vector2.Zero,Vector2.Zero,Vector2.Zero,Vector2.Zero, imageHandle, color);
        }

        public void AddRectangle(Rect rect, 
            Vector2 topLeftUV, Vector2 topRightUV,Vector2 bottomRightUV, Vector2 bottomLeftUV,
            ResourceHandle<Image> imageHandle, Color color)
        {

            var batchInfo = GetCreateBatchInfo(imageHandle);
            batchInfo.Ranges.Add((int) gb.GetNumVertices());

            var top = Math.Max(rect.GetBottom(), rect.GetTop());
            var bottom = Math.Min(rect.GetBottom(), rect.GetTop());
            var left = rect.GetLeft();
            var right = rect.GetRight();

            Vertex v1, v2, v3, v4;

            v1.Position = new Vector3(left, bottom, zcount);
            v2.Position = new Vector3(right, bottom, zcount);
            v3.Position = new Vector3(right, top, zcount);
            v4.Position = new Vector3(left, top, zcount);

            //TODO optimize precision/usage
            zcount += 0.001f;

            v1.Color = color;
            v2.Color = color;
            v3.Color = color;
            v4.Color = color;

            v1.UV = topLeftUV;
            v2.UV = topRightUV;
            v3.UV = bottomRightUV;
            v4.UV = bottomLeftUV;

            //Vertex buffer setup
            unsafe
            {
                gb.Add(new IntPtr(&v1), (uint) sizeof (Vertex));
                gb.Add(new IntPtr(&v2), (uint) sizeof (Vertex));
                gb.Add(new IntPtr(&v3), (uint) sizeof (Vertex));
                gb.Add(new IntPtr(&v4), (uint) sizeof (Vertex));
            }
        }

        public void Render(RenderBlock rb)
        {
            foreach (var batch in batches.Values)
            {
                if (batch.Ranges.Count > 0)
                {
                    RenderBatchRange newRange;
                    newRange.Start = (ushort)gb.GetNumIndices();

                    foreach (var range in batch.Ranges)
                    {
                        var vertexIndex = (ushort)range;
                        gb.AddIndex(vertexIndex++);
                        gb.AddIndex(vertexIndex++);
                        gb.AddIndex(vertexIndex++);
                        gb.AddIndex(vertexIndex);
                    }

                    newRange.End = (ushort)gb.GetNumIndices();
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

            zcount = -99;
        }

        ResourceHandle<Material> GetCreateMaterial(ResourceHandle<Image> imageHandle)
        {
            var id = imageHandle.Id;

            if (!materials.ContainsKey(id))
            {
                var mat = new Material("GwenGui");
                mat.SetBackfaceCulling(false);
                mat.SetBlending(BlendSource.SourceAlpha, BlendDestination.InverseSourceAlpha);

                if (id == 0)
                {
                    mat.SetShader("VertexColor");
                }
                else
                {
                    mat.SetShader("TexColor");
                    mat.SetTexture(0, imageHandle);
                    mat.GetTextureUnit(0).SetWrapMode(TextureWrapMode.Clamp);
                }

                materials.Add(id, ResourceHandle<Material>.Create(mat));
            }

            return materials[id];
        }

        BatchInfo GetCreateBatchInfo(ResourceHandle<Image> imageHandle)
        {
            var id = imageHandle.Id;

            if (!batches.ContainsKey(id))
            {
                var batch = new RenderBatch();
                batch.SetGeometryBuffer(gb);
                batch.SetRenderLayer(RenderLayer.Overlays);
                batch.SetPrimitiveType(PrimitiveType.Quads);

                var mat = GetCreateMaterial(imageHandle);
                batch.SetMaterial(mat);

                var batchInfo = new BatchInfo {Batch = batch, Ranges = new List<int>()};
                batches[id] = batchInfo;
            }

            return batches[id];
        } 
    };


    public class TextRenderer
    {
        static readonly Flood.TextureAtlas textureAtlas;
        static readonly GlyphCache glyphCache;

        static TextRenderer()
        {
            textureAtlas = new TextureAtlas(512);
            glyphCache = new GlyphCache();
        }

        public static Vector2i MeasureText(System.String text, Flood.GUI.Font font)
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

                curX = (int) (curX + 0.5);
            }

            return new Vector2i((int)(curX+0.5),font.Size);;
        }

        public static bool GetPositionTextIndex(string text, Flood.GUI.Font font, float x, out int index)
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

                curX = (int) (curX + 0.5);

                if(curX >= x)
                {
                    index = i;
                    return true;
                }
            }

            index = 0;
            return false;
        }

        public static void DrawText(GwenRenderer renderer, Flood.GUI.Font font, Vector2i position, String text)
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
                    var atlasImageHandle = textureAtlas.GetAtlasImageHandle();
                    Image atlasImage = atlasImageHandle.Resolve();

                    var texture = new Texture();
                    texture.SetImage(atlasImageHandle);

                    var renderRect = new Rect(position.X, (int)(position.Y + glyph.BaseLineOffset), 
                        (int)glyph.Width, (int)glyph.Height);

                    renderer.DrawTexturedRect(texture,renderRect,subTexture.LeftTopUV,subTexture.RightTopUV,subTexture.RightBottomUV,subTexture.LeftBottomUV);
                    texture.Dispose();
                }

                if (i < text.Length-1){
                    var kern = ttfont.GetKerning(text[i],text[i+1], font.Size);
                    position.X += (int)(glyph.Advance + kern.X + 0.5);
                    position.Y += (int) kern.Y;
                }
            }
        }
    };

    public class GwenRenderer : Flood.GUI.Renderers.Renderer
    {
        readonly ManagedGeometryBuffer buffer;

        Color color;
        Dictionary<Tuple<string, Flood.GUI.Font>, TextRenderer> stringCache;
    
        bool isClipEnabled;

        public GwenRenderer()
        {
        
            buffer = new ManagedGeometryBuffer();
        }

        public void Render(RenderBlock rb)
        {
            buffer.Render(rb);
        }

        public void Clear()
        {
            buffer.Clear();
        }

        public override void DrawFilledRect(Rect rect) 
        {
            rect = Translate(rect);
            buffer.AddRectangle(rect, color);
        }

        public override Color DrawColor 
        {
            get { return color; }
            set { color = value;}
        }

        public override void StartClip()
        {
            isClipEnabled = true;
        }

        public override void EndClip()
        {
            isClipEnabled = false;
        }

        public override void DrawTexturedRect(Texture tex, Rect rect, float u1, float v1, float u2, float v2)
        {
            rect = Translate(rect);

            if (isClipEnabled)
            {
                // cpu scissors test
                if (rect.Y < ClipRegion.Y)
                {
                    int oldHeight = rect.Height;
                    int delta = ClipRegion.Y - rect.Y;
                    rect.Y = ClipRegion.Y;
                    rect.Height -= delta;

                    if (rect.Height <= 0)
                    {
                        return;
                    }

                    float dv = (float)delta / (float)oldHeight;

                    v1 += dv * (v2 - v1);
                }

                if ((rect.Y + rect.Height) > (ClipRegion.Y + ClipRegion.Height))
                {
                    int oldHeight = rect.Height;
                    int delta = (rect.Y + rect.Height) - (ClipRegion.Y + ClipRegion.Height);

                    rect.Height -= delta;

                    if (rect.Height <= 0)
                    {
                        return;
                    }

                    float dv = (float)delta / (float)oldHeight;

                    v2 -= dv * (v2 - v1);
                }

                if (rect.X < ClipRegion.X)
                {
                    int oldWidth = rect.Width;
                    int delta = ClipRegion.X - rect.X;
                    rect.X = ClipRegion.X;
                    rect.Width -= delta;

                    if (rect.Width <= 0)
                    {
                        return;
                    }

                    float du = (float)delta / (float)oldWidth;

                    u1 += du * (u2 - u1);
                }

                if ((rect.X + rect.Width) > (ClipRegion.X + ClipRegion.Width))
                {
                    int oldWidth = rect.Width;
                    int delta = (rect.X + rect.Width) - (ClipRegion.X + ClipRegion.Width);

                    rect.Width -= delta;

                    if (rect.Width <= 0)
                    {
                        return;
                    }

                    float du = (float)delta / (float)oldWidth;

                    u2 -= du * (u2 - u1);
                }
            }

            buffer.AddRectangle(rect, new Vector2(u1,v1), new Vector2(u2,v1), new Vector2(u2,v2), new Vector2(u1,v2), tex.GetImage(), color);
        }

        public void DrawTexturedRect(Texture t, Rect rect, Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomRightUV, Vector2 bottomLeftUV)
        {
            rect = Translate(rect);

            buffer.AddRectangle(rect,topLeftUV,topRightUV,bottomRightUV,bottomLeftUV, t.GetImage(), color);
        }

        public override Vector2i MeasureText(Flood.GUI.Font font, string text) 
        {
            return TextRenderer.MeasureText(text,font);
        }

        public override void RenderText(Flood.GUI.Font font, Vector2i position, string text)
        {
           TextRenderer.DrawText(this, font, position, text);
        }

        public override Color PixelColor(Texture texture, uint x, uint y, Color defaultColor)
        {
            var image = texture.GetImage().Resolve();
            if(image == null)
                return defaultColor;

            var offset = (int)(4 * (x + y * texture.Width));
            var data = image.GetBuffer();

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

        Flood.GUI.Controls.Canvas canvas;

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

            mouse = inputManager.GetMouse();
            mouse.MouseMove += ProcessMouseMove;
            mouse.MouseDrag += ProcessMouseDrag;
            mouse.MouseButtonPress += ProcessMouseButtonPressed;
            mouse.MouseButtonRelease += ProcessMouseButtonReleased;
            mouse.MouseWheelMove += ProcessMouseWheel;

            keyboard = inputManager.GetKeyboard();
            keyboard.KeyPress += ProcessKeyDown;
            keyboard.KeyRelease += ProcessKeyUp;
        }

        ~GwenInput()
        {
            
        }

        public void Dispose()
        {
            Log.Info("Disposing GwenInput");

            mouse.MouseMove -= ProcessMouseMove;
            mouse.MouseDrag -= ProcessMouseDrag;
            mouse.MouseButtonPress -= ProcessMouseButtonPressed;
            mouse.MouseButtonRelease -= ProcessMouseButtonReleased;
            mouse.MouseWheelMove -= ProcessMouseWheel;

            keyboard.KeyPress -= ProcessKeyDown;
            keyboard.KeyRelease -= ProcessKeyUp;

            GC.SuppressFinalize(this);
        }

        public void Initialize(Canvas c)
        {
            canvas = c;
        }

        Key TranslateKeyCode(Keys key)
        {
            switch (key)
            {
            case Keys.Return: return Key.Return;
            case Keys.Escape: return Key.Escape;
            case Keys.Tab: return Key.Tab;
            case Keys.Space: return Key.Space;
            case Keys.Up: return Key.Up;
            case Keys.Down: return Key.Down;
            case Keys.Left: return Key.Left;
            case Keys.Right: return Key.Right;
            case Keys.Home: return Key.Home;
            case Keys.End: return Key.End;
            case Keys.Delete: return Key.Delete;
            case Keys.Back: return Key.Backspace;
            case Keys.LControl:
                this.m_AltGr = true;
                return Key.Control;
            case Keys.LAlt: return Key.Alt;
            case Keys.LShift: return Key.Shift;
            case Keys.RControl: return Key.Control;
            case Keys.RAlt: 
                if (m_AltGr)
                    canvas.Input_Key(Key.Control, false);
                return Key.Alt;
            case Keys.RShift: return Key.Shift;
                
            }
            return Key.Invalid;
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
        
            if (ch != ' ')
            {
                canvas.Input_Character(ch);
            }
        
            var key = TranslateKeyCode(keyEvent.KeyCode);
            canvas.Input_Key(key, true);
        }

        void ProcessKeyUp(KeyEvent keyEvent)
        {
            var ch = TranslateChar(keyEvent.KeyCode);
            var key = TranslateKeyCode(keyEvent.KeyCode);
            canvas.Input_Key(key, false);
        }
    }
}