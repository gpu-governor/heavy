#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <stdint.h>
#include"color.h"

//--------------------------GLOBAL VARIABLES-----------------------------------------

typedef struct {
    uint8_t r, g, b, a;
} Color;

// Global SDL state
static SDL_Window* main_window = NULL;
static SDL_Renderer* main_renderer = NULL;
static bool main_window_should_close = false;
static Uint32 main_target_frame_time = 0;

// Global font for text rendering
static TTF_Font* main_font = NULL;

//--------------------------UTILITY FUNCTIONS-----------------------------------------
// Initialize the window, renderer, and font
void init_window(int width, int height, const char *title, int target_fps) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return;
    }

    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return;
    }

    main_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!main_window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return;
    }

    main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!main_renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(main_window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // Load the default font
    main_font = TTF_OpenFont("FreeMono.ttf", 24);
    if (!main_font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        SDL_DestroyRenderer(main_renderer);
        SDL_DestroyWindow(main_window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // Set target frame time
    if (target_fps > 0) {
        main_target_frame_time = 1000 / target_fps;
    }
}

// Check if the window should close
bool window_should_close() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            main_window_should_close = true;
        }
    }
    return main_window_should_close;
}

// Begin drawing (optional setup, placeholder for future use)
void start_drawing() {
    // Placeholder to mimic Raylib's flow
}

// Set the background color
void clear_screen(Color color) {
    SDL_SetRenderDrawColor(main_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(main_renderer);
}

// Draw a rectangle
void draw_rect(int x, int y, int width, int height, Color color) {
    SDL_SetRenderDrawColor(main_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(main_renderer, &rect);
}

// Draw a circle
void draw_circle(int x, int y, int radius, Color color) {
    SDL_SetRenderDrawColor(main_renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // Horizontal offset
            int dy = radius - h; // Vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(main_renderer, x + dx, y + dy);
            }
        }
    }
}

