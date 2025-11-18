#pragma once
#ifndef CARD_TYPE_H
#define CARD_TYPE_H
#include <string>
#include <vector>
#include <functional>
#include <TeraToma/FnptrTypes.h>

namespace TeraToma {
    class GameAPI;
    enum struct CardAllegiance;
    
    /// @brief 
    class CardType {
    public:
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        CardType(std::string_view, std::string_view, CardAllegiance, bool);

        /// @brief 
        std::string name;
        /// @brief 
        std::string description;
        /// @brief 
        CardAllegiance allegiance;
        /// @brief 
        bool canWin;

        /// @brief 
        CardIndexedActionFnptr_t onResolve = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onPostResolve = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onLyingPostResolve = NULL;
        /// @brief 
        CardIndexedPredicateFnptr_t canFlip = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onFlip = NULL;
        /// @brief 
        CardIndexedPredicateFnptr_t canKill = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onKill = NULL;
        /// @brief 
        CardIndexedPredicateFnptr_t canActivate = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onActivate = NULL;
        /// @brief 
        CardIndexedPredicateFnptr_t canWinBase = NULL;
        /// @brief 
        CardIndexedPredicateFnptr_t canWinPost = NULL;

        /// @brief 
        static const CardType EMPTY;

        ~CardType();
    };
}
#endif