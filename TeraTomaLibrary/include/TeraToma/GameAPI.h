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
        std::function<bool(GameAPI*, size_t)> validCardSelectionFnptr = NULL;
        std::function<void(GameAPI*, size_t)> selectedFnptr = NULL;
        std::function<void(GameAPI*, size_t)> postSelectedFnptr = NULL;
        std::vector<size_t> selected;
        size_t selecting;
        size_t maxSelect;
        int32_t health;
        int32_t healthSoftCap;

        std::function<void(std::string_view, CardType*)> postLoadCardFnptr = NULL;
        std::function<void(GameAPI*)> postWinFnptr = NULL;
        std::function<void(GameAPI*, int32_t)> postHurtFnptr = NULL;

        CardType* LoadCard(std::string_view, std::string_view, std::string_view, CardAllegiance, bool);
        void HandleDeath(size_t);
        void Select(size_t);
        void Hurt(int32_t);
    };
}
#endif