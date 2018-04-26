#pragma once

#include "DeferredLightingTechnique.h"

namespace Renderer
{
	class DeferredDirectionalLightTechnique : public DeferredLightingTechnique
	{
	public:
		virtual	void		GatherLocations() override;

				void		SetLightDirection(const glm::vec3& vDirection);
				void		SetLightColor(const glm::vec3& vColor);
				void		SetAmbientIntensity( float fIntensity );
				void		SetDiffuseIntensity( float fIntensity );

	protected:
		GLint		m_iDirectionLocation;			// vec3
		GLint		m_iColorLocation;				// vec3
		GLint		m_iAmbientIntensityLocation;	// float
		GLint		m_iDiffuseIntensityLocation;	// float
	};
}
