#pragma once

#include "ogl.h"

namespace Renderer
{
	class GLRenderer;

	class Texture
	{
		friend class GLRenderer;
	public:
		Texture();
		~Texture();


		GLuint		GetID() const { return m_uId; }

	protected:
		bool		Load(std::string sFilename);

		GLuint		m_uId;
	};

	typedef std::shared_ptr<Texture> TexturePtr;
	typedef std::weak_ptr<Texture> TextureWeakPtr;
}