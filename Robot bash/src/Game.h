#pragma once

namespace Entities
{
	class Entity;
}

class Game
{
public:
	Game();
	~Game();

	bool	Init();
	bool	Update( float fDeltaTime );

private:
	Entities::Entity* m_pSuzanne;
	Entities::Entity* m_pDodecahedron;
};