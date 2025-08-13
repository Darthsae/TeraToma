#pragma once
#ifndef UI_LAYER_H
#define UI_LAYER_H
#include <string>
#include <unordered_map>
#include <TeraToma/UI/UIElement.h>

namespace TeraToma::UI {
    class UILayer {
    public:
        UILayer(std::string_view);

        std::string id;
        bool enabled;
        std::unordered_map<std::string, UIElement> uiElements;

        void Render(SDL_Renderer*, GameAPI*, Assets::Assets*);
        bool OnMouseLeftDown(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseLeftUp(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseMiddleDown(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseMiddleUp(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseRightDown(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        bool OnMouseRightUp(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        void Recalculate(SDL_Renderer*, GameAPI*, Assets::Assets*);

        //void AddElement(std::string_view, UIElement);
    };
}
#endif