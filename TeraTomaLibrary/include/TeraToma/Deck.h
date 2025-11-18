#pragma once
#ifndef DECK_H
#define DECK_H
#include <string>
#include <vector>

namespace TeraToma {
    /// @brief 
    class Deck {
    public:
        /// @brief 
        Deck();

        /// @brief 
        std::vector<std::string> cards;

        /// @brief 
        /// @param  
        void AddCard(std::string);

        /// @brief 
        /// @param  
        /// @return 
        std::vector<std::string> Shuffle(size_t);
        
        /// @brief 
        void Clear(void);
    };
}
#endif