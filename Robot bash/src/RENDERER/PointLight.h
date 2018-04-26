#pragma once

#include "Light.h"

namespace Renderer
{
	class PointLight : public Light
	{
	public:
		struct PointLightData : public LightData
		{
			PointLightData() : LightData(), m_vPosition( VECTOR_0 ), m_fConstantAttenuation( 0.f ), m_fLinearAttenuation( 0.f ), m_fExpAttenuation( 0.f ) {}

			glm::vec3		m_vPosition;
			float			m_fConstantAttenuation;
			float			m_fLinearAttenuation;
			float			m_fExpAttenuation;
		};

		PointLight();
		~PointLight();

		virtual bool					Init( Entity* pOwner );
		virtual bool					Update( float fDeltaTime ) override;
		virtual void					ImGuiDraw() override;

		inline void						SetConstantAttenuation( float fAttenuation ) { ((PointLightData*)m_pData)->m_fConstantAttenuation = fAttenuation; }
		inline float					GetConstantAttenuation() const { return ((PointLightData*) m_pData)->m_fConstantAttenuation; }
		inline void						SetLinearAttenuation( float fAttenuation ) { ((PointLightData*) m_pData)->m_fLinearAttenuation = fAttenuation; }
		inline float					GetLinearAttenuation() const { return ((PointLightData*) m_pData)->m_fLinearAttenuation; }
		inline void						SetExpAttenuation( float fAttenuation ) { ((PointLightData*) m_pData)->m_fExpAttenuation = fAttenuation; }
		inline float					GetExpAttenuation() const { return ((PointLightData*) m_pData)->m_fExpAttenuation; }

	protected:
	};
}