#include "StdAfx.h"
#include "Debug.h"

#include <cstdarg>

#include "imgui.h"
#include "IO/InputMgr.h"

namespace DEBUG
{
	bool bShowDebugMenu = false;
	bool bShowGBuffer = false;
	float fTimeFactor = 1.f;
	float fCameraSpeed = 1.f;
	Renderer::RenderTarget eTargetToDisplay = Renderer::renderTarget_Count;
	const char** aRenderTargetNames;

	void Init()
	{
		// clear the log file
		FILE* pFile = nullptr;
		fopen_s( &pFile, LOG_FILE, "w" );
		fclose( pFile );

		Input::InputMgr::RegisterKey( Input::KEY_F1, &DEBUG::ToggleMenu );
		aRenderTargetNames = new const char*[Renderer::renderTarget_All];
		for( int i = 0; i < Renderer::renderTarget_Count; ++i )
		{
			aRenderTargetNames[i] = Renderer::aRenderTargetInfos[i].sName;
		}
		aRenderTargetNames[Renderer::renderTarget_Count] = "All";
		aRenderTargetNames[Renderer::renderTarget_Depth] = "Depth";
		aRenderTargetNames[Renderer::renderTarget_Shadowmap] = "Shadow map";
	}

	void ToggleMenu( bool bDown )
	{
		if( bDown )
			bShowDebugMenu = !bShowDebugMenu;
	}

	void ShowMenu()
	{
		ImGui::Checkbox( "Debug GBuffer", &bShowGBuffer );
		if( bShowGBuffer )
		{
			int iRenderTarget = eTargetToDisplay;
			if( ImGui::Combo( "Render target", &iRenderTarget, aRenderTargetNames, (int) Renderer::renderTarget_All ) )
				eTargetToDisplay = (Renderer::RenderTarget)iRenderTarget;
		}

		ImGui::DragFloat( "Time factor", &fTimeFactor, 0.01f, 0.f, 10.f );
		ImGui::DragFloat( "Camera speed", &fCameraSpeed, 0.01f, 0.01f, 100.f );

		char buffer[256] = { 0 };

		ImGui::InputText( "test", buffer, 256 );
	}
}