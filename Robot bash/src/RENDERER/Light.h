#pragma once

#include "ENTITIES/EntityComponent.h"

using namespace Entities;

namespace Renderer
{
	enum LightType
	{
		lightType_None = -1,
		lightType_Directional,
		lightType_Point,
		lightType_Spot
	};

	class Light : public EntityComponent
	{
	public:
		struct LightData
		{
			LightData() : m_fDiffuseIntensity( 0.f ), m_vColor( COLOR_WHITE ) {}

			float						m_fDiffuseIntensity;
			glm::vec3					m_vColor;
		};

		Light();
		~Light();

		//virtual bool					Init(Entity* pOwner);
		virtual bool					Update(float fDeltaTime) override;
		virtual void					ImGuiDraw();
		virtual	LightData*				GetGLSLData() const { return m_pData; }

		inline void						SetIntensity( float fIntensity ) { m_pData->m_fDiffuseIntensity = fIntensity; }
		inline float					GetIntensity() const { return m_pData->m_fDiffuseIntensity; }
		inline void						SetColor( glm::vec3 vColor ) { m_pData->m_vColor = vColor; }
		inline glm::vec3				GetColor() const { return m_pData->m_vColor; }
		inline LightType				GetType() const { return m_eType; }
	
	protected:
		LightType						m_eType;
		LightData*						m_pData;
	};

}