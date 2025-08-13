#pragma once
#ifndef UI_IMAGE_COMPONENT_H
#define UI_IMAGE_COMPONENT_H
#include <string>
#include <unordered_map>
#include <TeraToma/UI/UIElementComponent.h>

namespace TeraToma::UI {
    class UIImageComponent : public UIElementComponent {
    public:
        UIImageComponent(std::string_view, bool);

        std::string texture;
        bool nineSliced;

        void Hookup(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*) override;
    };
}
#endif