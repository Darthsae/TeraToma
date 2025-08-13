#pragma once
#ifndef CARD_TYPE_H
#define CARD_TYPE_H
#include <string>
#include <vector>

namespace TeraToma {

    class CardType;
}

#include <TeraToma/GameAPI.h>

namespace TeraToma {
    typedef std::string (*ResponseFnptr_t)(GameAPI*);
    typedef void (*CardActionFnptr_t)(GameAPI*);
    
    class CardType {
    public:
        CardType(std::string_view, std::string_view, CardAllegiance);

        // Information of the card.
        std::string name;
        std::string description;
        CardAllegiance allegiance;

        // Functions of the card.
        CardActionFnptr_t onFlip = nullptr;
        CardActionFnptr_t onKill = nullptr;
        CardActionFnptr_t onActivate = nullptr;

        static const CardType EMPTY;
    };
}
#endif