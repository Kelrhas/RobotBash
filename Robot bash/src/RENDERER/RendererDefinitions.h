#pragma once

#include <GL/GL.h>
#include <glext.h>

namespace Renderer
{
	enum RenderTarget
	{
		renderTarget_WorldPos = 0,
		renderTarget_Albedo,
		renderTarget_Normal,
		renderTarget_TexCoord,
		//renderTarget_Metalness,
		//renderTarget_Roughness,
		//renderTarget_Reflectance,
		renderTarget_FinalLit,

		renderTarget_Count,		// display albedo, normal, metal, 

		// only for display
		renderTarget_Depth,		// special buffer
		renderTarget_Shadowmap,	// special buffer
		renderTarget_All,
	};

	struct RenderTargetInfo
	{
		const char* sName;
		GLenum eAttachment;
		uint32_t uInternalFormat;
		uint32_t uFormat;

		RenderTargetInfo( const char* sName, GLenum eAttachment, uint32_t uInternalFormat, uint32_t uFormat )
		{
			this->sName = sName;
			this->eAttachment = eAttachment;
			this->uInternalFormat = uInternalFormat;
			this->uFormat = uFormat;
		}
	};

	static const RenderTargetInfo aRenderTargetInfos[] =
	{
		RenderTargetInfo( "World pos",		GL_COLOR_ATTACHMENT0,	GL_RGB32F,				GL_RGB ),
		RenderTargetInfo( "Albedo",			GL_COLOR_ATTACHMENT1,	GL_RGB32F,				GL_RGB ),
		RenderTargetInfo( "Normal",			GL_COLOR_ATTACHMENT2,	GL_RGB8_SNORM,			GL_RGB ),
		RenderTargetInfo( "Tex coord",		GL_COLOR_ATTACHMENT3,	GL_RGB32F,				GL_RGB ),
		RenderTargetInfo( "Final lit",		GL_COLOR_ATTACHMENT4,	GL_RGB32F,				GL_RGB ),
		RenderTargetInfo( "Count",			GL_NONE,				GL_NONE,				GL_NONE ),
		RenderTargetInfo( "Depth",			GL_DEPTH_ATTACHMENT,	GL_DEPTH_COMPONENT24,	GL_DEPTH_COMPONENT ),
	};
}