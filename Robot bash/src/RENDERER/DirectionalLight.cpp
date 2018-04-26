#include "StdAfx.h"
#include "DirectionalLight.h"

namespace Renderer
{
	DirectionalLight::DirectionalLight()
	{
		m_eType = lightType_Directional;
	}

	DirectionalLight::~DirectionalLight()
	{

	}

	bool DirectionalLight::Init( Entity* pOwner )
	{
		Light::Init( pOwner );
		m_pData = new DirectionalLightData();
		return true;
	}

	void DirectionalLight::ImGuiDraw()
	{
		Light::ImGuiDraw();
		DirectionalLightData* pData = static_cast<DirectionalLightData*>(m_pData);
		if( ImGui::DragFloat3( "Direction", &pData->m_vDirection.x, 0.1f, -1.f, 1.f ) )
			pData->m_vDirection = glm::normalize( pData->m_vDirection );
	}

	glm::mat4 DirectionalLight::GetProjMatrix() const
	{
		return glm::ortho( -10.f, 11.f, -10.f, 11.f, 0.f, 30.f );
	}

	glm::mat4 DirectionalLight::GetViewMatrix() const
	{
		glm::vec3 pos( 10.f, 10.f, 10.f );
		return glm::lookAt( pos, pos + GetDirection(), VEC3_UP );
	}
}