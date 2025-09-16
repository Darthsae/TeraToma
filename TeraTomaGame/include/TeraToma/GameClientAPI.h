#pragma once
#ifndef GAME_CLIENT_API_H
#define GAME_CLIENT_API_H
namespace TeraToma {
    struct GameClientAPI;
}

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <TeraToma/GameAPI.h>
#include <TeraToma/Assets/Assets.h>
#include <TeraToma/UI/UIManager.h>

namespace TeraToma {
    struct GameClientAPI {
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_AudioDeviceID audioDevice;
        TeraToma::GameAPI gameAPI;
        TeraToma::Assets::Assets assets;
        TeraToma::UI::UIManager uiManager;
    };
}
#endif