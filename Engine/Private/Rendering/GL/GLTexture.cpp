#include "GLTexture.h"

#define GLLUT(format) glTypeLUT[static_cast<int>(format)]

const GLTextureLookup GLTexture::glTypeLUT[] =
{
	{ GL_R8, GL_RED, GL_UNSIGNED_BYTE },
	{ GL_RG8, GL_RG, GL_UNSIGNED_BYTE },
	{ GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE },
	{ GL_R16, GL_RED, GL_UNSIGNED_SHORT },
	{ GL_RG16, GL_RG, GL_UNSIGNED_SHORT },
	{ GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT },
	{ GL_R16F, GL_RED, GL_FLOAT },
	{ GL_RG16F, GL_RG, GL_FLOAT },
	{ GL_RGBA16F, GL_RGBA, GL_FLOAT },
	{ GL_R16F, GL_RED, GL_FLOAT },
	{ GL_RG16F, GL_RG, GL_FLOAT },
	{ GL_RGBA16F, GL_RGBA, GL_FLOAT },
	{ GL_R32F, GL_RED, GL_FLOAT },
	{ GL_RG32F, GL_RG, GL_FLOAT },
	{ GL_RGBA32F, GL_RGBA, GL_FLOAT },
	{ GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE }
};

GLuint GLTexture::glBoundTexture = 0;

GLTexture::GLTexture()
{
	glGenTextures(1, &id);
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &id);
}

void GLTexture::Init(int width_, int height_, TextureFormat format_, const void* data)
{
	if (width_ < 0 || height_ < 0)
		return;

	format = format_;
	width = width_;
	height = height_;

	Bind();

	// If data == nullptr then this call will just allocate the texture but not upload any data
	glTexImage2D(GL_TEXTURE_2D, 0, GLLUT(format).internalFormat, width, height, 0, GLLUT(format).format, GLLUT(format).type, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
}

void GLTexture::Update(int xoffset, int yoffset, int width_, int height_, const void* data)
{
	Bind();

	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width_, height_, GLLUT(format).format, GLLUT(format).type, data);
}

void GLTexture::GetImage(void* dst)
{
	Bind();

	glGetTexImage(GL_TEXTURE_2D, 0, GLLUT(format).format, GLLUT(format).type, dst);
}