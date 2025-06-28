//----------------------------------------------------------------------------------------------------
// App.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/App.hpp"

#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Renderer/RendererEx.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Platform/WindowEx.hpp"
#include "Game/Game.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
App*         g_theApp        = nullptr;       // Created and owned by Main_Windows.cpp
AudioSystem* g_theAudio      = nullptr;       // Created and owned by the App
BitmapFont*  g_theBitmapFont = nullptr;       // Created and owned by the App
Game*        g_theGame       = nullptr;       // Created and owned by the App
Renderer*    g_theRenderer   = nullptr;       // Created and owned by the App

RandomNumberGenerator* g_theRNG      = nullptr;       // Created and owned by the App
Window*                g_theWindow   = nullptr;       // Created and owned by the App
WindowEx*              g_theWindowEx = nullptr;       // Created and owned by the App
std::vector<HWND>      g_gameWindows;

//----------------------------------------------------------------------------------------------------
STATIC bool App::m_isQuitting = false;

App::App(HINSTANCE const& hInstance)
    : m_hInstance(hInstance)
{
}

//----------------------------------------------------------------------------------------------------
/// @brief
/// Create all engine subsystems in a specific order.
void App::Startup()
{
    //-Start-of-EventSystem---------------------------------------------------------------------------

    sEventSystemConfig constexpr eventSystemConfig;
    g_theEventSystem = new EventSystem(eventSystemConfig);
    g_theEventSystem->SubscribeEventCallbackFunction("OnCloseButtonClicked", OnWindowClose);
    g_theEventSystem->SubscribeEventCallbackFunction("quit", OnWindowClose);

    //-End-of-EventSystem-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-InputSystem---------------------------------------------------------------------------

    sInputSystemConfig constexpr inputConfig;
    g_theInput = new InputSystem(inputConfig);

    //-End-of-InputSystem-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-Window--------------------------------------------------------------------------------

    // sWindowConfig windowConfig;
    // windowConfig.m_aspectRatio = 2.f;
    // windowConfig.m_inputSystem = g_theInput;
    // windowConfig.m_windowTitle = "DEFAULT";
    // g_theWindow                = new Window(windowConfig);

    // sWindowExConfig windowExConfig;
    // windowExConfig.m_aspectRatio  = 2.f;
    // windowExConfig.m_inputSystem  = g_theInput;
    // windowExConfig.m_windowTitle  = "FirstMultipleWindows";
    // windowExConfig.m_iconFilePath = L"C:/p4/Personal/SD/FirstMultipleWindows/Run/Data/Images/Test_StbiFlippedAndOpenGL.ico";
    // g_theWindowEx                 = new WindowEx(windowExConfig);


    //-End-of-Window----------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-Renderer------------------------------------------------------------------------------

    // sRenderConfig renderConfig;
    // renderConfig.m_window = g_theWindow;
    // g_theRenderer         = new Renderer(renderConfig);

    // RendererEx::sRenderExConfig renderExConfig;
    // renderExConfig.m_window = g_theWindowEx;
    // g_theRendererEx         = new RendererEx(renderExConfig);
    // 初始化渲染器
    g_theRendererEx = new RendererEx();
    if (FAILED(g_theRendererEx->Initialize()))
    {
        MessageBox(nullptr, L"Failed to initialize renderer", L"Error", MB_OK);
    }

    //-End-of-Renderer--------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-DebugRender---------------------------------------------------------------------------

    sDebugRenderConfig debugConfig;
    debugConfig.m_renderer = g_theRenderer;
    debugConfig.m_fontName = "SquirrelFixedFont";

    //-End-of-DebugRender-----------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-DevConsole----------------------------------------------------------------------------

    // m_devConsoleCamera = new Camera();
    //
    // Vec2 const bottomLeft     = Vec2::ZERO;
    // Vec2 const screenTopRight = Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
    //
    // m_devConsoleCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);
    //
    // sDevConsoleConfig devConsoleConfig;
    // devConsoleConfig.m_defaultRenderer = g_theRenderer;
    // devConsoleConfig.m_defaultFontName = "SquirrelFixedFont";
    // devConsoleConfig.m_defaultCamera   = m_devConsoleCamera;
    // g_theDevConsole                    = new DevConsole(devConsoleConfig);

    //-End-of-DevConsole------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //-Start-of-AudioSystem---------------------------------------------------------------------------

    sAudioSystemConfig constexpr audioConfig;
    g_theAudio = new AudioSystem(audioConfig);

    //-End-of-AudioSystem-----------------------------------------------------------------------------

    g_theEventSystem->Startup();
    // g_theWindow->Startup();
    // g_theWindowEx->Startup();
    // WindowEx* window1             = g_theWindowEx->CreateChildWindow(L"ChildWindow", 100, 100, 400, 300);
    // WindowEx* window2             = g_theWindowEx->CreateChildWindow(L"ChildWindow", 600, 200, 400, 300);
    // WindowEx* window3             = g_theWindowEx->CreateChildWindow(L"ChildWindow 3", 300, 400, 400, 300);
    // if (window1 != nullptr)
    // {
    //     m_windowExs.push_back(window1);
    // }
    // if (window2 != nullptr)
    // {
    //     m_windowExs.push_back(window2);
    // }
    // m_windowExs.push_back(window3);
    // g_theRenderer->Startup();
    g_theRendererEx->Startup();
    // DebugRenderSystemStartup(debugConfig);
    // g_theDevConsole->StartUp();
    g_theInput->Startup();
    g_theAudio->Startup();

    // g_theBitmapFont = g_theRendererEx->CreateOrGetBitmapFontFromFile("Data/Fonts/SquirrelFixedFont"); // DO NOT SPECIFY FILE .EXTENSION!!  (Important later on.)
    g_theRNG  = new RandomNumberGenerator();
    g_theGame = new Game();

    CreateAndRegisterMultipleWindows(m_hInstance, 3);
}

