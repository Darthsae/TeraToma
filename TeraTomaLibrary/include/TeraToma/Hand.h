#pragma once
#ifndef HAND_H
#define HAND_H
#include <string>
#include <vector>

namespace TeraToma {
    class GameAPI;
    class Hand;
}

#include <TeraToma/CardInstance.h>

namespace TeraToma {
    /// @class Hand
    /// @brief Ytv
    class Hand {
    public:
        /// @brief 
        Hand();
        /// @brief 
        /// @param  
        /// @param  
        Hand(GameAPI*, std::vector<std::string>);

        /// @brief 
        std::vector<size_t> resolutionOrder;
        /// @brief 
        std::vector<CardInstance> cards;
        /// @brief 
        size_t cardCount;

        /// @brief 
        /// @param  
        void Resolve(GameAPI*);
    };
}
#endif