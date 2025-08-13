#pragma once
#ifndef TEXTURE_ASSET_H
#define TEXTURE_ASSET_H
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <TeraToma/Assets/Asset.h>
#include <stdint.h>

namespace TeraToma {
    namespace Assets {
        class TextureAsset: public Asset {
        public:
            float w, h;
            SDL_Texture* texture;

            TextureAsset(std::string, std::string);
            void Load(SDL_Renderer*, std::string) override;
            void Unload() override;
        };
    }
}
#endif