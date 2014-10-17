#pragma once

#include <ITexture.h>
#include <GLBase.h>

struct GLTextureLookup
{
	GLenum internalFormat;
	GLenum format;
	GLenum type;
};

class GLTexture : public ITexture
{
public:
	GLTexture();
	~GLTexture();

	// Disallow copy
	GLTexture(const GLTexture& other) = delete;
	GLTexture& operator=(const GLTexture& other) = delete;

	virtual void Init(int width, int height, TextureFormat format, const void* data) override;
	virtual void Update(int xoffset, int yoffset, int width, int height, const void* data) override;
	virtual void GetImage(void* dst) override;

	// Keep track of currently bound texture
	static GLuint glBoundTexture;
	inline void Bind()
	{
		if (glBoundTexture != id)
		{
			glBindTexture(GL_TEXTURE_2D, id);
			glBoundTexture = id;
		}
	}

	virtual TextureFormat GetFormat() override { return format; }
	virtual int GetWidth() override { return width; }
	virtual int GetHeight() override { return height; }
private:
	const static GLTextureLookup glTypeLUT[];
	
	TextureFormat format;
	int width, height;
	GLuint id;
	bool mipmapEnabled;
};