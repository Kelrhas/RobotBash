#pragma once

#include "Technique.h"

#include "DirectionalLight.h"
#include "PointLight.h"

#define MAX_POINT_LIGHTS 16

namespace Renderer
{
	class DeferredLightingTechnique : public Technique
	{
	public:
		virtual void		BindLocations() override;
		virtual	void		GatherLocations();

		void		SetEyeWorldPos( glm::vec3 vEyePos );
		void		SetLightMVMatrix( glm::mat4 mWVP );
		void		SetLightProjMatrix( glm::mat4 mWVP );
		void		SetShadowmap( GLint iTex );

		void		SetSunData( const DirectionalLight::DirectionalLightData* pLightData );
		void		SetPointLightData( int iIndex, const PointLight::PointLightData* pLightData );
		void		SetNbPointLight( int iNbLight );

	protected:
		// in
		GLint	m_iPositionLocation;			// vec3
		GLint	m_iUVLocation;					// vec2

		// uniform
		GLint	m_iWorldPosMapLocation;			// tex2d
		GLint	m_iAlbedoMapLocation;			// tex2d
		GLint	m_iNormalMapLocation;			// tex2d
		GLint	m_iUVMapLocation;				// tex2d
		GLint	m_iEyeWorldPosLocation;			// vec3
		GLint	m_iLightMVLocation;				// mat4
		GLint	m_iLightViewToProjLocation;		// mat4
		GLint	m_iShadowmapLocation;			// tex2d
		GLint	m_iPointLightNumberLocation;	// int
		//GLint	m_iSpotLightNumberLocation;		// int

		struct
		{
			GLint	Direction;					// vec3
			GLint	Color;						// vec3
			GLuint	AmbientIntensity;			// float
			GLuint	DiffuseIntensity;			// float
		} m_sunLightLocation;

		struct
		{
			GLuint Color;						// vec3
			GLuint AmbientIntensity;			// float
			GLuint DiffuseIntensity;			// float
			GLuint Position;					// vec3
			GLuint CstAttenuation;				// float
			GLuint LinearAttenuation;			// float
			GLuint ExpAttenuation;				// float
		} m_pointLightsLocation[MAX_POINT_LIGHTS];
		
	};
}
