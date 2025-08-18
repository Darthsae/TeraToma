#pragma once
#ifndef UI_IMAGE_COMPONENT_H
#define UI_IMAGE_COMPONENT_H
#include <string>
#include <unordered_map>
#include <TeraToma/UI/UIElementComponent.h>

namespace TeraToma::UI {
    /// @brief 
    class UIImageComponent : public UIElementComponent {
    public:
        /// @brief 
        /// @param  
        /// @param  
        UIImageComponent(std::string_view, bool);

        /// @brief 
        std::string texture;
        /// @brief 
        bool nineSliced;

        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        void Hookup(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*) override;
    };
}
#endif