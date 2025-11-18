#include <TeraToma/Hand.h>
#include <TeraToma/GameAPI.h>
#include <print>

namespace TeraToma {
    Hand::Hand() {

    }

    Hand::Hand(GameAPI* a_gameAPI, const std::vector<std::string>& a_cardTypes) {
        cardCount = a_cardTypes.size();
        cards = std::vector<CardInstance>();
        for (size_t i = 0; i < cardCount; i++) {
            cards.emplace_back(a_cardTypes.at(i), i);
        }
        resolutionOrder = std::vector<size_t>();
    }

    void Hand::Resolve(GameAPI* a_gameAPI) {
        for (size_t i : resolutionOrder) {
            CardType cardType = a_gameAPI->cardTypes.at(cards.at(i).names.back());
            if (cardType.onResolve != NULL) {
                cardType.onResolve(a_gameAPI, i);
            }
        }

        for (size_t i : resolutionOrder) {
            CardType cardType = a_gameAPI->cardTypes.at(cards.at(i).names.back());
            if (cardType.onPostResolve != NULL) {
                cardType.onPostResolve(a_gameAPI, i);
            }
        }

        std::vector<size_t> toRemove = std::vector<size_t>();
        for (size_t i : resolutionOrder) {
            if (cards.at(i).statuses.size() == 0) {
                continue;
            }

            for (size_t j = 0; j < cards.at(i).statuses.size(); ++j) {
                StatusType statusType = a_gameAPI->statusTypes.at(cards.at(i).statuses.at(j));
                if (!statusType.canApplyToCard || statusType.canApplyToCard(a_gameAPI, i)) {
                    if (statusType.applyToCard) {
                        statusType.applyToCard(a_gameAPI, i);
                    }
                } else {
                    toRemove.emplace_back(j);
                }
            }

            if (toRemove.size() != 0) {
                for (size_t j = toRemove.size() - 1; j >= 0; --j) {
                    cards.at(i).statuses.erase(cards.at(i).statuses.begin() + toRemove.at(j));
                }
                toRemove.clear();
            }
        }

        for (CardInstance& cardInstance : cards) {
            cardInstance.UpdateDisplay(a_gameAPI);
        }
    }

    void Hand::Clear(void) {
        cardCount = 0;
        cards.clear();
        resolutionOrder = std::vector<size_t>();
    }
}