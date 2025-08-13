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
    class Hand {
    public:
        Hand();
        Hand(GameAPI*, std::vector<std::string>);

        std::vector<size_t> resolutionOrder;
        std::vector<CardInstance> cards;
        size_t cardCount;

        void Resolve(GameAPI*);
    };
}
#endif