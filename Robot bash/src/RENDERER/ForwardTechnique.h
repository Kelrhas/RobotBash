#pragma once

#include "Technique.h"

namespace Renderer
{
	class ForwardTechnique : public Technique
	{
	public:
		virtual void		GatherLocations() override;

				void		SetMVP(const glm::mat4& mMVP);
				void		SetDiffuseMap(int iTexIndex);
				void		SetNormalMap(int iTexIndex);

	protected:
		// ins
		GLint	m_iPositionLocation;			// vec3
		GLint	m_iNormalLocation;				// vec3
		GLint	m_iUVLocation;					// vec2

		// uniforms
		GLint	m_iMVPLocation;					// mat4
		GLint	m_iDiffuseMapLocation;			// tex2d
		GLint	m_iNormalMapLocation;			// tex2d
	};
}