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
#include <TeraToma/GameClientAPI.h>

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
    //std::vector<>
    bool running = true;

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

    TeraToma::GameClientAPI clientAPI;

    if (!SDL_CreateWindowAndRenderer(TeraToma::APPLICATION_TITLE, TeraToma::WINDOW_WIDTH, TeraToma::WINDOW_HEIGHT, SDL_WINDOW_VULKAN, &clientAPI.window, &clientAPI.renderer)) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
    }

    clientAPI.audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    
    std::println("Pre API");
    clientAPI.gameAPI = TeraToma::GameAPI();
    std::println("Pre Assets");
    clientAPI.assets = TeraToma::Assets::Assets();
    std::println("Pre UI Manager");
    clientAPI.uiManager = TeraToma::UI::UIManager();
    clientAPI.assets.uiManager = &clientAPI.uiManager;

    #pragma region Loading Mods and Assets
    std::println("Pre Mods");
    std::println("Mods {}", std::filesystem::current_path().append("Mods").string());
    clientAPI.gameAPI.LoadMods();

    std::println("Pre Images");
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::recursive_directory_iterator(std::filesystem::current_path().append("Assets"))) {
        if (dir_entry.path().has_extension()) {
            if (dir_entry.path().extension().string() == ".png") {
                clientAPI.assets.AddTexture(clientAPI.renderer, dir_entry.path().string(), dir_entry.path().stem().string());
            } else if (dir_entry.path().extension().string() == ".ttf") {
                clientAPI.assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 12), 12);
                clientAPI.assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 16), 16);
                clientAPI.assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 18), 18);
                clientAPI.assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 20), 20);
                clientAPI.assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 24), 24);
                clientAPI.assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 30), 30);
                clientAPI.assets.AddFont(dir_entry.path().string(), std::format("{}-{}", dir_entry.path().stem().string(), 36), 36);
            } else if (dir_entry.path().extension().string() == ".wav") {
                clientAPI.assets.AddSound(clientAPI.audioDevice, dir_entry.path().string(), dir_entry.path().stem().string());
            }
        }
    }
    #pragma endregion

    cardFront = clientAPI.assets.textures.at("Card Front");
    cardUsed = clientAPI.assets.textures.at("Card Used");
    cardBack = clientAPI.assets.textures.at("Card Back");
    cardBackOverlay = clientAPI.assets.textures.at("Back Overlay");
    cardSlash = clientAPI.assets.textures.at("Card Slashed");
    cardSlashUsed = clientAPI.assets.textures.at("Used Card Slashed");
    killSFX = clientAPI.assets.sounds.at("Kill");
    cardFlipSFX = clientAPI.assets.sounds.at("Card Flip");

    clientAPI.gameAPI.postLoadCardFnptr = [](std::string_view a_mod, TeraToma::CardType* a_cardType) {
        //std::println("Whack");
    };

    std::println("Pre Mod Load");
    clientAPI.gameAPI.DoModLoading();

    std::println("Pre Mod Initialize");
    clientAPI.gameAPI.DoModInitialization();

    std::println("Pre Cards into Deck");
    clientAPI.gameAPI.DealStartingHand();
    
    std::println("Pre Card Rects");
    SDL_FRect* cardRects = (SDL_FRect*)malloc(5);
    SDL_FPoint point = {0, 0};
    uint8_t r, g, b, a;

    float bongo = (float)(TWO_PI / 1);

    #pragma region UI
    clientAPI.uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Main Menu"), 
        std::forward_as_tuple(std::string_view("Main Menu"))
    );
    clientAPI.uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Settings Menu"), 
        std::forward_as_tuple(std::string_view("Settings Menu"))
    );
    clientAPI.uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Game Play Menu"), 
        std::forward_as_tuple(std::string_view("Game Play Menu"))
    );
    clientAPI.uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Game Dia Menu"), 
        std::forward_as_tuple(std::string_view("Game Dia Menu"))
    );
    clientAPI.uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Game Ordering Menu"), 
        std::forward_as_tuple(std::string_view("Game Ordering Menu"))
    );
    clientAPI.uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Game Rewards Menu"), 
        std::forward_as_tuple(std::string_view("Game Rewards Menu"))
    );
    clientAPI.uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Pause Menu"), 
        std::forward_as_tuple(std::string_view("Pause Menu"))
    );

    TeraToma::UI::UIElement* element;

    TeraToma::UI::UILayer* gameRewardsMenu = &clientAPI.uiManager.uiLayers.at("Game Rewards Menu");
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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameRewardsMenu, element);
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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameRewardsMenu, element);

    TeraToma::UI::UIElement* rewardsDescriptionElement = &gameRewardsMenu->uiElements.at("Texter");
    std::shared_ptr<TeraToma::UI::UITextComponent> rewardsDescriptionText = std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(rewardsDescriptionElement->components.at("Texta"));

    TeraToma::UI::UILayer* gameDiaMenu = &clientAPI.uiManager.uiLayers.at("Game Dia Menu");
    gameDiaMenu->enabled = false;

    TeraToma::UI::UILayer* mainMenu = &clientAPI.uiManager.uiLayers.at("Main Menu");
    mainMenu->enabled = true;
    
    TeraToma::UI::UILayer* gameOrderingMenu = &clientAPI.uiManager.uiLayers.at("Game Ordering Menu");
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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, mainMenu, element);
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
                    std::string_view(clientAPI.gameAPI.hand.cards[i].names.back()), 
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
            ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
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
            ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
            gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                std::forward_as_tuple(std::format("card {}", i)), 
                std::forward_as_tuple(
                    std::string_view(clientAPI.gameAPI.hand.cards[i].names.back()), 
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
            ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
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
            ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
            a_gameAPI->hand.cards[i].onRespond = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance, std::string_view a_response) {
                gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = true;
                std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).components.at("Text"))->SetText(clientAPI.renderer, &clientAPI.assets, &gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)), std::string(a_response));
            };
            a_gameAPI->hand.cards[i].onShowResponse = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance) {
                gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = true;
            };
            a_gameAPI->hand.cards[i].onHideResponse = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance) {
                gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = false;
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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, mainMenu, element);

    mainMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Quit"), 
        std::forward_as_tuple(
            std::string_view("Quit"), 
            TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X - 100, 375, 200, 50}}
        )
    );
    element = &mainMenu->uiElements.at("Quit");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>(
                std::string_view("UI Panel"), 
                true
            )
        )
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, mainMenu, element);
    element->onMouseLeftDown.emplace_back([&](SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::UI::UILayer* a_uiLayer, TeraToma::UI::UIElement* a_element, SDL_FPoint* a_mousePos) {
        running = false;
        return true;
    });
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Text"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UITextComponent>(
                "Quit",
                "NotoSansMono-Regular-24",
                6,
                TeraToma::UI::UIAnchor::MIDDLE_CENTER
            )
        )
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, mainMenu, element);

    TeraToma::UI::UILayer* settingsMenu = &clientAPI.uiManager.uiLayers.at("Settings Menu");
    settingsMenu->enabled = false;

    TeraToma::UI::UILayer* pauseMenu = &clientAPI.uiManager.uiLayers.at("Pause Menu");
    pauseMenu->enabled = false;

    pauseMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Menu"), 
        std::forward_as_tuple(
            std::string_view("Menu"), 
            TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X - 100, 300, 200, 50}}
        )
    );
    element = &pauseMenu->uiElements.at("Menu");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>(
                std::string_view("UI Panel"), 
                true
            )
        )
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, pauseMenu, element);
    element->onMouseLeftDown.emplace_back([&](SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::UI::UILayer* a_uiLayer, TeraToma::UI::UIElement* a_element, SDL_FPoint* a_mousePos) {
        a_gameAPI->gameState = TeraToma::GameState::MAIN_MENU;
        mainMenu->enabled = true;
        pauseMenu->enabled = false;
        gameDiaMenu->enabled = false;
        gameOrderingMenu->enabled = false;
        return true;
    });
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Text"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UITextComponent>(
                "Menu",
                "NotoSansMono-Regular-24",
                6,
                TeraToma::UI::UIAnchor::MIDDLE_CENTER
            )
        )
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, pauseMenu, element);

    pauseMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Quit"), 
        std::forward_as_tuple(
            std::string_view("Quit"), 
            TeraToma::UI::UIRect{{TeraToma::WINDOW_CENTER_X - 100, 375, 200, 50}}
        )
    );
    element = &pauseMenu->uiElements.at("Quit");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>(
                std::string_view("UI Panel"), 
                true
            )
        )
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, pauseMenu, element);
    element->onMouseLeftDown.emplace_back([&](SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::UI::UILayer* a_uiLayer, TeraToma::UI::UIElement* a_element, SDL_FPoint* a_mousePos) {
        running = false;
        return true;
    });
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Text"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UITextComponent>(
                "Quit",
                "NotoSansMono-Regular-24",
                6,
                TeraToma::UI::UIAnchor::MIDDLE_CENTER
            )
        )
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, pauseMenu, element);

    TeraToma::UI::UILayer* gamePlayMenu = &clientAPI.uiManager.uiLayers.at("Game Play Menu");
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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gamePlayMenu, element);
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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gamePlayMenu, element);
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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gamePlayMenu, element);

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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gamePlayMenu, element);
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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gamePlayMenu, element);

    TeraToma::UI::UIElement* hpUIElement = &gamePlayMenu->uiElements.at("Health");
    std::shared_ptr<TeraToma::UI::UITextComponent> hpText = std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(hpUIElement->components.at("HP Text"));

    gamePlayMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Enemies"), 
        std::forward_as_tuple(
            std::string_view("Enemies"), 
            TeraToma::UI::UIRect{{0, 0, 140, 48}}
        )
    );
    element = &gamePlayMenu->uiElements.at("Enemies");
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Texture"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UIImageComponent>(
                std::string_view("UI Panel"), 
                true
            )
        )
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gamePlayMenu, element);
    element->components.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Enemies Text"), 
        std::forward_as_tuple(
            std::make_shared<TeraToma::UI::UITextComponent>(
                "10/10",
                "NotoSansMono-Regular-36",
                6,
                TeraToma::UI::UIAnchor::MIDDLE_CENTER
            )
        )
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gamePlayMenu, element);

    TeraToma::UI::UIElement* enemiesUIElement = &gamePlayMenu->uiElements.at("Enemies");
    std::shared_ptr<TeraToma::UI::UITextComponent> enemiesText = std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(enemiesUIElement->components.at("Enemies Text"));

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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gamePlayMenu, element);
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
    ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gamePlayMenu, element);
    #pragma endregion
    
    uint64_t deltaTime;
    std::function<void(void)> drawGame = [&]() {
        DrawGameBackground(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets);
        switch (clientAPI.gameAPI.gamePlayState) {
            case TeraToma::GamePlayState::REWARDS:
                for (size_t i = 0; i < cardChoices.size(); ++i) {
                    DrawCardType(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &clientAPI.gameAPI.cardTypes.at(cardChoices[i]), &cardRects[i]);
                }
                break;
            case TeraToma::GamePlayState::DRAWING:
                break;
            case TeraToma::GamePlayState::ORDERING: {
                for (size_t i = 0; i < clientAPI.gameAPI.hand.cardCount; ++i) {
                    DrawCardInstance(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &clientAPI.gameAPI.hand.cards[i], &cardRects[i]);
                }
                if (clientAPI.gameAPI.hand.resolutionOrder.size() == 0) {
                    break;
                }
                SDL_GetRenderDrawColor(clientAPI.renderer, &r, &g, &b, &a);
                SDL_SetRenderDrawColor(clientAPI.renderer, 255, 225, 225, SDL_ALPHA_OPAQUE);
                for (size_t i = 0; i < clientAPI.gameAPI.hand.resolutionOrder.size() - 1; ++i) {
                    float angleOne = (float)(clientAPI.gameAPI.hand.resolutionOrder[i] * bongo - ONE_HALF_PI);
                    float angleTwo = (float)(clientAPI.gameAPI.hand.resolutionOrder[i + 1] * bongo - ONE_HALF_PI);
                    SDL_RenderLine(clientAPI.renderer, TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angleOne), TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angleOne), TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angleTwo), TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angleTwo));
                }
                float angle = (float)(clientAPI.gameAPI.hand.resolutionOrder.back() * bongo - ONE_HALF_PI);
                SDL_RenderLine(clientAPI.renderer, TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angle), TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angle), point.x, point.y);
                SDL_SetRenderDrawColor(clientAPI.renderer, r, g, b, a);
                break;
            } case TeraToma::GamePlayState::PLAYING:
                for (size_t i = 0; i < clientAPI.gameAPI.hand.cardCount; ++i) {
                    DrawCardInstance(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &clientAPI.gameAPI.hand.cards[i], &cardRects[i]);
                }
                
                if (clientAPI.gameAPI.gameMouseState == TeraToma::GameMouseState::KILLING) {
                    SDL_GetRenderDrawColor(clientAPI.renderer, &r, &g, &b, &a);
                    SDL_SetRenderDrawColor(clientAPI.renderer, 225, 0, 0, 95);
                    SDL_RenderFillRect(clientAPI.renderer, NULL);
                    SDL_SetRenderDrawColor(clientAPI.renderer, r, g, b, a);
                }

                if (clientAPI.gameAPI.gameMouseState == TeraToma::GameMouseState::SELECTING) {
                    if (clientAPI.gameAPI.selected.size() == 0) {
                        break;
                    }

                    SDL_GetRenderDrawColor(clientAPI.renderer, &r, &g, &b, &a);
                    SDL_SetRenderDrawColor(clientAPI.renderer, 255, 255, 255, 75);
                    for (size_t i : clientAPI.gameAPI.selected) {
                        SDL_RenderFillRect(clientAPI.renderer, &cardRects[i]);
                    }
                    SDL_SetRenderDrawColor(clientAPI.renderer, r, g, b, a);
                }
                break;
        }
    };

    if (clientAPI.uiManager.dirtyRecalculate) {
        clientAPI.uiManager.Recalculate(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets);
    }

    clientAPI.gameAPI.postKillFnptr = [&](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        killSFX->Play(clientAPI.audioDevice);
        if (!a_gameAPI->hand.cards[a_index].canWin) {
            enemiesText->SetText(clientAPI.renderer, &clientAPI.assets, enemiesUIElement, std::format("{}/{}", a_gameAPI->health, a_gameAPI->healthSoftCap));
        }
    };

    clientAPI.gameAPI.postWinFnptr = [&](TeraToma::GameAPI* a_gameAPI) {
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

    clientAPI.gameAPI.postHurtFnptr = [&](TeraToma::GameAPI* a_gameAPI, int32_t a_hpChange) {
        hpText->SetText(clientAPI.renderer, &clientAPI.assets, hpUIElement, std::format("{}/{}", a_gameAPI->health, a_gameAPI->healthSoftCap));
    };

    SDL_Event event;
    clientAPI.gameAPI.round = 0;
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
                            if (!clientAPI.uiManager.OnMouseLeftDown(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &point)) {
                                switch (clientAPI.gameAPI.gameState) {
                                    case TeraToma::GameState::MAIN_MENU:
                                        break;
                                    case TeraToma::GameState::MODS_MENU:
                                        break;
                                    case TeraToma::GameState::SETTINGS:
                                        break;
                                    case TeraToma::GameState::PAUSE_MENU:
                                        break;
                                    case TeraToma::GameState::GAME:
                                        switch (clientAPI.gameAPI.gamePlayState) {
                                            case TeraToma::GamePlayState::REWARDS:
                                                for (size_t i = 0; i < cardChoices.size(); ++i) {
                                                    if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                        clientAPI.gameAPI.deck.AddCard(cardChoices[i]);
                                                        #pragma region Create Win
                                                        clientAPI.gameAPI.round += 1;
                                                        if (clientAPI.gameAPI.round % 2 == 0) {
                                                            clientAPI.gameAPI.hand.cardCount += 1;
                                                        }
                                                        clientAPI.gameAPI.gameState = TeraToma::GameState::GAME;
                                                        clientAPI.gameAPI.hand = TeraToma::Hand(&clientAPI.gameAPI, clientAPI.gameAPI.deck.Shuffle(clientAPI.gameAPI.hand.cardCount + RandomUniformUInt32(0, 1)));
                                                        clientAPI.gameAPI.gamePlayState = TeraToma::GamePlayState::ORDERING;
                                                        gameOrderingMenu->enabled = true;
                                                        gameRewardsMenu->enabled = false;
                                                        cardRects = (SDL_FRect*)realloc(cardRects, sizeof(SDL_FRect) * clientAPI.gameAPI.hand.cardCount);
                                                        bongo = (float)(TWO_PI / clientAPI.gameAPI.hand.cardCount);
                                                        for (size_t i = 0; i < clientAPI.gameAPI.hand.cardCount; ++i) {
                                                            float angle = (float)(i * bongo - ONE_HALF_PI);
                                                            cardRects[i] = {TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angle) - cardHalfWidth, TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angle) - cardHalfHeight, cardWidth, cardHeight};
                                                            gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                                                                std::forward_as_tuple(std::format("card {} number", i)), 
                                                                std::forward_as_tuple(
                                                                    std::string_view(clientAPI.gameAPI.hand.cards[i].names.back()), 
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
                                                            ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
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
                                                            ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
                                                            gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                                                                std::forward_as_tuple(std::format("card {}", i)), 
                                                                std::forward_as_tuple(
                                                                    std::string_view(clientAPI.gameAPI.hand.cards[i].names.back()), 
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
                                                            ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
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
                                                            ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
                                                            clientAPI.gameAPI.hand.cards[i].onRespond = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance, std::string_view a_response) {
                                                                gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = true;
                                                                std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).components.at("Text"))->SetText(clientAPI.renderer, &clientAPI.assets, &gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)), std::string(a_response));
                                                            };
                                                            clientAPI.gameAPI.hand.cards[i].onShowResponse = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance) {
                                                                gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = true;
                                                            };
                                                            clientAPI.gameAPI.hand.cards[i].onHideResponse = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance) {
                                                                gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = false;
                                                            };
                                                        }  
                                                        clientAPI.gameAPI.health = clientAPI.gameAPI.healthSoftCap;
                                                        hpText->SetText(clientAPI.renderer, &clientAPI.assets, hpUIElement, std::format("{}/{}", clientAPI.gameAPI.health, clientAPI.gameAPI.healthSoftCap));
                                                        #pragma endregion
                                                    }
                                                }
                                                break;
                                            case TeraToma::GamePlayState::ORDERING:
                                                for (size_t i = 0; i < clientAPI.gameAPI.hand.cardCount; ++i) {
                                                    if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                        if (std::find(clientAPI.gameAPI.hand.resolutionOrder.begin(), clientAPI.gameAPI.hand.resolutionOrder.end(), i) == clientAPI.gameAPI.hand.resolutionOrder.end()) {
                                                            clientAPI.gameAPI.hand.resolutionOrder.emplace_back(i);
                                                            if (clientAPI.gameAPI.hand.resolutionOrder.size() == clientAPI.gameAPI.hand.cards.size()) {
                                                                clientAPI.gameAPI.hand.Resolve(&clientAPI.gameAPI);
                                                                clientAPI.gameAPI.gamePlayState = TeraToma::GamePlayState::PLAYING;
                                                                clientAPI.gameAPI.gameMouseState = TeraToma::GameMouseState::FLIPPING;
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
                                                for (size_t i = 0; i < clientAPI.gameAPI.hand.cardCount; ++i) {
                                                    if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                        switch (clientAPI.gameAPI.gameMouseState) {
                                                            case TeraToma::GameMouseState::FLIPPING: {
                                                                TeraToma::CardInstance* cardInstance = &clientAPI.gameAPI.hand.cards[i];
                                                                TeraToma::CardType* cardType = &clientAPI.gameAPI.cardTypes.at(cardInstance->names.back());
                                                                if (!cardInstance->flipped && (!cardType->canFlip || cardType->canFlip(&clientAPI.gameAPI, i))) {
                                                                    cardFlipSFX->Play(clientAPI.audioDevice);
                                                                    cardInstance->flipped = true;
                                                                    if (cardType->onFlip) {
                                                                        cardType->onFlip(&clientAPI.gameAPI, i);
                                                                    }
                                                                } else if (!cardInstance->activated && (cardType->canActivate && cardType->canActivate(&clientAPI.gameAPI, i))) {
                                                                    cardInstance->activated = true;
                                                                    if (cardType->onActivate) {
                                                                        cardType->onActivate(&clientAPI.gameAPI, i);
                                                                    }
                                                                }
                                                                break;
                                                            } case TeraToma::GameMouseState::KILLING:
                                                                clientAPI.gameAPI.TryKill(i);
                                                                break;
                                                            case TeraToma::GameMouseState::SELECTING:
                                                                clientAPI.gameAPI.Select(i);
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
                            if (!clientAPI.uiManager.OnMouseMiddleDown(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &point)) {
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            if (!clientAPI.uiManager.OnMouseRightDown(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &point)) {
                                switch (clientAPI.gameAPI.gameState) {
                                    case TeraToma::GameState::GAME:
                                        switch (clientAPI.gameAPI.gamePlayState) {
                                            case TeraToma::GamePlayState::PLAYING:
                                                switch (clientAPI.gameAPI.gameMouseState) {
                                                    case TeraToma::GameMouseState::FLIPPING:
                                                    case TeraToma::GameMouseState::KILLING:
                                                        for (size_t i = 0; i < clientAPI.gameAPI.hand.cardCount; ++i) {
                                                            if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                                if (clientAPI.gameAPI.hand.cards[i].flipped) {
                                                                    for (size_t j = 0; j < clientAPI.gameAPI.hand.cards[i].statuses.size(); ++j) {
                                                                        std::println("{}: {}", j, clientAPI.gameAPI.hand.cards[i].statuses[j]);
                                                                    }

                                                                    textWonker->SetText(clientAPI.renderer, &clientAPI.assets, textWonk, std::format("{}\n{}\n{}", clientAPI.gameAPI.hand.cards[i].displayName, TeraToma::AllegianceDisplayStrings.at(clientAPI.gameAPI.cardTypes.at(clientAPI.gameAPI.hand.cards[i].displayName).allegiance), clientAPI.gameAPI.cardTypes.at(clientAPI.gameAPI.hand.cards[i].displayName).description));
                                                                }
                                                                break;
                                                            }
                                                        }
                                                        break;
                                                    case TeraToma::GameMouseState::SELECTING:
                                                        if (clientAPI.gameAPI.selected.size() > 0) {
                                                            clientAPI.gameAPI.selected.pop_back();
                                                        } else {
                                                            clientAPI.gameAPI.ClearSelection();
                                                        }
                                                        break;
                                                }
                                                break;
                                            case TeraToma::GamePlayState::ORDERING:
                                                if (clientAPI.gameAPI.hand.resolutionOrder.size() > 0) {
                                                    clientAPI.gameAPI.hand.resolutionOrder.pop_back();
                                                }
                                                break;
                                            case TeraToma::GamePlayState::REWARDS:
                                                for (size_t i = 0; i < cardChoices.size(); ++i) {
                                                    if (SDL_PointInRectFloat(&point, &cardRects[i])) {
                                                        rewardsDescriptionText->SetText(clientAPI.renderer, &clientAPI.assets, rewardsDescriptionElement, std::format("{}\n{}\n{}", cardChoices[i], TeraToma::AllegianceDisplayStrings.at(clientAPI.gameAPI.cardTypes.at(cardChoices[i]).allegiance), clientAPI.gameAPI.cardTypes.at(cardChoices[i]).description));
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
                            if (!clientAPI.uiManager.OnMouseLeftUp(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &point)) {
                            }
                            break;
                        case SDL_BUTTON_MIDDLE:
                            if (!clientAPI.uiManager.OnMouseMiddleUp(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &point)) {
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            if (!clientAPI.uiManager.OnMouseRightUp(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &point)) {
                            }
                            break;
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.key) {
                        case SDLK_ESCAPE:
                            switch (clientAPI.gameAPI.gameState) {
                                case TeraToma::GameState::MODS_MENU:
                                case TeraToma::GameState::SETTINGS:
                                    clientAPI.gameAPI.gameState = TeraToma::GameState::MAIN_MENU;
                                    break;
                                case TeraToma::GameState::PAUSE_MENU:
                                    clientAPI.gameAPI.gameState = TeraToma::GameState::GAME;
                                    pauseMenu->enabled = false;
                                    switch (clientAPI.gameAPI.gamePlayState) {
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
                                    clientAPI.gameAPI.gameState = TeraToma::GameState::PAUSE_MENU;
                                    pauseMenu->enabled = true;
                                    switch (clientAPI.gameAPI.gamePlayState) {
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
        
        switch (clientAPI.gameAPI.gameState) {
            case TeraToma::GameState::MAIN_MENU:
                break;
            case TeraToma::GameState::MODS_MENU:
                break;
            case TeraToma::GameState::SETTINGS:
                break;
            case TeraToma::GameState::PAUSE_MENU:
                break;
            case TeraToma::GameState::GAME:
                switch (clientAPI.gameAPI.gamePlayState) {
                    case TeraToma::GamePlayState::REWARDS:
                        break;
                    case TeraToma::GamePlayState::DRAWING:
                        break;
                    case TeraToma::GamePlayState::ORDERING:
                        break;
                    case TeraToma::GamePlayState::PLAYING:
                        if (clientAPI.gameAPI.health <= 0) {
                            // Handle Death
                            clientAPI.gameAPI.gameState = TeraToma::GameState::GAME;
                            clientAPI.gameAPI.Reset();
                            clientAPI.gameAPI.DealStartingHand();
                            clientAPI.gameAPI.hand = TeraToma::Hand(&clientAPI.gameAPI, clientAPI.gameAPI.deck.Shuffle(5));
                            clientAPI.gameAPI.gamePlayState = TeraToma::GamePlayState::ORDERING;
                            gameDiaMenu->Clear();
                            gameOrderingMenu->enabled = true;
                            gamePlayMenu->enabled = false;
                            gameDiaMenu->enabled = false;
                            cardRects = (SDL_FRect*)realloc(cardRects, sizeof(SDL_FRect) * clientAPI.gameAPI.hand.cardCount);
                            bongo = (float)(TWO_PI / clientAPI.gameAPI.hand.cardCount);
                            for (size_t i = 0; i < clientAPI.gameAPI.hand.cardCount; ++i) {
                                float angle = (float)(i * bongo - ONE_HALF_PI);
                                cardRects[i] = {TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angle) - cardHalfWidth, TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angle) - cardHalfHeight, cardWidth, cardHeight};
                                gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                                    std::forward_as_tuple(std::format("card {} number", i)), 
                                    std::forward_as_tuple(
                                        std::string_view(clientAPI.gameAPI.hand.cards[i].names.back()), 
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
                                ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
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
                                ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
                                gameDiaMenu->uiElements.emplace(std::piecewise_construct, 
                                    std::forward_as_tuple(std::format("card {}", i)), 
                                    std::forward_as_tuple(
                                        std::string_view(clientAPI.gameAPI.hand.cards[i].names.back()), 
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
                                ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
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
                                ).first->second->Hookup(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, gameDiaMenu, element);
                                clientAPI.gameAPI.hand.cards[i].onRespond = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance, std::string_view a_response) {
                                    gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = true;
                                    std::dynamic_pointer_cast<TeraToma::UI::UITextComponent>(gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).components.at("Text"))->SetText(clientAPI.renderer, &clientAPI.assets, &gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)), std::string(a_response));
                                };
                                clientAPI.gameAPI.hand.cards[i].onShowResponse = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance) {
                                    gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = true;
                                };
                                clientAPI.gameAPI.hand.cards[i].onHideResponse = [&](TeraToma::GameAPI* a_gameAPI, TeraToma::CardInstance* a_instance) {
                                    gameDiaMenu->uiElements.at(std::format("card {}", a_instance->index)).enabled = false;
                                };
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

        if (clientAPI.uiManager.dirtyRecalculate) {
            clientAPI.uiManager.Recalculate(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets);
        }

        SDL_SetRenderDrawColor(clientAPI.renderer, 5, 5, 25, SDL_ALPHA_OPAQUE);
        SDL_SetRenderDrawBlendMode(clientAPI.renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderClear(clientAPI.renderer);
        
        switch (clientAPI.gameAPI.gameState) {
            case TeraToma::GameState::MAIN_MENU:
                break;
            case TeraToma::GameState::MODS_MENU:
                break;
            case TeraToma::GameState::SETTINGS:
                break;
            case TeraToma::GameState::PAUSE_MENU:
                drawGame();
                SDL_GetRenderDrawColor(clientAPI.renderer, &r, &g, &b, &a);
                SDL_SetRenderDrawColor(clientAPI.renderer, 125, 125, 125, 175);
                SDL_RenderFillRect(clientAPI.renderer, NULL);
                SDL_SetRenderDrawColor(clientAPI.renderer, r, g, b, a);
                for (size_t i = 0; i < clientAPI.gameAPI.deck.cards.size(); ++i) {
                    rectToUse.x = i % largeCardAmount * cardLargeWidth;
                    rectToUse.y = (int)(i / largeCardAmount) * cardLargeHeight;
                    
                    DrawCardType(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets, &clientAPI.gameAPI.cardTypes.at(clientAPI.gameAPI.deck.cards[i]), &rectToUse);
                }
                break;
            case TeraToma::GameState::GAME:
                drawGame();
                break;
        }

        clientAPI.uiManager.Render(clientAPI.renderer, &clientAPI.gameAPI, &clientAPI.assets);

        if (clientAPI.gameAPI.blindness > 0) {
            SDL_GetRenderDrawColor(clientAPI.renderer, &r, &g, &b, &a);
            SDL_SetRenderDrawColor(clientAPI.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(clientAPI.renderer, NULL);
            SDL_SetRenderDrawColor(clientAPI.renderer, r, g, b, a);
            clientAPI.gameAPI.blindness -= std::min<uint64_t>(clientAPI.gameAPI.blindness, deltaTime);
        }

        // SDL_RenderFillRects(clientAPI.renderer);
        SDL_RenderPresent(clientAPI.renderer);
        SDL_Delay(8);
    }

    cardFront.reset();
    cardUsed.reset();
    cardBack.reset();
    cardBackOverlay.reset();
    cardSlash.reset();
    killSFX.reset();
    cardFlipSFX.reset();

    clientAPI.assets.Uninitialize();

    std::println("Pre Mod Uninitialize");
    clientAPI.gameAPI.DoModUninitialization();

    std::println("Pre Mod Unload");
    clientAPI.gameAPI.DoModUnloading();

    clientAPI.gameAPI.Destroy();
    
    SDL_CloseAudioDevice(clientAPI.audioDevice);
    SDL_DestroyRenderer(clientAPI.renderer);
    SDL_DestroyWindow(clientAPI.window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
