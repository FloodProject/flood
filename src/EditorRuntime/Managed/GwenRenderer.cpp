
#include "Editor/API.h"
#include "Graphics/GeometryBuffer.h"
#include "Engine/Geometry/Quad.h"
#include "Engine/Resources/TrueTypeFont.h"
#include "Engine/Texture/TextureAtlas.h"
#include "CLIInterop.h"
#include <vcclr.h>

#using "Editor.Client.dll"
#using "EngineManaged.dll"
#using "EngineBindings.dll"
#using <System.Drawing.dll>

using namespace Flood::GUI; 
using namespace System::Collections::Generic;

class ManagedGeometryBuffer {

	struct BatchInfo {
		RenderBatch batch;
		std::vector<int> ranges;
	};
	
	GeometryBuffer* gb;
	std::map<HandleId,BatchInfo> batches;
	std::map<HandleId,MaterialHandle> materials;

	float zcount;

	MaterialHandle GetCreateMaterialHandle(ImageHandle imageHandle){
		HandleId hId = imageHandle.getId();
		if(materials.find(hId) == materials.end()){
			MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "GwenGui");
			Material* mat = materialHandle.Resolve();
			mat->setBackfaceCulling(false);
			//mat->setDepthWrite(false);
			mat->setBlending(BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha );
			
			if(hId==0){
				mat->setShader("VertexColor");
			} else {
				mat->setShader("TexColor");
				mat->setTexture(0,imageHandle);
				mat->getTextureUnit(0).setWrapMode(TextureWrapMode::Clamp);
			}
			materials[hId] = materialHandle;
		}
		return materials[hId];
	}

	BatchInfo& GetCreateBatchInfo(ImageHandle imageHandle){
		HandleId hId = imageHandle.getId();
		if(batches.find(hId) == batches.end()){
			RenderBatch batch;
			batch.setGeometryBuffer(gb);
			batch.setRenderLayer(RenderLayer::Overlays);
			batch.setPrimitiveType(PrimitiveType::Quads);

			MaterialHandle materialHandle = GetCreateMaterialHandle(imageHandle);
			batch.setMaterial(materialHandle);

			BatchInfo bInfo;
			bInfo.batch = batch;
			batches[hId] = bInfo;
		}

		return batches[hId];
	}

public:

	struct Vertex
	{
		Vector3 position;
		Vector2 uv;
		Color color;
	};

	ManagedGeometryBuffer(){
		gb = AllocateHeap(GeometryBuffer);

		gb->declarations.add(VertexElement( VertexAttribute::Position, VertexDataType::Float, 3));
		gb->declarations.add(VertexElement( VertexAttribute::TexCoord0, VertexDataType::Float, 2));
		gb->declarations.add(VertexElement( VertexAttribute::Color, VertexDataType::Float, 4));
		gb->declarations.calculateStrides();

		zcount = -100;
	}

	~ManagedGeometryBuffer(){
		Deallocate(gb);
	}

	void addRectangle(System::Drawing::Rectangle rect, System::Drawing::Color color)
	{
		addRectangle(rect,Vector2::Zero,Vector2::Zero,Vector2::Zero,Vector2::Zero, 0, color);
	}

	void addRectangle(System::Drawing::Rectangle rect, Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomLeftUV,Vector2 bottomRightUV, ImageHandle imageHandle, System::Drawing::Color color)
	{
		BatchInfo& batchInfo = GetCreateBatchInfo(imageHandle); 
		batchInfo.ranges.push_back(gb->getNumVertices());

		int top = std::max(rect.Bottom,rect.Top);
		int bottom = std::min(rect.Bottom,rect.Top);
		int left = rect.Left;
		int right = rect.Right;

		Vertex v1,v2,v3,v4;

		v1.position = Vector3(left, bottom,zcount);
		v2.position = Vector3(right, bottom,zcount);
		v3.position = Vector3(right, top,zcount);
		v4.position = Vector3(left, top,zcount);

		//TODO optimize precision/usage
		zcount += 0.001;
		
		Color c = Color(color.R/255.0f,color.G/255.0f,color.B/255.0f,color.A/255.0f);

		v1.color = c;
		v2.color = c;
		v3.color = c;
		v4.color = c;

		v1.uv = topLeftUV;
		v2.uv = topRightUV;
		v3.uv = bottomRightUV;
		v4.uv = bottomLeftUV;;
		
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
				bInfo.batch.range.start = gb->getNumIndices();
				for(size_t ir = 0; ir < bInfo.ranges.size(); ++ir){
					int vertexIndex = bInfo.ranges[ir];
					gb->addIndex(vertexIndex++);
					gb->addIndex(vertexIndex++);
					gb->addIndex(vertexIndex++);
					gb->addIndex(vertexIndex);
				}
				bInfo.batch.range.end = gb->getNumIndices();

				RenderState state(&bInfo.batch);
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
		zcount = -100;
	}
};

