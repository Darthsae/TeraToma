#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_audio.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <TeraToma/GameAPI.h>
#include <TeraToma/Constants.h>
#include <TeraToma/Assets/Assets.h>
#include <TeraToma/Assets/AudioAsset.h>
#include <TeraToma/Assets/FontAsset.h>
#include <TeraToma/Assets/TextureAsset.h>
#include <TeraToma/UI/UIManager.h>
#include <TeraToma/UI/UILayer.h>
#include <TeraToma/UI/UIElement.h>
#include <TeraToma/UI/UITextComponent.h>
#include <TeraToma/UI/UIImageComponent.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <print>
#include <utility>
#include <tuple>
#include <numbers>
#include <memory>
#include <random>
#include <ctime>
#include <Random.h>

constexpr double TWO_PI = 2.0 * std::numbers::pi;
constexpr double ONE_HALF_PI = 0.5 * std::numbers::pi;
constexpr double ONE_FOURTH_PI =  0.25 * std::numbers::pi;
constexpr double ONE_EIGTH_PI = 0.125 * std::numbers::pi;
constexpr double ONE_SIXTEENTH_PI = 0.0625 * std::numbers::pi;

std::shared_ptr<TeraToma::Assets::TextureAsset> cardFront;
std::shared_ptr<TeraToma::Assets::TextureAsset> cardUsed;
std::shared_ptr<TeraToma::Assets::TextureAsset> cardBack;
std::shared_ptr<TeraToma::Assets::TextureAsset> cardBackOverlay;
std::shared_ptr<TeraToma::Assets::TextureAsset> cardSlash;
std::shared_ptr<TeraToma::Assets::TextureAsset> cardSlashUsed;

std::shared_ptr<TeraToma::Assets::AudioAsset> killSFX;
std::shared_ptr<TeraToma::Assets::AudioAsset> cardFlipSFX;

void DrawCardType(SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::CardType* a_card, SDL_FRect* a_cardLocation) {
    SDL_RenderTexture(a_renderer, cardFront->texture, NULL, a_cardLocation);
    SDL_RenderTexture(a_renderer, a_assets->textures.at(a_card->name)->texture, NULL, a_cardLocation);
}

void DrawCardInstance(SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::CardInstance* a_card, SDL_FRect* a_cardLocation) {
    if (a_card->flipped) {
        DrawCardType(a_renderer, a_gameAPI, a_assets, &a_gameAPI->cardTypes.at(a_card->dead ? a_card->names.back() : a_card->displayName), a_cardLocation);
        // Statuses drawn next.
        // Now if activated.
        if (a_card->activated) {
            SDL_RenderTexture(a_renderer, cardUsed->texture, NULL, a_cardLocation);
        }
    } else {
        SDL_RenderTexture(a_renderer, cardBack->texture, NULL, a_cardLocation);
        SDL_RenderTexture(a_renderer, cardBackOverlay->texture, NULL, a_cardLocation);
    }

    if (a_card->dead) {
        SDL_RenderTexture(a_renderer, (a_card->activated) ? cardSlashUsed->texture : cardSlash->texture, NULL, a_cardLocation);
    }
}

void DrawGameBackground(SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets) {
    SDL_SetRenderDrawColor(a_renderer, 15, 25, 15, SDL_ALPHA_OPAQUE);
}

