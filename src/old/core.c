#include <raylib.h>
#include <stdlib.h>
#include <string.h>
//--------------------------OBJ----------------------------------------------------
// Object structure for static sprites and tiles
typedef struct {
    int x;                  // Position on the x-axis
    int y;                  // Position on the y-axis
    Texture2D texture;      // Texture for the sprite
    float scale;            // Scale for rendering
    char *file_path;        // File path for the texture
    int frame_count;        // Number of animation frames (0 for static sprites)
    int current_frame;      // Current animation frame (for animated sprites)
    float frame_time;       // Time per frame (for animated sprites)
    float elapsed_time;     // Time accumulator (for animated sprites)
} Obj;

// Function to load a static sprite
Obj load_sprite(const char *file_path, int x, int y, float scale) {
    Obj new_asset = {0};
    new_asset.file_path = malloc(strlen(file_path) + 1);
    strcpy(new_asset.file_path, file_path);
    new_asset.x = x;
    new_asset.y = y;
    new_asset.scale = scale;
    new_asset.texture = LoadTexture(file_path);
    return new_asset;
}

// Function to render a static sprite or tileset
void render_sprite(const Obj *obj) {
    Rectangle dst = {
        obj->x,
        obj->y,
        obj->texture.width * obj->scale,
        obj->texture.height * obj->scale
    };
    DrawTexturePro(obj->texture, (Rectangle){0, 0, obj->texture.width, obj->texture.height}, dst, (Vector2){0, 0}, 0.0f, WHITE);
}


// Cleanup function to unload textures and free memory
void unload_sprite(Obj *obj) {
    UnloadTexture(obj->texture);
    free(obj->file_path);
}
//-------------------------------------------------------
// Object structure for static sprites and tiles
typedef struct {
    int x;                  // Position on the x-axis
    int y;                  // Position on the y-axis
    Texture2D texture;      // Texture for the sprite
    float scale;            // Scale for rendering
    char *file_path;        // File path for the texture
    int frame_count;        // Number of animation frames (0 for static sprites)
    int current_frame;      // Current animation frame (for animated sprites)
    float frame_time;       // Time per frame (for animated sprites)
    float elapsed_time;     // Time accumulator (for animated sprites)
    int tile_width;         // Tile width (for tilesets)
    int tile_height;        // Tile height (for tilesets)
    int tile_x;
    int tile_y;
} Obj_tile;

// Function to load a static sprite from a sprite sheet
Obj_tile load_sprite_from_sprite_sheet(const char *file_path, int x, int y, float scale,int tile_x,int tile_y, int tile_width, int tile_height) {
    Obj_tile new_asset = {0};
    new_asset.file_path = malloc(strlen(file_path) + 1);
    strcpy(new_asset.file_path, file_path);
    new_asset.x = x;
    new_asset.y = y;
    new_asset.scale = scale;
    new_asset.texture = LoadTexture(file_path);
	new_asset.tile_width = tile_width;
	new_asset.tile_height = tile_height;
	new_asset.tile_x = tile_x;
	new_asset.tile_x = tile_y;
    return new_asset;
}

