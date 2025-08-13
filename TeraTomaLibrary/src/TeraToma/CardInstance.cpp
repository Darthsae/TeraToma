#include <TeraToma/CardInstance.h>
#include <print>

namespace TeraToma {
    CardInstance::CardInstance() {

    }
    
    CardInstance::CardInstance(std::string a_cardType, size_t a_index) {
        //std::println("{} {}", a_cardType, a_index);
        names = std::vector<std::string>();
        names.emplace_back(a_cardType);
        statuses = std::vector<std::string>();
        index = a_index;
    }

    void CardInstance::UpdateDisplay(GameAPI* a_gameAPI) {
        displayName = names.back();
    }
}