#include "tools/sdl/Renderer.hpp"
#include "tools/utils/Log.hpp"

namespace tools::sdl {

static auto logger = tools::utils::new_logger("Renderer");

Renderer::Renderer() {}

Renderer::Renderer(const std::string &title, int width, int height) {
    _window_title = title;
    _width = width;
    _height = height;
}

Renderer::~Renderer() {}

bool Renderer::init() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        logger->error("Failed to initialize SDL.");
        return false;
    }

    if(TTF_Init() != 0) {
        logger->error("Failed to initialize TTF.");
        return false;
    }

    if(!get_screen_size()) {
        logger->error("Failed to get screen's size.");
        return false;
    }

    logger->info("Screen size = ({}, {})", _screen_width, _screen_height);

    if (_width > _screen_width)
        _width = _screen_width;
    if (_height > _screen_height)
        _height = _screen_height;

    _window = SDL_CreateWindow(
        _window_title.c_str(),
        _screen_width / 2 - _width / 2,
        _screen_height / 2 - _height / 2,
        _width,
        _height,
        0
    // #ifdef WINDOWS
    //     SDL_WINDOW_BORDERLESS
    // #elif defined(DEBUG)
    //     SDL_WINDOW_BORDERLESS
    // #else
    //     SDL_WINDOW_FULLSCREEN
    // #endif
    );

    if(_window == nullptr) {
        logger->error("Failed to create window.");
        return false;
    }

    _renderer = SDL_CreateRenderer(
        _window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if(_renderer == nullptr) {
        logger->error("Failed to create renderer.");
        return false;
    }

    logger->info("Width = {}, height = {}", get_width(), get_height());

    logger->info("Init done.");
    return true;
}

void Renderer::stop() {
    SDL_Quit();
}

void Renderer::refresh() {
    SDL_RenderPresent(_renderer);
}

bool Renderer::clear() {
    if(SDL_RenderClear(_renderer) == -1) {
        logger->error("Failed to clear renderer.");
        return false;
    }
    return true;
}

void Renderer::set_width(int width) {
    _width = width;
}

void Renderer::set_height(int height) {
    _height = height;
}

void Renderer::set_size(int width, int height) {
    _width = width;
    _height = height;
}

SDL_Texture* Renderer::get_render_target() {
    return SDL_GetRenderTarget(_renderer);
}

bool Renderer::set_render_target(SDL_Texture* dst) {
    if(SDL_SetRenderTarget(_renderer, dst) == -1) {
        logger->error("Failed to set rendering target.");
        return false;
    }
    return true;
}

bool Renderer::set_viewport(SDL_Rect* rect) {
    if(rect == nullptr) {
        logger->error("Cannot set viewport, nullptr.");
        return false;
    }

    if(SDL_RenderSetViewport(_renderer, rect) == -1) {
        logger->error("Failed to set viewport.");
        return false;
    }
    
    logger->info("Set viewport to h = {}, w = {}, x = {}, y = {}", rect->h, rect->w, rect->x, rect->y);
    return true;
}

bool Renderer::set_draw_color(SDL_Color& color) {
    return set_draw_color(color.r, color.g, color.b, color.a);   
}

bool Renderer::set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if(SDL_SetRenderDrawColor(
        _renderer,
        r,
        g,
        b,
        a
    ) == -1) {
        logger->error("Failed to set draw color.");
        return false;
    }

    return true; 
}

TTF_Font* Renderer::load_font(std::string font_path, int size) {
    if(font_path.empty()) {
        logger->error("Cannot load font, empty path.");
        return nullptr;
    }

    TTF_Font* font = TTF_OpenFont(font_path.c_str(), size);
    if(font == nullptr)
        logger->error("Failed to load font '{}'", font_path);
    else
        logger->info("Loaded font '{}'", font_path);
    return font;
}

SDL_Texture* Renderer::load_image(std::string img_path) {
    if(img_path.empty()) {
        logger->error("Cannot load image, empty path.");
        return nullptr;
    }

    SDL_Surface* surface = SDL_LoadBMP(img_path.c_str());
    if(surface == nullptr) {
        logger->error("Failed to create surface from image '{}'", img_path);
        return nullptr;
    }

    SDL_Texture* texture = surface_to_texture(surface);
    if(texture == nullptr) {
        logger->error("Failed to create texture from image '{}'", img_path);
        return nullptr;
    }

    logger->info("Loaded image '{}'", img_path);
    return texture;
}

