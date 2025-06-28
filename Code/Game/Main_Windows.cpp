//----------------------------------------------------------------------------------------------------
// Main_Windows.cpp
//----------------------------------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>

#include <cstdio>
#include <iostream>
#include <OleCtl.h>
#include <windows.h>			// #include this (massive, platform-specific) header in VERY few places (and .CPPs only)
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/RendererEx.hpp"
#include "Game/App.hpp"
#include "Game/GameCommon.hpp"

RendererEx* g_theRendererEx = nullptr;       // Created and owned by the App

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE const applicationInstanceHandle,
                   HINSTANCE,
                   LPSTR const commandLineString,
                   int)
{
    UNUSED(applicationInstanceHandle)
    UNUSED(commandLineString)

    g_theApp = new App(applicationInstanceHandle);
    g_theApp->Startup();
    g_theApp->RunMainLoop();
    g_theApp->Shutdown();

    GAME_SAFE_RELEASE(g_theApp);

    return 0;
}
