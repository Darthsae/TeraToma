#pragma once
#ifndef ASSET_H
#define ASSET_H
#include <SDL3/SDL.h>
#include <string>

namespace TeraToma {
    namespace Assets {
        /// @brief The asset base class.
        class Asset {
        public:
            /// @brief The id of the .
            std::string id;
            /// @brief The name of the .
            std::string name;

            /// @brief 
            virtual void Unload() = 0;
        };
    }
}
#endif