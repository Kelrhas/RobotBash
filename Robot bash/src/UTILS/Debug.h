#ifndef DEBUG_H
#define DEBUG_H

#include <assert.h>
#include <stdio.h>

#include "RENDERER/RendererDefinitions.h"

#define LOG_FILE "log.txt"

#define TODO __debugbreak();

namespace DEBUG
{
	extern bool bShowDebugMenu;
	extern bool bShowGBuffer;
	extern float fTimeFactor;
	extern float fCameraMoveSpeed;
	extern float fCameraZoomSpeed;
	extern Renderer::RenderTarget eTargetToDisplay;

	void Init();
	void ToggleMenu(bool bDown);
	void ShowMenu();

	static void Log(const char* sPrefix, const char* sMessage, ...)
	{
		FILE* pFile = nullptr;
		fopen_s(&pFile, LOG_FILE, "a");
		if (!pFile) {
			fprintf(
				stderr,
				"ERROR: could not open %s file for appending\n",
				LOG_FILE
			);
			return;
		}

		va_list argptr;
		va_start(argptr, sMessage);
		char sBufffer[4096];
		sprintf_s(sBufffer, "%s: %s\n", sPrefix, sMessage);
		vfprintf(pFile, sBufffer, argptr);
		fclose(pFile);

#ifdef _WINDOWS
		char sOutputBuffer[4096];
		vsprintf_s(sOutputBuffer, sBufffer, argptr);
		OutputDebugStringA(sOutputBuffer);
#else
		vprintf(sBufffer, argptr);
#endif
		va_end(argptr);
	}
}

#define Assert__(val)\
	{if(!(val))\
		__debugbreak();}
#define Assert_return__(val)\
	{if(!(val)){\
		__debugbreak();return;}}

#define Assert_return_result__(val)\
	{bool bRes = (val);\
	 if(!bRes ){\
		__debugbreak();return bRes;}}

#define Assert_return_value__(val, ret)\
	{if(!(val)){\
		__debugbreak();return ret;}}

#define Assert_GL__()\
	{int err = glGetError();\
	if(err != GL_NO_ERROR)\
		__debugbreak();}

#ifdef _DEBUG
#define Assert(val) Assert__(val)
#define Assert_DEBUG(val) Assert(val)
#define Assert_return(val) Assert_return__(val)
#define Assert_return_DEBUG(val) Assert_return(val)
#define Assert_return_result(val) Assert_return_result__(val)
#define Assert_return_result_DEBUG(val) Assert_return_result(val)
#define Assert_return_value(val, ret) Assert_return_value__(val, ret)
#define Assert_return_value_DEBUG(val, ret) Assert_return_value(val, ret)
#define Assert_GL Assert_GL__
#else
#define Assert(val) {val;}
#define Assert_DEBUG(val) {}
#define Assert_return(val) {if(!(val))return;}
#define Assert_return_DEBUG(val) {}
#define Assert_return_result(val) {if(!(val))return false;}
#define Assert_return_result_DEBUG(val) {}
#define Assert_return_value(val, ret) {if(!(val))return ret;}
#define Assert_return_value_DEBUG(val, ret) {}
#define Assert_GL(val) {}
#endif

#define LOG(str, ...) {DEBUG::Log("Log", str, ##__VA_ARGS__);}
#define LOG_ERROR(str, ...) {DEBUG::Log("ERROR", str, ##__VA_ARGS__);}
#define LOG_GL(str, ...) {DEBUG::Log("GL", str, ##__VA_ARGS__);}


		

#endif
