#pragma once
#ifndef ASSET_H
#define ASSET_H
#include <SDL3/SDL.h>
#include <string>

namespace TeraToma {
    namespace Assets {
        class Asset {
        public:
            std::string id;
            std::string name;

            virtual void Load(SDL_Renderer*, std::string) = NULL;
            virtual void Unload() = NULL;
        };
    }
}
#endif