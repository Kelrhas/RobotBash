#include "StdAfx.h"
#include "DirectionalLight.h"

#include "RENDERER/GLRenderer.h"

namespace Renderer
{
	GLfloat verts[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
	};
	GLfloat uvs[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	DirectionalLight::DirectionalLight()
		: m_fLeft( -14.f )
		, m_fRight( 11.f )
		, m_fBottom( -1.f )
		, m_fTop( 11.f )
		, m_fNear( 0.f )
		, m_fFar( 30.f )
	{
		m_eType = lightType_Directional;
	}

	DirectionalLight::~DirectionalLight()
	{

	}

	bool DirectionalLight::Init( Entity* pOwner )
	{
		Assert( Light::Init( pOwner ) );
		m_pData = new DirectionalLightData();

		glBindVertexArray( m_uVAO );

		glEnableVertexAttribArray( 0 );  // Vertex position
		glBindBuffer( GL_ARRAY_BUFFER, m_uVBOId[0] );
		glVertexAttribPointer( (GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
		glBufferData( GL_ARRAY_BUFFER, 6 * 3 * sizeof( float ), verts, GL_STATIC_DRAW );
		Assert_GL();

		glEnableVertexAttribArray( 1 ); // Texture coordinates
		glBindBuffer( GL_ARRAY_BUFFER, m_uVBOId[1] );
		glVertexAttribPointer( (GLuint) 1, 2, GL_FLOAT, GL_FALSE, 0, nullptr );
		glBufferData( GL_ARRAY_BUFFER, 6 * 2 * sizeof( float ), uvs, GL_STATIC_DRAW );
		Assert_GL();

		glBindVertexArray( 0 );

		return true;
	}

	void DirectionalLight::ImGuiDraw()
	{
		Light::ImGuiDraw();
		DirectionalLightData* pData = static_cast<DirectionalLightData*>(m_pData);
		if( ImGui::DragFloat3( "Direction", &pData->m_vDirection.x, 0.1f, -1.f, 1.f ) )
			pData->m_vDirection = glm::normalize( pData->m_vDirection );

		ImGui::DragFloat( "Left", &m_fLeft, 0.1f );
		ImGui::DragFloat( "Right", &m_fRight, 0.1f );
		ImGui::DragFloat( "Top", &m_fTop, 0.1f );
		ImGui::DragFloat( "Bottom", &m_fBottom, 0.1f );
	}

	glm::mat4 DirectionalLight::GetProjMatrix() const
	{
		return glm::ortho( m_fLeft, m_fRight, m_fBottom, m_fTop, m_fNear, m_fFar );;
	}

	glm::mat4 DirectionalLight::GetViewMatrix() const
	{
		glm::vec3 pos( 25.f, -10.f, 25.f );
		return glm::lookAt( pos, pos + GetDirection(), VEC3_UP );
	}
}