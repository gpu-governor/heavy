#include <raylib.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

//--------------------------OBJ----------------------------------------------------
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

//------------------------------------------MAIN-----------------------------------------
int main() {
    InitWindow(800, 600, "Sprite Example");
    SetTargetFPS(60);

    Obj_ss static_tile("img/Attack1.png", 300, 100, 2.0f, 126, 126); // Single tile
    Obj_ss animated_tile("img/Attack1.png", 500, 100, 2.0f, 126, 126, 7, 0.1f); // Animated tile
    Obj static_sprite("img/player/Idle/0.png", 100, 100, 2.0f);

    std::vector<std::string> idle_frames = {
        "img/player/Idle/0.png",
        "img/player/Idle/1.png",
        "img/player/Idle/2.png",
        "img/player/Idle/3.png"
    };
    Obj animated_sprite(idle_frames, 300, 300, 2.0f, 0.2f);

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        static_sprite.render();
        static_tile.render(delta_time);
        animated_sprite.render(delta_time);
        animated_tile.render(delta_time);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
