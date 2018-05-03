#pragma once

namespace Entities
{
	class Entity;

	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

				bool				Init();
				bool				Update( float fDeltaTime );
				void				ImGuiDraw();

				void				Reset();

				uint32_t			AddEntity( Entity* pEntity );
				void				RemoveEntity( Entity* pEntity );

				uint32_t			GetEntityID( const Entity* pEntity ) const;
				Entity*				GetEntityByID( uint32_t id ) const;

	private:
		static EntityManager*		s_pInstance;

		std::vector<Entity*>		m_oEntities;
	};
}


extern Entities::EntityManager* g_pEntityMgr;
