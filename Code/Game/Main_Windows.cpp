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
    // 創建隱藏的主窗口






    // 主訊息循環
    // MSG  msg     = {};
    // bool running = true;
    //
    // while (running)
    // {
    //     while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    //     {
    //         if (msg.message == WM_QUIT)
    //         {
    //             running = false;
    //             break;
    //         }
    //         TranslateMessage(&msg);
    //         DispatchMessage(&msg);
    //     }
    //
    //     if (running)
    //     {
    //         if (g_theRendererEx)
    //         {
    //             g_theRendererEx->Render();
    //         }
    //         Sleep(16); // ~60 FPS
    //     }
    // }

    // 清理
    // delete g_theRendererEx;
    // DestroyWindow(hiddenWindow);
    g_theApp->Shutdown();
    CoUninitialize();
    return 0;
    // g_theApp->RunMainLoop();
    // g_theApp->Shutdown();
    //
    // delete g_theApp;
    // g_theApp = nullptr;
    //
    // return 0;
}
