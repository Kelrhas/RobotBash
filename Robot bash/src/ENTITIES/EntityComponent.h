#pragma once

namespace Entities
{
	class Entity;

	// for virtual Init
	struct InitData
	{
	};

	class EntityComponent
	{
	public:
		EntityComponent();
		~EntityComponent(){};

		virtual bool				Init(Entity* pOwner);
		virtual bool				InitWithData( const InitData* pInitData ) { return true; };
		virtual bool				Update(float fDeltaTime) = 0;
		virtual void				ImGuiDraw() {}



				Entity*				GetOwner() { return m_pOwner; }

	protected:
		Entity*						m_pOwner;

	private:
	};
}