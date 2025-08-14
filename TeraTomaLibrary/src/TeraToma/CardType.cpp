#include <TeraToma/CardType.h>
#include <TeraToma/GameAPI.h>

namespace TeraToma {
    CardType::CardType(std::string_view a_name, std::string_view a_description, CardAllegiance a_allegiance, bool a_canWin) {
        name = a_name;
        description = a_description;
        allegiance = a_allegiance;
        canWin = a_canWin;
    }

    const CardType CardType::EMPTY = CardType("", "", CardAllegiance::NEUTRAL, true);
}