public ref class TextureUtil
{
public:

	static void LoadTextureInternal(Flood::GUI::Texture^ t, System::Drawing::Bitmap^ bmp)
	{
		System::Drawing::Imaging::BitmapData^ data = bmp->LockBits(System::Drawing::Rectangle(0, 0, bmp->Width, bmp->Height), System::Drawing::Imaging::ImageLockMode::ReadOnly, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
		int bytes = std::abs(data->Stride) * bmp->Height;
		LoadTextureInternal(t,(uint8*)data->Scan0.ToPointer(), bytes);
		bmp->UnlockBits(data);
	}

	static void LoadTextureInternal(Flood::GUI::Texture^ t, uint8* data, int size) 
	{
		ImageHandle iHandle = ImageCreate(AllocatorGetHeap(),t->Width,t->Height, PixelFormat::B8G8R8A8);
		
		//array to vector
		std::vector<byte> buffer(size);
		std::copy(data, data+size, buffer.begin());

		iHandle.Resolve()->setBuffer(buffer);
		t->RendererData = iHandle.getId();
		iHandle.addReference(); //hackzito
	}
};

ref class TextRenderer;

public ref class GwenRenderer : Flood::GUI::Renderers::Renderer {
		
	ManagedGeometryBuffer* buffer;

	System::Drawing::Color m_Color;
	System::Collections::Generic::Dictionary<System::Tuple<System::String^, Flood::GUI::Font^>^, TextRenderer^>^ m_StringCache;
	
	bool m_ClipEnabled;

public:
	GwenRenderer() : Flood::GUI::Renderers::Renderer()
	{
		
		buffer = AllocateHeap(ManagedGeometryBuffer);
	}

	void Render(RenderBlock& rb){
		buffer->Render(rb);
	}

	void Clear(){
		buffer->Clear();
	}
	

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

	virtual void StartClip() override;

	virtual void EndClip() override;

	virtual void DrawTexturedRect(Flood::GUI::Texture^ t, System::Drawing::Rectangle rect, float u1, float u2, float v1, float v2) override;

    void DrawTexturedRect(Flood::GUI::Texture^ t, System::Drawing::Rectangle rect, Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomLeftUV,Vector2 bottomRightUV);

	virtual System::Drawing::Point MeasureText(Flood::GUI::Font^ font, System::String^ text) override;

	virtual void RenderText(Flood::GUI::Font^ font, System::Drawing::Point position, System::String^ text) override;
	
	virtual void LoadTexture(Flood::GUI::Texture^ t) override;

	virtual void LoadTextureBitmap(Flood::GUI::Texture^ t, System::Drawing::Bitmap^ bitmap) override;

	virtual void FreeTexture(Flood::GUI::Texture^ t) override;

	virtual System::Drawing::Color PixelColor(Flood::GUI::Texture^ texture, System::UInt32 x, System::UInt32 y, System::Drawing::Color defaultColor) override;
	
};

public ref class TextRenderer
{
	typedef System::Collections::Generic::Dictionary<System::Tuple<System::String^, Flood::GUI::Font^>^, Flood::GUI::Texture^> StringTextureCache;
    
    static ::TextureAtlas* textureAtlas;
    static ::TrueTypeFont* font;

	static System::Drawing::StringFormat^ m_StringFormat;
	static System::Drawing::Graphics^ m_Graphics; // only used for text measurement
	static StringTextureCache^ m_StringCache = gcnew StringTextureCache();

    static Flood::GUI::Font^ tmpFont;

	static TextRenderer(){
		m_StringFormat = gcnew System::Drawing::StringFormat(System::Drawing::StringFormat::GenericTypographic);
		m_StringFormat->FormatFlags = m_StringFormat->FormatFlags | System::Drawing::StringFormatFlags::MeasureTrailingSpaces;
		m_Graphics = System::Drawing::Graphics::FromImage(gcnew System::Drawing::Bitmap(1024, 1024, System::Drawing::Imaging::PixelFormat::Format32bppArgb));
	
        textureAtlas = new ::TextureAtlas(512);
        font = new ::TrueTypeFont("");
    }

	static bool LoadFont(Flood::GUI::Font^ font)
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

        tmpFont = font;

		return true;
	}

	static void FreeFont(Flood::GUI::Font^ font)
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

	static  System::Drawing::Font^ ConvertFont(Flood::GUI::Font^ font)
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

	static Flood::GUI::Texture^ GetTexture(Flood::GUI::Font^ font, System::String^ text){
		System::Tuple<System::String^, Flood::GUI::Font^>^ key = gcnew System::Tuple<System::String^, Flood::GUI::Font^>(text, font);
		if (m_StringCache->ContainsKey(key))
			return m_StringCache[key];
		return nullptr;
	}

	static void AddTexture(Flood::GUI::Font^ font, System::String^ text, Flood::GUI::Texture^ texture){
		System::Tuple<System::String^, Flood::GUI::Font^>^ key = gcnew System::Tuple<System::String^, Flood::GUI::Font^>(text, font);
		m_StringCache->Add(key,texture);
	}

public:

	static void ClearCache(){
		for each(Flood::GUI::Texture^ tex in m_StringCache->Values){
			delete(tex);
		}
		m_StringCache->Clear();
	}

	//TODO use scale, remove renderer
	static Flood::GUI::Texture^ StringToTexture(System::String^ text, Flood::GUI::Font^ font, Flood::GUI::Renderers::Renderer^ renderer)
	{
		System::Drawing::Brush^ brush = System::Drawing::Brushes::White;
		Flood::GUI::Texture^ texture = GetTexture(font,text);
		if(texture != nullptr){ //TODO Check stringFormat
			return texture;
		}

		System::Drawing::Font^ sysFont = ConvertFont(font);

		System::Drawing::Point size = TextRenderer::MeasureText(sysFont, text, m_StringFormat);
		texture = gcnew Flood::GUI::Texture(renderer);
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
		TextureUtil::LoadTextureInternal(texture,bmp);
        AddTexture(font,text,texture);

		return texture;
	}

    static void DrawText(GwenRenderer^ renderer, System::Drawing::Point position, System::String^ text)
    {
        for(int i = 0; i < text->Length; i++)
        {
            char c = text[i];
            Glyph glyph;
            bool foundGlyph = TextRenderer::font->getGlyph(c,glyph);
            if(!foundGlyph)
                printf("glyph not Found\n");;

            if (glyph.image != HandleInvalid)
            {
                SubTexture subTexture;
                bool subTextureFound = TextRenderer::textureAtlas->getImageSubTexture(glyph.image,subTexture);
                if(!subTextureFound){
                    TextRenderer::textureAtlas->addImage(glyph.image);
                    subTextureFound = TextRenderer::textureAtlas->getImageSubTexture(glyph.image,subTexture);
                    if(!subTextureFound){
                        printf("subTexture not Found\n");
                        return;
                    }
                }

                ImageHandle atlasImageHandle = TextRenderer::textureAtlas->getAtlasImageHandle();
                Image* atlasImage = atlasImageHandle.Resolve();

                Flood::GUI::Texture^ texture = gcnew Flood::GUI::Texture(renderer);
                texture->Width = atlasImage->getWidth();
                texture->Height = atlasImage->getHeight();
                texture->RendererData = atlasImageHandle.getId();

                Image* glyphImage = glyph.image.Resolve();

                System::Drawing::Rectangle renderRect = System::Drawing::Rectangle(position.X, position.Y + glyph.baseLineOffset, glyphImage->getWidth(), glyphImage->getHeight());

                Vector2 topLeftUV, topRightUV, bottomLeftUV, bottomRightUV;

                Rect& rect = subTexture.rect;
            
                float width = atlasImage->getWidth();
                float height = atlasImage->getHeight();
                float u1 = rect.x/width;
                float v1 = rect.y/height;
                float u2 = (rect.x+rect.width)/width;
                float v2 = (rect.y+rect.height)/height;

                if(!subTexture.isRotated)
                {
                    topLeftUV = Vector2(u1,v1);
                    topRightUV = Vector2(u2,v1);
                    bottomLeftUV = Vector2(u1,v2);
                    bottomRightUV = Vector2(u2,v2);
                }
                else
                {
                    topLeftUV = Vector2(u1,v2);
                    topRightUV = Vector2(u1,v1);
                    bottomLeftUV = Vector2(u2,v2);
                    bottomRightUV = Vector2(u2,v1);
                }

                renderer->DrawTexturedRect(texture,renderRect,topLeftUV,topRightUV,bottomLeftUV,bottomRightUV);
                delete texture;
            }

            if (i < text->Length-1){
                Vector2i kern = TextRenderer::font->getKerning(text[i],text[i+1]);
                position.X += glyph.advance + kern.x;
                position.Y += kern.y;
            }
        }
    }

	static System::Drawing::Point MeasureText(System::String^ text, Flood::GUI::Font^ font)
	{
		System::Drawing::Font^ sysFont = ConvertFont(font);
		System::Drawing::SizeF size = m_Graphics->MeasureString(text, sysFont, System::Drawing::Point::Empty, m_StringFormat);
		return System::Drawing::Point((int)floor(size.Width+0.5), (int)floor(size.Height+0.5));
	}

};

	void GwenRenderer::StartClip()
	{
		m_ClipEnabled = true;
	}

	void GwenRenderer::EndClip()
	{
		m_ClipEnabled = false;
	}

	void GwenRenderer::DrawTexturedRect(Flood::GUI::Texture^ t, System::Drawing::Rectangle rect, float u1, float v1, float u2, float v2)
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

		buffer->addRectangle(rect, Vector2(u1,v1), Vector2(u2,v1),Vector2(u1,v2),Vector2(u2,v2), iHandle, m_Color);
	}

    void GwenRenderer::DrawTexturedRect(Flood::GUI::Texture^ t, System::Drawing::Rectangle rect, Vector2 topLeftUV, Vector2 topRightUV, Vector2 bottomLeftUV,Vector2 bottomRightUV)
	{
		if(t->RendererData == nullptr){
			DrawFilledRect(rect);
		}

		ImageHandle iHandle;
		HandleId hId = (HandleId)t->RendererData;
		iHandle.setId(hId);

		rect = Translate(rect);

		buffer->addRectangle(rect,topLeftUV,topRightUV,bottomLeftUV,bottomRightUV, iHandle, m_Color);
	}

	System::Drawing::Point GwenRenderer::MeasureText(Flood::GUI::Font^ font, System::String^ text)
	{
		return TextRenderer::MeasureText(text,font);
	}

	void GwenRenderer::RenderText(Flood::GUI::Font^ font, System::Drawing::Point position, System::String^ text)
	{
	   //Flood::GUI::Texture^ texture = TextRenderer::StringToTexture(text, font, this); // renders string on the texture
	   //DrawTexturedRect(texture, System::Drawing::Rectangle(position.X, position.Y, texture->Width, texture->Height),0,0,1,1);
	
       TextRenderer::DrawText(this,position,text);
    }
	
	void GwenRenderer::LoadTexture(Flood::GUI::Texture^ t)
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

	void GwenRenderer::LoadTextureBitmap(Flood::GUI::Texture^ t, System::Drawing::Bitmap^ bitmap)
	{
		TextureUtil::LoadTextureInternal(t,bitmap);
	}

	void GwenRenderer::FreeTexture(Flood::GUI::Texture^ t)
	{
		if (t->RendererData == nullptr)
			return;

		ImageHandle iHandle;
		HandleId hId = (HandleId)t->RendererData;
		iHandle.setId(hId);

		Image* img = iHandle.Resolve();

		GetResourceManager()->removeResource(img);
	}

	System::Drawing::Color GwenRenderer::PixelColor(Flood::GUI::Texture^ texture, System::UInt32 x, System::UInt32 y, System::Drawing::Color defaultColor)
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


