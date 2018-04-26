#ifndef GL_VAO_H
#define GL_VAO_H

#include "ogl.h"

namespace Renderer
{
	enum VAOBuffferFlag
	{
		VAO_BUFFER_INDEX = 0x1,
		VAO_BUFFER_POSITION = 0x2,
		VAO_BUFFER_NORMAL = 0x4,
		VAO_BUFFER_UV = 0x8,
		VAO_BUFFER_COLOR = 0x10,
	};

	class GLVAO
	{
		class GLVBO
		{
		public:
			GLVBO();
			~GLVBO();

			bool SetData(void* pData, int iNbItem, size_t iItemSize);

			GLuint GetID() const { return m_uID; }

		private:
			GLuint m_uID;
		};

	public:


		GLVAO(int eBufferFlags = VAO_BUFFER_POSITION);
		~GLVAO();

		bool SetData(VAOBuffferFlag eFlag, void* pData, int iNbItem);
		void Bind();

		GLuint GetID() const { return m_uID; }
	private:
		GLuint		m_uID;
		GLuint		m_uIndexVBOID;
		int			m_iBufferFlags;
		GLVBO*		m_pPositionVBO;
		GLVBO*		m_pNormalVBO;
		GLVBO*		m_pTexCoordVBO;
		GLVBO*		m_pColorVBO;
	};
}

#endif //!GL_VAO_H
