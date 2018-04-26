#pragma once

// https://github.com/ocornut/imgui

#ifndef _WINDEF_
struct HWND__; // Forward or never
typedef HWND__* HWND;
#endif

struct GLFWwindow;
struct ImDrawData;


IMGUI_API bool		ImGui_Impl_Init(HWND window, bool install_callbacks);
IMGUI_API void		ImGui_Impl_Shutdown();
IMGUI_API void		ImGui_Impl_NewFrame(float fLastDeltatime);
IMGUI_API void		ImGui_Impl_RenderDrawLists( ImDrawData* draw_data );

IMGUI_API void		ImGui_Impl_InvalidateDeviceObjects();
IMGUI_API bool		ImGui_Impl_CreateDeviceObjects();

IMGUI_API void		ImGui_Impl_MouseLeftButtonCallback( bool bDown );
IMGUI_API void		ImGui_Impl_MouseRightButtonCallback( bool bDown );
IMGUI_API void		ImGui_Impl_MouseWheelCallback( float fOffset );
//IMGUI_API void		ImGui_Impl_KeyDownCallback( Input::KeyCode eKeycode, bool bDown );
IMGUI_API void		ImGui_Impl_CharCallback( char c );
