#include <TeraToma/Assets/Assets.h>

namespace TeraToma::Assets {
    Assets::Assets() {
        textures = std::unordered_map<std::string, TextureAsset>();
        fonts = std::unordered_map<std::string, FontAsset>();
    }   

    void Assets::AddTexture(std::string_view a_name, TextureAsset a_texture) {
        textures.emplace(a_name, a_texture);
    }

    void Assets::AddFont(std::string_view a_name, FontAsset a_font) {
        fonts.emplace(a_name, a_font);
    }

    void Assets::AddSound(std::string_view a_name, AudioAsset a_audio) {
        sounds.emplace(a_name, a_audio);
    }

    void Assets::Uninitialize() {
        for (std::pair<const std::string, TextureAsset>& pair : textures) {
            pair.second.Unload();
        }

        textures.clear();

        for (std::pair<const std::string, FontAsset>& pair : fonts) {
            pair.second.Unload();
        }

        fonts.clear();

        for (std::pair<const std::string, AudioAsset>& pair : sounds) {
            pair.second.Unload();
        }

        sounds.clear();
    }
}