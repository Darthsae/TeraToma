#pragma once
#ifndef UI_TEXT_COMPONENT_H
#define UI_TEXT_COMPONENT_H
#include <TeraToma/UI/UIElementComponent.h>
#include <TeraToma/UI/UIRect.h>

namespace TeraToma::UI {
    /// @brief 
    class UITextComponent : public UIElementComponent {
    public:
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        UITextComponent(std::string, std::string, int, UIAnchor);

        /// @brief 
        std::string fontName = "NotoSansMono-Regular-12";
        /// @brief 
        std::string text;
        /// @brief 
        SDL_Texture* textTexture;
        /// @brief 
        UIAnchor textAnchor;
        /// @brief 
        UIRect textArea;

        /// @brief 
        int w;
        /// @brief 
        int h;
        /// @brief 
        int margins;

        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        void Hookup(SDL_Renderer*, GameAPI*, Assets::Assets*, UILayer*, UIElement*) override;
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        void SetText(SDL_Renderer*, Assets::Assets*, UIElement*, std::string);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        void SetAnchor(SDL_Renderer*, Assets::Assets*, UIElement*, UIAnchor);
    };
}
#endif