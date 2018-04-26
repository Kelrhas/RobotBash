#include "StdAfx.h"
#include "Texture.h"

#include "IO/BMPLoader.h"

namespace Renderer
{
	Texture::Texture()
		: m_uId(-1)
	{

	}

	Texture::~Texture()
	{

	}

	bool Texture::Load(std::string sFilename)
	{
		BMPLoader texLoader;
		texLoader.Load(sFilename.c_str());

		glGenTextures(1, &m_uId);
		glBindTexture(GL_TEXTURE_2D, m_uId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texLoader.GetWidth(), texLoader.GetHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, texLoader.GetData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		return true;
	}
}