//----------------------------------------------------------------------------------------------------
// All Destroy and ShutDown process should be reverse order of the StartUp
//
void App::Shutdown()
{
    for (WindowEx& window : windows)
    {
        if (window.m_displayContext) ReleaseDC((HWND)window.m_windowHandle, (HDC)window.m_displayContext);
    }

    // Destroy all Engine Subsystem
    GAME_SAFE_RELEASE(g_theGame);
    GAME_SAFE_RELEASE(g_theRNG);
    GAME_SAFE_RELEASE(g_theBitmapFont);

    g_theAudio->Shutdown();
    g_theInput->Shutdown();
    // g_theDevConsole->Shutdown();

    GAME_SAFE_RELEASE(m_devConsoleCamera);

    // DebugRenderSystemShutdown();
    // g_theRendererEx->Shutdown();
    // g_theRenderer->Shutdown();
    // g_theWindowEx->Shutdown();
    // g_theWindow->Shutdown();
    g_theEventSystem->Shutdown();

    GAME_SAFE_RELEASE(g_theAudio);
    GAME_SAFE_RELEASE(g_theRendererEx);
    GAME_SAFE_RELEASE(g_theRenderer);
    GAME_SAFE_RELEASE(g_theWindowEx);
    GAME_SAFE_RELEASE(g_theWindow);
    GAME_SAFE_RELEASE(g_theInput);
}

//----------------------------------------------------------------------------------------------------
// One "frame" of the game.  Generally: Input, Update, Render.  We call this 60+ times per second.
//
void App::RunFrame()
{
    BeginFrame();   // Engine pre-frame stuff
    Update();       // Game updates / moves / spawns / hurts / kills stuff
    Render();       // Game draws current state of things
    EndFrame();     // Engine post-frame stuff
}

//----------------------------------------------------------------------------------------------------
void App::RunMainLoop()
{
    // Program main loop; keep running frames until it's time to quit
    while (!m_isQuitting)
    {
        // Sleep(16); // Temporary code to "slow down" our app to ~60Hz until we have proper frame timing in
        RunFrame();
    }
}

//----------------------------------------------------------------------------------------------------
STATIC bool App::OnWindowClose(EventArgs& args)
{
    UNUSED(args)

    RequestQuit();

    return true;
}

//----------------------------------------------------------------------------------------------------
STATIC void App::RequestQuit()
{
    m_isQuitting = true;
}

void App::AddWindow(HWND const& hwnd)
{
    WindowEx window;
    window.m_windowHandle   = hwnd;
    window.m_displayContext = GetDC(hwnd);
    window.needsUpdate      = true;

    // UpdateWindowPosition(window);
    windows.push_back(window);
}

//----------------------------------------------------------------------------------------------------
void App::BeginFrame() const
{
    g_theEventSystem->BeginFrame();
    // g_theWindow->BeginFrame();
    g_theWindowEx->BeginFrame();
    // g_theRenderer->BeginFrame();
    // g_theRendererEx->BeginFrame();
    // DebugRenderBeginFrame();
    // g_theDevConsole->BeginFrame();
    g_theInput->BeginFrame();
    g_theAudio->BeginFrame();
}

//----------------------------------------------------------------------------------------------------
void App::Update()
{
    Clock::TickSystemClock();

    if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE))
    {
        CreateAndRegisterMultipleWindows(m_hInstance, 2);
    }

    // UpdateCursorMode();
    for (WindowEx& window : windows)
    {
        window.UpdateWindowDrift((float)Clock::GetSystemClock().GetDeltaSeconds() * 1.5f);
        window.UpdateWindowPosition();
    }
    g_theGame->Update();
}

//----------------------------------------------------------------------------------------------------
// Some simple OpenGL example drawing code.
// This is the graphical equivalent of printing "Hello, world."
//
// Ultimately this function (App::Render) will only call methods on Renderer (like Renderer::DrawVertexArray)
//	to draw things, never calling OpenGL (nor DirectX) functions directly.
//
void App::Render() const
{
    Rgba8 const clearColor = Rgba8(0, 0, 0, 0);

    // g_theRenderer->ClearScreen(clearColor);
    // g_theGame->Render();

    g_theRendererEx->Render(windows);

    AABB2 const box = AABB2(Vec2::ZERO, Vec2(1600.f, 30.f));

    // g_theDevConsole->Render(box);
}

//----------------------------------------------------------------------------------------------------
void App::EndFrame() const
{
    g_theEventSystem->EndFrame();
    // g_theWindow->EndFrame();
    // g_theWindowEx->EndFrame();
    // g_theRenderer->EndFrame();
    g_theRendererEx->EndFrame();
    // DebugRenderEndFrame();
    // g_theDevConsole->EndFrame();
    g_theInput->EndFrame();
    g_theAudio->EndFrame();
}

//----------------------------------------------------------------------------------------------------
void App::UpdateCursorMode()
{
     bool const doesWindowHasFocus   = GetActiveWindow() == g_theWindow->GetWindowHandle();
    bool const isAttractState       = g_theGame->GetCurrentGameState() == eGameState::ATTRACT;
     // bool const shouldUsePointerMode = !doesWindowHasFocus || g_theDevConsole->IsOpen() || isAttractState;
     bool const shouldUsePointerMode = !doesWindowHasFocus  || isAttractState;

     if (shouldUsePointerMode == true)
     {
         g_theInput->SetCursorMode(eCursorMode::POINTER);
     }
     else
     {
         g_theInput->SetCursorMode(eCursorMode::FPS);
     }
}
