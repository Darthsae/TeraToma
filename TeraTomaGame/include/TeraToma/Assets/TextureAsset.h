#pragma once
#ifndef TEXTURE_ASSET_H
#define TEXTURE_ASSET_H
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <TeraToma/Assets/Asset.h>
#include <stdint.h>

namespace TeraToma {
    namespace Assets {
        /// @brief 
        class TextureAsset: public Asset {
        public:
            /// @brief 
            /// @param  
            /// @param  
            TextureAsset(std::string, std::string);

            /// @brief 
            float w;
            /// @brief 
            float h;
            /// @brief 
            SDL_Texture* texture;

            /// @brief 
            /// @param  
            /// @param  
            void Load(SDL_Renderer*, std::string);
            /// @brief 
            void Unload() override;
        };
    }
}
#endif