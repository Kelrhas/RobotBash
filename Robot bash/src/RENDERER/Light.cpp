#include "StdAfx.h"
#include "Light.h"

namespace Renderer
{
	Light::Light()
		: m_pData(nullptr)
		, m_eType( lightType_None )
	{

	}

	Light::~Light()
	{

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
}
