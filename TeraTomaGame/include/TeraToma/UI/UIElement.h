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
    /// @brief 
    typedef std::function<void(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*)> UIEventFnptr;
    /// @brief 
    typedef std::function<bool(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*, SDL_FPoint*)> UIMousePredicateFnptr;

    /// @brief 
    class UIElement {
    public:
        /// @brief 
        /// @param  
        /// @param  
        UIElement(std::string_view, UIRect);

        /// @brief The enablement state of the UI Element.
        bool enabled;
        /// @brief The ID of the UI Element.
        std::string id;
        /// @brief 
        std::unordered_map<std::string, std::shared_ptr<UIElementComponent>> components;

        /// @brief 
        std::vector<UIEventFnptr> onRender;
        /// @brief 
        std::vector<UIMousePredicateFnptr> onMouseLeftDown;
        /// @brief 
        std::vector<UIMousePredicateFnptr> onMouseLeftUp;
        /// @brief 
        std::vector<UIMousePredicateFnptr> onMouseMiddleDown;
        /// @brief 
        std::vector<UIMousePredicateFnptr> onMouseMiddleUp;
        /// @brief 
        std::vector<UIMousePredicateFnptr> onMouseRightDown;
        /// @brief 
        std::vector<UIMousePredicateFnptr> onMouseRightUp;
        /// @brief 
        std::vector<UIEventFnptr> onRecalculate;

        /// @brief The UIRect for the display area.
        UIRect displayArea;
        /// @brief The UIRect for the internal area on a 1920x1080 screen.
        UIRect internalArea;

        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        void Render(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseLeftDown(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseLeftUp(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseMiddleDown(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseMiddleUp(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseRightDown(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseRightUp(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        void Recalculate(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*);
    };
}
#endif