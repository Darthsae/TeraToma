#pragma once
#ifndef UI_SCREEN_H
#define UI_SCREEN_H
#include <string>
#include <unordered_map>
#include <TeraToma/UI/UILayer.h>
#include <TeraToma/GameAPI.h>
#include <TeraToma/Assets/Assets.h>

namespace TeraToma::UI {
    /// @brief 
    class UIManager {
    public:
        /// @brief 
        UIManager();

        /// @brief 
        std::unordered_map<std::string, UILayer> uiLayers;
        /// @brief 
        bool dirtyRecalculate = true;

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
    };
}
#endif