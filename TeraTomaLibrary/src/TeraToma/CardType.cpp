#include <TeraToma/CardType.h>

namespace TeraToma {
    CardType::CardType(std::string_view a_name, std::string_view a_description, CardAllegiance a_allegiance) {
        name = a_name;
        description = description;
        allegiance = a_allegiance;
    }

    const CardType CardType::EMPTY = CardType("", "", CardAllegiance::NEUTRAL);
}