int main(int argc, char const **) {
    uint32_t setSeed = static_cast<uint32_t>(time(0));
    srand(setSeed);
    std::println("Seed {}", setSeed);
    float cardScale = 2; // 2.5
    float cardWidth = 38 * cardScale;
    float cardHeight = 56 * cardScale;
    float cardHalfWidth = cardWidth * 0.5f;
    float cardHalfHeight = cardHeight * 0.5f;
    float cardLargeScale = 4; // 2.5
    float cardLargeWidth = 38 * cardLargeScale;
    float cardLargeHeight = 56 * cardLargeScale;
    float cardLargeHalfWidth = cardLargeWidth * 0.5f;
    float cardLargeHalfHeight = cardLargeHeight * 0.5f;
    int wonk = (int)(TeraToma::WINDOW_WIDTH / cardWidth);
    int largeCardAmount = (int)(TeraToma::WINDOW_WIDTH / cardLargeWidth);
    SDL_FRect rectToUse = {0, 0, cardLargeWidth, cardLargeHeight};
    std::vector<std::string> cardChoices = std::vector<std::string>();

    SDL_SetAppMetadata(TeraToma::APPLICATION_TITLE, "0.1.0.0", "com.darthsae.teratoma");
    // SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, "Darthsae");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "game");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, "https://github.com/Darthsae/TeraToma");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
    }

    if (!TTF_Init()) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    if (!SDL_CreateWindowAndRenderer(TeraToma::APPLICATION_TITLE, TeraToma::WINDOW_WIDTH, TeraToma::WINDOW_HEIGHT, SDL_WINDOW_VULKAN, &window, &renderer)) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
    }

    SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    
    std::println("Pre API");
    TeraToma::GameAPI gameAPI = TeraToma::GameAPI();
    std::println("Pre Assets");
    TeraToma::Assets::Assets assets = TeraToma::Assets::Assets();
    std::println("Pre UI Manager");
    TeraToma::UI::UIManager uiManager = TeraToma::UI::UIManager();
    assets.uiManager = &uiManager;

    #pragma region Loading Mods and Assets
    std::println("Pre Mods");
    std::println("Mods {}", std::filesystem::current_path().append("Mods").string());
    gameAPI.LoadMods();

    std::println("Pre Images");
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::recursive_directory_iterator(std::filesystem::current_path().append("Assets"))) {
        if (dir_entry.path().has_extension()) {
            if (dir_entry.path().extension().string() == ".png") {
                assets.AddTexture(renderer, dir_entry.path().string(), dir_entry.path().stem().string());
            } else if (dir_entry.path().extension().string() == ".ttf") {
                assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 12), 12);
                assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 16), 16);
                assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 18), 18);
                assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 20), 20);
                assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 24), 24);
                assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 30), 30);
                assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 36), 36);
            } else if (dir_entry.path().extension().string() == ".wav") {
                assets.AddSound(audioDevice, dir_entry.path().string(), dir_entry.path().stem().string());
            }
        }
    }
    #pragma endregion

    cardFront = assets.textures.at("Card Front");
    cardUsed = assets.textures.at("Card Used");
    cardBack = assets.textures.at("Card Back");
    cardBackOverlay = assets.textures.at("Back Overlay");
    cardSlash = assets.textures.at("Card Slashed");
    cardSlashUsed = assets.textures.at("Used Card Slashed");
    killSFX = assets.sounds.at("Kill");
    cardFlipSFX = assets.sounds.at("Card Flip");

    gameAPI.postKillFnptr = [&](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        killSFX->Play(audioDevice);
    };

    gameAPI.postLoadCardFnptr = [](std::string_view a_mod, TeraToma::CardType* a_cardType) {
        //std::println("Whack");
    };

    std::println("Pre Mod Load");
    gameAPI.DoModLoading();

    std::println("Pre Mod Initialize");
    gameAPI.DoModInitialization();

    std::vector<std::string> normal = std::vector<std::string>();
    std::vector<std::string> unique = std::vector<std::string>();
    std::vector<std::string> normalOpposition = std::vector<std::string>();
    std::vector<std::string> uniqueOpposition = std::vector<std::string>();

    std::println("Pre Cards into Deck");
    for (std::pair<const std::string, TeraToma::CardType>& pair : gameAPI.cardTypes) {
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
        gameAPI.deck.AddCard(normal[option]);
        normal.erase(normal.begin() + option);
    }

    count = std::min<size_t>(1, unique.size());
    for (size_t i = 0; i < count; ++i) {
        option = RandomUniformUInt32(0, (uint32_t)unique.size());
        gameAPI.deck.AddCard(unique[option]);
        unique.erase(unique.begin() + option);
    }

    count = std::min<size_t>(1, normalOpposition.size());
    for (size_t i = 0; i < count; ++i) {
        option = RandomUniformUInt32(0, (uint32_t)normalOpposition.size());
        gameAPI.deck.AddCard(normalOpposition[option]);
        normalOpposition.erase(normalOpposition.begin() + option);
    }
    
    std::println("Pre Card Rects");
    SDL_FRect* cardRects = (SDL_FRect*)malloc(5);
    SDL_FPoint point = {0, 0};
    uint8_t r, g, b, a;

    float bongo = (float)(TWO_PI / 1);

    #pragma region UI
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Main Menu"), 
        std::forward_as_tuple(std::string_view("Main Menu"))
    );
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Settings Menu"), 
        std::forward_as_tuple(std::string_view("Settings Menu"))
    );
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Game Play Menu"), 
        std::forward_as_tuple(std::string_view("Game Play Menu"))
    );
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Game Dia Menu"), 
        std::forward_as_tuple(std::string_view("Game Dia Menu"))
    );
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Game Ordering Menu"), 
        std::forward_as_tuple(std::string_view("Game Ordering Menu"))
    );
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Game Rewards Menu"), 
        std::forward_as_tuple(std::string_view("Game Rewards Menu"))
    );
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Pause Menu"), 
        std::forward_as_tuple(std::string_view("Pause Menu"))
    );

    TeraToma::UI::UIElement* element;

    TeraToma::UI::UILayer* gameRewardsMenu = &uiManager.uiLayers.at("Game Rewards Menu");
    gameRewardsMenu->enabled = false;
    gameRewardsMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texter"), 
        std::forward_as_tuple(
            std::string_view("Texter"), 
            TeraToma::UI::UIRect{{200, 660, 1520, 320}}
        )
    );
    element = &gameRewardsMenu->uiElements.at("Texter");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>(
                std::string_view("UI Panel"), 
                true
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, gameRewardsMenu, element);
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texta"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UITextComponent>(
                "Description",
                "NotoSansMono-Regular-16",
                6,
                TeraToma::UI::UIAnchor::TOP_CENTER
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, gameRewardsMenu, element);

    TeraToma::UI::UIElement* rewardsDescriptionElement = &gameRewardsMenu->uiElements.at("Texter");
    std::shared_ptr<TeraToma::UI::UITextComponent> rewardsDescriptionText = std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(rewardsDescriptionElement->components.at("Texta"));

    TeraToma::UI::UILayer* gameDiaMenu = &uiManager.uiLayers.at("Game Dia Menu");
    gameDiaMenu->enabled = false;

    TeraToma::UI::UILayer* mainMenu = &uiManager.uiLayers.at("Main Menu");
    mainMenu->enabled = true;
    
    TeraToma::UI::UILayer* gameOrderingMenu = &uiManager.uiLayers.at("Game Ordering Menu");
    gameOrderingMenu->enabled = false;

    mainMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Play"), 
        std::forward_as_tuple(
            std::string_view("Play"), 
            TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X - 100, 300, 200, 50}}
        )
    );
    element = &mainMenu->uiElements.at("Play");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>(
                std::string_view("UI Panel"), 
                true
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, mainMenu, element);
    element->onMouseLeftDown.emplace_back([&](SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::UI::UILayer* a_uiLayer, TeraToma::UI::UIElement* a_element, SDL_FPoint* a_mousePos) {
        a_uiLayer->enabled = false;
        a_gameAPI->gameState = TeraToma::GameState::GAME;
        a_gameAPI->hand = TeraToma::Hand(a_gameAPI, a_gameAPI->deck.Shuffle(5));
        a_gameAPI->gamePlayState = TeraToma::GamePlayState::ORDERING;
        gameOrderingMenu->enabled = true;
        cardRects = (SDL_FRect*)realloc(cardRects, sizeof(SDL_FRect) * a_gameAPI->hand.cardCount);
        bongo = (float)(TWO_PI / a_gameAPI->hand.cardCount);
        for (size_t i = 0; i < a_gameAPI->hand.cardCount; ++i) {
            float angle = (float)(i * bongo - ONE_HALF_PI);
            cardRects[i] = {TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angle) - cardHalfWidth, TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angle) - cardHalfHeight, cardWidth, cardHeight};
            gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                std::forward_as_tuple(std::format("card {} number", i)), 
                std::forward_as_tuple(
                    std::string_view(gameAPI.hand.cards[i].names.back()), 
                    TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X + 150.0f * std::cosf(angle) - 25, TeraToma::WINDOW_CENTER_Y + 150.0f * std::sinf(angle) - 25, 50, 50}}
                )
            );
            element = &gameDiaMenu->uiElements.at(std::format("card {} number", i));
            element->components.emplace(std::piecewise_construct, 
                std::forward_as_tuple("Texture"), 
                std::forward_as_tuple(
                    std::make_shared<TeraToma::UI::UIImageComponent>(
                        std::string_view("UI Panel"), 
                        true
                    )
                )
            ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
            element->components.emplace(std::piecewise_construct, 
                std::forward_as_tuple("Text"), 
                std::forward_as_tuple(
                    std::make_shared<TeraToma::UI::UITextComponent>(
                        std::format("{}", i),
                        "NotoSansMono-Regular-12",
                        6,
                        TeraToma::UI::UIAnchor::MIDDLE_CENTER
                    )
                )
            ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
            gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                std::forward_as_tuple(std::format("card {}", i)), 
                std::forward_as_tuple(
                    std::string_view(gameAPI.hand.cards[i].names.back()), 
                    TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X + 550.0f * std::cosf(angle) - 75, TeraToma::WINDOW_CENTER_Y + 450.0f * std::sinf(angle) - 75, 150, 150}}
                )
            );
            element = &gameDiaMenu->uiElements.at(std::format("card {}", i));
            element->enabled = false;
            element->components.emplace(std::piecewise_construct, 
                std::forward_as_tuple("Texture"), 
                std::forward_as_tuple(
                    std::make_shared<TeraToma::UI::UIImageComponent>(
                        std::string_view("UI Panel"), 
                        true
                    )
                )
            ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
            element->components.emplace(std::piecewise_construct, 
                std::forward_as_tuple("Text"), 
                std::forward_as_tuple(
                    std::make_shared<TeraToma::UI::UITextComponent>(
                        "10/10",
                        "NotoSansMono-Regular-12",
                        6,
                        TeraToma::UI::UIAnchor::MIDDLE_CENTER
                    )
                )
            ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
            a_gameAPI->hand.cards[i].onRespond = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance, std::string_view a_response) {
                gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = true;
                std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).components.at("Text"))->SetText(renderer, &assets, &gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)), std::string(a_response));
            };
        }  
        return true;
    });
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Text"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UITextComponent>(
                "Play",
                "NotoSansMono-Regular-24",
                6,
                TeraToma::UI::UIAnchor::MIDDLE_CENTER
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, mainMenu, element);

    TeraToma::UI::UILayer* settingsMenu = &uiManager.uiLayers.at("Settings Menu");
    settingsMenu->enabled = false;

    TeraToma::UI::UILayer* pauseMenu = &uiManager.uiLayers.at("Pause Menu");
    pauseMenu->enabled = false;

    TeraToma::UI::UILayer* gamePlayMenu = &uiManager.uiLayers.at("Game Play Menu");
    gamePlayMenu->enabled = false;
    gamePlayMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Kill"), 
        std::forward_as_tuple(
            std::string_view("Kill"), 
            TeraToma::UI::UIRect{{1856, 1016, 64, 64}}
        )
    );
    element = &gamePlayMenu->uiElements.at("Kill");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>(
                std::string_view("Kill Button"), 
                false
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, gamePlayMenu, element);
    element->onMouseLeftDown.emplace_back([](SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::UI::UILayer* a_uiLayer, TeraToma::UI::UIElement* a_element, SDL_FPoint* a_mousePos) {
        if (a_gameAPI->gameMouseState == TeraToma::GameMouseState::FLIPPING) {
            a_gameAPI->gameMouseState = TeraToma::GameMouseState::KILLING;
        } else if (a_gameAPI->gameMouseState == TeraToma::GameMouseState::KILLING) {
            a_gameAPI->gameMouseState = TeraToma::GameMouseState::FLIPPING;
        }
        return true;
    });
    
    gamePlayMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texter"), 
        std::forward_as_tuple(
            std::string_view("Texter"), 
            TeraToma::UI::UIRect{{1600, 0, 320, 400}}
        )
    );
    element = &gamePlayMenu->uiElements.at("Texter");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>( 
                std::string_view("UI Panel"), 
                true
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, gamePlayMenu, element);
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texta"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UITextComponent>(
                "Description",
                "NotoSansMono-Regular-16",
                6,
                TeraToma::UI::UIAnchor::TOP_CENTER
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, gamePlayMenu, element);

    TeraToma::UI::UIElement* textWonk = &gamePlayMenu->uiElements.at("Texter");
    std::shared_ptr<TeraToma::UI::UITextComponent> textWonker = std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(textWonk->components.at("Texta"));

    gamePlayMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Health"), 
        std::forward_as_tuple(
            std::string_view("Health"), 
            TeraToma::UI::UIRect{{0, 1032, 140, 48}}
        )
    );
    element = &gamePlayMenu->uiElements.at("Health");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>(
                std::string_view("UI Panel"), 
                true
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, gamePlayMenu, element);
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("HP Text"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UITextComponent>(
                "10/10",
                "NotoSansMono-Regular-36",
                6,
                TeraToma::UI::UIAnchor::MIDDLE_CENTER
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, gamePlayMenu, element);

    TeraToma::UI::UIElement* hpUIElement = &gamePlayMenu->uiElements.at("Health");
    std::shared_ptr<TeraToma::UI::UITextComponent> hpText = std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(hpUIElement->components.at("HP Text"));

    gameOrderingMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Instructions"), 
        std::forward_as_tuple(
            std::string_view("Instructions"), 
            TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X - 300, 0, 600, 48}}
        )
    );
    element = &gameOrderingMenu->uiElements.at("Instructions");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>(
                std::string_view("UI Panel"), 
                true
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, gamePlayMenu, element);
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Instruction Text"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UITextComponent>(
                "Select the order your cards will trigger.",
                "NotoSansMono-Regular-20",
                6,
                TeraToma::UI::UIAnchor::MIDDLE_CENTER
            )
        )
    ).first->second->Hookup(renderer, &gameAPI, &assets, gamePlayMenu, element);
    #pragma endregion
    
    uint64_t deltaTime;
    std::function<void(void)> drawGame = [&]() {
        DrawGameBackground(renderer, &gameAPI, &assets);
        switch (gameAPI.gamePlayState) {
            case TeraToma::GamePlayState::REWARDS:
                for (size_t i = 0; i < cardChoices.size(); ++i) {
                    DrawCardType(renderer, &gameAPI, &assets, &gameAPI.cardTypes.at(cardChoices[i]), &cardRects[i]);
                }
                break;
            case TeraToma::GamePlayState::DRAWING:
                break;
            case TeraToma::GamePlayState::ORDERING: {
                for (size_t i = 0; i < gameAPI.hand.cardCount; ++i) {
                    DrawCardInstance(renderer, &gameAPI, &assets, &gameAPI.hand.cards[i], &cardRects[i]);
                }
                if (gameAPI.hand.resolutionOrder.size() == 0) {
                    break;
                }
                SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
                SDL_SetRenderDrawColor(renderer, 255, 225, 225, SDL_ALPHA_OPAQUE);
                for (size_t i = 0; i < gameAPI.hand.resolutionOrder.size() - 1; ++i) {
                    float angleOne = (float)(gameAPI.hand.resolutionOrder[i] * bongo - ONE_HALF_PI);
                    float angleTwo = (float)(gameAPI.hand.resolutionOrder[i + 1] * bongo - ONE_HALF_PI);
                    SDL_RenderLine(renderer, TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angleOne), TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angleOne), TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angleTwo), TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angleTwo));
                }
                float angle = (float)(gameAPI.hand.resolutionOrder.back() * bongo - ONE_HALF_PI);
                SDL_RenderLine(renderer, TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angle), TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angle), point.x, point.y);
                SDL_SetRenderDrawColor(renderer, r, g, b, a);
                break;
            } case TeraToma::GamePlayState::PLAYING:
                for (size_t i = 0; i < gameAPI.hand.cardCount; ++i) {
                    DrawCardInstance(renderer, &gameAPI, &assets, &gameAPI.hand.cards[i], &cardRects[i]);
                }
                
                if (gameAPI.gameMouseState == TeraToma::GameMouseState::KILLING) {
                    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
                    SDL_SetRenderDrawColor(renderer, 225, 0, 0, 95);
                    SDL_RenderFillRect(renderer, NULL);
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                }

                if (gameAPI.gameMouseState == TeraToma::GameMouseState::SELECTING) {
                    if (gameAPI.selected.size() == 0) {
                        break;
                    }

                    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 75);
                    for (size_t i : gameAPI.selected) {
                        SDL_RenderFillRect(renderer, &cardRects[i]);
                    }
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                }
                break;
        }
    };

    if (uiManager.dirtyRecalculate) {
        uiManager.Recalculate(renderer, &gameAPI, &assets);
    }

    gameAPI.postWinFnptr = [&](TeraToma::GameAPI* a_gameAPI) {
        // Reward
        a_gameAPI->gamePlayState = TeraToma::GamePlayState::REWARDS;
        gameDiaMenu->Clear();
        gamePlayMenu->enabled = false;
        gameDiaMenu->enabled = false;
        gameRewardsMenu->enabled = true;
        cardChoices.clear();
        cardRects = (SDL_FRect*)realloc(cardRects, sizeof(SDL_FRect) * 5);
        std::vector<std::string> allCards = std::vector<std::string>();
        for (std::pair<const std::string, TeraToma::CardType>& pair : a_gameAPI->cardTypes) {
            allCards.emplace_back(pair.first);
        }

        for (size_t i = 0; i < 5; ++i) {
            uint32_t theIndex = RandomUniformUInt32(0, (uint32_t)allCards.size());
            cardChoices.emplace_back(allCards[theIndex]);
            allCards.erase(allCards.begin() + theIndex);
            cardRects[i] = {TeraToma::WINDOW_CENTER_X - (cardLargeWidth + 20) * 2 + i * (cardLargeWidth + 20) - cardLargeHalfWidth, TeraToma::WINDOW_CENTER_Y - cardLargeHalfHeight, cardLargeWidth, cardLargeHeight};
        }
    };

    gameAPI.postHurtFnptr = [&](TeraToma::GameAPI* a_gameAPI, int32_t a_hpChange) {
        hpText->SetText(renderer, &assets, hpUIElement, std::format("{}/{}", a_gameAPI->health, a_gameAPI->healthSoftCap));
    };

    bool running = true;
    SDL_Event event;
    uint64_t lastTime = SDL_GetTicks();
    while (running) {
        deltaTime = SDL_GetTicks() - lastTime;
        lastTime = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            SDL_GetMouseState(&point.x, &point.y);
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            if (!uiManager.OnMouseLeftDown(renderer, &gameAPI, &assets, &point)) {
                                switch (gameAPI.gameState) {
                                    case TeraToma::GameState::MAIN_MENU:
                                        break;
                                    case TeraToma::GameState::MODS_MENU:
                                        break;
                                    case TeraToma::GameState::SETTINGS:
                                        break;
                                    case TeraToma::GameState::PAUSE_MENU:
                                        break;
                                    case TeraToma::GameState::GAME:
                                        switch (gameAPI.gamePlayState) {
                                            case TeraToma::GamePlayState::REWARDS:
                                                for (size_t i = 0; i < cardChoices.size(); ++i) {
                                                    if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                        gameAPI.deck.AddCard(cardChoices[i]);
                                                        #pragma region Create Win
                                                        gameAPI.gameState = TeraToma::GameState::GAME;
                                                        gameAPI.hand = TeraToma::Hand(&gameAPI, gameAPI.deck.Shuffle(gameAPI.hand.cardCount + RandomUniformUInt32(0, 1)));
                                                        gameAPI.gamePlayState = TeraToma::GamePlayState::ORDERING;
                                                        gameOrderingMenu->enabled = true;
                                                        gameRewardsMenu->enabled = false;
                                                        cardRects = (SDL_FRect*)realloc(cardRects, sizeof(SDL_FRect) * gameAPI.hand.cardCount);
                                                        bongo = (float)(TWO_PI / gameAPI.hand.cardCount);
                                                        for (size_t i = 0; i < gameAPI.hand.cardCount; ++i) {
                                                            float angle = (float)(i * bongo - ONE_HALF_PI);
                                                            cardRects[i] = {TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angle) - cardHalfWidth, TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angle) - cardHalfHeight, cardWidth, cardHeight};
                                                            gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                                                                std::forward_as_tuple(std::format("card {} number", i)), 
                                                                std::forward_as_tuple(
                                                                    std::string_view(gameAPI.hand.cards[i].names.back()), 
                                                                    TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X + 150.0f * std::cosf(angle) - 25, TeraToma::WINDOW_CENTER_Y + 150.0f * std::sinf(angle) - 25, 50, 50}}
                                                                )
                                                            );
                                                            element = &gameDiaMenu->uiElements.at(std::format("card {} number", i));
                                                            element->components.emplace(std::piecewise_construct, 
                                                                std::forward_as_tuple("Texture"), 
                                                                std::forward_as_tuple(
                                                                    std::make_shared<TeraToma::UI::UIImageComponent>(
                                                                        std::string_view("UI Panel"), 
                                                                        true
                                                                    )
                                                                )
                                                            ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
                                                            element->components.emplace(std::piecewise_construct, 
                                                                std::forward_as_tuple("Text"), 
                                                                std::forward_as_tuple(
                                                                    std::make_shared<TeraToma::UI::UITextComponent>(
                                                                        std::format("{}", i),
                                                                        "NotoSansMono-Regular-12",
                                                                        6,
                                                                        TeraToma::UI::UIAnchor::MIDDLE_CENTER
                                                                    )
                                                                )
                                                            ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
                                                            gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                                                                std::forward_as_tuple(std::format("card {}", i)), 
                                                                std::forward_as_tuple(
                                                                    std::string_view(gameAPI.hand.cards[i].names.back()), 
                                                                    TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X + 550.0f * std::cosf(angle) - 75, TeraToma::WINDOW_CENTER_Y + 450.0f * std::sinf(angle) - 75, 150, 150}}
                                                                )
                                                            );
                                                            element = &gameDiaMenu->uiElements.at(std::format("card {}", i));
                                                            element->enabled = false;
                                                            element->components.emplace(std::piecewise_construct, 
                                                                std::forward_as_tuple("Texture"), 
                                                                std::forward_as_tuple(
                                                                    std::make_shared<TeraToma::UI::UIImageComponent>(
                                                                        std::string_view("UI Panel"), 
                                                                        true
                                                                    )
                                                                )
                                                            ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
                                                            element->components.emplace(std::piecewise_construct, 
                                                                std::forward_as_tuple("Text"), 
                                                                std::forward_as_tuple(
                                                                    std::make_shared<TeraToma::UI::UITextComponent>(
                                                                        "10/10",
                                                                        "NotoSansMono-Regular-12",
                                                                        6,
                                                                        TeraToma::UI::UIAnchor::MIDDLE_CENTER
                                                                    )
                                                                )
                                                            ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
                                                            gameAPI.hand.cards[i].onRespond = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance, std::string_view a_response) {
                                                                gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = true;
                                                                std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).components.at("Text"))->SetText(renderer, &assets, &gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)), std::string(a_response));
                                                            };
                                                        }  
                                                        gameAPI.health = gameAPI.healthSoftCap;
                                                        hpText->SetText(renderer, &assets, hpUIElement, std::format("{}/{}", gameAPI.health, gameAPI.healthSoftCap));
                                                        #pragma endregion
                                                    }
                                                }
                                                break;
                                            case TeraToma::GamePlayState::ORDERING:
                                                for (size_t i = 0; i < gameAPI.hand.cardCount; ++i) {
                                                    if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                        if (std::find(gameAPI.hand.resolutionOrder.begin(), gameAPI.hand.resolutionOrder.end(), i) == gameAPI.hand.resolutionOrder.end()) {
                                                            gameAPI.hand.resolutionOrder.emplace_back(i);
                                                            //std::println("Resolution {} Cards {} Equal {}", gameAPI.hand.resolutionOrder.size(), gameAPI.hand.cards.size(), gameAPI.hand.resolutionOrder.size() == gameAPI.hand.cards.size());
                                                            if (gameAPI.hand.resolutionOrder.size() == gameAPI.hand.cards.size()) {
                                                                //std::println("What");
                                                                gameAPI.hand.Resolve(&gameAPI);
                                                                gameAPI.gamePlayState = TeraToma::GamePlayState::PLAYING;
                                                                gameAPI.gameMouseState = TeraToma::GameMouseState::FLIPPING;
                                                                gamePlayMenu->enabled = true;
                                                                gameDiaMenu->enabled = true;
                                                                gameOrderingMenu->enabled = false;
                                                            }
                                                            break;
                                                        }
                                                    }
                                                }
                                                break;
                                            case TeraToma::GamePlayState::PLAYING:
                                                for (size_t i = 0; i < gameAPI.hand.cardCount; ++i) {
                                                    if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                        switch (gameAPI.gameMouseState) {
                                                            case TeraToma::GameMouseState::FLIPPING: {
                                                                TeraToma::CardInstance* cardInstance = &gameAPI.hand.cards[i];
                                                                TeraToma::CardType* cardType = &gameAPI.cardTypes.at(cardInstance->names.back());
                                                                if (!cardInstance->flipped && (!cardType->canFlip || cardType->canFlip(&gameAPI, i))) {
                                                                    cardFlipSFX->Play(audioDevice);
                                                                    cardInstance->flipped = true;
                                                                    if (cardType->onFlip) {
                                                                        cardType->onFlip(&gameAPI, i);
                                                                    }
                                                                } else if (!cardInstance->activated && (cardType->canActivate && cardType->canActivate(&gameAPI, i))) {
                                                                    cardInstance->activated = true;
                                                                    if (cardType->onActivate) {
                                                                        cardType->onActivate(&gameAPI, i);
                                                                    }
                                                                }
                                                                break;
                                                            } case TeraToma::GameMouseState::KILLING:
                                                                gameAPI.TryKill(i);
                                                                break;
                                                            case TeraToma::GameMouseState::SELECTING:
                                                                gameAPI.Select(i);
                                                                break;
                                                        }
                                                        break;
                                                    }
                                                }
                                                break;
                                        }
                                        break;
                                    case TeraToma::GameState::GAME_OVER:
                                        break;
                                    default:
                                        break;
                                }
                            }
                            break;
                        case SDL_BUTTON_MIDDLE:
                            if (!uiManager.OnMouseMiddleDown(renderer, &gameAPI, &assets, &point)) {
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            if (!uiManager.OnMouseRightDown(renderer, &gameAPI, &assets, &point)) {
                                switch (gameAPI.gameState) {
                                    case TeraToma::GameState::GAME:
                                        switch (gameAPI.gamePlayState) {
                                            case TeraToma::GamePlayState::PLAYING:
                                                switch (gameAPI.gameMouseState) {
                                                    case TeraToma::GameMouseState::FLIPPING:
                                                    case TeraToma::GameMouseState::KILLING:
                                                        for (size_t i = 0; i < gameAPI.hand.cardCount; ++i) {
                                                            if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                                if (gameAPI.hand.cards[i].flipped) {
                                                                    // Display Description
                                                                    //std::println("DisplayName: {}; Description: {}", gameAPI.hand.cards[i].displayName, gameAPI.cardTypes.at(gameAPI.hand.cards[i].displayName).description);
                                                                    
                                                                    for (size_t j = 0; j < gameAPI.hand.cards[i].statuses.size(); ++j) {
                                                                        std::println("{}: {}", j, gameAPI.hand.cards[i].statuses[j]);
                                                                    }

                                                                    textWonker->SetText(renderer, &assets, textWonk, std::format("{}\n{}\n{}", gameAPI.hand.cards[i].displayName, TeraToma::AllegianceDisplayStrings.at(gameAPI.cardTypes.at(gameAPI.hand.cards[i].displayName).allegiance), gameAPI.cardTypes.at(gameAPI.hand.cards[i].displayName).description));
                                                                }
                                                                break;
                                                            }
                                                        }
                                                        break;
                                                    case TeraToma::GameMouseState::SELECTING:
                                                        if (gameAPI.selected.size() > 0) {
                                                            gameAPI.selected.pop_back();
                                                        } else {
                                                            gameAPI.ClearSelection();
                                                        }
                                                        break;
                                                }
                                                break;
                                            case TeraToma::GamePlayState::ORDERING:
                                                if (gameAPI.hand.resolutionOrder.size() > 0) {
                                                    gameAPI.hand.resolutionOrder.pop_back();
                                                }
                                                break;
                                            case TeraToma::GamePlayState::REWARDS:
                                                for (size_t i = 0; i < cardChoices.size(); ++i) {
                                                    if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                        rewardsDescriptionText->SetText(renderer, &assets, rewardsDescriptionElement, std::format("{}\n{}\n{}", cardChoices[i], TeraToma::AllegianceDisplayStrings.at(gameAPI.cardTypes.at(cardChoices[i]).allegiance), gameAPI.cardTypes.at(cardChoices[i]).description));
                                                        break;
                                                    }
                                                }
                                                break;
                                        }
                                        break;
                                }
                            }
                            break;
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            if (!uiManager.OnMouseLeftUp(renderer, &gameAPI, &assets, &point)) {
                            }
                            break;
                        case SDL_BUTTON_MIDDLE:
                            if (!uiManager.OnMouseMiddleUp(renderer, &gameAPI, &assets, &point)) {
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            if (!uiManager.OnMouseRightUp(renderer, &gameAPI, &assets, &point)) {
                            }
                            break;
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.key) {
                        case SDLK_ESCAPE:
                            switch (gameAPI.gameState) {
                                case TeraToma::GameState::MODS_MENU:
                                case TeraToma::GameState::SETTINGS:
                                    gameAPI.gameState = TeraToma::GameState::MAIN_MENU;
                                    break;
                                case TeraToma::GameState::PAUSE_MENU:
                                    gameAPI.gameState = TeraToma::GameState::GAME;
                                    pauseMenu->enabled = false;
                                    switch (gameAPI.gamePlayState) {
                                        case TeraToma::GamePlayState::PLAYING:
                                            gamePlayMenu->enabled = true;
                                            gameDiaMenu->enabled = true;
                                            break;
                                        case TeraToma::GamePlayState::ORDERING:
                                            gameOrderingMenu->enabled = true;
                                            break;
                                        case TeraToma::GamePlayState::REWARDS:
                                            gameRewardsMenu->enabled = true;
                                            break;
                                    }
                                    break;
                                case TeraToma::GameState::GAME:
                                    gameAPI.gameState = TeraToma::GameState::PAUSE_MENU;
                                    pauseMenu->enabled = true;
                                    switch (gameAPI.gamePlayState) {
                                        case TeraToma::GamePlayState::PLAYING:
                                            gamePlayMenu->enabled = false;
                                            gameDiaMenu->enabled = false;
                                            break;
                                        case TeraToma::GamePlayState::ORDERING:
                                            gameOrderingMenu->enabled = false;
                                            break;
                                        case TeraToma::GamePlayState::REWARDS:
                                            gameRewardsMenu->enabled = false;
                                            break;
                                    }
                                    break;
                            }
                            break;
                    }
                    break;
            }
        }
        
        switch (gameAPI.gameState) {
            case TeraToma::GameState::MAIN_MENU:
                break;
            case TeraToma::GameState::MODS_MENU:
                break;
            case TeraToma::GameState::SETTINGS:
                break;
            case TeraToma::GameState::PAUSE_MENU:
                break;
            case TeraToma::GameState::GAME:
                switch (gameAPI.gamePlayState) {
                    case TeraToma::GamePlayState::REWARDS:
                        break;
                    case TeraToma::GamePlayState::DRAWING:
                        break;
                    case TeraToma::GamePlayState::ORDERING:
                        break;
                    case TeraToma::GamePlayState::PLAYING:
                        if (gameAPI.health <= 0) {
                            // Handle Death
                            gameAPI.gameState = TeraToma::GameState::GAME;
                            gameAPI.hand = TeraToma::Hand(&gameAPI, gameAPI.deck.Shuffle(5));
                            gameAPI.gamePlayState = TeraToma::GamePlayState::ORDERING;
                            gameDiaMenu->Clear();
                            gameOrderingMenu->enabled = true;
                            gamePlayMenu->enabled = false;
                            gameDiaMenu->enabled = false;
                            cardRects = (SDL_FRect*)realloc(cardRects, sizeof(SDL_FRect) * gameAPI.hand.cardCount);
                            bongo = (float)(TWO_PI / gameAPI.hand.cardCount);
                            for (size_t i = 0; i < gameAPI.hand.cardCount; ++i) {
                                float angle = (float)(i * bongo - ONE_HALF_PI);
                                cardRects[i] = {TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angle) - cardHalfWidth, TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angle) - cardHalfHeight, cardWidth, cardHeight};
                                gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                                    std::forward_as_tuple(std::format("card {} number", i)), 
                                    std::forward_as_tuple(
                                        std::string_view(gameAPI.hand.cards[i].names.back()), 
                                        TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X + 150.0f * std::cosf(angle) - 25, TeraToma::WINDOW_CENTER_Y + 150.0f * std::sinf(angle) - 25, 50, 50}}
                                    )
                                );
                                element = &gameDiaMenu->uiElements.at(std::format("card {} number", i));
                                element->components.emplace(std::piecewise_construct, 
                                    std::forward_as_tuple("Texture"), 
                                    std::forward_as_tuple(
                                        std::make_shared<TeraToma::UI::UIImageComponent>(
                                            std::string_view("UI Panel"), 
                                            true
                                        )
                                    )
                                ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
                                element->components.emplace(std::piecewise_construct, 
                                    std::forward_as_tuple("Text"), 
                                    std::forward_as_tuple(
                                        std::make_shared<TeraToma::UI::UITextComponent>(
                                            std::format("{}", i),
                                            "NotoSansMono-Regular-12",
                                            6,
                                            TeraToma::UI::UIAnchor::MIDDLE_CENTER
                                        )
                                    )
                                ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
                                gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                                    std::forward_as_tuple(std::format("card {}", i)), 
                                    std::forward_as_tuple(
                                        std::string_view(gameAPI.hand.cards[i].names.back()), 
                                        TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X + 550.0f * std::cosf(angle) - 75, TeraToma::WINDOW_CENTER_Y + 450.0f * std::sinf(angle) - 75, 150, 150}}
                                    )
                                );
                                element = &gameDiaMenu->uiElements.at(std::format("card {}", i));
                                element->enabled = false;
                                element->components.emplace(std::piecewise_construct, 
                                    std::forward_as_tuple("Texture"), 
                                    std::forward_as_tuple(
                                        std::make_shared<TeraToma::UI::UIImageComponent>(
                                            std::string_view("UI Panel"), 
                                            true
                                        )
                                    )
                                ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
                                element->components.emplace(std::piecewise_construct, 
                                    std::forward_as_tuple("Text"), 
                                    std::forward_as_tuple(
                                        std::make_shared<TeraToma::UI::UITextComponent>(
                                            "10/10",
                                            "NotoSansMono-Regular-12",
                                            6,
                                            TeraToma::UI::UIAnchor::MIDDLE_CENTER
                                        )
                                    )
                                ).first->second->Hookup(renderer, &gameAPI, &assets, gameDiaMenu, element);
                                gameAPI.hand.cards[i].onRespond = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance, std::string_view a_response) {
                                    gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = true;
                                    std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).components.at("Text"))->SetText(renderer, &assets, &gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)), std::string(a_response));
                                };
                            }  
                            gameAPI.healthSoftCap = 10;
                            gameAPI.health = gameAPI.healthSoftCap;
                        }
                        break;
                }
                break;
            case TeraToma::GameState::GAME_OVER:
                break;
            default:
                break;
        }

        if (uiManager.dirtyRecalculate) {
            uiManager.Recalculate(renderer, &gameAPI, &assets);
        }

        SDL_SetRenderDrawColor(renderer, 5, 5, 25, SDL_ALPHA_OPAQUE);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderClear(renderer);
        
        switch (gameAPI.gameState) {
            case TeraToma::GameState::MAIN_MENU:
                break;
            case TeraToma::GameState::MODS_MENU:
                break;
            case TeraToma::GameState::SETTINGS:
                break;
            case TeraToma::GameState::PAUSE_MENU:
                drawGame();
                SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
                SDL_SetRenderDrawColor(renderer, 125, 125, 125, 175);
                SDL_RenderFillRect(renderer, NULL);
                SDL_SetRenderDrawColor(renderer, r, g, b, a);
                for (size_t i = 0; i < gameAPI.deck.cards.size(); ++i) {
                    rectToUse.x = i % largeCardAmount * cardLargeWidth;
                    rectToUse.y = (int)(i / largeCardAmount) * cardLargeHeight;
                    
                    DrawCardType(renderer, &gameAPI, &assets, &gameAPI.cardTypes.at(gameAPI.deck.cards[i]), &rectToUse);
                }
                break;
            case TeraToma::GameState::GAME:
                drawGame();
                break;
        }

        uiManager.Render(renderer, &gameAPI, &assets);

        if (gameAPI.blindness > 0) {
            SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, NULL);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            gameAPI.blindness -= std::min<uint64_t>(gameAPI.blindness, deltaTime);
        }

        // SDL_RenderFillRects(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(8);
    }

    cardFront.reset();
    cardUsed.reset();
    cardBack.reset();
    cardBackOverlay.reset();
    cardSlash.reset();
    killSFX.reset();
    cardFlipSFX.reset();

    assets.Uninitialize();

    std::println("Pre Mod Uninitialize");
    gameAPI.DoModUninitialization();

    std::println("Pre Mod Unload");
    gameAPI.DoModUnloading();

    gameAPI.Destroy();
    
    SDL_CloseAudioDevice(audioDevice);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
