#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <TeraToma/GameAPI.h>
#include <TeraToma/Constants.h>
#include <TeraToma/Assets/Assets.h>
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

constexpr double TWO_PI = 2.0 * std::numbers::pi;
constexpr double ONE_HALF_PI = 0.5 * std::numbers::pi;
constexpr double ONE_FOURTH_PI =  0.25 * std::numbers::pi;
constexpr double ONE_EIGTH_PI = 0.125 * std::numbers::pi;
constexpr double ONE_SIXTEENTH_PI = 0.0625 * std::numbers::pi;

void DrawCardType(SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::CardType* a_card, SDL_FRect* a_cardLocation) {
    SDL_RenderTexture(a_renderer, a_assets->textures.at("Card Front").texture, NULL, a_cardLocation);
    SDL_RenderTexture(a_renderer, a_assets->textures.at(a_card->name).texture, NULL, a_cardLocation);
}

void DrawCardInstance(SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::CardInstance* a_card, SDL_FRect* a_cardLocation) {
    if (a_card->flipped) {
        DrawCardType(a_renderer, a_gameAPI, a_assets, &a_gameAPI->cardTypes.at(a_card->displayName), a_cardLocation);
        // Statuses drawn next.
    } else {
        SDL_RenderTexture(a_renderer, a_assets->textures.at("Card Back").texture, NULL, a_cardLocation);
    }

    if (a_card->dead) {
        uint8_t r, g, b, a;
        SDL_GetRenderDrawColor(a_renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(a_renderer, 255, 25, 25, 125);
        SDL_RenderFillRect(a_renderer, a_cardLocation);
        SDL_SetRenderDrawColor(a_renderer, r, g, b, a);
    }
}

void DrawGameBackground(SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets) {
    SDL_SetRenderDrawColor(a_renderer, 57, 175, 57, SDL_ALPHA_OPAQUE);
}

int main(int argc, char const **) {
    std::println("Ok we've entered main.");
    float cardScale = 2;
    float cardWidth = 38 * cardScale;
    float cardHeight = 56 * cardScale;
    float cardHalfWidth = cardWidth * 0.5f;
    float cardHalfHeight = cardHeight * 0.5f;
    int wonk = (int)(TeraToma::WINDOW_WIDTH / cardWidth);

    SDL_SetAppMetadata(TeraToma::APPLICATION_TITLE, "0.1.0.0", "com.darthsae.teratoma");
    // SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, "Darthsae");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "game");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, "https://github.com/Darthsae/TeraToma");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
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

    
    std::println("Pre API");
    TeraToma::GameAPI gameAPI = TeraToma::GameAPI();
    std::println("Pre Assets");
    TeraToma::Assets::Assets assets = TeraToma::Assets::Assets();
    std::println("Pre UI Manager");
    TeraToma::UI::UIManager uiManager = TeraToma::UI::UIManager();

    #pragma region Loading Mods and Assets
    std::println("Pre Mods");
    std::println("Mods {}", std::filesystem::current_path().append("Mods").string());
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(std::filesystem::current_path().append("Mods"))) {
        std::println("Checking {} it is {} and extension {}", dir_entry.path().string(), dir_entry.is_regular_file(), dir_entry.path().extension().string());
        if (dir_entry.is_regular_file() && dir_entry.path().extension().string() == ".dll") {
            std::println("{} {} {}", dir_entry.path().stem().string(), dir_entry.path().extension().string(), dir_entry.path().string());
            gameAPI.mods.emplace(std::piecewise_construct, std::forward_as_tuple(dir_entry.path().stem().string()), std::forward_as_tuple(dir_entry.path().stem().string(), dir_entry.path().wstring()));
        }
    }

    std::println("Pre Images");
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::recursive_directory_iterator(std::filesystem::current_path().append("Assets"))) {
        if (dir_entry.path().has_extension()) {
            if (dir_entry.path().extension().string() == ".png") {
                TeraToma::Assets::TextureAsset textureAsset = TeraToma::Assets::TextureAsset(dir_entry.path().stem().string(), dir_entry.path().stem().string());
                textureAsset.Load(renderer, dir_entry.path().string());
                assets.AddTexture(dir_entry.path().stem().string(), textureAsset);
            } else if (dir_entry.path().extension().string() == ".ttf") {
                TeraToma::Assets::FontAsset fontAsset = TeraToma::Assets::FontAsset(dir_entry.path().stem().string(), dir_entry.path().stem().string(), 12);
                fontAsset.Load(renderer, dir_entry.path().string());
                assets.AddFont(dir_entry.path().stem().string(), fontAsset);
            }
        }
    }
    #pragma endregion

    gameAPI.postLoadCardFnptr = [](std::string_view a_mod, TeraToma::CardType* a_cardType) {
        std::println("Whack");
    };

    std::println("Pre Mod Load");
    for (std::pair<const std::string, TeraToma::Mod>& pair : gameAPI.mods) {
        pair.second.Load(&gameAPI);
    }

    std::println("Pre Mod Initialize");
    for (std::pair<const std::string, TeraToma::Mod>& pair : gameAPI.mods) {
        pair.second.Initialize(&gameAPI);
    }

    std::println("Pre Cards into Deck");
    for (std::pair<const std::string, TeraToma::CardType>& pair : gameAPI.cardTypes) {
        //std::println("{}", pair.first);
        gameAPI.deck.AddCard(pair.first);
    }
    
    std::println("Pre Card Rects");
    SDL_FRect* cardRects = (SDL_FRect*)malloc(5);
    SDL_FPoint point = {0, 0};
    uint8_t r, g, b, a;

    float bongo = (float)(TWO_PI / 1);

    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Main Menu"), 
        std::forward_as_tuple(std::string_view("Main Menu"))
    );
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Settings Menu"), 
        std::forward_as_tuple(std::string_view("Settings Menu"))
    );
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Pause Menu"), 
        std::forward_as_tuple(std::string_view("Pause Menu"))
    );
    uiManager.uiLayers.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Game Play Menu"), 
        std::forward_as_tuple(std::string_view("Game Play Menu"))
    );

    TeraToma::UI::UIElement* element;

    TeraToma::UI::UILayer* mainMenu = &uiManager.uiLayers.at("Main Menu");
    mainMenu->enabled = true;
    mainMenu->uiElements.emplace(std::piecewise_construct, 
        std::forward_as_tuple("Play"), 
        std::forward_as_tuple(
            std::string_view("Play"), 
            TeraToma::UI::UIRect{{0, 0, 200, 50}}
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
    element->onMouseLeftDown.emplace_back([&cardRects, &bongo, cardHalfWidth, cardHalfHeight, cardWidth, cardHeight](SDL_Renderer* a_renderer, TeraToma::GameAPI* a_gameAPI, TeraToma::Assets::Assets* a_assets, TeraToma::UI::UILayer* a_uiLayer, TeraToma::UI::UIElement* a_element, SDL_FPoint* a_mousePos) {
        a_uiLayer->enabled = false;
        a_gameAPI->gameState = TeraToma::GameState::GAME;
        a_gameAPI->hand = TeraToma::Hand(a_gameAPI, a_gameAPI->deck.Shuffle(5));
        a_gameAPI->gamePlayState = TeraToma::GamePlayState::ORDERING;
        cardRects = (SDL_FRect*)realloc(cardRects, sizeof(SDL_FRect) * a_gameAPI->hand.cardCount);
        bongo = (float)(TWO_PI / a_gameAPI->hand.cardCount);
        for (size_t i = 0; i < a_gameAPI->hand.cardCount; ++i) {
            float angle = (float)(i * bongo - ONE_HALF_PI);
            cardRects[i] = {TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angle) - cardHalfWidth, TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angle) - cardHalfHeight, cardWidth, cardHeight};
        }  
        return true;
    });

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

    uiManager.Recalculate(renderer, &gameAPI, &assets);

    std::function<void(void)> drawGame = [&]() {
        DrawGameBackground(renderer, &gameAPI, &assets);
        switch (gameAPI.gamePlayState) {
            case TeraToma::GamePlayState::REWARDS:
                break;
            case TeraToma::GamePlayState::DRAWING:
                break;
            case TeraToma::GamePlayState::ORDERING: {
                for (size_t i = 0; i < gameAPI.hand.cardCount; ++i) {
                    DrawCardInstance(renderer, &gameAPI, &assets, &gameAPI.hand.cards.at(i), &cardRects[i]);
                }
                if (gameAPI.hand.resolutionOrder.size() == 0) {
                    break;
                }
                SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
                SDL_SetRenderDrawColor(renderer, 255, 225, 225, SDL_ALPHA_OPAQUE);
                for (size_t i = 0; i < gameAPI.hand.resolutionOrder.size() - 1; ++i) {
                    float angleOne = (float)(gameAPI.hand.resolutionOrder.at(i) * bongo - ONE_HALF_PI);
                    float angleTwo = (float)(gameAPI.hand.resolutionOrder.at(i + 1) * bongo - ONE_HALF_PI);
                    SDL_RenderLine(renderer, TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angleOne), TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angleOne), TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angleTwo), TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angleTwo));
                }
                float angle = (float)(gameAPI.hand.resolutionOrder.back() * bongo - ONE_HALF_PI);
                SDL_RenderLine(renderer, TeraToma::WINDOW_CENTER_X + 300.0f * std::cosf(angle), TeraToma::WINDOW_CENTER_Y + 300.0f * std::sinf(angle), point.x, point.y);
                SDL_SetRenderDrawColor(renderer, r, g, b, a);
                break;
            } case TeraToma::GamePlayState::PLAYING:
                for (size_t i = 0; i < gameAPI.hand.cardCount; ++i) {
                    DrawCardInstance(renderer, &gameAPI, &assets, &gameAPI.hand.cards.at(i), &cardRects[i]);
                }
                
                if (gameAPI.gameMouseState == TeraToma::GameMouseState::KILLING) {
                    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
                    SDL_SetRenderDrawColor(renderer, 225, 0, 0, 95);
                    SDL_RenderFillRect(renderer, NULL);
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                }
                break;
        }
    };

    bool running = true;
    SDL_Event event;
    while (running) {
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
                                                            case TeraToma::GameMouseState::FLIPPING:
                                                                if (!gameAPI.hand.cards.at(i).flipped) {
                                                                    gameAPI.hand.cards.at(i).flipped = true;
                                                                    if (gameAPI.cardTypes.at(gameAPI.hand.cards.at(i).names.back()).onFlip) {
                                                                        gameAPI.cardTypes.at(gameAPI.hand.cards.at(i).names.back()).onFlip(&gameAPI);
                                                                    }
                                                                }
                                                                break;
                                                            case TeraToma::GameMouseState::KILLING:
                                                                if (!gameAPI.hand.cards.at(i).dead) {
                                                                    gameAPI.hand.cards.at(i).dead = true;
                                                                    if (gameAPI.cardTypes.at(gameAPI.hand.cards.at(i).names.back()).onKill) {
                                                                        gameAPI.cardTypes.at(gameAPI.hand.cards.at(i).names.back()).onKill(&gameAPI);
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
                                    if (gameAPI.gamePlayState == TeraToma::GamePlayState::PLAYING) {
                                        gamePlayMenu->enabled = true;
                                    }
                                    break;
                                case TeraToma::GameState::GAME:
                                    gameAPI.gameState = TeraToma::GameState::PAUSE_MENU;
                                    pauseMenu->enabled = true;
                                    if (gameAPI.gamePlayState == TeraToma::GamePlayState::PLAYING) {
                                        gamePlayMenu->enabled = false;
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
                        break;
                }
                break;
            case TeraToma::GameState::GAME_OVER:
                break;
            default:
                break;
        }

        SDL_SetRenderDrawColor(renderer, 100, 255, 125, SDL_ALPHA_OPAQUE);
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
                break;
            case TeraToma::GameState::GAME:
                drawGame();
                break;
        }

        uiManager.Render(renderer, &gameAPI, &assets);

        // SDL_RenderFillRects(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(4);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}