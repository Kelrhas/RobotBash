#include "StdAfx.h"
#include "win32main.h"
#include <ctime>
#include "imgui/imgui.h"

#include "ogl.h"

#include "Game.h"

#include "ENTITIES/EntityManager.h"
#include "ENTITIES/Entity.h"
#include "ENTITIES/EntityComponent.h"

#include "IO/BMPLoader.h"
#include "IO/InputMgr.h"

#include "RENDERER/Camera.h"
#include "RENDERER/DirectionalLight.h"
#include "RENDERER/GLRenderer.h"
#include "RENDERER/Light.h"
#include "RENDERER/Mesh.h"
#include "RENDERER/Technique.h"
#include "RENDERER/Texture.h"

#include "UTILS/Memory.h"
#include "UTILS/imgui_impl.h"

//////////////////////////////////////////////////////////////////////////
/// Global variables
//////////////////////////////////////////////////////////////////////////

WINDOWPLACEMENT windowPlacement;

GameInput gameInput[2];
uint8_t indexInput = 0;

//////////////////////////////////////////////////////////////////////////
/// Static = private to this file
//////////////////////////////////////////////////////////////////////////
static bool bFullscreen = false;




void SetInputState( GameButtonState* newState, GameButtonState* oldState, bool down )
{
	newState->isDown = down;
	newState->wasDown = oldState->isDown;
	oldState->isDown = down;
}

void SetInputAnalog( GameControllerInput* controller, int16_t leftX, int16_t leftY, int16_t rightX, int16_t rightY )
{
	if( leftX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
		controller->horizontalLeft = (leftX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / (32767.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	else if( leftX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
		controller->horizontalLeft = (leftX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	else
		controller->horizontalLeft = 0;

	if( leftY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
		controller->verticalLeft = (leftY - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / (32767.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	else if( leftY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
		controller->verticalLeft = (leftY + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	else
		controller->verticalLeft = 0;

	if( rightX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
		controller->horizontalRight = (rightX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / (32767.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	else if( rightX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
		controller->horizontalRight = (rightX + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	else
		controller->horizontalRight = 0;

	if( rightY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
		controller->verticalRight = (rightY - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / (32767.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	else if( rightY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
		controller->verticalRight = (rightY + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	else
		controller->verticalRight = 0;
}

void InitGLAPI()
{
	// Windows
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress( "wglSwapIntervalEXT" );

	// Misc
	glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC) wglGetProcAddress( "glDebugMessageCallback" );
	glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC) wglGetProcAddress( "glPushDebugGroup" );
	glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC) wglGetProcAddress( "glPopDebugGroup" );
	glObjectLabel = (PFNGLOBJECTLABELPROC) wglGetProcAddress( "glObjectLabel" );

	// Program
	glCreateProgram = (PFNGLCREATEPROGRAMPROC) wglGetProcAddress( "glCreateProgram" );
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC) wglGetProcAddress( "glDeleteProgram" );
	glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress( "glUseProgram" );
	glAttachShader = (PFNGLATTACHSHADERPROC) wglGetProcAddress( "glAttachShader" );
	glDetachShader = (PFNGLDETACHSHADERPROC) wglGetProcAddress( "glDetachShader" );
	glLinkProgram = (PFNGLLINKPROGRAMPROC) wglGetProcAddress( "glLinkProgram" );
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC) wglGetProcAddress( "glGetProgramiv" );
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress( "glGetShaderInfoLog" );
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress( "glGetUniformLocation" );
	glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress( "glUniform1i" );
	glUniform1ui = (PFNGLUNIFORM1UIPROC) wglGetProcAddress( "glUniform1ui" );
	glUniform1iv = (PFNGLUNIFORM1IVPROC) wglGetProcAddress( "glUniform1iv" );
	glUniform2iv = (PFNGLUNIFORM2IVPROC) wglGetProcAddress( "glUniform2iv" );
	glUniform3iv = (PFNGLUNIFORM3IVPROC) wglGetProcAddress( "glUniform3iv" );
	glUniform4iv = (PFNGLUNIFORM4IVPROC) wglGetProcAddress( "glUniform4iv" );
	glUniform1f = (PFNGLUNIFORM1FPROC) wglGetProcAddress( "glUniform1f" );
	glUniform1fv = (PFNGLUNIFORM1FVPROC) wglGetProcAddress( "glUniform1fv" );
	glUniform2fv = (PFNGLUNIFORM2FVPROC) wglGetProcAddress( "glUniform2fv" );
	glUniform3fv = (PFNGLUNIFORM3FVPROC) wglGetProcAddress( "glUniform3fv" );
	glUniform4fv = (PFNGLUNIFORM4FVPROC) wglGetProcAddress( "glUniform4fv" );
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress( "glUniformMatrix4fv" );
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) wglGetProcAddress( "glGetAttribLocation" );
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) wglGetProcAddress( "glVertexAttrib1f" );
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) wglGetProcAddress( "glVertexAttrib1fv" );
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) wglGetProcAddress( "glVertexAttrib2fv" );
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) wglGetProcAddress( "glVertexAttrib3fv" );
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) wglGetProcAddress( "glVertexAttrib4fv" );
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress( "glEnableVertexAttribArray" );
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) wglGetProcAddress( "glBindAttribLocation" );
	glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC) wglGetProcAddress( "glGetFragDataLocation" );
	glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC) wglGetProcAddress( "glBindFragDataLocation" );

	// Shader
	glCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress( "glCreateShader" );
	glDeleteShader = (PFNGLDELETESHADERPROC) wglGetProcAddress( "glDeleteShader" );
	glShaderSource = (PFNGLSHADERSOURCEPROC) wglGetProcAddress( "glShaderSource" );
	glCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress( "glCompileShader" );
	glGetShaderiv = (PFNGLGETSHADERIVPROC) wglGetProcAddress( "glGetShaderiv" );

	// FBO
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress( "glGenFramebuffers" );
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress( "glBindFramebuffer" );
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress( "glFramebufferTexture2D" );
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC) wglGetProcAddress( "glDrawBuffers" );
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress( "glCheckFramebufferStatus" );
	glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) wglGetProcAddress( "glBlitFramebuffer" );

	// VBO
	glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress( "glGenBuffers" );
	glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress( "glBindBuffer" );
	glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress( "glBufferData" );
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) wglGetProcAddress( "glVertexAttribPointer" );
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress( "glDeleteBuffers" );

	// VAO
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) wglGetProcAddress( "glGenVertexArrays" );
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) wglGetProcAddress( "glBindVertexArray" );
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) wglGetProcAddress( "glDeleteVertexArrays" );

	// Texture
	glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress( "glActiveTexture" );

	glBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress( "glBlendEquation" );
	glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) wglGetProcAddress( "glBlendEquationSeparate" );
}

