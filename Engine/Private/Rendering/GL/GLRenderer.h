#pragma once

#include <IRenderer.h>

class GLRenderer : public IRenderer
{
public:
	ITexture* CreateTexture() override;
	IShader* CreateShader() override;
private:
};