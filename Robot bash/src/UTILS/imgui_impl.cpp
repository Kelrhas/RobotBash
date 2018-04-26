#include <imgui.h>
#include "StdAfx.h"
#include "ogl.h"

#include "IO/InputMgr.h"


static HWND			g_Window = NULL;
static double       g_Time = 0.0f;
static bool         g_MousePressed[3] = { false, false, false };
static float        g_MouseWheel = 0.0f;
static GLuint		g_FontTexture = 0;
static int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
static unsigned int g_VboHandle = 0, g_VaoHandle = 0, g_ElementsHandle = 0;

// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
void ImGui_Impl_RenderDrawLists( ImDrawData* draw_data )
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int) (io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int) (io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if( fb_width == 0 || fb_height == 0 )
		return;
	draw_data->ScaleClipRects( io.DisplayFramebufferScale );

	// Backup GL state
	GLint last_program; glGetIntegerv( GL_CURRENT_PROGRAM, &last_program );
	GLint last_texture; glGetIntegerv( GL_TEXTURE_BINDING_2D, &last_texture );
	GLint last_active_texture; glGetIntegerv( GL_ACTIVE_TEXTURE, &last_active_texture );
	GLint last_array_buffer; glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &last_array_buffer );
	GLint last_element_array_buffer; glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer );
	GLint last_vertex_array; glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &last_vertex_array );
	GLint last_blend_src; glGetIntegerv( GL_BLEND_SRC, &last_blend_src );
	GLint last_blend_dst; glGetIntegerv( GL_BLEND_DST, &last_blend_dst );
	GLint last_blend_equation_rgb; glGetIntegerv( GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb );
	GLint last_blend_equation_alpha; glGetIntegerv( GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha );
	GLint last_viewport[4]; glGetIntegerv( GL_VIEWPORT, last_viewport );
	GLint last_scissor_box[4]; glGetIntegerv( GL_SCISSOR_BOX, last_scissor_box );
	GLboolean last_enable_blend = glIsEnabled( GL_BLEND );
	GLboolean last_enable_cull_face = glIsEnabled( GL_CULL_FACE );
	GLboolean last_enable_depth_test = glIsEnabled( GL_DEPTH_TEST );
	GLboolean last_enable_scissor_test = glIsEnabled( GL_SCISSOR_TEST );

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	glEnable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_SCISSOR_TEST );
	glActiveTexture( GL_TEXTURE0 );

	// Setup viewport, orthographic projection matrix
	glViewport( 0, 0, (GLsizei) fb_width, (GLsizei) fb_height );
	const float ortho_projection[4][4] =
	{
		{ 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
		{ 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
		{ 0.0f,                  0.0f,                  -1.0f, 0.0f },
		{ -1.0f,                  1.0f,                   0.0f, 1.0f },
	};
	glUseProgram( g_ShaderHandle );
	glUniform1i( g_AttribLocationTex, 0 );
	glUniformMatrix4fv( g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0] );
	glBindVertexArray( g_VaoHandle );

	for( int n = 0; n < draw_data->CmdListsCount; n++ )
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer( GL_ARRAY_BUFFER, g_VboHandle );
		glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr) cmd_list->VtxBuffer.Size * sizeof( ImDrawVert ), (const GLvoid*) cmd_list->VtxBuffer.Data, GL_STREAM_DRAW );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) cmd_list->IdxBuffer.Size * sizeof( ImDrawIdx ), (const GLvoid*) cmd_list->IdxBuffer.Data, GL_STREAM_DRAW );

		for( int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++ )
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if( pcmd->UserCallback )
			{
				pcmd->UserCallback( cmd_list, pcmd );
			}
			else
			{
				glBindTexture( GL_TEXTURE_2D, (GLuint) (intptr_t) pcmd->TextureId );
				glScissor( (int) pcmd->ClipRect.x, (int) (fb_height - pcmd->ClipRect.w), (int) (pcmd->ClipRect.z - pcmd->ClipRect.x), (int) (pcmd->ClipRect.w - pcmd->ClipRect.y) );
				glDrawElements( GL_TRIANGLES, (GLsizei) pcmd->ElemCount, sizeof( ImDrawIdx ) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset );
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	// Restore modified GL state
	glUseProgram( last_program );
	glActiveTexture( last_active_texture );
	glBindTexture( GL_TEXTURE_2D, last_texture );
	glBindVertexArray( last_vertex_array );
	glBindBuffer( GL_ARRAY_BUFFER, last_array_buffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer );
	glBlendEquationSeparate( last_blend_equation_rgb, last_blend_equation_alpha );
	glBlendFunc( last_blend_src, last_blend_dst );
	if( last_enable_blend ) glEnable( GL_BLEND ); else glDisable( GL_BLEND );
	if( last_enable_cull_face ) glEnable( GL_CULL_FACE ); else glDisable( GL_CULL_FACE );
	if( last_enable_depth_test ) glEnable( GL_DEPTH_TEST ); else glDisable( GL_DEPTH_TEST );
	if( last_enable_scissor_test ) glEnable( GL_SCISSOR_TEST ); else glDisable( GL_SCISSOR_TEST );
	glViewport( last_viewport[0], last_viewport[1], (GLsizei) last_viewport[2], (GLsizei) last_viewport[3] );
	glScissor( last_scissor_box[0], last_scissor_box[1], (GLsizei) last_scissor_box[2], (GLsizei) last_scissor_box[3] );
}

