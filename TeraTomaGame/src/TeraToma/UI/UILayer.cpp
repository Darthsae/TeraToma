#include <TeraToma/UI/UILayer.h>

namespace TeraToma::UI { 
    UILayer::UILayer(std::string_view a_id) {
        id = std::string(a_id);
        enabled = true;
        uiElements = std::unordered_map<std::string, UIElement>();
    }

    void UILayer::Render(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets) {
        for (std::pair<const std::string, UIElement>& pair : this->uiElements) {
            if (!pair.second.enabled) {
                continue;
            }

            pair.second.Render(a_renderer, a_gameAPI, a_assets, this);
        }
    }

    bool UILayer::OnMouseLeftDown(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UIElement>& pair : this->uiElements) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseLeftDown(a_renderer, a_gameAPI, a_assets, this, a_mousePos)) {
                return true;
            }
        }

        return false;
    }

    bool UILayer::OnMouseLeftUp(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UIElement>& pair : this->uiElements) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseLeftUp(a_renderer, a_gameAPI, a_assets, this, a_mousePos)) {
                return true;
            }
        }

        return false;
    }

    bool UILayer::OnMouseMiddleDown(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UIElement>& pair : this->uiElements) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseMiddleDown(a_renderer, a_gameAPI, a_assets, this, a_mousePos)) {
                return true;
            }
        }

        return false;
    }

    bool UILayer::OnMouseMiddleUp(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UIElement>& pair : this->uiElements) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseMiddleUp(a_renderer, a_gameAPI, a_assets, this, a_mousePos)) {
                return true;
            }
        }

        return false;
    }

    bool UILayer::OnMouseRightDown(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UIElement>& pair : this->uiElements) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseRightDown(a_renderer, a_gameAPI, a_assets, this, a_mousePos)) {
                return true;
            }
        }

        return false;
    }

    bool UILayer::OnMouseRightUp(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UIElement>& pair : this->uiElements) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseRightUp(a_renderer, a_gameAPI, a_assets, this, a_mousePos)) {
                return true;
            }
        }

        return false;
    }

    void UILayer::Recalculate(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets) {
        for (std::pair<const std::string, UIElement>& pair : this->uiElements) {
            pair.second.Recalculate(a_renderer, a_gameAPI, a_assets, this);
        }
    }
}