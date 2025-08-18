#pragma once
#ifndef CARD_INSTANCE_H
#define CARD_INSTANCE_H
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace TeraToma {
    class GameAPI;
    class CardType;

    /// @brief 
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

    /// @brief 
    const std::unordered_map<CardAllegiance, std::string> AllegianceStrings = {
        {CardAllegiance::VILLAGE, "Village"},
        {CardAllegiance::NEUTRAL, "Neutral"},
        {CardAllegiance::OUTCAST, "Outcast"},
        {CardAllegiance::OUTSIDE, "Outside"},
        {CardAllegiance::UNDEATH, "Undeath"},
        {CardAllegiance::MONSTER, "Monster"},
        {CardAllegiance::DEMONIC, "Demonic"},
        {CardAllegiance::DIVINES, "Divines"}
    };

    /// @brief 
    const std::unordered_map<CardAllegiance, std::string> AllegianceDisplayStrings = {
        {CardAllegiance::VILLAGE, "Villager"},
        {CardAllegiance::NEUTRAL, "Neutral"},
        {CardAllegiance::OUTCAST, "Outcast"},
        {CardAllegiance::OUTSIDE, "Outsider"},
        {CardAllegiance::UNDEATH, "Undead"},
        {CardAllegiance::MONSTER, "Monster"},
        {CardAllegiance::DEMONIC, "Demon"},
        {CardAllegiance::DIVINES, "Divine"}
    };

    /// @brief 
    class CardInstance {
    public:
        /// @brief 
        CardInstance();
        /// @brief 
        /// @param  
        /// @param  
        CardInstance(std::string, size_t);

        /// @brief 
        std::string displayName;
        /// @brief 
        std::vector<std::string> names;
        /// @brief 
        std::vector<std::string> statuses;
        /// @brief 
        size_t index;
        /// @brief 
        bool flipped = false;
        /// @brief 
        bool dead = false;
        /// @brief 
        bool activated = false;
        /// @brief 
        bool lies = false;
        /// @brief 
        bool canWin = false;

        /// @brief 
        std::function<void(GameAPI*, CardInstance*, std::string_view)> onRespond = NULL;

        /// @brief 
        /// @param  
        /// @param  
        void Respond(GameAPI*, std::string_view);
        /// @brief 
        /// @param  
        void UpdateDisplay(GameAPI*);
        /// @brief 
        /// @param  
        /// @param  
        void CanWinBase(GameAPI*, const CardType&);
        /// @brief 
        /// @param  
        /// @param  
        void CanWinPost(GameAPI*, const CardType&);

        ~CardInstance();
    };
}
#endif