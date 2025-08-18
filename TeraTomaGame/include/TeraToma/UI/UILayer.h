#pragma once
#ifndef UI_LAYER_H
#define UI_LAYER_H
#include <string>
#include <unordered_map>
#include <TeraToma/UI/UIElement.h>

namespace TeraToma::UI {
    /// @brief 
    class UILayer {
    public:
        /// @brief 
        /// @param  
        UILayer(std::string_view);

        /// @brief 
        std::string id;
        /// @brief 
        bool enabled;
        /// @brief 
        std::unordered_map<std::string, UIElement> uiElements;

        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        void Render(SDL_Renderer*, GameAPI*, Assets::Assets*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseLeftDown(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseLeftUp(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseMiddleDown(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseMiddleUp(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseRightDown(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        bool OnMouseRightUp(SDL_Renderer*, GameAPI*, Assets::Assets*, SDL_FPoint*);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        void Recalculate(SDL_Renderer*, GameAPI*, Assets::Assets*);
        /// @brief 
        void Clear();
    };
}
#endif