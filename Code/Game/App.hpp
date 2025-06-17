//----------------------------------------------------------------------------------------------------
// App.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Platform/WindowEx.hpp"

//-Forward-Declaration--------------------------------------------------------------------------------
class Camera;
class Game;

//----------------------------------------------------------------------------------------------------
class App
{
public:
    App(HINSTANCE const& hInstance);
    ~App() = default;
    void Startup();
    void Shutdown();
    void RunFrame();

    void RunMainLoop();

    static bool OnWindowClose(EventArgs& args);
    static void RequestQuit();
    static bool m_isQuitting;

    void AddWindow(HWND const& hwnd);

private:
    void BeginFrame() const;
    void Update();
    void Render() const;
    void EndFrame() const;
    void UpdateCursorMode();

    HINSTANCE             m_hInstance;
    Camera*               m_devConsoleCamera = nullptr;
    std::vector<WindowEx> windows;
};