// Function to render a static sprite or tileset
void render_sprite_tile(const Obj_tile *obj) {
    Rectangle dst = {
        obj->x,
        obj->y,
        obj->tile_width * obj->scale,
        obj->tile_height * obj->scale
    };    
    Rectangle src = {
        obj->tile_x * obj->tile_width,  // x position in sprite sheet
        obj->tile_y * obj->tile_height, // y position in sprite sheet
        obj->tile_width,                // width of the sprite
        obj->tile_height                // height of the sprite
    };
    DrawTexturePro(obj->texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}
//--------------------------------ANIMATED TILE-----------------------
// Object structure for static sprites and tiles
typedef struct {
    int x;                  // Position on the x-axis
    int y;                  // Position on the y-axis
    Texture2D texture;      // Texture for the sprite
    float scale;            // Scale for rendering
    char *file_path;        // File path for the texture
    int frame_count;        // Number of animation frames
    int current_frame;      // Current animation frame
    float frame_time;       // Time per frame
    float elapsed_time;     // Time accumulator
    int tile_width;         // Tile width
    int tile_height;        // Tile height
} Obj_tile_animated;

// Function to load a static sprite from a sprite sheet
Obj_tile_animated load_sprite_from_sprite_sheet_animated(const char *file_path, int x, int y, float scale,int tile_width, int tile_height, int frame_count, float frame_time) {
    
    Obj_tile_animated new_asset = {0};
    new_asset.file_path = malloc(strlen(file_path) + 1);
    strcpy(new_asset.file_path, file_path);
    new_asset.x = x;
    new_asset.y = y;
    new_asset.scale = scale;
    new_asset.texture = LoadTexture(file_path);
    new_asset.tile_width = tile_width;
    new_asset.tile_height = tile_height;
    new_asset.frame_count = frame_count;
    new_asset.frame_time = frame_time;
    new_asset.current_frame = 0;
    new_asset.elapsed_time = 0.0f;

    return new_asset;
}

// Function to render a static sprite or tileset
void render_sprite_tile_animated(Obj_tile_animated *obj, float delta_time) {
    obj->elapsed_time += delta_time;

    // Advance the frame if enough time has elapsed
    if (obj->elapsed_time >= obj->frame_time) {
        obj->current_frame = (obj->current_frame + 1) % obj->frame_count;
        obj->elapsed_time = 0.0f;
    }

    // Calculate the frames per row
    int frames_per_row = obj->texture.width / obj->tile_width;

    // Calculate frame position (row and column)
    int frame_x = obj->current_frame % frames_per_row;
    int frame_y = obj->current_frame / frames_per_row;

    // Source rectangle for the current frame
    Rectangle src = {
        frame_x * obj->tile_width,   // x position
        frame_y * obj->tile_height, // y position
        obj->tile_width,            // width of the frame
        obj->tile_height            // height of the frame
    };

    // Destination rectangle for rendering
    Rectangle dst = {
        obj->x,
        obj->y,
        obj->tile_width * obj->scale,
        obj->tile_height * obj->scale
    };

    // Draw the texture
    DrawTexturePro(obj->texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}



//------------------------------------------ Obj animated -----------------------------------------
// Object structure for animated sprites
typedef struct {
    int x;                  // Position on the x-axis
    int y;                  // Position on the y-axis
    Texture2D *textures;    // Array of textures for animation frames
    int frame_count;        // Number of animation frames
    int current_frame;      // Current animation frame
    float frame_time;       // Time per frame
    float elapsed_time;     // Time accumulator
    float scale;            // Scale for rendering
    char **file_paths;      // Array of file paths for the textures
} Obj_animated;



// Function to load an animated sprite from multiple files
Obj_animated load_sprite_animated(const char **file_paths, int frame_count, int x, int y, float scale, float frame_time) {
    Obj_animated animated_asset = {0};
    animated_asset.textures = malloc(sizeof(Texture2D) * frame_count);
    animated_asset.file_paths = malloc(sizeof(char *) * frame_count);
    animated_asset.frame_count = frame_count;
    animated_asset.current_frame = 0;
    animated_asset.frame_time = frame_time;
    animated_asset.elapsed_time = 0.0f;
    animated_asset.x = x;
    animated_asset.y = y;
    animated_asset.scale = scale;

    // Load all frames
    for (int i = 0; i < frame_count; i++) {
        animated_asset.file_paths[i] = malloc(strlen(file_paths[i]) + 1);
        strcpy(animated_asset.file_paths[i], file_paths[i]);
        animated_asset.textures[i] = LoadTexture(file_paths[i]);
    }

    return animated_asset;
}


// Function to render an animated sprite
void render_sprite_animated(Obj_animated *obj, float delta_time) {
    if (obj->frame_count > 1) {
        obj->elapsed_time += delta_time;
        if (obj->elapsed_time >= obj->frame_time) {
            obj->current_frame = (obj->current_frame + 1) % obj->frame_count;
            obj->elapsed_time = 0.0f;
        }
    }

    Texture2D current_texture = obj->textures[obj->current_frame];

    Rectangle dst = {
        obj->x,
        obj->y,
        current_texture.width * obj->scale,
        current_texture.height * obj->scale
    };
    DrawTexturePro(current_texture, (Rectangle){0, 0, current_texture.width, current_texture.height}, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

void unload_sprite_animated(Obj_animated *obj) {
    for (int i = 0; i < obj->frame_count; i++) {
        UnloadTexture(obj->textures[i]);
        free(obj->file_paths[i]);
    }
    free(obj->textures);
    free(obj->file_paths);
}

//--------------------------------- MAIN ---------------------------------------
// Main function
int main(void) {
    InitWindow(800, 600, "Sprite Example");
    SetTargetFPS(60);

    Obj static_sprite = load_sprite("img/player/Idle/0.png", 100, 100, 2.0f);
    Obj_tile new_sprite = load_sprite_from_sprite_sheet("img/Attack1.png", 300, 100, 2.0f,0,0,126,126);
    Obj_tile_animated animated_tile = load_sprite_from_sprite_sheet_animated("img/Attack1.png", 500, 100, 2.0f, 126, 126, 7, 0.1f);

    const char *idle_frames[] = {
        "img/player/Idle/0.png",
        "img/player/Idle/1.png",
        "img/player/Idle/2.png",
        "img/player/Idle/3.png"
    };
    Obj_animated animated_sprite = load_sprite_animated(idle_frames, 4, 300, 300, 2.0f, 0.2f);

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        render_sprite(&static_sprite);
        render_sprite_tile(&new_sprite);
        render_sprite_animated(&animated_sprite, delta_time);

        render_sprite_tile_animated(&animated_tile, delta_time);
        

        EndDrawing();
    }

    unload_sprite(&static_sprite);
    unload_sprite_animated(&animated_sprite);
    // unload_sprite(&tileset_sprite);

    CloseWindow();
    return 0;
}
