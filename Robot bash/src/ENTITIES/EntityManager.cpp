#include "stdafx.h"
#include "EntityManager.h"

#include "Entity.h"

Entities::EntityManager* g_pEntityMgr = nullptr;

namespace Entities
{
	EntityManager::EntityManager()
	{}

	EntityManager::~EntityManager()
	{}

	bool EntityManager::Init()
	{
		return true;
	}

	bool EntityManager::Update( float fDeltaTime )
	{
		for( auto it = m_oEntities.begin(); it != m_oEntities.end(); ++it )
		{
			(*it)->Update( fDeltaTime );
		}

		return true;
	}

	void EntityManager::ImGuiDraw()
	{
		if( ImGui::Begin( "Entities" ) )
		{
			for( auto it = m_oEntities.begin(); it != m_oEntities.end(); ++it )
			{
				(*it)->ImGuiDraw();
			}
			ImGui::End();
		}
	}

	void EntityManager::Reset()
	{}

	uint32_t EntityManager::AddEntity( Entity * pEntity )
	{
		m_oEntities.push_back( pEntity );
		return m_oEntities.size() - 1;
	}

	void EntityManager::RemoveEntity( Entity * pEntity )
	{
		for( auto it = m_oEntities.begin(); it != m_oEntities.end(); ++it )
		{
			if( *it == pEntity )
			{
				m_oEntities.erase( it );
			}
		}
	}

	uint32_t EntityManager::GetEntityID( const Entity* pEntity ) const
	{
		uint32_t id = 0;
		for( auto it = m_oEntities.begin(); it != m_oEntities.end(); ++it, ++id )
		{
			if( *it == pEntity )
				return id;
		}
	}

	Entity* EntityManager::GetEntityByID( uint32_t id ) const
	{
		if( id >= m_oEntities.size() ) // -1 as uint
			return nullptr;

		return m_oEntities[id];
	}
}