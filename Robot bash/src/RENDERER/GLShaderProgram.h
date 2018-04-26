#pragma once

#include "ogl.h"

namespace Renderer
{
	class Technique;

	class GLShader
	{
		friend class Technique;
	public:
		GLShader(GLenum uShaderType);
		~GLShader();
		bool	Load(std::string sFileName);
		bool	Compile();

		uint64_t	GetTimestamp();

		GLuint	GetID() const { return m_uID; }
		
	private:
		bool	LiveUpdate();
		bool	m_bNeedReloading;

		std::string	m_sFileName;
		uint64_t	m_uLastTimestamp;
		GLenum		m_uType;
		GLuint		m_uID;
		char*		m_pCode;
	};
}
