#include <TeraToma/UI/UITextComponent.h>
#include <TeraToma/UI/UIElement.h>
#include <TeraToma/UI/UIManager.h>
#include <print>

namespace TeraToma::UI {
    UITextComponent::UITextComponent(std::string a_text, std::string a_font, int a_margins, UIAnchor a_anchor) {
        text = a_text;
        fontName = a_font;
        margins = a_margins;
        textAnchor = a_anchor;
    }

    void UITextComponent::Hookup(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, UIElement* a_element) {
        std::println("Woah: \"{}\"", this->text.c_str());
        SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(a_assets->fonts.at(fontName)->font, this->text.c_str(), this->text.size(), {255, 255, 255, SDL_ALPHA_OPAQUE}, (int)a_element->displayArea.GetWidth() - margins * 2);
        textTexture = SDL_CreateTextureFromSurface(a_renderer, surface);
        SDL_DestroySurface(surface);

        a_element->onRecalculate.emplace_back([this](SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, UIElement* a_element) {
            SDL_DestroyTexture(textTexture);
            SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(a_assets->fonts.at(fontName)->font, this->text.c_str(), this->text.size(), {255, 255, 255, SDL_ALPHA_OPAQUE}, (int)a_element->displayArea.GetWidth() - margins * 2);
            w = surface->w;
            h = surface->h;
            textTexture = SDL_CreateTextureFromSurface(a_renderer, surface);
            SDL_DestroySurface(surface);

            textArea = {{a_element->displayArea.GetX() + margins, a_element->displayArea.GetY() + margins, (float)w, (float)h}};
            switch (textAnchor) {
                case UIAnchor::TOP_LEFT:
                    textArea.SetTopLeft(a_element->displayArea.GetTopLeft());
                    textArea.SetX(textArea.GetX() + margins);
                    textArea.SetY(textArea.GetY() + margins);
                    break;
                case UIAnchor::TOP_CENTER:
                    textArea.SetTopCenter(a_element->displayArea.GetTopCenter());
                    textArea.SetY(textArea.GetY() + margins);
                    break;
                case UIAnchor::TOP_RIGHT:
                    textArea.SetTopRight(a_element->displayArea.GetTopRight());
                    textArea.SetX(textArea.GetX() - margins);
                    textArea.SetY(textArea.GetY() + margins);
                    break;
                case UIAnchor::MIDDLE_LEFT:
                    textArea.SetMiddleLeft(a_element->displayArea.GetMiddleLeft());
                    textArea.SetX(textArea.GetX() + margins);
                    break;
                case UIAnchor::MIDDLE_CENTER:
                    textArea.SetMiddleCenter(a_element->displayArea.GetMiddleCenter());
                    break;
                case UIAnchor::MIDDLE_RIGHT:
                    textArea.SetMiddleRight(a_element->displayArea.GetMiddleRight());
                    textArea.SetX(textArea.GetX() - margins);
                    break;
                case UIAnchor::BOTTOM_LEFT:
                    textArea.SetBottomLeft(a_element->displayArea.GetBottomLeft());
                    textArea.SetX(textArea.GetX() + margins);
                    textArea.SetY(textArea.GetY() - margins);
                    break;
                case UIAnchor::BOTTOM_CENTER:
                    textArea.SetBottomCenter(a_element->displayArea.GetBottomCenter());
                    textArea.SetY(textArea.GetY() - margins);
                    break;
                case UIAnchor::BOTTOM_RIGHT:
                    textArea.SetBottomRight(a_element->displayArea.GetBottomRight());
                    textArea.SetX(textArea.GetX() - margins);
                    textArea.SetY(textArea.GetY() - margins);
                    break;
            }
        });

        a_element->onRender.emplace_back([this](SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, UIElement* a_element) {
            SDL_RenderTexture(a_renderer, textTexture, NULL, &textArea.rect);
        });
        
       SetAnchor(a_renderer, a_assets, a_element, textAnchor);
    }

    void UITextComponent::SetText(SDL_Renderer* a_renderer, Assets::Assets* a_assets, UIElement* a_element, std::string a_text) {
        text = a_text;
        a_assets->uiManager->dirtyRecalculate = true;
    }

    void UITextComponent::SetAnchor(SDL_Renderer* a_renderer, Assets::Assets* a_assets, UIElement* a_element, UIAnchor a_anchor) {
        textAnchor = a_anchor;
        a_assets->uiManager->dirtyRecalculate = true;
    }
}