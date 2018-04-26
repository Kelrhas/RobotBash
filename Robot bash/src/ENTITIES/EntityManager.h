#pragma once

namespace Entities
{
	class Entity;

	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		bool Init();
		bool Update( float fDeltaTime );
		void ImGuiDraw();

		void Reset();

		void AddEntity( Entity* pEntity );
		void RemoveEntity( Entity* pEntity );

	private:
		static EntityManager*						s_pInstance;

		std::vector<Entity*>						m_oEntities;
	};
}


extern Entities::EntityManager* g_pEntityMgr;
