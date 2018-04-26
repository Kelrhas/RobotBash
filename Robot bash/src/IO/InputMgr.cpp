#include "StdAfx.h"
#include "InputMgr.h"

#include "win32main.h"

#include <Windowsx.h>

#define KEY_DOWN_FLAG 0x0001
#define KEY_WASDOWN_FLAG 0x0002

namespace Input
{

	InputMgr* InputMgr::s_pInstance = nullptr;

	InputMgr::InputMgr()
		: m_pWindow(nullptr)
	{
		Assert(s_pInstance == nullptr);

		s_pInstance = this;
	}

	InputMgr::~InputMgr()
	{

	}

	bool InputMgr::Init(HWND pWindow)
	{
		m_pWindow = pWindow;

		return true;
	}

	bool InputMgr::Update(float fDeltaTime)
	{
		bool bResult = false;
		MSG message;
		while (PeekMessage(&message, m_pWindow, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		{
			switch (message.message)
			{
			//case WM_SYSKEYDOWN:
			//case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				uint32_t vkCode = (uint32_t)message.wParam;
				bool bWasDown = ((message.lParam & (1 << 30)) != 0);
				bool bIsDown = ((message.lParam & (1 << 31)) == 0);

				KeyCode eKeyCode = (KeyCode)vkCode;
				if (bIsDown)
				{
					m_oKeyMap[eKeyCode].uFlags |= KEY_DOWN_FLAG;
				}
				else
				{
					m_oKeyMap[eKeyCode].uFlags &= ~KEY_DOWN_FLAG;
					m_oKeyMap[eKeyCode].fTimeDown = 0;
				}

				if (bWasDown)
					m_oKeyMap[eKeyCode].uFlags |= KEY_WASDOWN_FLAG;
				else
					m_oKeyMap[eKeyCode].uFlags &= ~KEY_WASDOWN_FLAG;

				if (bIsDown != bWasDown)
				{
					for (BoolFuncVec::const_iterator it(m_oEventKeyMap[eKeyCode].begin()); it != m_oEventKeyMap[eKeyCode].end(); ++it)
					{
						(*it)(bIsDown);
					}

					for( std::vector<KeyCodeBoolFunc>::const_iterator it( m_oEventKey.begin() ); it != m_oEventKey.end(); ++it )
					{
						(*it)(eKeyCode, bIsDown);
					}
				}

				LOG("%d -> %d %s", vkCode, eKeyCode, bIsDown ? "down" : "up");
				bResult = true;
			}
			break;

			case WM_CHAR:
			{
				char str[3] = "0\n";
				str[0] = (char) message.wParam;
				OutputDebugStringA( str );

				for( CharFuncVec::const_iterator it( m_oEventChar.begin() ); it != m_oEventChar.end(); ++it )
				{
					(*it)((char)message.wParam);
				}
				bResult = true;
			}
			break;

			default:
				break;
			}
		}

		while (PeekMessage(&message, m_pWindow, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
		{
			switch( message.message )
			{
			case WM_MOUSEMOVE:
			{
				glm::vec2 vMousePos( GET_X_LPARAM( message.lParam ), GET_Y_LPARAM( message.lParam ) );
				vMousePos.x /= WINDOW_WIDTH;
				vMousePos.y /= WINDOW_HEIGHT;
				if( glm::any( glm::isnan( m_vMousePos ) ) )
					m_vMousePos = vMousePos;
				else
				{
					glm::vec2 vDelta = vMousePos - m_vMousePos;
					for( Vec2FuncVec::const_iterator it( m_oEventMouseMove.begin() ); it != m_oEventMouseMove.end(); ++it )
						(*it)(vDelta);
					m_vMousePos = vMousePos;
				}
				bResult = true;
				break;
			}

			case WM_LBUTTONDOWN:
				m_oMouseInfos[0].uFlags |= KEY_DOWN_FLAG;
				for( BoolFuncVec::const_iterator it( m_oEventMouse[0].begin() ); it != m_oEventMouse[0].end(); ++it )
					(*it)(true);
				bResult = true;
				break;

			case WM_LBUTTONUP:
				m_oMouseInfos[0].uFlags &= ~KEY_DOWN_FLAG;
				m_oMouseInfos[0].fTimeDown = 0;
				for( BoolFuncVec::const_iterator it( m_oEventMouse[0].begin() ); it != m_oEventMouse[0].end(); ++it )
					(*it)(false);
				bResult = true;
				break;

			case WM_RBUTTONDOWN:
				m_oMouseInfos[1].uFlags |= KEY_DOWN_FLAG;
				for( BoolFuncVec::const_iterator it( m_oEventMouse[1].begin() ); it != m_oEventMouse[1].end(); ++it )
					(*it)(true);
				bResult = true;
				break;

			case WM_RBUTTONUP:
				m_oMouseInfos[1].uFlags &= ~KEY_DOWN_FLAG;
				m_oMouseInfos[1].fTimeDown = 0;
				for( BoolFuncVec::const_iterator it( m_oEventMouse[1].begin() ); it != m_oEventMouse[1].end(); ++it )
					(*it)(false);
				bResult = true;
				break;

			case WM_MBUTTONDOWN:
				m_oMouseInfos[2].uFlags |= KEY_DOWN_FLAG;
				for( BoolFuncVec::const_iterator it( m_oEventMouse[2].begin() ); it != m_oEventMouse[2].end(); ++it )
					(*it)(true);
				bResult = true;
				break;

			case WM_MBUTTONUP:
				m_oMouseInfos[2].uFlags &= ~KEY_DOWN_FLAG;
				m_oMouseInfos[2].fTimeDown = 0;
				for( BoolFuncVec::const_iterator it( m_oEventMouse[2].begin() ); it != m_oEventMouse[2].end(); ++it )
					(*it)(false);
				bResult = true;
				break;

			case WM_MOUSEWHEEL:
				short iDelta = HIWORD( message.wParam );
				float fValue = ((float)iDelta ) / WHEEL_DELTA;
				for( FloatFuncVec::const_iterator it( m_oEventMouseWheel.begin() ); it != m_oEventMouseWheel.end(); ++it )
					(*it)(fValue);
				bResult = true;
				break;

			}
		}
		
		for (int i = 0; i < MAX_INPUT_KEYS; ++i)
		{
			if (m_oKeyMap[i].uFlags & KEY_DOWN_FLAG)
				m_oKeyMap[i].fTimeDown += fDeltaTime;
		}

		for (int i = 0; i < 5; ++i)
		{
			if (m_oMouseInfos[i].uFlags & KEY_DOWN_FLAG)
				m_oMouseInfos[i].fTimeDown += fDeltaTime;
		}

		return bResult;
;
	}

	void InputMgr::RegisterKey(KeyCode eKeycode, BoolFunc pEventFunc )
	{
		s_pInstance->m_oEventKeyMap[eKeycode].push_back(pEventFunc);
	}

	void InputMgr::RegisterKey(KeyCodeBoolFunc pEventFunc )
	{
		s_pInstance->m_oEventKey.push_back(pEventFunc);
	}

	void InputMgr::RegisterChar( CharFunc pEventFunc )
	{
		s_pInstance->m_oEventChar.push_back( pEventFunc );
	}

	void InputMgr::RegisterLeftMouseButton(BoolFunc pEventFunc )
	{
		s_pInstance->m_oEventMouse[0].push_back(pEventFunc);
	}

	void InputMgr::RegisterRightMouseButton( BoolFunc pEventFunc )
	{
		s_pInstance->m_oEventMouse[1].push_back( pEventFunc );
	}

	void InputMgr::RegisterMiddleMouseButton( BoolFunc pEventFunc )
	{
		s_pInstance->m_oEventMouse[2].push_back( pEventFunc );
	}

	void InputMgr::RegisterMouseWheelButton( FloatFunc pEventFunc )
	{
		s_pInstance->m_oEventMouseWheel.push_back( pEventFunc );
	}

	void InputMgr::RegisterMouseMove( Vec2Func pEventFunc )
	{
		s_pInstance->m_oEventMouseMove.push_back( pEventFunc );
	}

	bool InputMgr::IsKeyPressed(KeyCode eKeyCode)
	{
		return (s_pInstance->m_oKeyMap[eKeyCode].uFlags & KEY_DOWN_FLAG) && !(s_pInstance->m_oKeyMap[eKeyCode].uFlags & KEY_WASDOWN_FLAG);
	}

	bool InputMgr::IsKeyDown(KeyCode eKeyCode)
	{
		return s_pInstance->m_oKeyMap[eKeyCode].uFlags & KEY_DOWN_FLAG;
	}

	bool InputMgr::IsMouseLeftDown()
	{
		return s_pInstance->m_oMouseInfos[0].uFlags & KEY_DOWN_FLAG;
	}

	float InputMgr::GetTimeDown(KeyCode eKeyCode)
	{
		return s_pInstance->m_oKeyMap[eKeyCode].fTimeDown;
	}

	char InputMgr::GetCharFromCode( KeyCode eKeyCode )
	{
		return (char) eKeyCode;
	}

	glm::vec2 InputMgr::GetCursorPos()
	{
		POINT p;
		::GetCursorPos(&p);
		if (ScreenToClient(s_pInstance->m_pWindow, &p))
			return glm::vec2(p.x, p.y);

		return glm::vec2(-1, -1);
	}
}