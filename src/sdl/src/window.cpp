#include "tools/sdl/window.hpp"

namespace tools::sdl {

std::atomic<bool> Window::_sdl_initialized = false;
std::atomic<int> Window::_instances_count = 0;

bool Window::sdl_init() {
    bool ret = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        ret = false;
    }
    _sdl_initialized = ret;
    return ret;
}

void Window::sdl_cleanup() {
    SDL_Quit();
    _sdl_initialized = false;
}

Window::Window() {
    _width = 640;
    _height = 320;
    init();
}

Window::Window(const std::string& title, int width, int height) {
    _window_title = title;
    _width = width;
    _height = height;
    init();
}

Window::~Window() {
    if (_initialized)
        stop();
}

bool Window::init() {
    if (_instances_count == 0)
        sdl_init();

    if (TTF_Init() != 0) {
        return false;
    }

    if (!get_screen_size()) {
        return false;
    }

    if (_width > _screen_width)
        _width = _screen_width;
    if (_height > _screen_height)
        _height = _screen_height;

    _window = SDL_CreateWindow(_window_title.c_str(), SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, _width, _height,
                               SDL_WINDOW_RESIZABLE);

    if (_window == nullptr) {
        return false;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

    if (_renderer == nullptr) {
        return false;
    }

    _instances_count++;
    _initialized = true;
    return true;
}

void Window::stop() {
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    _initialized = false;

    _instances_count--;
    if (_instances_count == 0) {
        sdl_cleanup();
    }
}

void Window::refresh() {
    SDL_RenderPresent(_renderer);
}

bool Window::clear() {
    if (SDL_RenderClear(_renderer) == -1) {
        return false;
    }
    return true;
}

void Window::set_width(int width) {
    _width = width;
}

void Window::set_height(int height) {
    _height = height;
}

void Window::set_size(int width, int height) {
    _width = width;
    _height = height;
}

void Window::set_position(int x, int y) {
    SDL_SetWindowPosition(_window, x, y);
}

void Window::center() {
    SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED);
}

SDL_Texture* Window::get_render_target() {
    return SDL_GetRenderTarget(_renderer);
}

bool Window::set_render_target(SDL_Texture* dst) {
    if (SDL_SetRenderTarget(_renderer, dst) == -1) {
        return false;
    }
    return true;
}

bool Window::set_viewport(SDL_Rect* rect) {
    if (rect == nullptr) {
        return false;
    }

    if (SDL_RenderSetViewport(_renderer, rect) == -1) {
        return false;
    }

    return true;
}

bool Window::set_draw_color(SDL_Color& color) {
    return set_draw_color(color.r, color.g, color.b, color.a);
}

bool Window::set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (SDL_SetRenderDrawColor(_renderer, r, g, b, a) == -1) {
        return false;
    }

    return true;
}

TTF_Font* Window::load_font(std::string font_path, int size) {
    if (font_path.empty()) {
        return nullptr;
    }

    TTF_Font* font = TTF_OpenFont(font_path.c_str(), size);
    return font;
}

SDL_Texture* Window::load_image(std::string img_path) {
    if (img_path.empty()) {
        return nullptr;
    }

    SDL_Surface* surface = SDL_LoadBMP(img_path.c_str());
    if (surface == nullptr) {
        return nullptr;
    }

    SDL_Texture* texture = surface_to_texture(surface);
    if (texture == nullptr) {
        return nullptr;
    }

    return texture;
}

SDL_Texture* Window::load_text(std::string text, SDL_Color color,
                               TTF_Font* font) {
    SDL_Surface* surface = nullptr;
    if (font == nullptr) {
        if (_default_font == nullptr) {
            return nullptr;
        }
        surface = TTF_RenderText_Solid(_default_font, text.c_str(), color);
    } else {
        surface = TTF_RenderText_Solid(font, text.c_str(), color);
    }

    if (surface == nullptr) {
        return nullptr;
    }

    SDL_Texture* texture = surface_to_texture(surface);
    if (texture == nullptr) {
        return nullptr;
    }

    return texture;
}

bool Window::render_texture(SDL_Texture* texture, SDL_Rect* dst,
                            SDL_Rect* portion) {
    if (texture == nullptr) {
        return false;
    }

    if (SDL_RenderCopy(_renderer, texture, portion, dst) == -1) {
        return false;
    }
    return true;
}

bool Window::crop_texture(SDL_Texture* src, SDL_Texture*& dst, SDL_Rect* rect) {
    if (src == nullptr) {
        return false;
    }

    if (rect == nullptr) {
        return false;
    }

    if (rect->h == 0 || rect->w == 0) {
        return false;
    }

    dst = this->create_blank_render_target(rect->w, rect->h);

    if (dst == nullptr) {
        return false;
    }

    if (!this->render_to_texture(src, dst, rect)) {
        return false;
    }

    return true;
}

bool Window::get_screen_size() {
    SDL_DisplayMode mode = SDL_DisplayMode();
    if (SDL_GetDesktopDisplayMode(0, &mode) == -1) {
        return false;
    }
    _screen_width = mode.w;
    _screen_height = mode.h;
    return true;
}

SDL_Texture* Window::surface_to_texture(SDL_Surface* surface) {
    if (surface == nullptr) {
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if (texture == nullptr) {
        return nullptr;
    }

    SDL_FreeSurface(surface);
    return texture;
}

bool Window::render_to_texture(SDL_Texture* src, SDL_Texture* dst,
                               SDL_Rect* dstRect) {
    // Backup rendering target.
    SDL_Texture* target = this->get_render_target();

    // Set render target to destination texture.
    if (!this->set_render_target(dst)) {
        return false;
    }

    if (this->render_texture(src, nullptr, dstRect) == false) {
        return false;
    }

    // Restore rendering target.
    if (!this->set_render_target(target)) {
        return false;
    }

    return true;
}

SDL_Texture* Window::create_blank_render_target(int width, int height) {
    return SDL_CreateTexture(_renderer, SDL_GetWindowPixelFormat(_window),
                             SDL_TEXTUREACCESS_TARGET, width, height);
}

bool Window::draw_rectangle(SDL_Rect* rect, bool fill) {
    if (rect == nullptr) {
        return false;
    }

    if (SDL_RenderDrawRect(_renderer, rect) == -1) {
        return false;
    }

    if (fill && SDL_RenderFillRect(_renderer, rect) == -1) {
        return false;
    }

    return true;
}

} // namespace tools::sdl