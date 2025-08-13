#include <TeraToma/UI/UIElement.h>

namespace TeraToma::UI { 
    UIElement::UIElement(std::string_view a_id, UIRect a_internalArea) {
        id = std::string(a_id);
        enabled = true;
        internalArea = a_internalArea;
    }

    void UIElement::Render(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer) {
        for (const UIEventFnptr& ptr : onRender) {
            ptr(a_renderer, a_gameAPI, a_assets, a_uiLayer, this);
        }
    }
    
    bool UIElement::OnMouseLeftDown(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, SDL_FPoint* a_mousePos) {
        if (!SDL_PointInRectFloat(a_mousePos, &displayArea.rect)) {
            return false;
        }
        
        bool blocking = false;

        for (const UIMousePredicateFnptr& ptr : onMouseLeftDown) {
            blocking = blocking || ptr(a_renderer, a_gameAPI, a_assets, a_uiLayer, this, a_mousePos);
        }

        return blocking;
    }

    bool UIElement::OnMouseLeftUp(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, SDL_FPoint* a_mousePos) {
        if (!SDL_PointInRectFloat(a_mousePos, &displayArea.rect)) {
            return false;
        }
        
        bool blocking = false;

        for (const UIMousePredicateFnptr& ptr : onMouseLeftUp) {
            blocking = blocking || ptr(a_renderer, a_gameAPI, a_assets, a_uiLayer, this, a_mousePos);
        }

        return blocking;
    }

    bool UIElement::OnMouseMiddleDown(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, SDL_FPoint* a_mousePos) {
        if (!SDL_PointInRectFloat(a_mousePos, &displayArea.rect)) {
            return false;
        }
        
        bool blocking = false;

        for (const UIMousePredicateFnptr& ptr : onMouseMiddleDown) {
            blocking = blocking || ptr(a_renderer, a_gameAPI, a_assets, a_uiLayer, this, a_mousePos);
        }

        return blocking;
    }

    bool UIElement::OnMouseMiddleUp(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, SDL_FPoint* a_mousePos) {
        if (!SDL_PointInRectFloat(a_mousePos, &displayArea.rect)) {
            return false;
        }
        
        bool blocking = false;

        for (const UIMousePredicateFnptr& ptr : onMouseMiddleUp) {
            blocking = blocking || ptr(a_renderer, a_gameAPI, a_assets, a_uiLayer, this, a_mousePos);
        }

        return blocking;
    }

    bool UIElement::OnMouseRightDown(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, SDL_FPoint* a_mousePos) {
        if (!SDL_PointInRectFloat(a_mousePos, &displayArea.rect)) {
            return false;
        }
        
        bool blocking = false;

        for (const UIMousePredicateFnptr& ptr : onMouseRightDown) {
            blocking = blocking || ptr(a_renderer, a_gameAPI, a_assets, a_uiLayer, this, a_mousePos);
        }

        return blocking;
    }

    bool UIElement::OnMouseRightUp(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, SDL_FPoint* a_mousePos) {
        if (!SDL_PointInRectFloat(a_mousePos, &displayArea.rect)) {
            return false;
        }
        
        bool blocking = false;

        for (const UIMousePredicateFnptr& ptr : onMouseRightUp) {
            blocking = blocking || ptr(a_renderer, a_gameAPI, a_assets, a_uiLayer, this, a_mousePos);
        }

        return blocking;
    }
    
    void UIElement::Recalculate(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer) {
        int w, h;
        SDL_GetCurrentRenderOutputSize(a_renderer, &w, &h);
        float widthScale = (w / 1920.0f);
        float heightScale = (h / 1080.0f);
        displayArea = {{internalArea.GetX() * widthScale, internalArea.GetY() * heightScale, internalArea.GetWidth() * widthScale, internalArea.GetHeight() * heightScale}};
    }
}