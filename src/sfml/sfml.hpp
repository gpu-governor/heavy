#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cstdint>
#include"color.h"
//--------------------------GLOBAL VARIABLES-----------------------------------------

// Global SFML state
static sf::RenderWindow main_window;
static bool main_window_should_close = false;
static sf::Font main_font;

struct Color {
    uint8_t r, g, b, a;
};

//--------------------------UTILITY FUNCTIONS-----------------------------------------

// Initialize the window and font
void init_window(int width, int height, const char* title, int target_fps) {
    main_window.create(sf::VideoMode(width, height), title);
    main_window.setFramerateLimit(target_fps);

    if (!main_font.loadFromFile("FreeMono.ttf")) {
        throw std::runtime_error("Failed to load font: FreeMono.ttf");
    }
}

// Check if the window should close
bool window_should_close() {
    sf::Event event;
    while (main_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
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
    main_window.clear(sf::Color(color.r, color.g, color.b, color.a));
}

// Draw a rectangle
void draw_rect(int x, int y, int width, int height, Color color) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    main_window.draw(rect);
}

// Draw a circle
void draw_circle(int x, int y, int radius, Color color) {
    sf::CircleShape circle(radius);
    circle.setPosition(x - radius, y - radius);
    circle.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    main_window.draw(circle);
}

// Draw text using the global font
void draw_text(const char* text, int x, int y, Color color) {
    sf::Text sf_text(text, main_font, 24);
    sf_text.setPosition(x, y);
    sf_text.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    main_window.draw(sf_text);
}

// End drawing and present to the screen
void stop_drawing() {
    main_window.display();
}

// Close and clean up
void quit_window() {
    main_window.close();
}

//--------------------------CLASS OBJ--------------------------------------------------

/**
 * @class Obj
 * @brief Represents a general object that can be rendered with static or animated textures.
 */
class Obj {
public:
    int x, y;                        // Position
    float scale;                     // Scale for rendering
    std::vector<sf::Texture> textures; // Textures (single or multiple for animation)
    int current_frame;               // Current animation frame
    float frame_time;                // Time per frame (defaults to 1.0f for static objects)
    float elapsed_time;              // Time accumulator
    sf::Sprite sprite;               // Sprite for rendering

    Obj(const std::string& path, int x_pos, int y_pos, float scale_factor, float frame_duration = 1.0f)
        : x(x_pos), y(y_pos), scale(scale_factor), current_frame(0), frame_time(frame_duration), elapsed_time(0.0f) {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        textures.push_back(std::move(texture));
        sprite.setTexture(textures[0]);
        sprite.setScale(scale, scale);
    }

    Obj(const std::vector<std::string>& paths, int x_pos, int y_pos, float scale_factor, float frame_duration)
        : x(x_pos), y(y_pos), scale(scale_factor), current_frame(0), frame_time(frame_duration), elapsed_time(0.0f) {
        for (const auto& path : paths) {
            sf::Texture texture;
            if (!texture.loadFromFile(path)) {
                throw std::runtime_error("Failed to load texture: " + path);
            }
            textures.push_back(std::move(texture));
        }
        sprite.setTexture(textures[0]);
        sprite.setScale(scale, scale);
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
        sprite.setTexture(textures[current_frame]);
        sprite.setPosition(x, y);
        main_window.draw(sprite);
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

        int frames_per_row = textures[0].getSize().x / tile_width;

        int tile_x = (current_frame % frames_per_row) * tile_width;
        int tile_y = ((current_frame / frames_per_row) + row_offset) * tile_height;

        sprite.setTextureRect(sf::IntRect(tile_x, tile_y, tile_width, tile_height));
        sprite.setPosition(x, y);
        sprite.setScale(scale, scale);
        main_window.draw(sprite);
    }
};
//--------------------------MAIN-----------------------------------------------------

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
