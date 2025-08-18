#pragma once
#ifndef STATUS_H
#define STATUS_H
#include <string>
#include <functional>
#include <TeraToma/FnptrTypes.h>

namespace TeraToma {
    class GameAPI;
    
    /// @class StatusType
    /// @brief 
    class StatusType {
    public:
        /// @brief 
        /// @param[in] a_name The name of the status type.
        /// @param[in] a_description The description of the status type.
        StatusType(std::string, std::string);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        StatusType(std::string, std::string, const std::vector<std::string>&);

        /// @brief 
        std::string name;
        /// @brief 
        std::string description;
        /// @brief 
        std::vector<std::string> tags;

        /// @brief 
        CardIndexedPredicateFnptr_t canApplyToCard = NULL;
        /// @brief 
        CardIndexedActionFnptr_t applyToCard = NULL;
        /// @brief 
        CardIndexedPredicateFnptr_t canFlip = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onPreFlip = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onPostFlip = NULL;
        /// @brief 
        CardIndexedPredicateFnptr_t canKill = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onPreKill = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onPostKill = NULL;
        /// @brief 
        CardIndexedPredicateFnptr_t canActivate = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onPreActivate = NULL;
        /// @brief 
        CardIndexedActionFnptr_t onPostActivate = NULL;
        /// @brief 
        CardTypedPredicateFnptr_t canPreWinBase = NULL;
        /// @brief 
        CardTypedPredicateFnptr_t canPostWinBase = NULL;
        /// @brief 
        CardTypedPredicateFnptr_t canPreWinPost = NULL;
        /// @brief 
        CardTypedPredicateFnptr_t canPostWinPost = NULL;
        /// @brief 
        CardIndexedPredicateFnptr_t lies = NULL;

        ~StatusType();
    };
}
#endif