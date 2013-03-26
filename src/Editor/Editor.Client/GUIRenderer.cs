﻿using System;
using System.Collections.Generic;
using System.Drawing;
using Flood;
using Flood.GUI;
using Flood.GUI.Controls;
using Flood.GUI.Input;

using Color = Flood.Color;
using Image = Flood.Image;
using StringTextureCache = System.Collections.Generic.Dictionary<
    System.Tuple<System.String, Flood.GUI.Font>, Flood.GUI.Texture>;

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
            gb.Declarations.Add(new VertexElement(VertexAttribute.Color, VertexDataType.Float, 4));
            gb.Declarations.CalculateStrides();

            zcount = -99;
        }

        ~ManagedGeometryBuffer()
        {
            //gb.Dispose();
        }

        public void AddRectangle(System.Drawing.Rectangle rect, System.Drawing.Color color)
        {
            var handle = new ResourceHandle<Image>(ResourceHandle<Image>.Invalid);
            AddRectangle(rect,Vector2.Zero,Vector2.Zero,Vector2.Zero,Vector2.Zero, handle, color);
        }

        public void AddRectangle(Rectangle rect, 
            Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomLeftUV,Vector2 bottomRightUV,
            ResourceHandle<Image> imageHandle, System.Drawing.Color color)
        {
            var batchInfo = GetCreateBatchInfo(imageHandle);
            batchInfo.Ranges.Add((int) gb.GetNumVertices());

            var top = Math.Max(rect.Bottom, rect.Top);
            var bottom = Math.Min(rect.Bottom, rect.Top);
            var left = rect.Left;
            var right = rect.Right;

            Vertex v1, v2, v3, v4;

            v1.Position = new Vector3(left, bottom, zcount);
            v2.Position = new Vector3(right, bottom, zcount);
            v3.Position = new Vector3(right, top, zcount);
            v4.Position = new Vector3(left, top, zcount);

            //TODO optimize precision/usage
            zcount += 0.001f;

            var c = new Color(color.R/255.0f, color.G/255.0f, color.B/255.0f, color.A/255.0f);

            v1.Color = c;
            v2.Color = c;
            v3.Color = c;
            v4.Color = c;

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

    public class TextureUtil
    {
        public static void LoadTextureInternal(Flood.GUI.Texture tex, System.Drawing.Bitmap bmp)
        {
            var data = bmp.LockBits(new System.Drawing.Rectangle(0, 0, bmp.Width, bmp.Height),
                System.Drawing.Imaging.ImageLockMode.ReadOnly,
                System.Drawing.Imaging.PixelFormat.Format32bppArgb);

            var bytes = (uint)(Math.Abs(data.Stride) * bmp.Height);
            LoadTextureInternal(tex, data.Scan0, bytes);
            bmp.UnlockBits(data);
        }

        public static void LoadTextureInternal(Flood.GUI.Texture tex, IntPtr data, uint size) 
        {
            var handle = Image.Create(Allocator.GetHeap(), (uint)tex.Width, (uint)tex.Height,
                PixelFormat.B8G8R8A8);

            var image = handle.Resolve();
            image.SetBuffer(data, size);

            tex.RendererData = handle;
        }
    };

    public class TextRenderer
    {
        static readonly Flood.TextureAtlas textureAtlas;

        static TextRenderer()
        {
            textureAtlas = new TextureAtlas(512);
        }

        static Flood.Font GetOrLoadFont(Flood.GUI.Font font)
        {
            var ttfont = font.RendererData as Flood.Font;
            if(ttfont != null)
                return ttfont;

            ttfont = new TrueTypeFont("");
            font.RendererData = ttfont; 
            return ttfont;
        }

        public static System.Drawing.Point MeasureText(System.String text, Flood.GUI.Font font)
        {
            var ret = new Point(0,font.Size);

            var ttfont = GetOrLoadFont(font);
            for(var i = 0; i < text.Length; i++)
            {
                var c = text[i];
                Glyph glyph;
                var foundGlyph = ttfont.GetGlyph(c, out glyph);
                if (!foundGlyph)
                {
                    Log.Warn("Glyph not found for character " + c);
                    continue;
                }

                var kernX = 0;
                if(i < text.Length - 1) 
                    kernX = ttfont.GetKerning(text[i], text[i + 1]).X;
                ret.X += (int) (glyph.Advance + kernX + 0.5);
            }

            return ret;
        }

        public static void DrawText(GwenRenderer renderer, Flood.GUI.Font font, Point position, String text)
        {
            var ttfont = GetOrLoadFont(font);
            for(var i = 0; i < text.Length; i++)
            {
                char c = text[i];
                Glyph glyph;
                var foundGlyph = ttfont.GetGlyph(c, out glyph);
                if(!foundGlyph)
                {
                    Log.Warn("Glyph not found for character "+c);
                    continue;
                }

                if (glyph.Image.Id != ResourceHandle<Resource>.Invalid)
                {
                    SubTexture subTexture;
                    var subTextureFound = textureAtlas.GetImageSubTexture(glyph.Image, out subTexture);
                    if(!subTextureFound){
                        textureAtlas.AddImage(glyph.Image);
                        subTextureFound = textureAtlas.GetImageSubTexture(glyph.Image, out subTexture);
                        if(!subTextureFound)
                        {
                            Log.Warn("subTexture not Found\n");
                            continue;
                        }
                    }

                    var atlasImageHandle = textureAtlas.GetAtlasImageHandle();
                    Image atlasImage = atlasImageHandle.Resolve();

                    var texture = new Flood.GUI.Texture(renderer);
                    texture.Width = (int) atlasImage.GetWidth();
                    texture.Height = (int) atlasImage.GetHeight();
                    texture.RendererData = atlasImageHandle;

                    Image glyphImage = glyph.Image.Resolve();

                    var renderRect = new System.Drawing.Rectangle(position.X, position.Y + glyph.BaseLineOffset, 
                        (int)glyphImage.GetWidth(), (int)glyphImage.GetHeight());

                    Vector2 topLeftUV, topRightUV, bottomLeftUV, bottomRightUV;

                    var rect = subTexture.Rect;
            
                    float width = atlasImage.GetWidth();
                    float height = atlasImage.GetHeight();
                    float u1 = rect.X/width;
                    float v1 = rect.Y/height;
                    float u2 = (rect.X+rect.Width)/width;
                    float v2 = (rect.Y+rect.Height)/height;

                    if(!subTexture.IsRotated)
                    {
                        topLeftUV = new Vector2(u1,v1);
                        topRightUV = new Vector2(u2,v1);
                        bottomLeftUV = new Vector2(u1,v2);
                        bottomRightUV = new Vector2(u2,v2);
                    }
                    else
                    {
                        topLeftUV = new Vector2(u1,v2);
                        topRightUV = new Vector2(u1,v1);
                        bottomLeftUV = new Vector2(u2,v2);
                        bottomRightUV = new Vector2(u2,v1);
                    }

                    renderer.DrawTexturedRect(texture,renderRect,topLeftUV,topRightUV,bottomLeftUV,bottomRightUV);
                    texture.Dispose();
                }

                if (i < text.Length-1){
                    Vector2i kern = ttfont.GetKerning(text[i],text[i+1]);
                    position.X += (int)(glyph.Advance + kern.X + 0.5);
                    position.Y += kern.Y;
                }
            }
        }
    };

    public class GwenRenderer : Flood.GUI.Renderers.Renderer
    {
        readonly ManagedGeometryBuffer buffer;

        System.Drawing.Color color;
        Dictionary<Tuple<string, Flood.GUI.Font>, TextRenderer> stringCache;
    
        bool isClipEnabled;

        public GwenRenderer()
        {
            buffer = new ManagedGeometryBuffer();
        }

        public override void Dispose()
        {
            TextRenderer.ClearCache();
            base.Dispose();
        }

        public void Render(RenderBlock rb)
        {
            buffer.Render(rb);
        }

        public void Clear()
        {
            buffer.Clear();
        }

        public override void DrawFilledRect(System.Drawing.Rectangle rect) 
        {
            rect = Translate(rect);
            buffer.AddRectangle(rect, color);
        }

        public override System.Drawing.Color DrawColor 
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

        public override void DrawTexturedRect(Flood.GUI.Texture tex, System.Drawing.Rectangle rect, float u1, float v1, float u2, float v2)
        {
            if(tex.RendererData == null)
                DrawFilledRect(rect);

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

            var handle = (ResourceHandle<Image>) tex.RendererData;

            buffer.AddRectangle(rect, new Vector2(u1,v1), new Vector2(u2,v1), new Vector2(u1,v2), new Vector2(u2,v2), handle, color);
        }

        public void DrawTexturedRect(Flood.GUI.Texture t, System.Drawing.Rectangle rect, Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomLeftUV,Vector2 bottomRightUV)
        {
            if(t.RendererData == null){
                DrawFilledRect(rect);
            }

            var handle = (ResourceHandle<Image>) t.RendererData;

            rect = Translate(rect);

            buffer.AddRectangle(rect,topLeftUV,topRightUV,bottomLeftUV,bottomRightUV, handle, color);
        }

        public override System.Drawing.Point MeasureText(Flood.GUI.Font font, System.String text) 
        {
            return TextRenderer.MeasureText(text,font);
        }

        public override void RenderText(Flood.GUI.Font font, System.Drawing.Point position, System.String text)
        {
           TextRenderer.DrawText(this, font, position, text);
        }

        public override void LoadTexture(Flood.GUI.Texture tex)
        {
            var resMan = FloodEngine.GetEngine().GetResourceManager();
            var options = new ResourceLoadOptions {Name = tex.Name, AsynchronousLoad = false};
            var resourceHandle = resMan.LoadResource<Image>(options);
        
            if(resourceHandle.Id == ResourceHandle<Resource>.Invalid)
            {
                tex.RendererData = null;
                return;
            }

            var image = resourceHandle.Resolve();
            tex.Width = (int)image.GetWidth();
            tex.Height = (int)image.GetHeight();
            tex.RendererData = resourceHandle;
        }

        public override void LoadTextureBitmap(Flood.GUI.Texture t, System.Drawing.Bitmap bitmap)
        {
            TextureUtil.LoadTextureInternal(t,bitmap);
        }

        public override void FreeTexture(Flood.GUI.Texture tex)
        {
            if (tex.RendererData == null)
                return;

            var handle = (ResourceHandle<Image>) tex.RendererData;
            var image = handle.Resolve();

            FloodEngine.GetEngine().GetResourceManager().RemoveResource(image);
        }

        public override System.Drawing.Color PixelColor(Flood.GUI.Texture texture,
            uint x, uint y, System.Drawing.Color defaultColor)
        {
            if(texture.RendererData == null)
                return defaultColor;

            var handle = (ResourceHandle<Image>) texture.RendererData;
            var image = handle.Resolve();

            int offset = (int)(4 * (x + y * texture.Width));
            var data = image.GetBuffer();

            var pixel = System.Drawing.Color.FromArgb(data[offset + 3], data[offset + 0],
                data[offset + 1], data[offset + 2]);
        
            // Retrieving the entire texture for a single pixel read
            // is kind of a waste - maybe cache this pointer in the texture
            // data and then release later on? It's never called during runtime
            // - only during initialization.
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


