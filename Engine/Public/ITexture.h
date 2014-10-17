#pragma once

enum class TextureFormat
{
	// 8 bits per channel formats
	R8,
	RG8,
	RGBA8,
	// 16 bits per channel formats
	R16,
	RG16,
	RGBA16,
	// 16 bits per channel float formats
	R16F,
	RG16F,
	RGBA16F,
	// 32 bits per channel float formats
	R32F,
	RG32F,
	RGBA32F,
	// SRGB 8 bit + 8 bit alpha
	SRGB8A8,
};

class ITexture
{
public:
	virtual ~ITexture() {}
	virtual void Init(int width, int height, TextureFormat format, const void* data = nullptr) = 0;
	virtual void Update(int xoffset, int yoffset, int width, int height, const void* data) = 0;
	virtual void GetImage(void* dst) = 0;

	virtual TextureFormat GetFormat() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
};