#pragma once
#ifndef DECK_H
#define DECK_H
#include <string>
#include <vector>

namespace TeraToma {
    class Deck {
    public:
        Deck();

        std::vector<std::string> cards;

        void AddCard(std::string);
        std::vector<std::string> Shuffle(size_t);
    };
}
#endif