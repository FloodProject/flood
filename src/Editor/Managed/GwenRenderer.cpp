
#include "Editor/API.h"
#include "Graphics/GeometryBuffer.h"
#include "Engine/Geometry/Quad.h"
#include "CLIInterop.h"
#include <vcclr.h>

#using "GUI_d.dll"
#using "EditorManaged_d.dll"
#using <System.Drawing.dll>

using namespace Gwen; 
using namespace System::Collections::Generic;

class ManagedGeometryBuffer {

	struct BatchInfo {
		RenderBatch* batch;
		std::vector<int> ranges;
	};
	
	GeometryBuffer* gb;
	std::map<HandleId,BatchInfo> batches;

	BatchInfo& GetCreateBatchInfo(ImageHandle imageHandle){
		HandleId hId = imageHandle.getId();
		if(batches.find(hId) != batches.end()){
			return batches[hId];
		}else {
			RenderBatch* batch = new RenderBatch();
			batch->setGeometryBuffer(gb);
			batch->setRenderLayer(RenderLayer::Overlays);
			batch->setPrimitiveType(PrimitiveType::Quads);
			BatchInfo bInfo;
			bInfo.batch = batch;
			batches[hId] = bInfo;

			MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "GwenGui");
			Material* mat = materialHandle.Resolve();
			mat->setBackfaceCulling(false);
			mat->setDepthWrite(false);
			mat->setBlending(BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha );
			if(hId>0){
				mat->setShader("TexColor");
				mat->setTexture(0,imageHandle);
				mat->getTextureUnit(0).setWrapMode(TextureWrapMode::Clamp);
			}
			batch->setMaterial(materialHandle);

			return batches[hId];
		}
	}

public:

	struct Vertex
	{
		Vector2 position;
		Vector2 uv;
		Color color;
	};

	ManagedGeometryBuffer(){
		gb = AllocateHeap(GeometryBuffer);

		gb->declarations.add(VertexElement( VertexAttribute::Position, VertexDataType::Float, 2));
		gb->declarations.add(VertexElement( VertexAttribute::TexCoord0, VertexDataType::Float, 2));
		gb->declarations.add(VertexElement( VertexAttribute::Color, VertexDataType::Float, 4));
		gb->declarations.calculateStrides();
	}

	~ManagedGeometryBuffer(){
		Deallocate(gb);
	}

	void addRectangle(System::Drawing::Rectangle rect, System::Drawing::Color color)
	{
		addRectangle(rect,Vector2::Zero,Vector2::Zero, 0, color);
	}

	void addRectangle(System::Drawing::Rectangle rect, Vector2 uv1, Vector2 uv2, ImageHandle imageHandle, System::Drawing::Color color)
	{
		BatchInfo& batchInfo = GetCreateBatchInfo(imageHandle); 
		batchInfo.ranges.push_back(gb->getNumVertices());

		int top = std::max(rect.Bottom,rect.Top);
		int bottom = std::min(rect.Bottom,rect.Top);
		int left = rect.Left;
		int right = rect.Right;

		Vertex v1,v2,v3,v4;

		v1.position = Vector2(left, bottom);
		v2.position = Vector2(right, bottom);
		v3.position = Vector2(right, top);
		v4.position = Vector2(left, top);
		
		Color c = Color(color.R/255.0f,color.G/255.0f,color.B/255.0f,color.A/255.0f);

		v1.color = c;
		v2.color = c;
		v3.color = c;
		v4.color = c;

		v1.uv = uv1;
		v2.uv = Vector2(uv2.x, uv1.y);
		v3.uv = uv2;
		v4.uv = Vector2(uv1.x, uv2.y);
		
		// Vertex buffer setup
		gb->add((uint8*)&v1,sizeof(Vertex));
		gb->add((uint8*)&v2,sizeof(Vertex));
		gb->add((uint8*)&v3,sizeof(Vertex));
		gb->add((uint8*)&v4,sizeof(Vertex));
	}

	
	void Render(RenderBlock& rb)
	{
		for(auto it = batches.begin(); it != batches.end(); ++it){

			BatchInfo& bInfo = it->second;

			if(bInfo.ranges.size() > 0)
			{
				bInfo.batch->range.start = gb->getNumIndices();
				for(size_t ir = 0; ir < bInfo.ranges.size(); ++ir){
					int vertexIndex = bInfo.ranges[ir];
					gb->addIndex(vertexIndex++);
					gb->addIndex(vertexIndex++);
					gb->addIndex(vertexIndex++);
					gb->addIndex(vertexIndex);
				}
				bInfo.batch->range.end = gb->getNumIndices();

				RenderState state(bInfo.batch);
				rb.renderables.push_back(state);
			}
		}
	}

	void Clear(){
		for(auto it = batches.begin(); it != batches.end(); ++it){
			BatchInfo& bInfo = it->second;
			bInfo.ranges.clear();
		}
		gb->clear();
		gb->indexData.clear();
	}
};

