#pragma once

#include "Light.h"

namespace Renderer
{
	class DirectionalLight : public Light
	{
	public:
		struct DirectionalLightData : public LightData
		{
			glm::vec3			m_vDirection;
		};

		DirectionalLight();
		~DirectionalLight();

		virtual bool					Init( Entity* pOwner ) override;
		virtual void					ImGuiDraw() override;

				glm::mat4				GetProjMatrix() const;
				glm::mat4				GetViewMatrix() const;

		inline void						SetDirection( glm::vec3 vDir) { ((DirectionalLightData*)m_pData)->m_vDirection = vDir; }
		inline glm::vec3				GetDirection() const { return ((DirectionalLightData*)m_pData)->m_vDirection;}

	private:
		float	m_fLeft;
		float	m_fRight;
		float	m_fTop;
		float	m_fBottom;
		float	m_fFar;
		float	m_fNear;
	};
}