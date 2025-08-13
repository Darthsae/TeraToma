#pragma once
#ifndef UI_TEXT_COMPONENT_H
#define UI_TEXT_COMPONENT_H
#include <TeraToma/UI/UIElementComponent.h>

namespace TeraToma::UI {
    class UITextComponent : public UIElementComponent {
    public:
        ///
        void Hookup(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*) override;
    };
}
#endif