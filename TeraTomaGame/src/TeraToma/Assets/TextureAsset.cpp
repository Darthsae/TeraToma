#include <TeraToma/Assets/Assets.h>
#include <iostream>

using namespace TeraToma::Assets;

TextureAsset::TextureAsset(std::string a_id, std::string a_name) {
    this->id = a_id;
    this->name = a_name;
}

void TeraToma::Assets::TextureAsset::Load(SDL_Renderer* a_renderer, std::string a_path) {
    SDL_Surface* surface = IMG_Load(a_path.c_str());
    if (surface == nullptr) {
        std::cout << a_path << std::endl;
        std::cout << "Error loading image: " << SDL_GetError() << std::endl;
    }

    this->texture = SDL_CreateTextureFromSurface(a_renderer, surface);
    SDL_DestroySurface(surface);
    if (this->texture == nullptr) {
        std::cout << "Error creating texture: " << SDL_GetError() << std::endl;
    }

    SDL_SetTextureScaleMode(this->texture, SDL_SCALEMODE_NEAREST);

    if (!SDL_GetTextureSize(this->texture, &this->w, &this->h)) {
        std::cout << "Error getting size: " << SDL_GetError() << std::endl;
    }
}

void TeraToma::Assets::TextureAsset::Unload() {
    SDL_DestroyTexture(this->texture);
}