//----------------------------------------------------------------------------------------------------
// GameCommon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <vector>
#include <windows.h>
//-Forward-Declaration--------------------------------------------------------------------------------
struct Rgba8;
struct Vec2;
class App;
class AudioSystem;
class BitmapFont;
class Game;
class Renderer;
class RendererEx;
class RandomNumberGenerator;

// one-time declaration
extern App*                   g_theApp;
extern AudioSystem*           g_theAudio;
extern BitmapFont*            g_theBitmapFont;
extern Game*                  g_theGame;
extern Renderer*              g_theRenderer;
extern RendererEx*            g_theRendererEx;
extern RandomNumberGenerator* g_theRNG;
extern std::vector<HWND>      g_gameWindows;

//-----------------------------------------------------------------------------------------------
// initial settings
//
float constexpr SCREEN_SIZE_X = 1600.f;
float constexpr SCREEN_SIZE_Y = 800.f;

//-----------------------------------------------------------------------------------------------
// DebugRender-related
//
void DebugDrawRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);
void DebugDrawLine(Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);
void DebugDrawGlowCircle(Vec2 const& center, float radius, Rgba8 const& color, float glowIntensity);
void DebugDrawGlowBox(Vec2 const& center, Vec2 const& dimensions, Rgba8 const& color, float glowIntensity);
void DebugDrawBoxRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);

//----------------------------------------------------------------------------------------------------
template <typename T>
void SafeDeletePointer(T*& pointer)
{
    if (pointer != nullptr)
    {
        delete pointer;
        pointer = nullptr;
    }
}

void CreateAndRegisterMultipleWindows(HINSTANCE hInstance, int windowCount);
HWND CreateGameWindow(HINSTANCE hInstance, const wchar_t* title, int x, int y, int width, int height);
