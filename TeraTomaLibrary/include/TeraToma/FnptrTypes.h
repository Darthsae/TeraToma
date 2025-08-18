#pragma once
#ifndef FNPTR_TYPES_H
#define FNPTR_TYPES_H
#include <functional>
#include <stdint.h>
#include <string>

namespace TeraToma {
    class GameAPI;
    class CardInstance;
    class CardType;

    /// @brief 
    typedef std::function<std::string(GameAPI*, size_t)> CardResponseFnptr_t;
    /// @brief 
    typedef std::function<void(GameAPI*, size_t)> CardIndexedActionFnptr_t;
    /// @brief 
    typedef std::function<bool(GameAPI*, size_t)> CardIndexedPredicateFnptr_t;
    /// @brief 
    typedef std::function<void(GameAPI*, CardInstance*)> CardInstancedActionFnptr_t;
    /// @brief 
    typedef std::function<bool(GameAPI*, CardInstance*)> CardInstancedPredicateFnptr_t;
    /// @brief 
    typedef std::function<void(GameAPI*, CardInstance*, const CardType&)> CardTypedActionFnptr_t;
    /// @brief 
    typedef std::function<bool(GameAPI*, CardInstance*, const CardType&)> CardTypedPredicateFnptr_t;
}
#endif