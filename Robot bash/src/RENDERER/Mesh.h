#pragma once

#include "ENTITIES/EntityComponent.h"
#include "RENDERER/Texture.h"
#include "RENDERER/GLVAO.h"

namespace Renderer
{
	class GLVAO;

	struct MeshInitData : public Entities::InitData
	{
		std::string	sFileName;
		bool		bForward;
		std::string	sDiffuseMap;

		MeshInitData()
			: sFileName("")
			, bForward(false)
			, sDiffuseMap("")
		{}
	};

	class Mesh : public Entities::EntityComponent
	{
	public:
									Mesh();
		virtual						~Mesh();

		virtual	bool				Init( Entities::Entity* pOwner ) override;
		virtual	bool				InitWithData(const Entities::InitData* pInitData) override;
		virtual bool				Update(float fDeltaTime) override;

		GLint					GetVAOId() const { return m_pVAO->GetID(); }


	protected:
				bool				Load(const char* sFileName);
				bool				Render();

		size_t						m_uNbVertex;
		GLVAO*						m_pVAO;
		bool						m_bForwardRendering;
		TextureWeakPtr				m_xDiffuseMap;

		std::string					m_sName;
	};
}