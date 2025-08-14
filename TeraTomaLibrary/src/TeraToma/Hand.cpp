#include <TeraToma/Hand.h>
#include <TeraToma/GameAPI.h>
#include <print>

namespace TeraToma {
    Hand::Hand() {

    }

    Hand::Hand(GameAPI* a_gameAPI, std::vector<std::string> a_cardTypes) {
        cardCount = a_cardTypes.size();
        cards = std::vector<CardInstance>();
        for (size_t i = 0; i < cardCount; i++) {
            cards.emplace_back(a_cardTypes.at(i), i);
        }
        resolutionOrder = std::vector<size_t>();
    }

    void Hand::Resolve(GameAPI* a_gameAPI) {
        for (size_t i : resolutionOrder) {
            //std::println("i {} Name {}", i, cards.at(i).names.at(0));
            CardType cardType = a_gameAPI->cardTypes.at(cards.at(i).names.at(0));
            if (cardType.onActivate != NULL) {
                cardType.onActivate(a_gameAPI, i);
            }
        }

        for (CardInstance& cardInstance : cards) {
            cardInstance.UpdateDisplay(a_gameAPI);
        }
    }
}