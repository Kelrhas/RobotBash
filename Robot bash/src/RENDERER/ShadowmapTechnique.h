#pragma once

#include "Technique.h"

namespace Renderer
{

	class ShadowmapTechnique : public Technique
	{
	public:
		virtual void		BindLocations() override;
		virtual	void		GatherLocations();

				void		SetMVP( const glm::mat4& mMVP );

	protected:
		// ins
		GLint	m_iPositionLocation;			// vec3

		// uniforms
		GLint	m_iMVPLocation;					// mat4
	};
}