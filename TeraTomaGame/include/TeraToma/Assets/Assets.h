#pragma once
#ifndef ASSETS_H
#define ASSETS_H
#include <string>
#include <unordered_map>
#include <memory>
#include <TeraToma/Assets/AudioAsset.h>
#include <TeraToma/Assets/FontAsset.h>
#include <TeraToma/Assets/TextureAsset.h>

namespace TeraToma {
    namespace UI {
        class UIManager;
    }

    namespace Assets {
        /// @brief 
        class Assets {
        public:
            /// @brief 
            Assets();

            /// @brief 
            std::unordered_map<std::string, std::shared_ptr<FontAsset>> fonts;
            /// @brief 
            std::unordered_map<std::string, std::shared_ptr<AudioAsset>> sounds;
            /// @brief 
            std::unordered_map<std::string, std::shared_ptr<TextureAsset>> textures;
            /// @brief 
            TeraToma::UI::UIManager* uiManager;

            /// @brief 
            /// @param  
            /// @param  
            /// @param  
            void AddTexture(SDL_Renderer*, std::string, std::string);
            /// @brief 
            /// @param  
            /// @param  
            /// @param  
            void AddFont(std::string, std::string, float);
            /// @brief 
            /// @param  
            /// @param  
            /// @param  
            void AddSound(SDL_AudioDeviceID, std::string, std::string);
            /// @brief 
            void Uninitialize();
        };
    }
}
#endif