static const char* ImGui_Impl_GetClipboardText( void* user_data )
{
	return ""; // glfwGetClipboardString((GLFWwindow*)user_data);
}

static void ImGui_Impl_SetClipboardText( void* user_data, const char* text )
{
	//glfwSetClipboardString((GLFWwindow*)user_data, text);
}

void ImGui_Impl_MouseLeftButtonCallback( bool bDown )
{
	ImGui::GetIO().MouseDown[0] = bDown;
}

void ImGui_Impl_MouseRightButtonCallback( bool bDown )
{
	ImGui::GetIO().MouseDown[1] = bDown;
}

void ImGui_Impl_MouseWheelCallback( float fOffset )
{
	g_MouseWheel += fOffset;
}

void ImGui_Impl_KeyDownCallback( Input::KeyCode eKeycode, bool bDown )
{
	ImGuiIO& io = ImGui::GetIO();

	io.KeysDown[eKeycode] = bDown;
	io.KeyCtrl = io.KeysDown[Input::KeyCode::KEY_LeftControl] || io.KeysDown[Input::KeyCode::KEY_RightControl];
	io.KeyShift = io.KeysDown[Input::KeyCode::KEY_LeftShift] || io.KeysDown[Input::KeyCode::KEY_RightShift];
	io.KeyAlt = io.KeysDown[Input::KeyCode::KEY_LeftAlt] || io.KeysDown[Input::KeyCode::KEY_RightAlt];
	//io.KeySuper = io.KeysDown[Input::KeyCode::KEY_Lef] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void ImGui_Impl_CharCallback( char c )
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharacter( c );
}

bool ImGui_Impl_CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	Assert( io.Fonts->AddFontFromFileTTF( "Data/Arial.ttf", 14 ) != nullptr );
	io.Fonts->GetTexDataAsRGBA32( &pixels, &width, &height );   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

															  // Upload texture to graphics system
	GLint last_texture;
	glGetIntegerv( GL_TEXTURE_BINDING_2D, &last_texture );
	glGenTextures( 1, &g_FontTexture );
	glBindTexture( GL_TEXTURE_2D, g_FontTexture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

	// Store our identifier
	io.Fonts->TexID = (void *) (intptr_t) g_FontTexture;

	// Restore state
	glBindTexture( GL_TEXTURE_2D, last_texture );

	return true;
}

