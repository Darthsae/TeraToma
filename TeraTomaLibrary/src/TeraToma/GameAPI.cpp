#include <TeraToma/GameAPI.h>
#include <utility>
#include <tuple>

namespace TeraToma {
    GameAPI::GameAPI() {
        cardTypes = std::unordered_map<std::string, CardType>();
        statusTypes = std::unordered_map<std::string, StatusType>();
        mods = std::unordered_map<std::string, Mod>();
        deck = Deck();
        hand = Hand(this, std::vector<std::string>());
        selected = std::vector<size_t>();
        selecting = 0;
        maxSelect = 0;
        healthSoftCap = 10;
        health = healthSoftCap;
    }

    CardType* GameAPI::LoadCard(std::string_view a_mod, std::string_view a_name, std::string_view a_description, CardAllegiance a_allegiance, bool a_canWin) {
        std::unordered_map<std::string, CardType>::iterator iterator;
        bool success;
        
        // I just hate using auto and type inference.
        std::tie(iterator, success) = this->cardTypes.try_emplace(std::string(a_name), a_name, a_description, a_allegiance, a_canWin);
        
        if (success) {
            CardType* card = &(iterator->second);
            if (postLoadCardFnptr) {
                postLoadCardFnptr(a_mod, card);
            }
            return card;
        }

        return nullptr;
    }   

    void GameAPI::HandleDeath(size_t a_index) {
        bool canWin = true;
        CardType* cardType = nullptr;
        size_t index_thing = 0;
        for (CardInstance& card : hand.cards) {
            if (!card.dead) {
                cardType = &cardTypes.at(card.names.back());
                card.canWin = cardType->canWin;
                if (cardType->canWinBase) {
                    card.canWin = cardType->canWinBase(this, index_thing);
                }
            }
            ++index_thing;
        }

        index_thing = 0;
        for (CardInstance& card : hand.cards) {
            if (!card.dead) {
                cardType = &cardTypes.at(card.names.back());
                if (cardType->canWinPost) {
                    card.canWin = cardType->canWinPost(this, index_thing);
                }
            }
            ++index_thing;
        }

        for (CardInstance& card : hand.cards) {
            if (!card.dead) {
                canWin = canWin && card.canWin;
            }
        }

        if (canWin && postWinFnptr) {
            postWinFnptr(this);
        }
    }
    
    void GameAPI::Select(size_t a_index) {
        if (std::find(selected.begin(), selected.end(), a_index) == selected.end() && validCardSelectionFnptr && validCardSelectionFnptr(this, a_index)) {
            selected.push_back(a_index);
            
            if (maxSelect >= selected.size()) {
                if (selectedFnptr) {
                    selectedFnptr(this, selecting);
                    selectedFnptr = nullptr;
                }

                gameMouseState = GameMouseState::FLIPPING;

                selected.clear();
                maxSelect = 0;

                if (postSelectedFnptr) {
                    postSelectedFnptr(this, selecting);
                    postSelectedFnptr = nullptr;
                }
            }
        }
    }

    void GameAPI::Hurt(int32_t a_healthAmount) {
        health -= a_healthAmount;

        if (postHurtFnptr) {
            postHurtFnptr(this, a_healthAmount);
        }
    }
}
