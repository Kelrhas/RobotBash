#pragma once

#include <map>
#include <vector>

#include "InputDefinitions.h"

namespace Input
{
	class InputMgr
	{
	public:
		struct KeyInfo 
		{
			uint8_t uKeyCode;
			uint8_t uFlags;
			float	fTimeDown;

			KeyInfo() : uKeyCode(255), uFlags(0x0), fTimeDown(0.f) {}
		};
		typedef std::function<void( KeyCode, bool )> KeyCodeBoolFunc;

		InputMgr();
		~InputMgr();

		bool Init(HWND pWindow);
		bool Update(float fDeltaTime);

		static void RegisterKey( KeyCodeBoolFunc pEventFunc );
		static void RegisterKey( KeyCode eKeycode, BoolFunc pEventFunc );
		static void RegisterChar( CharFunc pEventFunc );
		static void RegisterLeftMouseButton( BoolFunc pEventFunc );
		static void RegisterRightMouseButton( BoolFunc pEventFunc );
		static void RegisterMiddleMouseButton( BoolFunc pEventFunc );
		static void RegisterMouseWheelButton( FloatFunc pEventFunc );
		static void RegisterMouseMove( Vec2Func pEventFunc );
		static bool IsKeyPressed( KeyCode eKeyCode );
		static bool IsKeyDown( KeyCode eKeyCode );
		static bool IsMouseLeftDown();
		static float GetTimeDown( KeyCode eKeyCode );
		static char GetCharFromCode( KeyCode eKeyCode );

		static glm::vec2 GetCursorPos();


	protected:	
		static InputMgr*						s_pInstance;
			
		HWND									m_pWindow;
		KeyInfo									m_oKeyMap[MAX_INPUT_KEYS];
		KeyInfo									m_oMouseInfos[5];
		std::vector<KeyCodeBoolFunc>			m_oEventKey;
		std::map<KeyCode, BoolFuncVec>			m_oEventKeyMap;
		CharFuncVec								m_oEventChar;
		BoolFuncVec								m_oEventMouse[3];
		FloatFuncVec							m_oEventMouseWheel;
		Vec2FuncVec								m_oEventMouseMove;
		glm::vec2								m_vMousePos;

	};
}