void LoadXInput()
{
	HMODULE module = LoadLibraryA( "Xinput1_3.dll" );
	if( module )
	{
		//XInputGetState = (x_input_get_state*)GetProcAddress(module, "XInputGetState");
		//XInputSetState = (x_input_set_state*)GetProcAddress(module, "XInputSetState");
	}
}

void ToggleFullscreen( HWND hWnd )
{
	DWORD windowStyle = GetWindowLong( hWnd, GWL_STYLE );
	if( windowStyle & WS_CAPTION )
	{
		MONITORINFO monitor = { sizeof( monitor ) };
		if( GetWindowPlacement( hWnd, &windowPlacement ) &&
			GetMonitorInfo( MonitorFromWindow( hWnd, MONITOR_DEFAULTTOPRIMARY ), &monitor ) )
		{
			SetWindowLong( hWnd, GWL_STYLE, windowStyle & ~WS_CAPTION );
			SetWindowPos( hWnd, HWND_TOP,
						  monitor.rcMonitor.left,
						  monitor.rcMonitor.top,
						  monitor.rcMonitor.right - monitor.rcMonitor.left,
						  monitor.rcMonitor.bottom - monitor.rcMonitor.top,
						  SWP_NOOWNERZORDER | SWP_FRAMECHANGED );

			bFullscreen = true;
		}
	}
	else
	{
		SetWindowLong( hWnd, GWL_STYLE, windowStyle | WS_CAPTION );
		SetWindowPlacement( hWnd, &windowPlacement );
		SetWindowPos( hWnd, nullptr, 0, 0, 0, 0,
					  SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
					  SWP_NOOWNERZORDER | SWP_FRAMECHANGED );
		bFullscreen = false;
	}
}

LRESULT CALLBACK MainWindowCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT result = 0;
	switch( uMsg )
	{
	case WM_SIZE:
	{
		WPARAM fwSizeType = wParam; // resizing flag
		if( g_pRenderer != nullptr && fwSizeType != SIZE_MINIMIZED )
		{
			WORD nWidth = LOWORD( lParam ); // width of client area
			WORD nHeight = HIWORD( lParam ); // height of client area 

			g_pRenderer->Resize( nWidth, nHeight );
		}
	}
	break;

	//case WM_ACTIVATEAPP:
	//	break;

	case WM_CLOSE:
		//if (MessageBox(hWnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
	{
		DestroyWindow( hWnd );
	}
	break;


	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	case WM_QUIT:
	//	exit(0);
		break;

	case WM_SETCURSOR:
#if _DEBUG
		SetCursor( LoadCursor( nullptr, IDC_ARROW ) );
#else
		SetCursor( 0 );
#endif
		break;


	default:
	{
		result = DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	break;
	}

	if( uMsg == WM_POINTERLEAVE || uMsg == WM_MOUSELEAVE || uMsg == WM_POINTERCAPTURECHANGED )
	{
		return result;
	}


	return result;
}

