#pragma once
#ifndef UI_ELEMENT_COMPONENT_H
#define UI_ELEMENT_COMPONENT_H
#include <string>
#include <TeraToma/GameAPI.h>
#include <TeraToma/Assets/Assets.h>

namespace TeraToma::UI {
    class UIElement;
    class UILayer;

    class UIElementComponent {
    public:
        ///
        std::string id;
        std::string elementID;

        virtual void Hookup(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*) = NULL;
    };
}
#endif