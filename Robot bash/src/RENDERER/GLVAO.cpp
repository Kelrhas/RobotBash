#include "StdAfx.h"
#include "GLVAO.h"

namespace Renderer
{
	/************************************************************************/
	/* GLVBO                                                               */
	/************************************************************************/
	GLVAO::GLVBO::GLVBO()
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
	}

	/************************************************************************/
	/* GLVAO                                                               */
	/************************************************************************/
	GLVAO::GLVAO(int eBufferFlags /*= VAO_BUFFER_POSITION*/)
		: m_uID((GLuint)-1)
		, m_iBufferFlags(0)
		, m_pPositionVBO(nullptr)
		, m_pNormalVBO(nullptr)
		, m_pTexCoordVBO(nullptr)
		, m_pColorVBO(nullptr)
	{
		Assert( eBufferFlags != 0 );

		m_iBufferFlags = eBufferFlags | VAO_BUFFER_INDEX | VAO_BUFFER_POSITION;

		// VAO generation
		glGenVertexArrays(1, &m_uID);
		glBindVertexArray(m_uID);

		// VBO for indices
		glGenBuffers(1, &m_uIndexVBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uIndexVBOID);


		m_pPositionVBO = new GLVBO();
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_pPositionVBO->GetID());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	}

	GLVAO::~GLVAO()
	{
		//TODO  free GL memory
		delete m_pPositionVBO;
		delete m_pNormalVBO;
		delete m_pTexCoordVBO;
		delete m_pColorVBO;
	}

	bool GLVAO::SetData(VAOBuffferFlag eFlag, void* pData, int iNbItem)
	{
		switch (eFlag)
		{
		case Renderer::VAO_BUFFER_INDEX:
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, iNbItem * sizeof(uint32_t), pData, GL_STATIC_DRAW);
			break;

		case Renderer::VAO_BUFFER_POSITION:
			m_pPositionVBO->SetData(pData, iNbItem, sizeof(float) * 3);
			break;
		case Renderer::VAO_BUFFER_NORMAL:
		{
			if ((m_iBufferFlags & VAO_BUFFER_NORMAL) == 0)
			{
				m_iBufferFlags |= VAO_BUFFER_NORMAL;
				m_pNormalVBO = new GLVBO();
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, m_pNormalVBO->GetID());
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
			}

			m_pNormalVBO->SetData(pData, iNbItem, sizeof(float) * 3);
		}
		break;
		case Renderer::VAO_BUFFER_UV:
		{
			if ((m_iBufferFlags & VAO_BUFFER_UV) == 0)
			{
				m_iBufferFlags |= VAO_BUFFER_UV;
				m_pTexCoordVBO = new GLVBO();
				glEnableVertexAttribArray(2);
				glBindBuffer(GL_ARRAY_BUFFER, m_pTexCoordVBO->GetID());
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			}
			m_pTexCoordVBO->SetData(pData, iNbItem, sizeof(float) * 2);
		}
		break;

		case Renderer::VAO_BUFFER_COLOR:
		{
			if ((m_iBufferFlags & VAO_BUFFER_COLOR) == 0)
			{
				m_iBufferFlags |= VAO_BUFFER_COLOR;
				m_pColorVBO = new GLVBO();
				glEnableVertexAttribArray(3);
				glBindBuffer(GL_ARRAY_BUFFER, m_pColorVBO->GetID());
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			}
			m_pColorVBO->SetData(pData, iNbItem, sizeof(float) * 3);
		}
		break;

		default:
			break;
		}
		return true;
	}

	void GLVAO::Bind()
	{
		glBindVertexArray(m_uID);
	}

}