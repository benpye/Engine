#include "GLRenderer.h"
#include "GLTexture.h"

IShader* GLRenderer::CreateShader()
{
	return nullptr;
}

ITexture* GLRenderer::CreateTexture()
{
	return new GLTexture();
}