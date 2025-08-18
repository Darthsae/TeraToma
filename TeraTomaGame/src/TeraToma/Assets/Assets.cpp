#include <TeraToma/Assets/Assets.h>
#include <print>

namespace TeraToma::Assets {
    Assets::Assets() {
        textures = std::unordered_map<std::string, std::shared_ptr<TextureAsset>>();
        fonts = std::unordered_map<std::string, std::shared_ptr<FontAsset>>();
        sounds = std::unordered_map<std::string, std::shared_ptr<AudioAsset>>();
    }   

    void Assets::AddTexture(SDL_Renderer* a_renderer, std::string a_path, std::string a_name) {
        std::println("Texture: {} {}", a_path, a_name);
        std::shared_ptr<TextureAsset> textureAsset = std::make_shared<TextureAsset>(a_name, a_name);
        textureAsset->Load(a_renderer, a_path);
        textures.emplace(std::piecewise_construct,
            std::forward_as_tuple(a_name), 
            std::forward_as_tuple(std::move(textureAsset)));
    }

    void Assets::AddFont(std::string a_path, std::string a_name, float a_pt) {
        std::println("Font: {} {} {}", a_path, a_name, a_pt);
        std::shared_ptr<FontAsset> fontAsset = std::make_shared<FontAsset>(a_name, a_name, a_pt);
        fontAsset->Load(a_path);
        fonts.emplace(std::piecewise_construct,
            std::forward_as_tuple(a_name), 
            std::forward_as_tuple(std::move(fontAsset)));
    }

    void Assets::AddSound(SDL_AudioDeviceID a_audioDevice, std::string a_path, std::string a_name) {
        std::println("Audio: {} {}", a_path, a_name);
        std::shared_ptr<AudioAsset> audioAsset = std::make_shared<AudioAsset>(a_name, a_name); 
        audioAsset->Load(a_audioDevice, a_path);
        sounds.emplace(std::piecewise_construct,
            std::forward_as_tuple(a_name), 
            std::forward_as_tuple(std::move(audioAsset)));
    }

    void Assets::Uninitialize() {
        for (std::pair<const std::string, std::shared_ptr<TextureAsset>>& pair : textures) {
            pair.second->Unload();
        }

        textures.clear();

        for (std::pair<const std::string, std::shared_ptr<FontAsset>>& pair : fonts) {
            pair.second->Unload();
        }

        fonts.clear();

        for (std::pair<const std::string, std::shared_ptr<AudioAsset>>& pair : sounds) {
            pair.second->Unload();
        }

        sounds.clear();
    }
}