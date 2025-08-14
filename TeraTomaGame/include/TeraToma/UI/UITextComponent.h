#pragma once
#ifndef UI_TEXT_COMPONENT_H
#define UI_TEXT_COMPONENT_H
#include <TeraToma/UI/UIElementComponent.h>
#include <TeraToma/UI/UIRect.h>

namespace TeraToma::UI {
    class UITextComponent : public UIElementComponent {
    public:
        UITextComponent(std::string, int, UIAnchor);

        std::string text;
        SDL_Texture* textTexture;
        UIAnchor textAnchor;
        UIRect textArea;

        int w, h, margins;

        void Hookup(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*) override;
        void SetText(SDL_Renderer*, Assets::Assets*, UIElement*, std::string);
        void SetAnchor(SDL_Renderer*, Assets::Assets*, UIElement*, UIAnchor);
    };
}
#endif