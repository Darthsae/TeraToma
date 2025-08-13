#pragma once
#ifndef UI_SCREEN_H
#define UI_SCREEN_H
#include <string>
#include <unordered_map>
#include <TeraToma/UI/UILayer.h>
#include <TeraToma/GameAPI.h>
#include <TeraToma/Assets/Assets.h>

namespace TeraToma::UI {
    class UIManager {
    public:
        UIManager();

        std::unordered_map<std::string, UILayer> uiLayers;

        void Render(SDL_Renderer*, GameAPI*, Assets::Assets*);
        bool OnMouseLeftDown(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseLeftUp(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseMiddleDown(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseMiddleUp(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseRightDown(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseRightUp(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        void Recalculate(SDL_Renderer*, GameAPI*, Assets::Assets*);
    };
}
#endif