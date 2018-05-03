#pragma once

#include "Technique.h"

namespace Renderer
{

	class DeferredGeometryTechnique : public Technique
	{
	public:
		virtual void		BindLocations() override;
		virtual	void		GatherLocations();

				void		SetModelMatrix( const glm::mat4& mMVP );
				void		SetMVP( const glm::mat4& mMVP );
				void		SetNormalMatrix( const glm::mat4& mNormalMtx );
				void		SetDiffuseMap( int iTexIndex );
				void		SetNormalMap( int iTexIndex );
				void		SetObjectId( uint32_t uId );

	protected:
		// ins
		GLint	m_iPositionLocation;			// vec3
		GLint	m_iNormalLocation;				// vec3
		GLint	m_iUVLocation;					// vec2

		// uniforms
		GLint	m_iModelMatrixLocation;			// mat4
		GLint	m_iMVPLocation;					// mat4
		GLint	m_iNormalMatrixLocation;		// mat4

		GLint	m_iDiffuseMapLocation;			// tex2d
		GLint	m_iNormalMapLocation;			// tex2d

		GLint	m_iObjectIdLocation;			// uint
	};
}