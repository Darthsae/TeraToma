#include <TeraToma/Deck.h>
#include <Random.h>

namespace TeraToma {
    Deck::Deck() {
        cards = std::vector<std::string>();
    }

    void Deck::AddCard(std::string a_card) {
        cards.emplace_back(a_card);
    }

    std::vector<std::string> Deck::Shuffle(size_t a_handSize) {
        size_t cardNumber = std::min(a_handSize, cards.size());
        std::vector<std::string> handCards = std::vector<std::string>();
        std::vector<std::string> usable(cards);

        while (handCards.size() < cardNumber) {
            uint32_t cardChoice = RandomUniformUInt32(0, (uint32_t)usable.size());
            handCards.emplace_back(usable[cardChoice]);
            usable.erase(usable.begin() + cardChoice);
        }

        return handCards;
    }

    void Deck::Clear(void) {
        cards.clear();
    }
}