void APIENTRY OpenGLDebugMessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam )
{
	if( severity == GL_DEBUG_SEVERITY_NOTIFICATION ) return;
	if( id == 131169 || id == 131185 || id == 131218 || id == 131204 ) return;

	char str[4096];
	sprintf_s( str, "%s", "Debug message" );

	switch( source )
	{
	case GL_DEBUG_SOURCE_API:             sprintf_s( str, "%s %s", str, "Source: API" ); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sprintf_s( str, "%s %s", str, "Source: Window System" ); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sprintf_s( str, "%s %s", str, "Source: Shader Compiler" ); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     sprintf_s( str, "%s %s", str, "Source: Third Party" ); break;
	case GL_DEBUG_SOURCE_APPLICATION:     sprintf_s( str, "%s %s", str, "Source: Application" ); break;
	case GL_DEBUG_SOURCE_OTHER:           sprintf_s( str, "%s %s", str, "Source: Other" ); break;
	}

	switch( type )
	{
	case GL_DEBUG_TYPE_ERROR:               sprintf_s( str, "%s %s", str, "Type: Error" ); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: sprintf_s( str, "%s %s", str, "Type: Deprecated Behaviour" ); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  sprintf_s( str, "%s %s", str, "Type: Undefined Behaviour" ); break;
	case GL_DEBUG_TYPE_PORTABILITY:         sprintf_s( str, "%s %s", str, "Type: Portability" ); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         sprintf_s( str, "%s %s", str, "Type: Performance" ); break;
	case GL_DEBUG_TYPE_MARKER:              sprintf_s( str, "%s %s", str, "Type: Marker" ); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          sprintf_s( str, "%s %s", str, "Type: Push Group" ); break;
	case GL_DEBUG_TYPE_POP_GROUP:           sprintf_s( str, "%s %s", str, "Type: Pop Group" ); break;
	case GL_DEBUG_TYPE_OTHER:               sprintf_s( str, "%s %s", str, "Type: Other" ); break;
	}

	switch( severity )
	{
	case GL_DEBUG_SEVERITY_HIGH:         sprintf_s( str, "%s %s", str, "Severity: high" ); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       sprintf_s( str, "%s %s", str, "Severity: medium" ); break;
	case GL_DEBUG_SEVERITY_LOW:          sprintf_s( str, "%s %s", str, "Severity: low" ); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: sprintf_s( str, "%s %s", str, "Severity: notification" ); break;
	}

	sprintf_s( str, "%s:\n\t%s", str, message );

	Assert( severity == GL_DEBUG_SEVERITY_LOW );

	LOG_GL( str );
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	LARGE_INTEGER lastCounter;
	QueryPerformanceCounter( &lastCounter );
	LARGE_INTEGER clockFrequency;
	QueryPerformanceFrequency( &clockFrequency );
	double dClockFrequency = (double) clockFrequency.QuadPart;

	WNDCLASS windowClass = {};
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = MainWindowCallback;
	windowClass.lpszClassName = L"FirstWindow";

	DWORD windowStyle = WS_VISIBLE;
	if( bFullscreen )
	{
		windowStyle |= WS_POPUP | WS_MAXIMIZE;
	}
	else
	{
		windowStyle |= WS_OVERLAPPEDWINDOW | /*WS_SYSMENU |*/ WS_MINIMIZEBOX;
	}

	RECT windowSize = { WINDOW_OFFSET_X, WINDOW_OFFSET_Y, WINDOW_WIDTH + WINDOW_OFFSET_X, WINDOW_HEIGHT + WINDOW_OFFSET_Y };
	if( !AdjustWindowRectEx( &windowSize, windowStyle, false, 0 ) ) // recupere la taille de la fenetre en fonction d'une taille de client
	{
		return 1;
	}

	if( !RegisterClass( &windowClass ) )
		return 2;

	uint32_t uWindowWidth = windowSize.right - windowSize.left;
	uint32_t uWindowHeight = windowSize.bottom - windowSize.top;
	HWND  window = CreateWindowEx( 0, L"FirstWindow", L"Test window", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, uWindowWidth, uWindowHeight, nullptr, nullptr, hInstance, nullptr );
	if( !window )
		return 1;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof( PIXELFORMATDESCRIPTOR ),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED,    //Flags
		PFD_TYPE_RGBA,				//The kind of framebuffer. RGBA or palette.
		24,							//Colordepth of the framebuffer, without alpha
		8, 0, 8, 0, 8, 0, 8, 0,		//bit and shift per channel
		0, 0, 0, 0, 0,				//Accumulation buffer ?
		24,							//Number of bits for the depthbuffer
		8,							//Number of bits for the stencilbuffer
		0,							//Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE, 0, 0, 0, 0	//various ignored stuff
	};

	HDC hDeviceContext = GetDC( window );

	int  iWinPixelFormat;
	iWinPixelFormat = ChoosePixelFormat( hDeviceContext, &pfd );
	SetPixelFormat( hDeviceContext, iWinPixelFormat, &pfd );

	HGLRC hGLRenderContext = wglCreateContext( hDeviceContext );
	wglMakeCurrent( hDeviceContext, hGLRenderContext );

	InitGLAPI();

	// disable Vsync
	wglSwapIntervalEXT( 0 );

	bool bRunning = false;

	Input::InputMgr oInputMgr;
	oInputMgr.Init( window );
	oInputMgr.RegisterKey( Input::KeyCode::KEY_Escape, [&bRunning] ( bool ){bRunning = false; } );


	//LoadXInput();

	DEBUG::Init();
	ImGui::CreateContext();
	ImGui_Impl_Init( window, true );
	ImGui::StyleColorsDark();

	//Memory::GetInstance()->CreateMainMemory(MEGABYTE(10));
	//Memory::GetInstance()->CreateDisplayMemory(MEGABYTE(10));

	srand( (unsigned int) time( 0 ) );

	g_pRenderer = new Renderer::GLRenderer();
	Assert( g_pRenderer->Init( WINDOW_WIDTH, WINDOW_HEIGHT ) );

	g_pEntityMgr = new Entities::EntityManager();
	Assert( g_pEntityMgr->Init() );

	Game oGame;
	oGame.Init();

	Renderer::Camera pCamera;
	pCamera.Init();
	pCamera.SetActive();
	pCamera.SetPerspective( glm::radians( 60.0f ), 0.1f, 100.f );
	pCamera.LookAt( glm::vec3( 7.8f, 7.8f, 7.8f ), glm::vec3( 0, 0, 0 ) );
	

	//Renderer::Mesh* pCube = new Renderer::Mesh();
	//pCube->Init();
	//pCube->Load("Data/models/cube.obj");

	glEnable( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( OpenGLDebugMessageCallback, nullptr );
	SwapBuffers( hDeviceContext );

	MSG message;
	bRunning = true;
	while( bRunning )
	{

		LARGE_INTEGER endCounter;
		QueryPerformanceCounter( &endCounter );
		int64_t counterElapsed = endCounter.QuadPart - lastCounter.QuadPart;
		double dTimeElapsed = (double) counterElapsed / dClockFrequency;
		double dFPS = dClockFrequency / (double) counterElapsed;
		lastCounter = endCounter;
		float fDeltaTime = (float) dTimeElapsed * DEBUG::fTimeFactor;

		// update input messages
		if( !oInputMgr.Update( fDeltaTime ) )
		{
			// and then process the other messages
			while( PeekMessage( &message, window, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &message );
				DispatchMessage( &message );
			}

			// definitely quit
			if( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) )
			{
				if( message.message == WM_QUIT )
				{
					bRunning = false;
					break;
				}
				else
				{
					TranslateMessage( &message );
					DispatchMessage( &message );
				}
			}
		}

		ImGui_Impl_NewFrame( fDeltaTime );

		//if( DEBUG::bShowDebugMenu )
		//	ImGui::Text( "FPS %f", dFPS );

		oGame.Update( fDeltaTime );
		g_pEntityMgr->Update( fDeltaTime );
		g_pRenderer->GetCamera()->Update( fDeltaTime );

		Assert( g_pRenderer->Render() );

		SwapBuffers( hDeviceContext ); // will wait for Vsync if enabled

		// TODO sound
	}

	ImGui_Impl_Shutdown();
	ImGui::DestroyContext();

	wglDeleteContext( hGLRenderContext );

	return 0;
}


/* links:
http://antongerdelan.net/opengl/hellotriangle.html
https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
https://sites.google.com/site/opengltutorialsbyaks/introduction-to-opengl-3-2---tutorial-01
https://www.opengl.org/discussion_boards/showthread.php/177999-GCC-OpenGL-without-glut

*/