bool ImGui_Impl_CreateDeviceObjects()
{
	// Backup GL state
	GLint last_texture, last_array_buffer, last_vertex_array;
	glGetIntegerv( GL_TEXTURE_BINDING_2D, &last_texture );
	glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &last_array_buffer );
	glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &last_vertex_array );

	const GLchar *vertex_shader =
		"#version 330\n"
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 UV;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
		"#version 330\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
		"}\n";

	g_ShaderHandle = glCreateProgram();
	g_VertHandle = glCreateShader( GL_VERTEX_SHADER );
	g_FragHandle = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( g_VertHandle, 1, &vertex_shader, 0 );
	glShaderSource( g_FragHandle, 1, &fragment_shader, 0 );
	glCompileShader( g_VertHandle );
	glCompileShader( g_FragHandle );
	glAttachShader( g_ShaderHandle, g_VertHandle );
	glAttachShader( g_ShaderHandle, g_FragHandle );
	glLinkProgram( g_ShaderHandle );

	g_AttribLocationTex = glGetUniformLocation( g_ShaderHandle, "Texture" );
	g_AttribLocationProjMtx = glGetUniformLocation( g_ShaderHandle, "ProjMtx" );
	g_AttribLocationPosition = glGetAttribLocation( g_ShaderHandle, "Position" );
	g_AttribLocationUV = glGetAttribLocation( g_ShaderHandle, "UV" );
	g_AttribLocationColor = glGetAttribLocation( g_ShaderHandle, "Color" );

	glGenBuffers( 1, &g_VboHandle );
	glGenBuffers( 1, &g_ElementsHandle );

	glGenVertexArrays( 1, &g_VaoHandle );
	glBindVertexArray( g_VaoHandle );
	glBindBuffer( GL_ARRAY_BUFFER, g_VboHandle );
	glEnableVertexAttribArray( g_AttribLocationPosition );
	glEnableVertexAttribArray( g_AttribLocationUV );
	glEnableVertexAttribArray( g_AttribLocationColor );

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	glVertexAttribPointer( g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof( ImDrawVert ), (GLvoid*) OFFSETOF( ImDrawVert, pos ) );
	glVertexAttribPointer( g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof( ImDrawVert ), (GLvoid*) OFFSETOF( ImDrawVert, uv ) );
	glVertexAttribPointer( g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof( ImDrawVert ), (GLvoid*) OFFSETOF( ImDrawVert, col ) );
#undef OFFSETOF

	ImGui_Impl_CreateFontsTexture();

	// Restore modified GL state
	glBindTexture( GL_TEXTURE_2D, last_texture );
	glBindBuffer( GL_ARRAY_BUFFER, last_array_buffer );
	glBindVertexArray( last_vertex_array );

	return true;
}

void ImGui_Impl_InvalidateDeviceObjects()
{
	if( g_VaoHandle ) glDeleteVertexArrays( 1, &g_VaoHandle );
	if( g_VboHandle ) glDeleteBuffers( 1, &g_VboHandle );
	if( g_ElementsHandle ) glDeleteBuffers( 1, &g_ElementsHandle );
	g_VaoHandle = g_VboHandle = g_ElementsHandle = 0;

	if( g_ShaderHandle && g_VertHandle ) glDetachShader( g_ShaderHandle, g_VertHandle );
	if( g_VertHandle ) glDeleteShader( g_VertHandle );
	g_VertHandle = 0;

	if( g_ShaderHandle && g_FragHandle ) glDetachShader( g_ShaderHandle, g_FragHandle );
	if( g_FragHandle ) glDeleteShader( g_FragHandle );
	g_FragHandle = 0;

	if( g_ShaderHandle ) glDeleteProgram( g_ShaderHandle );
	g_ShaderHandle = 0;

	/*if (g_FontTexture)
	{
		glDeleteTextures(1, &g_FontTexture);
		ImGui::GetIO().Fonts->TexID = 0;
		g_FontTexture = 0;
	}*/
}

