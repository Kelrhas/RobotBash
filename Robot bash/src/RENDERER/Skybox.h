#pragma once

//#include "DeferredLightTechnique.h"
#include "SkyboxTechnique.h"

namespace Renderer
{
	class GLVAO;

	class Skybox
	{
	public:
		Skybox();
		~Skybox();

		void	Load( std::string folder );
		void	Draw( glm::mat4 mMVP );

	protected:
		GLuint				m_iTexture;
		GLVAO*				m_pVAO;

		SkyboxTechnique		m_oTechnique;
	};
}