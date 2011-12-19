#if 0

// Fixed render state management.
void setupRenderFixed( const RenderState&, const LightQueue& );
bool setupRenderFixedMatrix( const RenderState& state );
bool setupRenderFixedOverlay( const RenderState& state );
//bool setupRenderFixedShadow( LightQueue& lights );
//bool setupRenderFixedLight( const RenderState&, const LightQueue& );
//bool setupRenderFixedOverlay( const RenderState& );

//-----------------------------------//

bool RenderDevice::setupRenderFixedMatrix( const RenderState& state )
{
	CameraPtr camera = activeView->getCamera();
	if( !camera ) return false;

	const Frustum& frustum = camera->getFrustum();

	const Matrix4x4& view = Matrix4x4(camera->getViewMatrix());
	const Matrix4x4& projection = frustum.matProjection;

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projection.m11);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&view.m11);

	return true;
}

//-----------------------------------//

bool RenderDevice::setupRenderFixedOverlay( const RenderState& state )
{
	Vector2i size = activeTarget->getSettings().getSize();
	
	Matrix4x4 projection = Matrix4x4::createOrthographic(0, size.x, size.y, 0, 0, 100);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projection.m11);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.375f, 0.375f, 0.0f);

	return true;
}

//-----------------------------------//

void RenderDevice::setupRenderFixed(const RenderState& state, const LightQueue& lights)
{
 RenderBatch* renderable = state.renderable;
	bindBuffers.renderable);

	GeometryBuffer* gb =.renderable->getGeometryBuffer().get();
	BufferManager* buffers = activeContext->bufferManager;
	
	VertexBuffer* vb = buffers->getVertexBuffer(gb).get();

	vb->bindPointers();
	
	Material* material = state.material;
	setupRenderStateMaterial(state, false);

	RenderLayer::Enum stage =.renderable->getRenderLayer();

	if( stage != RenderLayer::Overlays )
	{
		if( !setupRenderFixedMatrix(state) )
			return;
	}
	else if( stage == RenderLayer::Overlays )
	{
		if( !setupRenderFixedOverlay(state) )
			return;
	}

	glDisable(GL_LIGHTING);

	glPushMatrix();

	const Matrix4x4& model = state.modelMatrix;
	glMultMatrixf(&model.m11);

	if( .renderable->onPreRender.empty() )
	{
		// Call the user pre render hook.
	.renderable->onPreRender(activeView, state);
	}

	render.renderable);
	
	if( .renderable->onPostRender.empty() )
	{
		// Call the user post render hook.
	.renderable->onPostRender(activeView, state);
	}

	glPopMatrix();

	undoRenderStateMaterial(material);

	vb->unbindPointers();
	unbindBuffers.renderable);
}

//-----------------------------------//

#endif