SDL_Texture* Renderer::load_text(
    std::string text,
    SDL_Color color,
    TTF_Font* font
) {
    SDL_Surface* surface = nullptr;
    if(font == nullptr) {
        if(_default_font == nullptr)
        {
            logger->error("Cannot load text, no default font and no font provided.");
            return nullptr;
        }
        surface = TTF_RenderText_Solid(_default_font, text.c_str(), color);
    }
    else {
        surface = TTF_RenderText_Solid(font, text.c_str(), color);
    }

    if(surface == nullptr) {
        logger->error("Failed to create surface from text.");
        return nullptr;
    }

    SDL_Texture* texture = surface_to_texture(surface);
    if(texture == nullptr) {
        logger->error("Failed to create texture from image.");
        return nullptr;
    }
    
    logger->info("Loaded text '{}'", text);
    return texture;
}

bool Renderer::render_texture(SDL_Texture* texture, SDL_Rect* dst, SDL_Rect* portion) {
    if(texture == nullptr) {
        logger->error("Cannot render texture, nullptr.");
        return false;
    }

    if(SDL_RenderCopy(_renderer, texture, portion, dst) == -1) {
        logger->error("Failed to render texture.");
        return false;
    }
    return true;
}

bool Renderer::crop_texture(SDL_Texture* src, SDL_Texture*& dst, SDL_Rect* rect) {
    if(src == nullptr) {
        logger->error("Cannot crop texture, source = nullptr.");
        return false;
    }

    if(rect == nullptr) {
        logger->error("Cannot crop texture, rectangle = nullptr.");
        return false;
    } 

    if(rect->h == 0 || rect->w == 0) {
        logger->error("Cannot crop texture, rectangle has either height or width at 0.");
        return false;
    }

    dst = this->create_blank_render_target(rect->w, rect->h);

    if(dst == nullptr) {
        logger->error("Failed to create target texture.");
        return false;
    }

    if(!this->render_to_texture(src, dst, rect)) {
        logger->error("Failed to crop texture.");
        return false;
    }

    return true;
}

bool Renderer::get_screen_size() {
    SDL_DisplayMode mode = SDL_DisplayMode();
    if(SDL_GetDesktopDisplayMode(0, &mode) == -1) {
        logger->error("Failed to get display information.");
        return false;
    }
    _screen_width = mode.w;
    _screen_height = mode.h;
    return true;
}

SDL_Texture* Renderer::surface_to_texture(SDL_Surface* surface) {
    if(surface == nullptr) {
        logger->error("Cannot create texture from surface, nullptr.");
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if(texture == nullptr) {
        logger->error("Failed to create texture from surface.");
        return nullptr;
    }

    SDL_FreeSurface(surface);
    return texture;
}

bool Renderer::render_to_texture(SDL_Texture* src, SDL_Texture* dst, SDL_Rect* dstRect) {
    // Backup rendering target.
    SDL_Texture* target = this->get_render_target();

    // Set render target to destination texture.
    if(!this->set_render_target(dst)) {
        logger->error("Failed to set rendering target prior to render to texture.");
        return false;
    }

    if(this->render_texture(src, nullptr, dstRect) == false) {
        logger->error("Failed to render texture to target texture.");
        return false;
    }

    // Restore rendering target.
    if(!this->set_render_target(target)) {
        logger->error("Failed to set rendering target back to default.");
        return false;
    }

    return true;
}

SDL_Texture* Renderer::create_blank_render_target(int width, int height) {
    return SDL_CreateTexture(_renderer, SDL_GetWindowPixelFormat(_window), SDL_TEXTUREACCESS_TARGET, width, height);
}

bool Renderer::draw_rectangle(SDL_Rect* rect, bool fill) {
    if(rect == nullptr) {
        logger->error("draw_rectangle : rect = nullptr.");
        return false;
    }

    if(SDL_RenderDrawRect(_renderer, rect) == -1) {
        logger->error("draw_rectangle : Failed to render rectangle.");
        return false;
    }

    if (fill && SDL_RenderFillRect(_renderer, rect) == -1) {
        logger->error("Failed to fill rectangle.");
        return false;
    }

    return true;
}

} // namespace tools::sdl