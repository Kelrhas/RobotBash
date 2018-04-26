#pragma once

#include "Technique.h"

namespace Renderer
{
	class SkyboxTechnique : public Technique
	{
	public:
		virtual void		GatherLocations() override;

				void		SetMVP( const glm::mat4& mMVP );
				void		SetDiffuseMap( int iTexIndex );

	protected:
		// ins
		GLint	m_iPositionLocation;			// vec3
		GLint	m_iUVLocation;					// vec2

		// uniforms
		GLint	m_iMVPLocation;					// mat4
		GLint	m_iDiffuseMapLocation;			// tex2d
	};
}