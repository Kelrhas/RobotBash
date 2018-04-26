#pragma once

#include "ogl.h"
#include "RENDERER/GLShaderProgram.h"
#include "RENDERER/RendererDefinitions.h"

namespace Renderer
{
	/*enum GBufferTextureType
	{
		GBUFFER_TEXTURE_POSITION,
		GBUFFER_TEXTURE_ALBEDO,
		GBUFFER_TEXTURE_NORMAL,
		GBUFFER_TEXTURE_TEXCOORD,

		GBUFFER_TEXTURE_COUNT,
		GBUFFER_TEXTURE_DEPTH
	};*/

	class GBuffer
	{
		friend class GLRenderer;
	public:
		GBuffer();
		~GBuffer();

		bool			Init(uint32_t uWindowWidth, uint32_t uWindowHeight);
		void			BindForWriting();
		void			BindForReading();
		void			SetReadBuffer(RenderTarget eTextureType);
		void			BindTextures();
		void			BindTexture(RenderTarget eRenderTarget, uint8_t id);
		void			DrawQuad();

	private:
		GLuint m_uFBOId;
		GLuint m_uVBOId[2]; // for the final quad
		GLuint m_uVAO;
		GLuint m_aTexturesID[renderTarget_Count];
		GLuint m_uDepthTexture;
	};

}