bool ImGui_Impl_Init( HWND window, bool install_callbacks )
{
	g_Window = window;

	ImGuiIO& io = ImGui::GetIO();

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_Tab] = Input::KEY_Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = Input::KEY_Left;
	io.KeyMap[ImGuiKey_RightArrow] = Input::KEY_Right;
	io.KeyMap[ImGuiKey_UpArrow] = Input::KEY_Up;
	io.KeyMap[ImGuiKey_DownArrow] = Input::KEY_Down;
	io.KeyMap[ImGuiKey_PageUp] = Input::KEY_PageUp;
	io.KeyMap[ImGuiKey_PageDown] = Input::KEY_PageDown;
	io.KeyMap[ImGuiKey_Home] = Input::KEY_Home;
	io.KeyMap[ImGuiKey_End] = Input::KEY_End;
	io.KeyMap[ImGuiKey_Delete] = Input::KEY_Delete;
	io.KeyMap[ImGuiKey_Backspace] = Input::KEY_Backspace;
	io.KeyMap[ImGuiKey_Enter] = Input::KEY_Enter;
	io.KeyMap[ImGuiKey_Escape] = Input::KEY_Escape;
	io.KeyMap[ImGuiKey_A] = Input::KEY_A;
	io.KeyMap[ImGuiKey_C] = Input::KEY_C;
	io.KeyMap[ImGuiKey_V] = Input::KEY_V;
	io.KeyMap[ImGuiKey_X] = Input::KEY_X;
	io.KeyMap[ImGuiKey_Y] = Input::KEY_Y;
	io.KeyMap[ImGuiKey_Z] = Input::KEY_Z;

	//io.RenderDrawListsFn = ImGui_Impl_RenderDrawLists;       // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.SetClipboardTextFn = ImGui_Impl_SetClipboardText;
	io.GetClipboardTextFn = ImGui_Impl_GetClipboardText;
	io.ClipboardUserData = g_Window;
#ifdef _WIN32
	io.ImeWindowHandle = window;
#endif

	if( install_callbacks )
	{
		Input::InputMgr::RegisterLeftMouseButton( ImGui_Impl_MouseLeftButtonCallback );
		Input::InputMgr::RegisterRightMouseButton( ImGui_Impl_MouseRightButtonCallback );
		Input::InputMgr::RegisterMouseWheelButton( ImGui_Impl_MouseWheelCallback );
		Input::InputMgr::RegisterKey( &ImGui_Impl_KeyDownCallback );
		Input::InputMgr::RegisterChar( &ImGui_Impl_CharCallback );
		/*glfwSetMouseButtonCallback(window, ImGui_Impl_MouseButtonCallback);
		glfwSetScrollCallback(window, ImGui_Impl_ScrollCallback);
		glfwSetKeyCallback(window, ImGui_Impl_KeyCallback);
		glfwSetCharCallback(window, ImGui_Impl_CharCallback);*/
	}


	ImGui_Impl_CreateDeviceObjects();

	return true;
}

void ImGui_Impl_Shutdown()
{
	ImGui_Impl_InvalidateDeviceObjects();
}

void ImGui_Impl_NewFrame( float fLastDeltatime )
{
	/*if (!g_FontTexture)
		ImGui_Impl_CreateDeviceObjects();*/

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	RECT windowSize, clientSize;
	GetWindowRect( g_Window, &windowSize );
	GetClientRect( g_Window, &clientSize );
	int w = clientSize.right - clientSize.left, h = clientSize.bottom - clientSize.top;
	int display_w = clientSize.right - clientSize.left, display_h = clientSize.bottom - clientSize.top;

	io.DisplaySize = ImVec2( (float) display_w, (float) display_h );
	io.DisplayFramebufferScale = ImVec2( w > 0 ? ((float) display_w / w) : 0, h > 0 ? ((float) display_h / h) : 0 );

	// Setup time step
	/*double current_time = glfwGetTime();
	io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
	g_Time = current_time;*/
	io.DeltaTime = fLastDeltatime;

	// Setup inputs
	// (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
	//if (glfwGetWindowAttrib(g_Window, GLFW_FOCUSED))
	{
		/*double mouse_x, mouse_y;
		glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
		io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);*/   // Mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
		glm::vec2 pos = Input::InputMgr::GetCursorPos();
		io.MousePos = ImVec2( pos.x, pos.y );
	}
	/*else
	{
		io.MousePos = ImVec2(-1, -1);
	}*/

	/*for (int i = 0; i < 3; i++)
	{
		io.MouseDown[i] = g_MousePressed[i];// || glfwGetMouseButton(g_Window, i) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		g_MousePressed[i] = false;
	}*/

	io.MouseWheel = g_MouseWheel;
	g_MouseWheel = 0.0f;

	// Hide OS mouse cursor if ImGui is drawing it
	ShowCursor( !io.MouseDrawCursor );

	// Start the frame
	ImGui::NewFrame();
}