class GwenInput
{

private:
	InputManager* inputManager;

	gcroot<Flood::GUI::Controls::Canvas^> m_Canvas;

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
		inputManager->getMouse()->onMouseDrag.Connect(this, &GwenInput::ProcessMouseDrag);
		inputManager->getMouse()->onMouseButtonPress.Connect(this, &GwenInput::ProcessMouseButtonPressed);
		inputManager->getMouse()->onMouseButtonRelease.Connect(this, &GwenInput::ProcessMouseButtonReleased);
		inputManager->getMouse()->onMouseWheelMove.Connect(this, &GwenInput::ProcessMouseWheel);

		inputManager->getKeyboard()->onKeyPress.Connect(this, &GwenInput::ProcessKeyDown);
		inputManager->getKeyboard()->onKeyRelease.Connect(this, &GwenInput::ProcessKeyUp);
	}

	void Initialize(gcroot<Flood::GUI::Controls::Canvas^> c)
	{
		m_Canvas = c;
	}


private:
	Flood::GUI::Key TranslateKeyCode(Keys key)
	{

		switch (key)
		{
		case Keys::Return: return Flood::GUI::Key::Return;
		case Keys::Escape: return Flood::GUI::Key::Escape;
		case Keys::Tab: return Flood::GUI::Key::Tab;
		case Keys::Space: return Flood::GUI::Key::Space;
		case Keys::Up: return Flood::GUI::Key::Up;
		case Keys::Down: return Flood::GUI::Key::Down;
		case Keys::Left: return Flood::GUI::Key::Left;
		case Keys::Right: return Flood::GUI::Key::Right;
		case Keys::Home: return Flood::GUI::Key::Home;
		case Keys::End: return Flood::GUI::Key::End;
		case Keys::Delete: return Flood::GUI::Key::Delete;
		case Keys::LControl:
			this->m_AltGr = true;
			return Flood::GUI::Key::Control;
		case Keys::LAlt: return Flood::GUI::Key::Alt;
		case Keys::LShift: return Flood::GUI::Key::Shift;
		case Keys::RControl: return Flood::GUI::Key::Control;
		case Keys::RAlt: 
			if (this->m_AltGr)
			{
				this->m_Canvas->Input_Key(Flood::GUI::Key::Control, false);
			}
			return Flood::GUI::Key::Alt;
		case Keys::RShift: return Flood::GUI::Key::Shift;
				
		}
		return Flood::GUI::Key::Invalid;
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

	void ProcessMouseDrag(const MouseDragEvent& mouseDragEvent){
		
		int dx = mouseDragEvent.x - m_MouseX;
		int dy = mouseDragEvent.y - m_MouseY;

		m_MouseX = mouseDragEvent.x;
		m_MouseY = mouseDragEvent.y;

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
		wchar_t ch = TranslateChar(keyEvent.keyCode);

		if (Flood::GUI::Input::InputHandler::DoSpecialKeys(m_Canvas, ch))
			return;
		
		if (ch != ' ')
		{
			m_Canvas->Input_Character(ch);
		}
		
		Flood::GUI::Key iKey = TranslateKeyCode(keyEvent.keyCode);

		m_Canvas->Input_Key(iKey, true);
	}

	void ProcessKeyUp(const KeyEvent& keyEvent)
	{
		char ch = TranslateChar(keyEvent.keyCode);

		Flood::GUI::Key iKey = TranslateKeyCode(keyEvent.keyCode);

		m_Canvas->Input_Key(iKey, false);
	}

	/* void KeyPress(object sender, KeyPressEventArgs e)
	{
		m_Canvas->Input_Character(e.KeyChar);
	}*/

};

