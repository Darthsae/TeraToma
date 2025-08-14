#pragma once
#ifndef CARD_TYPE_H
#define CARD_TYPE_H
#include <string>
#include <vector>
#include <functional>

namespace TeraToma {
    class GameAPI;
    enum struct CardAllegiance;

    typedef std::function<std::string(GameAPI*, size_t)> ResponseFnptr_t;
    typedef std::function<void(GameAPI*, size_t)> CardActionFnptr_t;
    typedef std::function<bool(GameAPI*, size_t)> CardPredicateFnptr_t;
    
    class CardType {
    public:
        CardType(std::string_view, std::string_view, CardAllegiance, bool);

        // Information of the card.
        std::string name;
        std::string description;
        CardAllegiance allegiance;
        bool canWin;

        // Functions of the card.
        CardPredicateFnptr_t canFlip = NULL;
        CardActionFnptr_t onFlip = NULL;
        CardPredicateFnptr_t canKill = NULL;
        CardActionFnptr_t onKill = NULL;
        CardPredicateFnptr_t canActivate = NULL;
        CardActionFnptr_t onActivate = NULL;
        CardPredicateFnptr_t canWinBase = NULL;
        CardPredicateFnptr_t canWinPost = NULL;

        static const CardType EMPTY;
    };
}
#endif