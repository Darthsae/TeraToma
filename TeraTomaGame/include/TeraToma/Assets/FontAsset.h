#pragma once
#ifndef FONT_ASSET_H
#define FONT_ASSET_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <TeraToma/Assets/Asset.h>
#include <stdint.h>

namespace TeraToma {
    namespace Assets {
        class FontAsset: public Asset {
        public:
            TTF_Font* font;
            float ptsize;

            FontAsset(std::string, std::string, float);
            void Load(SDL_Renderer*, std::string) override;
            void Unload() override;
        };
    }
}
#endif