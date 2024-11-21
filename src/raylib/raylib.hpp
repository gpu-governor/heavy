/**
 * @file heavy.hpp
 * @brief This is a single header file only,  for working with game objects (sprite, x, y, animation, ...) in Raylib and c++
 *  uncomment the example main as the end of this file to see it in action
 */
#include <raylib.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include"color.h"


//--------------------------OBJ----------------------------------------------------
/**
 * @class Obj
 * @brief Represents a general object that can be rendered with static or animated textures.
 * @brief you can use to render objects of a single sprite (e.g player.png) where the sprite is a single image and may not need to change (!animatable)
 * @brief if also the object needs to be animated but the sprite are of different files; create an array of the files path and then pass the array variable to where you would pass the single image path as usual
 * @brief the both above are made possible by default parameters to functions and constructor so that depending on what users want then the changed variables will be passed
 */
class Obj {
public:
    int x, y;                      // Position
    float scale;                   // Scale for rendering
    std::vector<Texture2D> textures; // Textures (single or multiple for animation)
    int current_frame;             // Current animation frame
    float frame_time;              // Time per frame (defaults to 1.0f for static objects)
    float elapsed_time;            // Time accumulator

    Obj(const std::string& path, int x_pos, int y_pos, float scale_factor, float frame_duration = 1.0f)
        : x(x_pos), y(y_pos), scale(scale_factor), current_frame(0), frame_time(frame_duration), elapsed_time(0.0f) {
        Texture2D texture = LoadTexture(path.c_str());
        if (texture.id != 0) {
            textures.push_back(texture);
        } else {
            throw std::runtime_error("Failed to load texture: " + path);
        }
    }

    Obj(const std::vector<std::string>& paths, int x_pos, int y_pos, float scale_factor, float frame_duration)
        : x(x_pos), y(y_pos), scale(scale_factor), current_frame(0), frame_time(frame_duration), elapsed_time(0.0f) {
        for (const auto& path : paths) {
            Texture2D texture = LoadTexture(path.c_str());
            if (texture.id != 0) {
                textures.push_back(texture);
            } else {
                throw std::runtime_error("Failed to load texture: " + path);
            }
        }
    }

    ~Obj() {
        for (auto& texture : textures) {
            UnloadTexture(texture);
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

        const Texture2D& texture = textures[current_frame];
        Rectangle dst = {
            static_cast<float>(x),
            static_cast<float>(y),
            texture.width * scale,
            texture.height * scale
        };
        DrawTexturePro(texture, {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)}, dst, {0, 0}, 0.0f, WHITE);
    }
};

//--------------------------OBJ SPRITE SHEET-----------------------------------------
/**
 * @class Obj_ss
 * @brief Represents a sprite sheet object, supporting static and animated tiles.
 * 
 * This class handles rendering static or animated tiles from a single sprite sheet.
 */
class Obj_ss : public Obj {
public:
    int tile_width;         // Tile width
    int tile_height;        // Tile height
    int frame_count;        // Number of animation frames (optional)
    float elapsed_time;     // Time accumulator for animations

    Obj_ss(const std::string& path, int x_pos, int y_pos, float scale_factor, 
           int t_width, int t_height, int frames = 1, float frame_duration = 1.0f)
        : Obj(path, x_pos, y_pos, scale_factor, frame_duration),
          tile_width(t_width), tile_height(t_height), frame_count(frames), elapsed_time(0.0f) {}

    void render(float delta_time = 0.0f) {
        if (textures.empty() || tile_width <= 0 || tile_height <= 0) return;

        if (frame_count > 1) { // Handle animation if more than 1 frame
            elapsed_time += delta_time;
            if (elapsed_time >= frame_time) {
                current_frame = (current_frame + 1) % frame_count;
                elapsed_time = 0.0f;
            }
        }

        const Texture2D& texture = textures[0];
        int frames_per_row = texture.width / tile_width;

        int tile_x = current_frame % frames_per_row;
        int tile_y = current_frame / frames_per_row;

        Rectangle src = {
            static_cast<float>(tile_x * tile_width),
            static_cast<float>(tile_y * tile_height),
            static_cast<float>(tile_width),
            static_cast<float>(tile_height)
        };
        Rectangle dst = {
            static_cast<float>(x),
            static_cast<float>(y),
            tile_width * scale,
            tile_height * scale
        };
        DrawTexturePro(texture, src, dst, {0, 0}, 0.0f, WHITE);
    }
};
//---------------------------------------- other func -------------------------------------------

 void init_window(int width, int height, const char* title, int target_fps){
    InitWindow(width, height, title);
    SetTargetFPS(target_fps);
 }

 bool window_should_close() {
     return WindowShouldClose();
 }
void start_drawing(){
	BeginDrawing();
}
void stop_drawing(){
	EndDrawing();
}
// Set the background color
void clear_screen(Color color) {
    ClearBackground((Color){color.r, color.g, color.b, color.a});
}

// Draw a rectangle
void draw_rect(int x, int y, int width, int height, Color color) {
    Color raylib_color = (Color){color.r, color.g, color.b, color.a};
    DrawRectangle(x, y, width, height, raylib_color);
}

// Draw a circle
void draw_circle(int x, int y, int radius, Color color) {
    Color raylib_color = (Color){color.r, color.g, color.b, color.a};
    DrawCircle(x, y, radius, raylib_color);
}

// Draw text using a font (make sure to load the font before calling this)
void draw_text(const char* text, int x, int y, Color color) {
    Color raylib_color = (Color){color.r, color.g, color.b, color.a};
    DrawText(text, x, y, 24, raylib_color);
}
//------------------------------------------MAIN-----------------------------------------
/**
 * @brief Main function demonstrating usage of static and animated sprite sheets.
 * 
 * Initializes a Raylib window and renders multiple objects, including:
 * - Static sprites
 * - Static tiles from a sprite sheet
 * - Animated tiles from a sprite sheet
 * - Fully animated objects
 * 
 * @return Exit status code (0 for success).
 */


// int main() {
// 	init_window(800, 600, "Sprite Example", 60);
// 
// 	// using ObJ_sprite sheet: both static and animated are both the same just that static uses a constructors with default parameters
//     Obj_ss static_tile("img/Attack1.png", 300, 100, 2.0f, 126, 126); // Single tile
//     Obj_ss animated_tile("img/Attack1.png", 500, 100, 2.0f, 126, 126, 7, 0.1f); // Animated tile
// 
// 
//     // using single obj: one as a single sprite file path. the other as an array of animated sprite
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
//         float delta_time = GetFrameTime();
// 
// 		start_drawing();
//         clear_screen(COLOR_WHITE); //color included from color.h
// 
// 		// the render methods
//         static_sprite.render();
//         static_tile.render(delta_time);
//         animated_sprite.render(delta_time);
//         animated_tile.render(delta_time);
// 
// 		stop_drawing();
//     }
// 
//     CloseWindow();
//     return 0;
// }
