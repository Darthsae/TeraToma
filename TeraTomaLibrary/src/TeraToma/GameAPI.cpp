#include <TeraToma/GameAPI.h>
#include <TeraToma/Mod.h>
#include <Random.h>
#include <utility>
#include <tuple>
#include <print>
#include <filesystem>
#include <iostream>
#include <fstream>

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

    void GameAPI::LoadMods(void) {
        for (std::filesystem::directory_entry const& dirEntry : std::filesystem::directory_iterator(std::filesystem::current_path().append("Mods"))) {
            if (dirEntry.is_regular_file() && dirEntry.path().extension().string() == ".dll") {
                std::println("{} {} {}", dirEntry.path().stem().string(), dirEntry.path().extension().string(), dirEntry.path().string());
                mods.emplace(std::piecewise_construct, std::forward_as_tuple(dirEntry.path().stem().string()), std::forward_as_tuple(dirEntry.path().stem().string(), dirEntry.path().wstring()));
            }
        }
    }
    
    void GameAPI::DoModLoading(void) {
        for (std::pair<const std::string, TeraToma::Mod>& pair : mods) {
            pair.second.Load(this);
        }
    }

    void GameAPI::DoModInitialization(void) {
        for (std::pair<const std::string, TeraToma::Mod>& pair : mods) {
            pair.second.Initialize(this);
        }
    }

    void GameAPI::DoModUninitialization(void) {
        for (std::pair<const std::string, TeraToma::Mod>& pair : mods) {
            pair.second.Uninitialize(this);
        }
    }

    void GameAPI::DoModUnloading(void) {
        for (std::pair<const std::string, TeraToma::Mod>& pair : mods) {
            pair.second.Unload(this);
        }
    }

    void GameAPI::DealStartingHand(void) {
        std::vector<std::string> normal = std::vector<std::string>();
        std::vector<std::string> unique = std::vector<std::string>();
        std::vector<std::string> normalOpposition = std::vector<std::string>();
        std::vector<std::string> uniqueOpposition = std::vector<std::string>();

        for (std::pair<const std::string, TeraToma::CardType>& pair : cardTypes) {
            //std::println("{}", pair.first);
            switch (pair.second.allegiance) {
                case TeraToma::CardAllegiance::VILLAGE:
                case TeraToma::CardAllegiance::NEUTRAL:
                case TeraToma::CardAllegiance::MONSTER:
                    if (pair.second.canWin) {
                        normal.emplace_back(pair.first);
                    } else {
                        normalOpposition.emplace_back(pair.first);
                    }
                    break;
                case TeraToma::CardAllegiance::OUTCAST:
                case TeraToma::CardAllegiance::OUTSIDE:
                case TeraToma::CardAllegiance::UNDEATH:
                case TeraToma::CardAllegiance::DEMONIC:
                case TeraToma::CardAllegiance::DIVINES:
                    if (pair.second.canWin) {
                        unique.emplace_back(pair.first);
                    } else {
                        uniqueOpposition.emplace_back(pair.first);
                    }
                    break;
            }
        }

        uint32_t option;
        size_t count;
        count = std::min<size_t>(3, normal.size());
        for (size_t i = 0; i < count; ++i) {
            option = RandomUniformUInt32(0, (uint32_t)normal.size());
            deck.AddCard(normal[option]);
            normal.erase(normal.begin() + option);
        }

        count = std::min<size_t>(1, unique.size());
        for (size_t i = 0; i < count; ++i) {
            option = RandomUniformUInt32(0, (uint32_t)unique.size());
            deck.AddCard(unique[option]);
            unique.erase(unique.begin() + option);
        }

        count = std::min<size_t>(1, normalOpposition.size());
        for (size_t i = 0; i < count; ++i) {
            option = RandomUniformUInt32(0, (uint32_t)normalOpposition.size());
            deck.AddCard(normalOpposition[option]);
            normalOpposition.erase(normalOpposition.begin() + option);
        }
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

    StatusType* GameAPI::LoadStatus(std::string_view a_mod, std::string a_name, std::string a_description) {
        std::unordered_map<std::string, StatusType>::iterator iterator;
        bool success;
        
        // I just hate using auto and type inference.
        std::tie(iterator, success) = this->statusTypes.try_emplace(std::string(a_name), a_name, a_description);
        
        if (success) {
            StatusType* status = &(iterator->second);
            return status;
        }

        return nullptr;
    }

    StatusType* GameAPI::LoadStatus(std::string_view a_mod, std::string a_name, std::string a_description, const std::vector<std::string>& a_tags) {
        std::unordered_map<std::string, StatusType>::iterator iterator;
        bool success;
        
        // I just hate using auto and type inference.
        std::tie(iterator, success) = this->statusTypes.try_emplace(std::string(a_name), a_name, a_description, a_tags);
        
        if (success) {
            StatusType* status = &(iterator->second);
            return status;
        }

        return nullptr;
    }

    void GameAPI::HandleDeath(size_t a_index) {
        bool canWin = true;
        CardType* cardType = nullptr;
        size_t index_thing = 0;
        for (CardInstance& card : hand.cards) {
            if (!card.dead) {
                card.CanWinBase(this, cardTypes.at(card.names.back()));
            }
            ++index_thing;
        }

        index_thing = 0;
        for (CardInstance& card : hand.cards) {
            if (!card.dead) {
                card.CanWinPost(this, cardTypes.at(card.names.back()));
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
        //std::println("Outside If: a_index {} selected size {}", a_index, selected.size());
        if ((selected.size() == 0 || std::find(selected.begin(), selected.end(), a_index) == selected.end()) && validCardSelectionFnptr && validCardSelectionFnptr(this, a_index)) {
            //std::println("Before Push Back: a_index {} selected size {}", a_index, selected.size());
            selected.push_back(a_index);
            //std::println("After Push Back: a_index {} selected size {}", a_index, selected.size());
            std::println("{} <= {} == {}", maxSelect, selected.size(), maxSelect <= selected.size());
            if (maxSelect <= selected.size()) {
                //std::println("Inside Max: a_index {} selected size {}", a_index, selected.size());
                if (selectedFnptr) {
                    //std::println("Inside SelectedFnptr: a_index {} selected size {}", a_index, selected.size());
                    selectedFnptr(this, selecting);
                    selectedFnptr = NULL;
                }

                gameMouseState = GameMouseState::FLIPPING;

                selected.clear();
                maxSelect = 0;

                //std::println("Before PostSelectedFnptr: a_index {} selected size {}", a_index, selected.size());
                if (postSelectedFnptr) {
                    //std::println("Inside PostSelectedFnptr: a_index {} selected size {}", a_index, selected.size());
                    postSelectedFnptr(this, selecting);
                    postSelectedFnptr = NULL;
                }
            }
        }
    }

    void GameAPI::ClearSelection() {
        hand.cards[selecting].activated = false;
        gameMouseState = GameMouseState::FLIPPING;
        selectedFnptr = NULL;
        selected.clear();
        maxSelect = 0;
        postSelectedFnptr = NULL;
    }

    void GameAPI::Hurt(int32_t a_healthAmount) {
        health -= a_healthAmount;

        if (postHurtFnptr) {
            postHurtFnptr(this, a_healthAmount);
        }
    }

    void GameAPI::TryKill(size_t a_index) {
        CardInstance* cardInstance = &hand.cards[a_index];
        if (!cardInstance->dead && (!cardTypes.at(cardInstance->names.back()).canKill || cardTypes.at(cardInstance->names.back()).canKill(this, a_index))) {
            
            cardInstance->dead = true;
            if (cardTypes.at(cardInstance->names.back()).onKill) {
                cardTypes.at(cardInstance->names.back()).onKill(this, a_index);
            }
            HandleDeath(a_index);
            if (postKillFnptr) {
                postKillFnptr(this, a_index);
            }
        }
    }

    void GameAPI::Destroy() {
        validCardSelectionFnptr = NULL;
        selectedFnptr = NULL;
        postSelectedFnptr = NULL;
        postLoadCardFnptr = NULL;
        postWinFnptr = NULL;
        postHurtFnptr = NULL;
        postKillFnptr = NULL;
        cardTypes.clear();
        statusTypes.clear();
        mods.clear();
    }
}
