#pragma once
#ifndef CARD_INSTANCE_H
#define CARD_INSTANCE_H
#include <string>
#include <vector>

namespace TeraToma {
    class GameAPI;

    enum struct CardAllegiance {
        VILLAGE,
        NEUTRAL,
        OUTCAST,
        OUTSIDE,
        UNDEATH,
        MONSTER,
        DEMONIC,
        DIVINES,
        SIZE_OF
    };

    class CardInstance {
    public:
        CardInstance();
        CardInstance(std::string, size_t);

        std::string displayName;
        std::vector<std::string> names;
        std::vector<std::string> statuses;
        size_t index;
        bool flipped = false;
        bool dead = false;
        bool lies;

        void UpdateDisplay(GameAPI*);
    };
}
#endif