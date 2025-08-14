#pragma once
#ifndef AUDIO_ASSET_H
#define AUDIO_ASSET_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <TeraToma/Assets/Asset.h>
#include <stdint.h>

namespace TeraToma {
    namespace Assets {
        class AudioAsset: public Asset {
        public:
            SDL_AudioSpec wavSpec;
            uint8_t* wavStart;
            uint32_t wavLength;
            SDL_AudioStream *stream;

            AudioAsset(std::string, std::string);
            void Load(SDL_AudioDeviceID, std::string);
            void Play(SDL_AudioDeviceID);
            void Unload() override;
        };
    }
}
#endif