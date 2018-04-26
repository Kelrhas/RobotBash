#include "stdafx.h"
#include "PointLight.h"

#include "ENTITIES/Entity.h"

namespace Renderer
{
	PointLight::PointLight()
	{
		m_eType = lightType_Point;
	}

	PointLight::~PointLight()
	{

	}

	bool PointLight::Init( Entity* pOwner )
	{
		Assert( Light::Init( pOwner ) );
		m_pData = new PointLightData();

		return true;
	}

	bool PointLight::Update( float fDeltaTime )
	{
		static_cast<PointLightData*>(m_pData)->m_vPosition = m_pOwner->GetNode()->GetWorldPosition();
		return true;
	}

	void PointLight::ImGuiDraw()
	{
		Light::ImGuiDraw();
		PointLightData* pData = static_cast<PointLightData*>(m_pData);
		ImGui::DragFloat( "Cst attenuation", &pData->m_fConstantAttenuation, 0.01f, 0.f, 50.f );
		ImGui::DragFloat( "Lin attenuation", &pData->m_fLinearAttenuation, 0.01f, 0.f, 50.f );
		ImGui::DragFloat( "Exp attenuation", &pData->m_fExpAttenuation, 0.01f, 0.f, 50.f );
	}
}