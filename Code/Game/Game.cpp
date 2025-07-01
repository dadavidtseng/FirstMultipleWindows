//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"

#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
// #include "Engine/Renderer/RendererEx.hpp"
#include "Game/App.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
Game::Game()
{
    g_theEventSystem->SubscribeEventCallbackFunction("OnWindowSizeChanged", OnWindowSizeChanged);
    m_screenCamera = new Camera();

    Vec2 const bottomLeft     = Vec2::ZERO;
    Vec2 const screenTopRight = Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
    // Vec2 const screenTopRight = Vec2(1920.0f, 1080.0f);

    m_screenCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);
    m_screenCamera->SetNormalizedViewport(AABB2::ZERO_TO_ONE);

    m_gameClock = new Clock(Clock::GetSystemClock());
}

Game::~Game()
{
    GAME_SAFE_RELEASE(m_screenCamera);
}

//----------------------------------------------------------------------------------------------------
void Game::Update()
{
    // DebugAddScreenText(Stringf("Time: %.2f\nFPS: %.2f\nScale: %.1f", m_gameClock->GetTotalSeconds(), 1.f / m_gameClock->GetDeltaSeconds(), m_gameClock->GetTimeScale()), m_screenCamera->GetOrthographicTopRight() - Vec2(250.f, 60.f), 20.f, Vec2::ZERO, 0.f, Rgba8::WHITE, Rgba8::WHITE);
    UpdateFromInput();
    AdjustForPauseAndTimeDistortion();
}

//----------------------------------------------------------------------------------------------------
void Game::Render() const
{
    //-Start-of-Screen-Camera-------------------------------------------------------------------------
    g_theRenderer->BeginCamera(*m_screenCamera);

    if (m_gameState == eGameState::ATTRACT)
    {
        RenderAttractMode();
    }
    else if (m_gameState == eGameState::GAME)
    {
        RenderGame();
    }

    g_theRenderer->EndCamera(*m_screenCamera);
    //-End-of-Screen-Camera---------------------------------------------------------------------------

    if (m_gameState == eGameState::GAME)
    {
        // DebugRenderScreen(b*m_screenCamera);
    }
}

bool Game::OnGameStateChanged(EventArgs& args)
{
    String const newGameState = args.GetValue("OnGameStateChanged", "DEFAULT");

    if (newGameState == "ATTRACT")
    {
        App::RequestQuit();
    }

    return true;
}

bool Game::OnWindowSizeChanged(EventArgs& args)
{
    int newHeight = args.GetValue("newHeight", -1);
    int newWidth  = args.GetValue("newWidth", -1);
    DebuggerPrintf("OnWindowSizeChanged (%d, %d)\n", newWidth, newHeight);
    // g_theGame->m_screenCamera->SetViewport(AABB2(Vec2::ZERO, Vec2(newWidth, newHeight)));
    return true;
}

eGameState Game::GetCurrentGameState() const
{
    return m_gameState;
}

