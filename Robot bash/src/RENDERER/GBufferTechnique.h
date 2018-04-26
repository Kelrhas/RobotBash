#pragma once

#include "Technique.h"

namespace Renderer
{
	class GBufferTechnique : public Technique
	{
	public:
		virtual void		GatherLocations() override;

				void		SetTarget( int iTargetMode );
				void		SetDiffuseMap(int iTexIndex);
				void		SetNormalMap(int iTexIndex);
				void		SetDepthMap(int iTexIndex);
				void		SetFinalLitMap(int iTexIndex);
				void		SetDepthPlanes(float fNear, float fFar);

	protected:
		// in
		GLint	m_iPositionLocation;			// vec3
		GLint	m_iUVLocation;					// vec2

		// uniforms
		GLint	m_iTargetLocation;				// bool
		GLint	m_iDiffuseMapLocation;			// tex2d
		GLint	m_iNormalMapLocation;			// tex2d
		GLint	m_iDepthMapLocation;			// tex2d
		GLint	m_iFinalMapLocation;			// tex2d
		GLint	m_iNearPlaneLocation;			// float
		GLint	m_iFarPlaneLocation;			// float
	};
}