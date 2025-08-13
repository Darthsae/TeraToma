#pragma once
#ifndef GAME_API_H
#define GAME_API_H
#include <string>
#include <unordered_map>
#include <functional>

namespace TeraToma {
    enum struct GameState {
        MAIN_MENU,
        MODS_MENU,
        SETTINGS,
        PAUSE_MENU,
        GAME,
        GAME_OVER
    };

    enum struct GamePlayState {
        REWARDS,
        DRAWING,
        ORDERING,
        PLAYING
    };

    enum struct GameMouseState {
        FLIPPING,
        KILLING,
        SELECTING
    };

    class GameAPI;
}

#include <TeraToma/Mod.h>
#include <TeraToma/Hand.h>
#include <TeraToma/Deck.h>
#include <TeraToma/Status.h>
#include <TeraToma/CardType.h>

namespace TeraToma {
    class GameAPI {
    public:
        GameAPI();

        std::unordered_map<std::string, CardType> cardTypes;
        std::unordered_map<std::string, StatusType> statusTypes;
        std::unordered_map<std::string, Mod> mods;
        Deck deck;
        Hand hand;
        GameState gameState = GameState::MAIN_MENU;
        GamePlayState gamePlayState = GamePlayState::DRAWING;
        GameMouseState gameMouseState = GameMouseState::FLIPPING;

        std::function<void(std::string_view, CardType*)> postLoadCardFnptr = nullptr;

        CardType* LoadCard(std::string_view, std::string_view, std::string_view, CardAllegiance);
    };
}
#endif