void Game::ChangeGameState(eGameState const newGameState)
{
    if (newGameState == m_gameState) return;

    EventArgs args;

    if (newGameState == eGameState::ATTRACT) args.SetValue("OnGameStateChanged", "ATTRACT");
    else if (newGameState == eGameState::GAME) args.SetValue("OnGameStateChanged", "GAME");

    m_gameState = newGameState;

    g_theEventSystem->FireEvent("OnGameStateChanged", args);
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromInput()
{
    if (m_gameState == eGameState::ATTRACT)
    {
        if (g_theInput->IsKeyDown(KEYCODE_W)) m_position.y += 10.f;
        if (g_theInput->IsKeyDown(KEYCODE_A)) m_position.x -= 10.f;
        if (g_theInput->IsKeyDown(KEYCODE_S)) m_position.y -= 10.f;
        if (g_theInput->IsKeyDown(KEYCODE_D)) m_position.x += 10.f;
        if (g_theInput->IsKeyDown(KEYCODE_L))
        {
            m_windowPosition.x += 10.f;
            for (Window& window : g_theApp->windows)
            {
                window.UpdateWindowPosition(Vec2(m_windowPosition.x, m_windowPosition.y));
            }
        }
        if (g_theInput->IsKeyDown(KEYCODE_J))
        {
            m_windowPosition.x -= 10.f;
            for (Window& window : g_theApp->windows)
            {
                window.UpdateWindowPosition(Vec2(m_windowPosition.x, m_windowPosition.y));
            }
        }

        if (g_theInput->IsKeyDown(KEYCODE_I))
        {
            m_windowPosition.y += 10.f;
            for (Window& window : g_theApp->windows)
            {
                window.UpdateWindowPosition(Vec2(m_windowPosition.x, m_windowPosition.y));
            }
        }

        if (g_theInput->IsKeyDown(KEYCODE_K))
        {
            m_windowPosition.y -= 10.f;

            for (Window& window : g_theApp->windows)
            {
                window.UpdateWindowPosition(Vec2(m_windowPosition.x, m_windowPosition.y));
            }
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
        {
            App::RequestQuit();
        }

        if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE))
        {
            ChangeGameState(eGameState::GAME);
            SoundID const clickSound = g_theAudio->CreateOrGetSound("Data/Audio/TestSound.mp3", eAudioSystemSoundDimension::Sound2D);
            g_theAudio->StartSound(clickSound, false, 1.f, 0.f, 0.5f);
        }
    }
    else if (m_gameState == eGameState::GAME)
    {
        if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
        {
            ChangeGameState(eGameState::ATTRACT);
            SoundID const clickSound = g_theAudio->CreateOrGetSound("Data/Audio/TestSound.mp3", eAudioSystemSoundDimension::Sound2D);
            g_theAudio->StartSound(clickSound);
        }
    }
}

//----------------------------------------------------------------------------------------------------
void Game::AdjustForPauseAndTimeDistortion()
{
    if (g_theInput->WasKeyJustPressed(KEYCODE_P))
    {
        m_gameClock->TogglePause();
    }

    if (g_theInput->WasKeyJustPressed(KEYCODE_O))
    {
        m_gameClock->StepSingleFrame();
    }

    if (g_theInput->IsKeyDown(KEYCODE_T))
    {
        m_gameClock->SetTimeScale(0.1f);
    }

    if (g_theInput->WasKeyJustReleased(KEYCODE_T))
    {
        m_gameClock->SetTimeScale(1.f);
    }
}

//----------------------------------------------------------------------------------------------------
void Game::RenderAttractMode() const
{
    VertexList_PCU verts;
    AddVertsForAABB2D(verts, AABB2(Vec2::ZERO, Vec2(1920.0f, 1080.0f)));
    g_theRenderer->SetModelConstants();
    g_theRenderer->SetBlendMode(eBlendMode::ALPHA);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_NONE);
    g_theRenderer->SetSamplerMode(eSamplerMode::BILINEAR_CLAMP);
    g_theRenderer->SetDepthMode(eDepthMode::DISABLED);
    // g_theRenderer->BindTexture(nullptr);
    g_theRenderer->BindTexture(g_theRenderer->CreateOrGetTextureFromFile("Data/Images/WindowKill.png"));
    g_theRenderer->BindShader(g_theRenderer->CreateOrGetShaderFromFile("Data/Shaders/Default"));
    g_theRenderer->DrawVertexArray(verts);


    DebugDrawRing(Vec2(SCREEN_SIZE_X * 0.5f + m_position.x, SCREEN_SIZE_Y * 0.5f + m_position.y), 300.f, 10.f, Rgba8::YELLOW);
}

//----------------------------------------------------------------------------------------------------
void Game::RenderGame() const
{
    DebugDrawLine(Vec2(100.f, 100.f), Vec2(1500.f, 700.f), 10.f, Rgba8(100, 200, 100));
    DebugDrawLine(Vec2(1500.f, 100.f), Vec2(100.f, 700.f), 10.f, Rgba8(100, 200, 100));
}
