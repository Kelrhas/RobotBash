#include "StdAfx.h"
#include "EntityComponent.h"

namespace Entities
{

	EntityComponent::EntityComponent()
		: m_pOwner(nullptr)
	{
	}

	bool EntityComponent::Init(Entity* pOwner)
	{
		Assert_return_result(pOwner != nullptr);
		m_pOwner = pOwner;
		return true;
	}
}