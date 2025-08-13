#pragma once
#ifndef ASSETS_H
#define ASSETS_H
#include <string>
#include <unordered_map>
#include <TeraToma/Assets/FontAsset.h>
#include <TeraToma/Assets/TextureAsset.h>

namespace TeraToma::Assets {
    class Assets {
    public:
        Assets();

        void AddTexture(std::string_view, TextureAsset);
        void AddFont(std::string_view, FontAsset);
        void Uninitialize();

        std::unordered_map<std::string, FontAsset> fonts;
        std::unordered_map<std::string, TextureAsset> textures;
    };
}
#endif