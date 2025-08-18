#pragma once
#ifndef GAME_API_H
#define GAME_API_H
#include <string>
#include <unordered_map>
#include <functional>

namespace TeraToma {
    /// @brief 
    enum struct GameState {
        MAIN_MENU,
        MODS_MENU,
        SETTINGS,
        PAUSE_MENU,
        GAME,
        GAME_OVER
    };

    /// @brief 
    enum struct GamePlayState {
        REWARDS,
        DRAWING,
        ORDERING,
        PLAYING
    };

    /// @brief 
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
    /// @brief 
    class GameAPI {
    public:
        /// @brief 
        GameAPI();

        /// @brief 
        std::unordered_map<std::string, CardType> cardTypes;
        /// @brief 
        std::unordered_map<std::string, StatusType> statusTypes;
        /// @brief 
        std::unordered_map<std::string, Mod> mods;
        /// @brief 
        Deck deck;
        /// @brief 
        Hand hand;
        /// @brief 
        GameState gameState = GameState::MAIN_MENU;
        /// @brief 
        GamePlayState gamePlayState = GamePlayState::DRAWING;
        /// @brief 
        GameMouseState gameMouseState = GameMouseState::FLIPPING;
        /// @brief 
        std::function<bool(GameAPI*, size_t)> validCardSelectionFnptr = NULL;
        /// @brief 
        std::function<void(GameAPI*, size_t)> selectedFnptr = NULL;
        /// @brief 
        std::function<void(GameAPI*, size_t)> postSelectedFnptr = NULL;
        /// @brief 
        std::vector<size_t> selected;
        /// @brief 
        size_t selecting;
        /// @brief 
        size_t maxSelect;
        /// @brief 
        int32_t health;
        /// @brief 
        int32_t healthSoftCap;

        /// @brief 
        uint64_t blindness = 0;

        /// @brief 
        std::function<void(std::string_view, CardType*)> postLoadCardFnptr = NULL;
        /// @brief 
        std::function<void(GameAPI*)> postWinFnptr = NULL;
        /// @brief 
        std::function<void(GameAPI*, int32_t)> postHurtFnptr = NULL;
        /// @brief 
        std::function<void(GameAPI*, size_t)> postKillFnptr = NULL;

        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        CardType* LoadCard(std::string_view, std::string_view, std::string_view, CardAllegiance, bool);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        StatusType* LoadStatus(std::string_view, std::string, std::string);
        /// @brief 
        /// @param  
        /// @param  
        /// @param  
        /// @param  
        /// @return 
        StatusType* LoadStatus(std::string_view, std::string, std::string, const std::vector<std::string>&);
        /// @brief 
        /// @param  
        void HandleDeath(size_t);
        /// @brief 
        /// @param  
        void Select(size_t);
        /// @brief 
        void ClearSelection(void);
        /// @brief 
        /// @param  
        void Hurt(int32_t);
        /// @brief
        /// @param
        void TryKill(size_t);
        /// @brief 
        void Destroy(void);
    };
}
#endif