public ref class TextRenderer
{
	typedef System::Collections::Generic::Dictionary<System::Tuple<System::String^, Gwen::Font^>^, Gwen::Texture^> StringTextureCache;

	static System::Drawing::StringFormat^ m_StringFormat;
	static System::Drawing::Graphics^ m_Graphics; // only used for text measurement
	static StringTextureCache^ m_StringCache = gcnew StringTextureCache();

	static TextRenderer(){
		m_StringFormat = gcnew System::Drawing::StringFormat(System::Drawing::StringFormat::GenericTypographic);
		m_StringFormat->FormatFlags = m_StringFormat->FormatFlags | System::Drawing::StringFormatFlags::MeasureTrailingSpaces;
		m_Graphics = System::Drawing::Graphics::FromImage(gcnew System::Drawing::Bitmap(1024, 1024, System::Drawing::Imaging::PixelFormat::Format32bppArgb));
	}

	static void LoadTextureInternal(Gwen::Texture^ t, System::Drawing::Bitmap^ bmp)
	{
		ImageHandle iHandle = ImageCreate(AllocatorGetHeap(),t->Width,t->Height, PixelFormat::B8G8R8A8);
		
		//array to vector
		array<byte>^ imageBytes;
		{
			System::Drawing::Imaging::BitmapData^ data = bmp->LockBits(System::Drawing::Rectangle(0, 0, bmp->Width, bmp->Height), System::Drawing::Imaging::ImageLockMode::ReadOnly, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
			int bytes  = std::abs(data->Stride) * bmp->Height;
			imageBytes = gcnew array<byte>(bytes);
			System::Runtime::InteropServices::Marshal::Copy(data->Scan0, imageBytes, 0, bytes);
		}
		//array to vector
		std::vector<byte> buffer(imageBytes->Length);
		{
			pin_ptr<byte> pin(&imageBytes[0]);
			byte *first(pin), *last(pin + imageBytes->Length);
			std::copy(first, last, buffer.begin());
		}

		iHandle.Resolve()->setBuffer(buffer);
		t->RendererData = iHandle.getId();
		iHandle.addReference(); //hackzito
	}

	static bool LoadFont(Gwen::Font^ font)
    {
        //Debug.Print(String.Format("LoadFont {0}", font->FaceName));
        font->RealSize = font->Size;// * Scale;
        System::Drawing::Font^ sysFont = safe_cast<System::Drawing::Font^>(font->RendererData);

        if (sysFont != nullptr)
            delete(sysFont);

        // apaprently this can't fail @_@
        // "If you attempt to use a font that is not supported, or the font is not installed on the machine that is running the application, the Microsoft Sans Serif font will be substituted."
        sysFont = gcnew System::Drawing::Font(font->FaceName, font->Size);
        font->RendererData = sysFont; 
        return true;
   }

   static void FreeFont(Gwen::Font^ font)
   {
        //Debug.Print(String.Format("FreeFont {0}", font->FaceName));
        if (font->RendererData == nullptr)
            return;

        //Debug.Print(String.Format("FreeFont {0} - actual free", font.FaceName));
        System::Drawing::Font^ sysFont = safe_cast<System::Drawing::Font^>(font->RendererData);
        if (sysFont == nullptr)
        //    throw new System::InvalidOperationException("Freeing empty font");
			return;

        //sysFont->Dispose();
        font->RendererData = nullptr;
    }

    static  System::Drawing::Font^ ConvertFont(Gwen::Font^ font)
	{
		System::Drawing::Font^ sysFont = safe_cast<System::Drawing::Font^>(font->RendererData);
        if (sysFont == nullptr || std::abs(font->RealSize - font->Size /** Scale*/) > 2)
        {
            FreeFont(font);
            LoadFont(font);
            sysFont = safe_cast<System::Drawing::Font^>(font->RendererData);
        }
		return sysFont;
	}

	static System::Drawing::Point MeasureText(System::Drawing::Font^ font, System::String^ text, System::Drawing::StringFormat^ stringFormat)
    {
        System::Drawing::SizeF size = m_Graphics->MeasureString(text, font, System::Drawing::Point::Empty, stringFormat);
        return System::Drawing::Point((int)floor(size.Width+0.5), (int)floor(size.Height+0.5));
    }

	static Gwen::Texture^ GetTexture(Gwen::Font^ font, System::String^ text){
		System::Tuple<System::String^, Gwen::Font^>^ key = gcnew System::Tuple<System::String^, Gwen::Font^>(text, font);
		if (m_StringCache->ContainsKey(key))
			return m_StringCache[key];
		return nullptr;
	}

	static void AddTexture(Gwen::Font^ font, System::String^ text, Gwen::Texture^ texture){
		System::Tuple<System::String^, Gwen::Font^>^ key = gcnew System::Tuple<System::String^, Gwen::Font^>(text, font);
		m_StringCache->Add(key,texture);
	}

public:

	static void ClearCache(){
		for each(Gwen::Texture^ tex in m_StringCache->Values){
			delete(tex);
		}
		m_StringCache->Clear();
	}

	//TODO use scale, remove renderer
    static Gwen::Texture^ StringToTexture(System::String^ text, Gwen::Font^ font, Gwen::Renderer::Base^ renderer)
    {
		System::Drawing::Brush^ brush = System::Drawing::Brushes::White;
		Gwen::Texture^ texture = GetTexture(font,text);
        if(texture != nullptr){ //TODO Check stringFormat
            return texture;
        }

		System::Drawing::Font^ sysFont = ConvertFont(font);

		System::Drawing::Point size = TextRenderer::MeasureText(sysFont, text, m_StringFormat);
		texture = gcnew Gwen::Texture(renderer);
		texture->Width = size.X;
		texture->Height = size.Y;

		System::Drawing::Bitmap^ bmp = gcnew System::Drawing::Bitmap(size.X, size.Y, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
        System::Drawing::Graphics^ gfx = System::Drawing::Graphics::FromImage(bmp);

        // NOTE:    TextRenderingHint.AntiAliasGridFit looks sharper and in most cases better
        //          but it comes with a some problems.
        //
        //          1.  Graphic.MeasureString and format.MeasureCharacterRanges 
        //              seem to return wrong values because of this.
        //
        //          2.  While typing the kerning changes in random places in the sentence.
        // 
        //          Until 1st problem is fixed we should use TextRenderingHint.AntiAlias...  :-(

        gfx->TextRenderingHint = System::Drawing::Text::TextRenderingHint::AntiAlias;
        gfx->Clear(System::Drawing::Color::Transparent);

        gfx->DrawString(text, sysFont, brush, System::Drawing::Point::Empty, m_StringFormat); // render text on the bitmap
		LoadTextureInternal(texture,bmp);
		AddTexture(font,text,texture);
		return texture;
    }

	static System::Drawing::Point MeasureText(System::String^ text, Gwen::Font^ font)
    {
		System::Drawing::Font^ sysFont = ConvertFont(font);
        System::Drawing::SizeF size = m_Graphics->MeasureString(text, sysFont, System::Drawing::Point::Empty, m_StringFormat);
        return System::Drawing::Point((int)floor(size.Width+0.5), (int)floor(size.Height+0.5));
    }

};

public ref class GwenRenderer : Gwen::Renderer::Base {
		
	ManagedGeometryBuffer* buffer;

	System::Drawing::Color m_Color;
	System::Collections::Generic::Dictionary<System::Tuple<System::String^, Gwen::Font^>^, TextRenderer^>^ m_StringCache;
	
	bool m_ClipEnabled;
	/*private const int MaxVerts = 1024;
    private Color m_Color;
    private int m_VertNum;
    private readonly Vertex[] m_Vertices;
    private readonly int m_VertexSize;

    private readonly Dictionary<Tuple<String, Font>, TextRenderer> m_StringCache;
    
    private int m_DrawCallCount;
    
    private bool m_TextureEnabled;
    static private int m_LastTextureID;

    private bool m_WasBlendEnabled, m_WasTexture2DEnabled, m_WasDepthTestEnabled;
    private int m_PrevBlendSrc, m_PrevBlendDst, m_PrevAlphaFunc;
    private float m_PrevAlphaRef;
    private bool m_RestoreRenderState;

   
	*/

public:
	GwenRenderer(/*bool restoreRenderState = true*/)
        : Gwen::Renderer::Base()
    {
		
		buffer = AllocateHeap(ManagedGeometryBuffer);
		
		/* m_Vertices = new Vertex[MaxVerts];
        m_VertexSize = Marshal.SizeOf(m_Vertices[0]);
        
        
        
        m_RestoreRenderState = restoreRenderState;*/
    }

	void Render(RenderBlock& rb){
		buffer->Render(rb);
	}

	void Clear(){
		buffer->Clear();
	}

	/* 
    virtual void Dispose()
    {
        FlushTextCache();
        base.Dispose();
    }
	*/
    virtual void Begin() override
    {
       /* if (m_RestoreRenderState)
        {
            // Get previous parameter values before changing them.
            GL.GetInteger(GetPName.BlendSrc, out m_PrevBlendSrc);
            GL.GetInteger(GetPName.BlendDst, out m_PrevBlendDst);
            GL.GetInteger(GetPName.AlphaTestFunc, out m_PrevAlphaFunc);
            GL.GetFloat(GetPName.AlphaTestRef, out m_PrevAlphaRef);

            m_WasBlendEnabled = GL.IsEnabled(EnableCap.Blend);
            m_WasTexture2DEnabled = GL.IsEnabled(EnableCap.Texture2D);
            m_WasDepthTestEnabled = GL.IsEnabled(EnableCap.DepthTest);
        }

        // Set default values and enable/disable caps.
        GL.BlendFunc(BlendingFactorSrc.SrcAlpha, BlendingFactorDest.OneMinusSrcAlpha);
        GL.AlphaFunc(AlphaFunction.Greater, 1.0f);
        GL.Enable(EnableCap.Blend);
        GL.Disable(EnableCap.DepthTest);
        GL.Disable(EnableCap.Texture2D);

        m_VertNum = 0;
        m_DrawCallCount = 0;
        m_ClipEnabled = false;
        m_TextureEnabled = false;
        m_LastTextureID = -1;

        GL.EnableClientState(ArrayCap.VertexArray);
        GL.EnableClientState(ArrayCap.ColorArray);
        GL.EnableClientState(ArrayCap.TextureCoordArray);
    */}/*

    virtual void End()
    {
        Flush();

        if (m_RestoreRenderState)
        {
            GL.BindTexture(TextureTarget.Texture2D, 0);

            // Restore the previous parameter values.
            GL.BlendFunc((BlendingFactorSrc)m_PrevBlendSrc, (BlendingFactorDest)m_PrevBlendDst);
            GL.AlphaFunc((AlphaFunction)m_PrevAlphaFunc, m_PrevAlphaRef);

            if (!m_WasBlendEnabled)
                GL.Disable(EnableCap.Blend);

            if (m_WasTexture2DEnabled && !m_TextureEnabled)
                GL.Enable(EnableCap.Texture2D);

            if (m_WasDepthTestEnabled)
                GL.Enable(EnableCap.DepthTest);
        }

        GL.DisableClientState(ArrayCap.VertexArray);
        GL.DisableClientState(ArrayCap.ColorArray);
        GL.DisableClientState(ArrayCap.TextureCoordArray);
    }

    /// <summary>
    /// Returns number of cached strings in the text cache.
    /// </summary>
    public int TextCacheSize { get { return m_StringCache.Count; } }

    public int DrawCallCount { get { return m_DrawCallCount; } }

    public int VertexCount { get { return m_VertNum; } }
    /// <summary>
    /// Clears the text rendering cache. Make sure to call this if cached strings size becomes too big (check TextCacheSize).
    /// </summary>
    public void FlushTextCache()
    {
        // todo: some auto-expiring cache? based on number of elements or age
        foreach (var textRenderer in m_StringCache.Values)
        {
            textRenderer.Dispose();
        }
        m_StringCache.Clear();
    }

    private unsafe void Flush()
    {
        if (m_VertNum == 0) return;

        fixed (short* ptr1 = &m_Vertices[0].x)
        fixed (byte* ptr2 = &m_Vertices[0].r)
        fixed (float* ptr3 = &m_Vertices[0].u)
        {
            GL.VertexPointer(2, VertexPointerType.Short, m_VertexSize, (IntPtr)ptr1);
            GL.ColorPointer(4, ColorPointerType.UnsignedByte, m_VertexSize, (IntPtr)ptr2);
            GL.TexCoordPointer(2, TexCoordPointerType.Float, m_VertexSize, (IntPtr)ptr3);

            GL.DrawArrays(BeginMode.Quads, 0, m_VertNum);
        }

        m_DrawCallCount++;
        m_VertNum = 0;
    }
	*/
    virtual void DrawFilledRect(System::Drawing::Rectangle rect) override
    {
		rect = Translate(rect);

		buffer->addRectangle(rect, m_Color);
    }
	
    property virtual System::Drawing::Color DrawColor 
	{
		System::Drawing::Color get() override { return m_Color; }
		void set (System::Drawing::Color value) override  { m_Color = value;}
	}


    virtual void StartClip() override
    {
        m_ClipEnabled = true;
    }

    virtual void EndClip() override
    {
        m_ClipEnabled = false;
    }

    virtual void DrawTexturedRect(Gwen::Texture^ t, System::Drawing::Rectangle rect, float u1, float v1, float u2, float v2) override
    {
		if(t->RendererData == nullptr){
			DrawFilledRect(rect);
		}

		ImageHandle iHandle;
		HandleId hId = (HandleId)t->RendererData;
		iHandle.setId(hId);

		rect = Translate(rect);

		if (m_ClipEnabled)
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

		buffer->addRectangle(rect,Vector2(u1,v1), Vector2(u2,v2), iHandle, m_Color);
	}

    virtual System::Drawing::Point MeasureText(Gwen::Font^ font, System::String^ text) override
    {
        return TextRenderer::MeasureText(text,font);
    }

    virtual void RenderText(Gwen::Font^ font, System::Drawing::Point position, System::String^ text) override
    {
       Gwen::Texture^ texture = TextRenderer::StringToTexture(text, font, this); // renders string on the texture
	   DrawTexturedRect(texture, System::Drawing::Rectangle(position.X, position.Y, texture->Width, texture->Height),0,0,1,1);
    }
	
    virtual void LoadTexture(Gwen::Texture^ t) override
    {
		ResourceLoadOptions options; 
		options.name = clix::marshalString<clix::E_UTF8>(t->Name);
		options.asynchronousLoad = false;
		ImageHandle iHandle = HandleCast<Image>(GetResourceManager()->loadResource(options));
		
		if(iHandle == HandleInvalid){
			t->RendererData = nullptr;
			return;
		}

		Image* img = iHandle.Resolve();
		t->Width =	img->getWidth();
		t->Height = img->getHeight();
		t->RendererData = iHandle.getId();
	}

    virtual void LoadTextureRaw(Gwen::Texture^ t, array<byte>^ pixelData) override
    {/*
        Bitmap bmp;
        try
        {
            unsafe
            {
                fixed (byte* ptr = &pixelData[0])
                    bmp = new Bitmap(t.Width, t.Height, 4 * t.Width, PixelFormat.Format32bppArgb, (IntPtr)ptr);
            }
        }
        catch (Exception)
        {
            t.Failed = true;
            return;
        }

        int glTex;

        // Create the opengl texture
        GL.GenTextures(1, out glTex);
        GL.BindTexture(TextureTarget.Texture2D, glTex);
        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMagFilter.Nearest);
        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Nearest);

        // Sort out our GWEN texture
        t.RendererData = glTex;

        var data = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly,
            PixelFormat.Format32bppArgb);

        GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, t.Width, t.Height, 0, global::OpenTK.Graphics.OpenGL.PixelFormat.Rgba, PixelType.UnsignedByte, data.Scan0);

        m_LastTextureID = glTex;

        bmp.UnlockBits(data);
        bmp.Dispose();
    */}

    virtual void FreeTexture(Gwen::Texture^ t) override
    {/*
        if (t.RendererData == null)
            return;
        int tex = (int)t.RendererData;
        if (tex == 0)
            return;
        GL.DeleteTextures(1, ref tex);
        t.RendererData = null;
    */}

    virtual System::Drawing::Color PixelColor(Gwen::Texture^ texture, System::UInt32 x, System::UInt32 y, System::Drawing::Color defaultColor) override
    {
		if(texture->RendererData == nullptr){
			return defaultColor;
		}

		ImageHandle iHandle;
		HandleId hId = (HandleId)texture->RendererData;
		iHandle.setId(hId);

		Image* img = iHandle.Resolve();

        System::Drawing::Color pixel;
        long offset = 4 * (x + y * texture->Width);
        std::vector<uint8>& data = img->getBuffer();

        pixel = System::Drawing::Color::FromArgb(data[offset + 3], data[offset + 0], data[offset + 1], data[offset + 2]);
        
        // Retrieving the entire texture for a single pixel read
        // is kind of a waste - maybe cache this pointer in the texture
        // data and then release later on? It's never called during runtime
        // - only during initialization.
        return pixel;
    }

};

class GwenInput
{

private:
	InputManager* inputManager;

	gcroot<Gwen::Control::Canvas^> m_Canvas;

    int m_MouseX;
    int m_MouseY;

    bool m_AltGr;

public:
    GwenInput(InputManager* inputManager)
    {
		this->inputManager = inputManager;
			
		m_Canvas = nullptr;
		m_MouseX = 0;
		m_MouseY = 0;
		m_AltGr = false;

		inputManager->getMouse()->onMouseMove.Connect(this, &GwenInput::ProcessMouseMove);
		inputManager->getMouse()->onMouseButtonPress.Connect(this, &GwenInput::ProcessMouseButtonPressed);
		inputManager->getMouse()->onMouseButtonRelease.Connect(this, &GwenInput::ProcessMouseButtonReleased);
		inputManager->getMouse()->onMouseWheelMove.Connect(this, &GwenInput::ProcessMouseWheel);

		inputManager->getKeyboard()->onKeyPress.Connect(this, &GwenInput::ProcessKeyDown);
		inputManager->getKeyboard()->onKeyRelease.Connect(this, &GwenInput::ProcessKeyUp);

    }


    void Initialize(gcroot<Gwen::Control::Canvas^> c)
    {
        m_Canvas = c;
    }


private:
	Gwen::Key TranslateKeyCode(Keys key)
    {

		switch (key)
        {
		case Keys::Return: return Gwen::Key::Return;
        case Keys::Escape: return Gwen::Key::Escape;
        case Keys::Tab: return Gwen::Key::Tab;
        case Keys::Space: return Gwen::Key::Space;
        case Keys::Up: return Gwen::Key::Up;
        case Keys::Down: return Gwen::Key::Down;
        case Keys::Left: return Gwen::Key::Left;
        case Keys::Right: return Gwen::Key::Right;
        case Keys::Home: return Gwen::Key::Home;
        case Keys::End: return Gwen::Key::End;
        case Keys::Delete: return Gwen::Key::Delete;
        case Keys::LControl:
            this->m_AltGr = true;
            return Gwen::Key::Control;
        case Keys::LAlt: return Gwen::Key::Alt;
        case Keys::LShift: return Gwen::Key::Shift;
        case Keys::RControl: return Gwen::Key::Control;
        case Keys::RAlt: 
            if (this->m_AltGr)
            {
                this->m_Canvas->Input_Key(Gwen::Key::Control, false);
            }
            return Gwen::Key::Alt;
        case Keys::RShift: return Gwen::Key::Shift;
                
        }
        return Gwen::Key::Invalid;
    }

    static char TranslateChar(Keys key)
    {
        if (key >= Keys::A && key <= Keys::Z)
            return (char)('a' + ((int)key - (int) Keys::A));
        return ' ';
    }

public:
	void ProcessMouseMove(const MouseMoveEvent& mouseMoveEvent){
		
		int dx = mouseMoveEvent.x - m_MouseX;
		int dy = mouseMoveEvent.y - m_MouseY;

		m_MouseX = mouseMoveEvent.x;
		m_MouseY = mouseMoveEvent.y;

		m_Canvas->Input_MouseMoved(m_MouseX, m_MouseY, dx, dy);
		
	}

	void ProcessMouseButtonPressed(const MouseButtonEvent& mouseButtonEvent){
		m_Canvas->Input_MouseButton((int) mouseButtonEvent.button, true);
	}

	void ProcessMouseButtonReleased(const MouseButtonEvent& mouseButtonEvent){
		m_Canvas->Input_MouseButton((int) mouseButtonEvent.button, false);
	}

	void ProcessMouseWheel(const MouseWheelEvent& mouseWheelEvent){
		m_Canvas->Input_MouseWheel(mouseWheelEvent.delta*60);
	}

    void ProcessKeyDown(const KeyEvent& keyEvent)
    {
        char ch = TranslateChar(keyEvent.keyCode);

        if (Gwen::Input::InputHandler::Instance->DoSpecialKeys(m_Canvas, ch))
            return;
        /*
        if (ch != ' ')
        {
            m_Canvas.Input_Character(ch);
        }
        */
        Key iKey = TranslateKeyCode(keyEvent.keyCode);

        m_Canvas->Input_Key(iKey, true);
    }

    void ProcessKeyUp(const KeyEvent& keyEvent)
    {
        char ch = TranslateChar(keyEvent.keyCode);

        Key iKey = TranslateKeyCode(keyEvent.keyCode);

        m_Canvas->Input_Key(iKey, false);
    }

   /* void KeyPress(object sender, KeyPressEventArgs e)
    {
        m_Canvas->Input_Character(e.KeyChar);   
    }*/

};

public ref class GUI {

	FlushEditor::Editor^ managedEditor;
	GwenRenderer^ renderer;
	GwenInput* input;

public:
	GUI(InputManager* inputManager){
		Initialize(inputManager);
	}

	delegate void Foo(int);

	void Initialize(InputManager* inputManager){

		renderer = gcnew GwenRenderer();
		managedEditor = gcnew FlushEditor::Editor();

		managedEditor->GUI->Init(renderer,"DefaultSkin.png");

		input = new GwenInput(inputManager);
		input->Initialize(managedEditor->GUI->Canvas);
	}

	void Close() {
		TextRenderer::ClearCache();
		delete(managedEditor);
	}

	void Render(RenderBlock& rb){
		renderer->Clear();
		managedEditor->GUI->Render();
		renderer->Render(rb);
	}
};

static gcroot<GUI^> gs_GUIInstance = nullptr;

void InitializeGUI(InputManager* inputManager){
	gs_GUIInstance = gcnew GUI(inputManager);
}

void CloseGUI() {
	gs_GUIInstance->Close();
	delete(gs_GUIInstance);
	gs_GUIInstance = nullptr; 
}

void RenderGUI(RenderBlock& rb) {
	gs_GUIInstance->Render(rb);
}

