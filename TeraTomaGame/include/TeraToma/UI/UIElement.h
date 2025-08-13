#pragma once
#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

namespace TeraToma::UI {
    class UIElement;
}

#include <TeraToma/UI/UIElementComponent.h>
#include <TeraToma/UI/UIRect.h>

namespace TeraToma::UI {
    typedef std::function<void(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*)> UIEventFnptr;
    typedef std::function<bool(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*, SDL_FPoint*)> UIMousePredicateFnptr;

    class UIElement {
    public:
        UIElement(std::string_view, UIRect);

        bool enabled;
        std::string id;
        std::unordered_map<std::string, std::shared_ptr<UIElementComponent>> components;

        std::vector<UIEventFnptr> onRender;
        std::vector<UIMousePredicateFnptr> onMouseLeftDown;
        std::vector<UIMousePredicateFnptr> onMouseLeftUp;
        std::vector<UIMousePredicateFnptr> onMouseMiddleDown;
        std::vector<UIMousePredicateFnptr> onMouseMiddleUp;
        std::vector<UIMousePredicateFnptr> onMouseRightDown;
        std::vector<UIMousePredicateFnptr> onMouseRightUp;
        std::vector<UIEventFnptr> onRecalculate;

        UIRect displayArea;
        UIRect internalArea;

        void Render(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*);
        bool OnMouseLeftDown(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        bool OnMouseLeftUp(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        bool OnMouseMiddleDown(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        bool OnMouseMiddleUp(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        bool OnMouseRightDown(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        bool OnMouseRightUp(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        void Recalculate(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*);
    };
}
#endif