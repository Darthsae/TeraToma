#include <TeraToma/UI/UIManager.h>

namespace TeraToma::UI { 
    UIManager::UIManager() {
        uiLayers = std::unordered_map<std::string, UILayer>();
    }

    void UIManager::Render(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets) {
        for (std::pair<const std::string, UILayer>& pair : this->uiLayers) {
            if (!pair.second.enabled) {
                continue;
            }

            pair.second.Render(a_renderer, a_gameAPI, a_assets);
        }
    }

    bool UIManager::OnMouseLeftDown(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UILayer>& pair : this->uiLayers) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseLeftDown(a_renderer, a_gameAPI, a_assets, a_mousePos)) {
                return true;
            }
        }
        
        return false;
    }

    bool UIManager::OnMouseLeftUp(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UILayer>& pair : this->uiLayers) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseLeftUp(a_renderer, a_gameAPI, a_assets, a_mousePos)) {
                return true;
            }
        }
        
        return false;
    }

    bool UIManager::OnMouseMiddleDown(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UILayer>& pair : this->uiLayers) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseMiddleDown(a_renderer, a_gameAPI, a_assets, a_mousePos)) {
                return true;
            }
        }
        
        return false;
    }

    bool UIManager::OnMouseMiddleUp(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UILayer>& pair : this->uiLayers) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseMiddleUp(a_renderer, a_gameAPI, a_assets, a_mousePos)) {
                return true;
            }
        }
        
        return false;
    }

    bool UIManager::OnMouseRightDown(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UILayer>& pair : this->uiLayers) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseRightDown(a_renderer, a_gameAPI, a_assets, a_mousePos)) {
                return true;
            }
        }
        
        return false;
    }

    bool UIManager::OnMouseRightUp(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, SDL_FPoint* a_mousePos) {
        for (std::pair<const std::string, UILayer>& pair : this->uiLayers) {
            if (!pair.second.enabled) {
                continue;
            }

            if (pair.second.OnMouseRightUp(a_renderer, a_gameAPI, a_assets, a_mousePos)) {
                return true;
            }
        }
        
        return false;
    }

    void UIManager::Recalculate(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets) {
        dirtyRecalculate = false;
        for (std::pair<const std::string, UILayer>& pair : this->uiLayers) {
            pair.second.Recalculate(a_renderer, a_gameAPI, a_assets);
        }
    }
}