// Draw text using the global font
void draw_text(const char *text, int x, int y, Color color) {
    if (!main_font) return; // Ensure the font is loaded
    SDL_Color sdl_color = {color.r, color.g, color.b, color.a};
    SDL_Surface *surface = TTF_RenderText_Solid(main_font, text, sdl_color);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(main_renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    if (texture) {
        SDL_RenderCopy(main_renderer, texture, NULL, &dest);
        SDL_DestroyTexture(texture);
    }
}

// End drawing and present to the screen (with FPS delay)
void stop_drawing() {
    SDL_RenderPresent(main_renderer);

    // Delay to maintain FPS
    if (main_target_frame_time > 0) {
        static Uint32 last_tick = 0;
        Uint32 elapsed = SDL_GetTicks() - last_tick;
        if (elapsed < main_target_frame_time) {
            SDL_Delay(main_target_frame_time - elapsed);
        }
        last_tick = SDL_GetTicks();
    }
}

// Close and clean up SDL and font
void quit_window() {
    if (main_font) TTF_CloseFont(main_font);
    if (main_renderer) SDL_DestroyRenderer(main_renderer);
    if (main_window) SDL_DestroyWindow(main_window);
    TTF_Quit();
    SDL_Quit();
}

//--------------------------CLASS OBJ--------------------------------------------------

/**
 * @class Obj
 * @brief Represents a general object that can be rendered with static or animated textures.
 */
class Obj {
public:
    int x, y;                      // Position
    float scale;                   // Scale for rendering
    std::vector<SDL_Texture*> textures; // Textures (single or multiple for animation)
    int current_frame;             // Current animation frame
    float frame_time;              // Time per frame (defaults to 1.0f for static objects)
    float elapsed_time;            // Time accumulator

    Obj(const std::string& path, int x_pos, int y_pos, float scale_factor, float frame_duration = 1.0f)
        : x(x_pos), y(y_pos), scale(scale_factor), current_frame(0), frame_time(frame_duration), elapsed_time(0.0f) {
        SDL_Texture* texture = IMG_LoadTexture(main_renderer, path.c_str());
        if (texture) {
            textures.push_back(texture);
        } else {
            throw std::runtime_error("Failed to load texture: " + path);
        }
    }

    Obj(const std::vector<std::string>& paths, int x_pos, int y_pos, float scale_factor, float frame_duration)
        : x(x_pos), y(y_pos), scale(scale_factor), current_frame(0), frame_time(frame_duration), elapsed_time(0.0f) {
        for (const auto& path : paths) {
            SDL_Texture* texture = IMG_LoadTexture(main_renderer, path.c_str());
            if (texture) {
                textures.push_back(texture);
            } else {
                throw std::runtime_error("Failed to load texture: " + path);
            }
        }
    }

    ~Obj() {
        for (auto& texture : textures) {
            SDL_DestroyTexture(texture);
        }
    }

    void render(float delta_time = 0.0f) {
        if (textures.empty()) return;

        if (textures.size() > 1) {
            elapsed_time += delta_time;
            if (elapsed_time >= frame_time) {
                current_frame = (current_frame + 1) % textures.size();
                elapsed_time = 0.0f;
            }
        }

        SDL_Texture* texture = textures[current_frame];
        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        SDL_Rect dst = {
            x,
            y,
            static_cast<int>(width * scale),
            static_cast<int>(height * scale)
        };
        SDL_RenderCopy(main_renderer, texture, nullptr, &dst);
    }
};

//--------------------------CLASS OBJ_SPRITE SHEET-----------------------------------

/**
 * @class Obj_ss
 * @brief Represents a sprite sheet object, supporting static and animated tiles.
 */
class Obj_ss : public Obj {
public:
    int tile_width;         // Tile width
    int tile_height;        // Tile height
    int frame_count;        // Number of animation frames
    int row_offset;         // Starting row offset

    // Original constructor (default row_offset = 0)
    Obj_ss(const std::string& path, int x_pos, int y_pos, float scale_factor, 
           int t_width, int t_height, int frames = 1, float frame_duration = 1.0f)
        : Obj(path, x_pos, y_pos, scale_factor, frame_duration),
          tile_width(t_width), tile_height(t_height), frame_count(frames), row_offset(0) {}

    // Overloaded constructor with row offset parameter
    Obj_ss(const std::string& path, int x_pos, int y_pos, float scale_factor, 
           int t_width, int t_height, int frames, float frame_duration, int start_row)
        : Obj(path, x_pos, y_pos, scale_factor, frame_duration),
          tile_width(t_width), tile_height(t_height), frame_count(frames), row_offset(start_row) {}

    void render(float delta_time = 0.0f) {
        if (textures.empty() || tile_width <= 0 || tile_height <= 0) return;

        if (frame_count > 1) {
            elapsed_time += delta_time;
            if (elapsed_time >= frame_time) {
                current_frame = (current_frame + 1) % frame_count;
                elapsed_time = 0.0f;
            }
        }

        SDL_Texture* texture = textures[0];
        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        int frames_per_row = width / tile_width;

        int tile_x = current_frame % frames_per_row;
        int tile_y = (current_frame / frames_per_row) + row_offset; // Use row_offset here

        SDL_Rect src = {
            tile_x * tile_width,
            tile_y * tile_height,
            tile_width,
            tile_height
        };
        SDL_Rect dst = {
            x,
            y,
            static_cast<int>(tile_width * scale),
            static_cast<int>(tile_height * scale)
        };
        SDL_RenderCopy(main_renderer, texture, &src, &dst);
    }
};


//--------------------------MAIN-----------------------------------------------------
// 
// int main() {
//     init_window(800, 600, "Simplified Game", 60);
// 
//     Obj_ss static_tile("img/Attack1.png", 300, 100, 2.0f, 126, 126); // Single tile
//     Obj_ss animated_tile("img/Attack1.png", 500, 100, 2.0f, 126, 126, 7, 0.1f); // Animated tile
//     Obj static_sprite("img/player/Idle/0.png", 100, 100, 2.0f);
// 
//     std::vector<std::string> idle_frames = {
//         "img/player/Idle/0.png",
//         "img/player/Idle/1.png",
//         "img/player/Idle/2.png",
//         "img/player/Idle/3.png"
//     };
//     Obj animated_sprite(idle_frames, 300, 300, 2.0f, 0.2f);
// 
//     while (!window_should_close()) {
//         float delta_time = 1.0f / 60.0f; // Simulate frame time
// 
//         start_drawing();
//         clear_screen(COLOR_WHITE);
// 
//         static_sprite.render();
//         static_tile.render(delta_time);
//         animated_sprite.render(delta_time);
//         animated_tile.render(delta_time);
// 
//         stop_drawing();
//     }
// 
//     quit_window();
//     return 0;
// }
// 


