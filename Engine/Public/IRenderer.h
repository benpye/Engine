#pragma once

#include <ITexture.h>
#include <IShader.h>

class IRenderer
{
public:
	virtual ~IRenderer() {}

	virtual ITexture* CreateTexture() = 0;
	virtual IShader* CreateShader() = 0;
};