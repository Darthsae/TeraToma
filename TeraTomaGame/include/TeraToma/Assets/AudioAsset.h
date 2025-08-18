#pragma once
#ifndef AUDIO_ASSET_H
#define AUDIO_ASSET_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <TeraToma/Assets/Asset.h>
#include <stdint.h>

namespace TeraToma {
    namespace Assets {
        /// @brief 
        class AudioAsset: public Asset {
        public:
            /// @brief 
            /// @param  
            /// @param  
            AudioAsset(std::string, std::string);

            /// @brief 
            SDL_AudioSpec wavSpec;
            /// @brief 
            uint8_t* wavStart;
            /// @brief 
            uint32_t wavLength;
            /// @brief 
            SDL_AudioStream *stream;

            /// @brief 
            /// @param  
            /// @param  
            void Load(SDL_AudioDeviceID, std::string);
            /// @brief 
            /// @param  
            void Play(SDL_AudioDeviceID);
            /// @brief 
            void Unload() override;
        };
    }
}
#endif