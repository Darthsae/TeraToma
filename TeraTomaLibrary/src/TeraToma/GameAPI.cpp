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
    }

    CardType* GameAPI::LoadCard(std::string_view a_mod, std::string_view a_name, std::string_view a_description, CardAllegiance a_allegiance) {
        std::unordered_map<std::string, CardType>::iterator iterator;
        bool success;
        
        // I just hate using auto and type inference.
        std::tie(iterator, success) = this->cardTypes.try_emplace(std::string(a_name), a_name, a_description, a_allegiance);
        
        if (success) {
            CardType* card = &(iterator->second);
            if (postLoadCardFnptr != nullptr) {
                postLoadCardFnptr(a_mod, card);
            }
            return card;
        }

        return nullptr;
    }   
}
