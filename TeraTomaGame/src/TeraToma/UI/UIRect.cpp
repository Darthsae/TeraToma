#include <TeraToma/UI/UIRect.h>

namespace TeraToma::UI {
    void UIRect::SetTopLeft(SDL_FPoint a_point) {
        rect.x = a_point.x;
        rect.y = a_point.y;
    }

    void UIRect::SetTopCenter(SDL_FPoint a_point) {
        SetCenterX(a_point.x);
        rect.y = a_point.y;
    }

    void UIRect::SetTopRight(SDL_FPoint a_point) {
        SetRightX(a_point.x);
        rect.y = a_point.y;
    }

    void UIRect::SetMiddleLeft(SDL_FPoint a_point) {
        rect.x = a_point.x;
        SetCenterY(a_point.y);
    }

    void UIRect::SetMiddleCenter(SDL_FPoint a_point) {
        SetCenterX(a_point.x);
        SetCenterY(a_point.y);
    }

    void UIRect::SetMiddleRight(SDL_FPoint a_point) {
        SetRightX(a_point.x);
        SetCenterY(a_point.y);
    }

    void UIRect::SetBottomLeft(SDL_FPoint a_point) {
        rect.x = a_point.x;
        SetBottomY(a_point.y);
    }

    void UIRect::SetBottomCenter(SDL_FPoint a_point) {
        SetCenterX(a_point.x);
        SetBottomY(a_point.y);
    }

    void UIRect::SetBottomRight(SDL_FPoint a_point) {
        SetRightX(a_point.x);
        SetBottomY(a_point.y);
    }

    SDL_FPoint UIRect::GetTopLeft(void) {
        return {rect.x, rect.y};
    }

    SDL_FPoint UIRect::GetTopCenter(void) {
        return {GetCenterX(), rect.y};
    }

    SDL_FPoint UIRect::GetTopRight(void) {
        return {GetRightX(), rect.y};
    }

    SDL_FPoint UIRect::GetMiddleLeft(void) {
        return {rect.x, GetCenterY()};
    }

    SDL_FPoint UIRect::GetMiddleCenter(void) {
        return {GetCenterX(), GetCenterY()};
    }

    SDL_FPoint UIRect::GetMiddleRight(void) {
        return {GetRightX(), GetCenterY()};
    }

    SDL_FPoint UIRect::GetBottomLeft(void) {
        return {rect.x, GetBottomY()};
    }

    SDL_FPoint UIRect::GetBottomCenter(void) {
        return {GetCenterX(), GetBottomY()};
    }

    SDL_FPoint UIRect::GetBottomRight(void) {
        return {GetRightX(), GetBottomY()};
    }

    void UIRect::SetX(float a_x) {
        rect.x = a_x;
    }

    void UIRect::SetY(float a_y) {
        rect.y = a_y;
    }

    void UIRect::SetCenterX(float a_x) {
        rect.x = a_x - GetHalfWidth();
    }

    void UIRect::SetCenterY(float a_y) {
        rect.y = a_y - GetHalfHeight();
    }

    void UIRect::SetRightX(float a_x) {
        rect.x = a_x - GetWidth();
    }

    void UIRect::SetBottomY(float a_y) {
        rect.y = a_y - GetHeight();
    }

    void UIRect::SetWidth(float a_width) {
        rect.w = a_width;
    }

    void UIRect::SetHeight(float a_height) {
        rect.h = a_height;
    }
    
    void UIRect::SetSize(SDL_FPoint a_size) {
        rect.w = a_size.x;
        rect.h = a_size.y;
    }

    void UIRect::SetHalfWidth(float a_width) {
        rect.w = a_width * 2;
    }

    void UIRect::SetHalfHeight(float a_height) {
        rect.h = a_height * 2;
    }

    void UIRect::SetHalfSize(SDL_FPoint a_size) {
        SetHalfWidth(a_size.x);
        SetHalfHeight(a_size.y);
    }

    float UIRect::GetX(void) {
        return rect.x;
    }

    float UIRect::GetY(void) {
        return rect.y;
    }

    float UIRect::GetCenterX(void) {
        return rect.x + GetHalfWidth();
    }

    float UIRect::GetCenterY(void) {
        return rect.y + GetHalfHeight();
    }

    float UIRect::GetRightX(void) {
        return rect.x + rect.w;
    }

    float UIRect::GetBottomY(void) {
        return rect.y + rect.h;
    }

    float UIRect::GetWidth(void) {
        return rect.w;
    }

    float UIRect::GetHeight(void) {
        return rect.h;
    }

    SDL_FPoint UIRect::GetSize(void) {
        return {rect.w, rect.h};
    }

    float UIRect::GetHalfWidth(void) {
        return rect.w * 0.5f;
    }

    float UIRect::GetHalfHeight(void) {
        return rect.h * 0.5f;
    }

    SDL_FPoint UIRect::GetHalfSize(void) {
        return {GetHalfWidth(), GetHalfHeight()};
    }
}