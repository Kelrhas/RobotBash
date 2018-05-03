#include "StdAfx.h"
#include "Light.h"

#include "RENDERER/GLRenderer.h"

namespace Renderer
{
	Light::Light()
		: m_pData(nullptr)
		, m_eType( lightType_None )
		, m_uVAO( -1 )
	{
		m_uVBOId[0] = -1;
		m_uVBOId[1] = -1;
	}

	Light::~Light()
	{

	}

	bool Light::Init( Entity* pOwner )
	{
		EntityComponent::Init( pOwner );

		glGenBuffers( 2, m_uVBOId );
		glGenVertexArrays( 1, &m_uVAO );

		return true;
	}

	bool Light::Update(float fDeltaTime)
	{
		return true;
	}

	void Light::ImGuiDraw()
	{
		EntityComponent::ImGuiDraw();
		ImGui::ColorEdit3( "Color", &m_pData->m_vColor.r );
		ImGui::DragFloat( "Intensity", &m_pData->m_fDiffuseIntensity, 0.01f, 0.f, 10.f );
	}

	bool Light::Render() const
	{
		glBindVertexArray( m_uVAO );

		glDrawArrays( GL_TRIANGLES, 0, 6 );

		glBindVertexArray( 0 );

		return true;
	}
}