public ref class NativeGUI {
public:

	Flood::Editor::Editor^ managedEditor;
	GwenRenderer^ renderer;
	GwenInput* input;

	NativeGUI(InputManager* inputManager){
		Initialize(inputManager);
	}

	delegate void Foo(int);

	void Initialize(InputManager* inputManager) {
		renderer = gcnew GwenRenderer();
		managedEditor = gcnew Flood::Editor::Editor(renderer,"DefaultSkin.png");

		input = new GwenInput(inputManager);
		input->Initialize(managedEditor->MainWindow->Canvas);
	}

	void Close() {
		TextRenderer::ClearCache();
		delete(managedEditor);
	}

	void Render(RenderBlock& rb){
		renderer->Clear();
		managedEditor->MainWindow->Render();
		renderer->Render(rb);
	}

	void SetSize(int x, int y){
		managedEditor->MainWindow->Canvas->SetSize(x, y);
	}
};

static gcroot<::NativeGUI^> gs_GUIInstance = nullptr;

void InitializeGUI(InputManager* inputManager){
	gs_GUIInstance = gcnew NativeGUI(inputManager);
}

void UpdateGUI() {
	gs_GUIInstance->managedEditor->Update();
}

void ResizeGUI(int x, int y) {
	gs_GUIInstance->SetSize(x, y);
}

void CloseGUI() {
	gs_GUIInstance->Close();
	delete(gs_GUIInstance);
	gs_GUIInstance = nullptr; 
}

void RenderGUI(RenderBlock& rb) {
	gs_GUIInstance->Render(rb);
}

void SetMainWindow(Window* window) {
	auto NativeWindow = gcnew Flood::Window(System::IntPtr(window));
	gs_GUIInstance->managedEditor->MainWindow->NativeWindow = NativeWindow;
}

