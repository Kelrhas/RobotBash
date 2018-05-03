#ifndef GL_VAO_H
#define GL_VAO_H

#include "ogl.h"

namespace Renderer
{
	enum VAOBufffer
	{
		VBO_BUFFER_INDEX,
		VBO_BUFFER_POSITION,
		VBO_BUFFER_NORMAL,
		VBO_BUFFER_UV,
		VBO_BUFFER_COLOR,

		VBO_BUFFER_COUNT
	};

	class GLVAO
	{
	public:
		GLVAO();
		~GLVAO();

		bool SetData( VAOBufffer eFlag, void* pData, int iNbItem);
		void Bind();

		GLuint GetID() const { return m_uID; }

	private:
		GLuint		m_uID;
		int			m_iBufferFlags;
		GLuint		m_pVBOArray[VBO_BUFFER_COUNT];
	};
}

#endif //!GL_VAO_H
