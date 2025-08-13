#pragma once
#ifndef UI_TEXT_COMPONENT_H
#define UI_TEXT_COMPONENT_H
#include <TeraToma/UI/UIElementComponent.h>

namespace TeraToma::UI {
    class UITextComponent : public UIElementComponent {
    public:
        UITextComponent(std::string);

        std::string text;
        SDL_Texture* textTexture;

        void Hookup(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*) override;
        void SetText(SDL_Renderer*, Assets::Assets*, UIElement*, std::string);
    };
}
#endif