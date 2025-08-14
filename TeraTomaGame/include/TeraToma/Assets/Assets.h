#pragma once
#ifndef ASSETS_H
#define ASSETS_H
#include <string>
#include <unordered_map>
#include <TeraToma/Assets/AudioAsset.h>
#include <TeraToma/Assets/FontAsset.h>
#include <TeraToma/Assets/TextureAsset.h>

namespace TeraToma {
    namespace UI {
        class UIManager;
    }

    namespace Assets {
        class Assets {
        public:
            Assets();

            std::unordered_map<std::string, FontAsset> fonts;
            std::unordered_map<std::string, AudioAsset> sounds;
            std::unordered_map<std::string, TextureAsset> textures;
            TeraToma::UI::UIManager* uiManager;

            void AddTexture(std::string_view, TextureAsset);
            void AddFont(std::string_view, FontAsset);
            void AddSound(std::string_view, AudioAsset);
            void Uninitialize();
        };
    }
}
#endif