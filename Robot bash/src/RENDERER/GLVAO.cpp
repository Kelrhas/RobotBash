#include "StdAfx.h"
#include "GLVAO.h"

namespace Renderer
{
	/************************************************************************/
	/* GLVBO                                                               */
	/************************************************************************/
	/*GLVAO::GLVBO::GLVBO()
	{
		glGenBuffers(1, &m_uID);
		glBindBuffer(GL_ARRAY_BUFFER, m_uID);
	}

	GLVAO::GLVBO::~GLVBO()
	{

	}

	bool GLVAO::GLVBO::SetData(void* pData, int iNbItem, size_t iItemSize)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_uID);
		glBufferData(GL_ARRAY_BUFFER, iNbItem * iItemSize, pData, GL_STATIC_DRAW);

		return true;
	}*/

	/************************************************************************/
	/* GLVAO                                                               */
	/************************************************************************/
	GLVAO::GLVAO()
		: m_uID((GLuint)-1)
		, m_iBufferFlags(0)
	{
		for(int i=0; i < VBO_BUFFER_COUNT; ++i)
			m_pVBOArray[i] = -1;

		m_iBufferFlags = VBO_BUFFER_INDEX | VBO_BUFFER_POSITION;

		// VAO generation
		glGenVertexArrays(1, &m_uID);
		glBindVertexArray(m_uID);
		Assert_GL();

		// VBO for indices
		glGenBuffers(1, &m_pVBOArray[VBO_BUFFER_INDEX]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pVBOArray[VBO_BUFFER_INDEX] );
		Assert_GL();

		glGenBuffers( 1, &m_pVBOArray[VBO_BUFFER_POSITION] );
		glBindBuffer( GL_ARRAY_BUFFER, m_pVBOArray[VBO_BUFFER_POSITION] );
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		Assert_GL();

		// unbind to be sure
		glBindVertexArray( 0 );
	}

	GLVAO::~GLVAO()
	{
		for( int i = 0; i < VBO_BUFFER_COUNT; ++i )
		{
			if( m_pVBOArray[i] != -1 )
			{
				glDeleteBuffers( 1, &m_pVBOArray[i] );
			}
		}
		glDeleteVertexArrays( 1, &m_uID );
		Assert_GL();

		//delete m_pPositionVBO;
		//delete m_pNormalVBO;
		//delete m_pTexCoordVBO;
		//delete m_pColorVBO;
	}

	bool GLVAO::SetData( VAOBufffer eVBOBuffer, void* pData, int iNbItem)
	{
		int iVBOFlag = 1 << eVBOBuffer;
		glBindVertexArray( m_uID );
		switch (eVBOBuffer)
		{
		case Renderer::VBO_BUFFER_INDEX:
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_pVBOArray[VBO_BUFFER_INDEX] );
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, iNbItem * sizeof(uint32_t), pData, GL_STATIC_DRAW);
			Assert_GL();
			break;

		case Renderer::VBO_BUFFER_POSITION:
			glBindBuffer( GL_ARRAY_BUFFER, m_pVBOArray[VBO_BUFFER_POSITION] );
			glBufferData( GL_ARRAY_BUFFER, iNbItem * sizeof( float ) * 3, pData, GL_STATIC_DRAW );
			Assert_GL();
			break;
		case Renderer::VBO_BUFFER_NORMAL:
		{
			if ((m_iBufferFlags & iVBOFlag) == 0)
			{
				m_iBufferFlags |= iVBOFlag;
				glGenBuffers( 1, &m_pVBOArray[VBO_BUFFER_NORMAL] );
				glEnableVertexAttribArray(1);
				Assert_GL();
			}

			glBindBuffer( GL_ARRAY_BUFFER, m_pVBOArray[VBO_BUFFER_NORMAL] );
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_TRUE, 0, nullptr );
			Assert_GL();

			glBufferData( GL_ARRAY_BUFFER, iNbItem * sizeof( float ) * 3, pData, GL_STATIC_DRAW );
			Assert_GL();
		}
		break;
		case Renderer::VBO_BUFFER_UV:
		{
			if ((m_iBufferFlags & iVBOFlag) == 0)
			{
				m_iBufferFlags |= iVBOFlag;
				glGenBuffers( 1, &m_pVBOArray[VBO_BUFFER_UV] );
				glEnableVertexAttribArray(2);
				Assert_GL();
			}
			glBindBuffer( GL_ARRAY_BUFFER, m_pVBOArray[VBO_BUFFER_UV] );
			glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, nullptr );
			Assert_GL();

			glBufferData( GL_ARRAY_BUFFER, iNbItem * sizeof( float ) * 2, pData, GL_STATIC_DRAW );
			Assert_GL();
		}
		break;

		case Renderer::VBO_BUFFER_COLOR:
		{
			if ((m_iBufferFlags & iVBOFlag) == 0)
			{
				m_iBufferFlags |= iVBOFlag;
				glGenBuffers( 1, &m_pVBOArray[VBO_BUFFER_COLOR] );
				glEnableVertexAttribArray(3);
				Assert_GL();
			}
			glBindBuffer( GL_ARRAY_BUFFER, m_pVBOArray[VBO_BUFFER_COLOR] );
			glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
			Assert_GL();

			glBufferData( GL_ARRAY_BUFFER, iNbItem * sizeof( float ) * 3, pData, GL_STATIC_DRAW );
			Assert_GL();
		}
		break;

		default:
			TODO;
			break;
		}
		// unbind to be sure
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray( 0 );

		return true;
	}

	void GLVAO::Bind()
	{
		glBindVertexArray(m_uID);
	}

}