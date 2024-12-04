#ifndef KEY_HPP
#define KEY_HPP

#include <unordered_map>

// Framework selection (define one based on your project)
// #define USE_RAYLIB
// #define USE_SDL
// #define USE_SFML

#ifdef USE_RAYLIB
#include "raylib.h"
#elif defined(USE_SDL)
#include <SDL2/SDL.h>
#elif defined(USE_SFML)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#else
#error "Please define USE_RAYLIB, USE_SDL, or USE_SFML to specify the framework."
#endif

// Enum for unified key inputs
enum class UnifiedKey {
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Left, Right, Up, Down,
    Space, Enter, Escape, Tab, Backspace,
    Shift, Ctrl, Alt,
    MouseLeft, MouseRight, MouseMiddle,
    Unknown
};

// Function to check if a key is pressed
inline bool key_pressed(UnifiedKey key) {
#ifdef USE_RAYLIB
    static std::unordered_map<UnifiedKey, int> raylibKeyMap = {
        {UnifiedKey::A, KEY_A}, {UnifiedKey::B, KEY_B}, {UnifiedKey::C, KEY_C},
        {UnifiedKey::D, KEY_D}, {UnifiedKey::E, KEY_E}, {UnifiedKey::F, KEY_F},
        {UnifiedKey::G, KEY_G}, {UnifiedKey::H, KEY_H}, {UnifiedKey::I, KEY_I},
        {UnifiedKey::J, KEY_J}, {UnifiedKey::K, KEY_K}, {UnifiedKey::L, KEY_L},
        {UnifiedKey::M, KEY_M}, {UnifiedKey::N, KEY_N}, {UnifiedKey::O, KEY_O},
        {UnifiedKey::P, KEY_P}, {UnifiedKey::Q, KEY_Q}, {UnifiedKey::R, KEY_R},
        {UnifiedKey::S, KEY_S}, {UnifiedKey::T, KEY_T}, {UnifiedKey::U, KEY_U},
        {UnifiedKey::V, KEY_V}, {UnifiedKey::W, KEY_W}, {UnifiedKey::X, KEY_X},
        {UnifiedKey::Y, KEY_Y}, {UnifiedKey::Z, KEY_Z},
        {UnifiedKey::Num0, KEY_ZERO}, {UnifiedKey::Num1, KEY_ONE}, {UnifiedKey::Num2, KEY_TWO},
        {UnifiedKey::Num3, KEY_THREE}, {UnifiedKey::Num4, KEY_FOUR}, {UnifiedKey::Num5, KEY_FIVE},
        {UnifiedKey::Num6, KEY_SIX}, {UnifiedKey::Num7, KEY_SEVEN}, {UnifiedKey::Num8, KEY_EIGHT},
        {UnifiedKey::Num9, KEY_NINE},
        {UnifiedKey::Left, KEY_LEFT}, {UnifiedKey::Right, KEY_RIGHT},
        {UnifiedKey::Up, KEY_UP}, {UnifiedKey::Down, KEY_DOWN},
        {UnifiedKey::Space, KEY_SPACE}, {UnifiedKey::Enter, KEY_ENTER},
        {UnifiedKey::Escape, KEY_ESCAPE}, {UnifiedKey::Tab, KEY_TAB},
        {UnifiedKey::Backspace, KEY_BACKSPACE}, {UnifiedKey::Shift, KEY_LEFT_SHIFT},
        {UnifiedKey::Ctrl, KEY_LEFT_CONTROL}, {UnifiedKey::Alt, KEY_LEFT_ALT},
        {UnifiedKey::MouseLeft, MOUSE_LEFT_BUTTON},
        {UnifiedKey::MouseRight, MOUSE_RIGHT_BUTTON},
        {UnifiedKey::MouseMiddle, MOUSE_MIDDLE_BUTTON}
    };
    return IsKeyPressed(raylibKeyMap[key]);

#elif defined(USE_SDL)
    static std::unordered_map<UnifiedKey, SDL_Keycode> sdlKeyMap = {
        {UnifiedKey::A, SDLK_a}, {UnifiedKey::B, SDLK_b}, {UnifiedKey::C, SDLK_c},
        {UnifiedKey::D, SDLK_d}, {UnifiedKey::E, SDLK_e}, {UnifiedKey::F, SDLK_f},
        {UnifiedKey::G, SDLK_g}, {UnifiedKey::H, SDLK_h}, {UnifiedKey::I, SDLK_i},
        {UnifiedKey::J, SDLK_j}, {UnifiedKey::K, SDLK_k}, {UnifiedKey::L, SDLK_l},
        {UnifiedKey::M, SDLK_m}, {UnifiedKey::N, SDLK_n}, {UnifiedKey::O, SDLK_o},
        {UnifiedKey::P, SDLK_p}, {UnifiedKey::Q, SDLK_q}, {UnifiedKey::R, SDLK_r},
        {UnifiedKey::S, SDLK_s}, {UnifiedKey::T, SDLK_t}, {UnifiedKey::U, SDLK_u},
        {UnifiedKey::V, SDLK_v}, {UnifiedKey::W, SDLK_w}, {UnifiedKey::X, SDLK_x},
        {UnifiedKey::Y, SDLK_y}, {UnifiedKey::Z, SDLK_z},
        {UnifiedKey::Num0, SDLK_0}, {UnifiedKey::Num1, SDLK_1}, {UnifiedKey::Num2, SDLK_2},
        {UnifiedKey::Num3, SDLK_3}, {UnifiedKey::Num4, SDLK_4}, {UnifiedKey::Num5, SDLK_5},
        {UnifiedKey::Num6, SDLK_6}, {UnifiedKey::Num7, SDLK_7}, {UnifiedKey::Num8, SDLK_8},
        {UnifiedKey::Num9, SDLK_9},
        {UnifiedKey::Left, SDLK_LEFT}, {UnifiedKey::Right, SDLK_RIGHT},
        {UnifiedKey::Up, SDLK_UP}, {UnifiedKey::Down, SDLK_DOWN},
        {UnifiedKey::Space, SDLK_SPACE}, {UnifiedKey::Enter, SDLK_RETURN},
        {UnifiedKey::Escape, SDLK_ESCAPE}, {UnifiedKey::Tab, SDLK_TAB},
        {UnifiedKey::Backspace, SDLK_BACKSPACE}, {UnifiedKey::Shift, SDLK_LSHIFT},
        {UnifiedKey::Ctrl, SDLK_LCTRL}, {UnifiedKey::Alt, SDLK_LALT}
    };
    const Uint8* state = SDL_GetKeyboardState(NULL);
    SDL_Scancode scancode = SDL_GetScancodeFromKey(sdlKeyMap[key]);
    return state[scancode];

#elif defined(USE_SFML)
    static std::unordered_map<UnifiedKey, sf::Keyboard::Key> sfmlKeyMap = {
        {UnifiedKey::A, sf::Keyboard::A}, {UnifiedKey::B, sf::Keyboard::B},
        {UnifiedKey::C, sf::Keyboard::C}, {UnifiedKey::D, sf::Keyboard::D},
        {UnifiedKey::E, sf::Keyboard::E}, {UnifiedKey::F, sf::Keyboard::F},
        {UnifiedKey::G, sf::Keyboard::G}, {UnifiedKey::H, sf::Keyboard::H},
        {UnifiedKey::I, sf::Keyboard::I}, {UnifiedKey::J, sf::Keyboard::J},
        {UnifiedKey::K, sf::Keyboard::K}, {UnifiedKey::L, sf::Keyboard::L},
        {UnifiedKey::M, sf::Keyboard::M}, {UnifiedKey::N, sf::Keyboard::N},
        {UnifiedKey::O, sf::Keyboard::O}, {UnifiedKey::P, sf::Keyboard::P},
        {UnifiedKey::Q, sf::Keyboard::Q}, {UnifiedKey::R, sf::Keyboard::R},
        {UnifiedKey::S, sf::Keyboard::S}, {UnifiedKey::T, sf::Keyboard::T},
        {UnifiedKey::U, sf::Keyboard::U}, {UnifiedKey::V, sf::Keyboard::V},
        {UnifiedKey::W, sf::Keyboard::W}, {UnifiedKey::X, sf::Keyboard::X},
        {UnifiedKey::Y, sf::Keyboard::Y}, {UnifiedKey::Z, sf::Keyboard::Z},
        {UnifiedKey::Num0, sf::Keyboard::Num0}, {UnifiedKey::Num1, sf::Keyboard::Num1},
        {UnifiedKey::Num2, sf::Keyboard::Num2}, {UnifiedKey::Num3, sf::Keyboard::Num3},
        {UnifiedKey::Num4, sf::Keyboard::Num4}, {UnifiedKey::Num5, sf::Keyboard::Num5},
        {UnifiedKey::Num6, sf::Keyboard::Num6}, {UnifiedKey::Num7, sf::Keyboard::Num7},
        {UnifiedKey::Num8, sf::Keyboard::Num8}, {UnifiedKey::Num9, sf::Keyboard::Num9},
        {UnifiedKey::Left, sf::Keyboard::Left}, {UnifiedKey::Right, sf::Keyboard::Right},
        {UnifiedKey::Up, sf::Keyboard::Up}, {UnifiedKey::Down, sf::Keyboard::Down},
        {UnifiedKey::Space, sf::Keyboard::Space}, {UnifiedKey::Enter, sf::Keyboard::Enter},
        {UnifiedKey::Escape, sf::Keyboard::Escape}, {UnifiedKey::Tab, sf::Keyboard::Tab},
        {UnifiedKey::Backspace, sf::Keyboard::Backspace}, {UnifiedKey::Shift, sf::Keyboard::LShift},
        {UnifiedKey::Ctrl, sf::Keyboard::LControl}, {UnifiedKey::Alt, sf::Keyboard::LAlt}
    };
    return sf::Keyboard::isKeyPressed(sfmlKeyMap[key]);

#else
    return false; // Unsupported framework
#endif
}

// Function to check if a key is being held down
inline bool key_down(UnifiedKey key) {
#ifdef USE_RAYLIB
    static std::unordered_map<UnifiedKey, int> raylibKeyMap = {
        {UnifiedKey::A, KEY_A}, {UnifiedKey::B, KEY_B}, {UnifiedKey::C, KEY_C},
        {UnifiedKey::D, KEY_D}, {UnifiedKey::E, KEY_E}, {UnifiedKey::F, KEY_F},
        {UnifiedKey::G, KEY_G}, {UnifiedKey::H, KEY_H}, {UnifiedKey::I, KEY_I},
        {UnifiedKey::J, KEY_J}, {UnifiedKey::K, KEY_K}, {UnifiedKey::L, KEY_L},
        {UnifiedKey::M, KEY_M}, {UnifiedKey::N, KEY_N}, {UnifiedKey::O, KEY_O},
        {UnifiedKey::P, KEY_P}, {UnifiedKey::Q, KEY_Q}, {UnifiedKey::R, KEY_R},
        {UnifiedKey::S, KEY_S}, {UnifiedKey::T, KEY_T}, {UnifiedKey::U, KEY_U},
        {UnifiedKey::V, KEY_V}, {UnifiedKey::W, KEY_W}, {UnifiedKey::X, KEY_X},
        {UnifiedKey::Y, KEY_Y}, {UnifiedKey::Z, KEY_Z},
        {UnifiedKey::Num0, KEY_ZERO}, {UnifiedKey::Num1, KEY_ONE}, {UnifiedKey::Num2, KEY_TWO},
        {UnifiedKey::Num3, KEY_THREE}, {UnifiedKey::Num4, KEY_FOUR}, {UnifiedKey::Num5, KEY_FIVE},
        {UnifiedKey::Num6, KEY_SIX}, {UnifiedKey::Num7, KEY_SEVEN}, {UnifiedKey::Num8, KEY_EIGHT},
        {UnifiedKey::Num9, KEY_NINE},
        {UnifiedKey::Left, KEY_LEFT}, {UnifiedKey::Right, KEY_RIGHT},
        {UnifiedKey::Up, KEY_UP}, {UnifiedKey::Down, KEY_DOWN},
        {UnifiedKey::Space, KEY_SPACE}, {UnifiedKey::Enter, KEY_ENTER},
        {UnifiedKey::Escape, KEY_ESCAPE}, {UnifiedKey::Tab, KEY_TAB},
        {UnifiedKey::Backspace, KEY_BACKSPACE}, {UnifiedKey::Shift, KEY_LEFT_SHIFT},
        {UnifiedKey::Ctrl, KEY_LEFT_CONTROL}, {UnifiedKey::Alt, KEY_LEFT_ALT},
        {UnifiedKey::MouseLeft, MOUSE_LEFT_BUTTON},
        {UnifiedKey::MouseRight, MOUSE_RIGHT_BUTTON},
        {UnifiedKey::MouseMiddle, MOUSE_MIDDLE_BUTTON}
    };
    return IsKeyDown(raylibKeyMap[key]);

#elif defined(USE_SDL)
    static std::unordered_map<UnifiedKey, SDL_Keycode> sdlKeyMap = {
        {UnifiedKey::A, SDLK_a}, {UnifiedKey::B, SDLK_b}, {UnifiedKey::C, SDLK_c},
        {UnifiedKey::D, SDLK_d}, {UnifiedKey::E, SDLK_e}, {UnifiedKey::F, SDLK_f},
        {UnifiedKey::G, SDLK_g}, {UnifiedKey::H, SDLK_h}, {UnifiedKey::I, SDLK_i},
        {UnifiedKey::J, SDLK_j}, {UnifiedKey::K, SDLK_k}, {UnifiedKey::L, SDLK_l},
        {UnifiedKey::M, SDLK_m}, {UnifiedKey::N, SDLK_n}, {UnifiedKey::O, SDLK_o},
        {UnifiedKey::P, SDLK_p}, {UnifiedKey::Q, SDLK_q}, {UnifiedKey::R, SDLK_r},
        {UnifiedKey::S, SDLK_s}, {UnifiedKey::T, SDLK_t}, {UnifiedKey::U, SDLK_u},
        {UnifiedKey::V, SDLK_v}, {UnifiedKey::W, SDLK_w}, {UnifiedKey::X, SDLK_x},
        {UnifiedKey::Y, SDLK_y}, {UnifiedKey::Z, SDLK_z},
        {UnifiedKey::Num0, SDLK_0}, {UnifiedKey::Num1, SDLK_1}, {UnifiedKey::Num2, SDLK_2},
        {UnifiedKey::Num3, SDLK_3}, {UnifiedKey::Num4, SDLK_4}, {UnifiedKey::Num5, SDLK_5},
        {UnifiedKey::Num6, SDLK_6}, {UnifiedKey::Num7, SDLK_7}, {UnifiedKey::Num8, SDLK_8},
        {UnifiedKey::Num9, SDLK_9},
        {UnifiedKey::Left, SDLK_LEFT}, {UnifiedKey::Right, SDLK_RIGHT},
        {UnifiedKey::Up, SDLK_UP}, {UnifiedKey::Down, SDLK_DOWN},
        {UnifiedKey::Space, SDLK_SPACE}, {UnifiedKey::Enter, SDLK_RETURN},
        {UnifiedKey::Escape, SDLK_ESCAPE}, {UnifiedKey::Tab, SDLK_TAB},
        {UnifiedKey::Backspace, SDLK_BACKSPACE}, {UnifiedKey::Shift, SDLK_LSHIFT},
        {UnifiedKey::Ctrl, SDLK_LCTRL}, {UnifiedKey::Alt, SDLK_LALT}
    };
    const Uint8* state = SDL_GetKeyboardState(NULL);
    SDL_Scancode scancode = SDL_GetScancodeFromKey(sdlKeyMap[key]);
    return state[scancode] == 1;

#elif defined(USE_SFML)
    static std::unordered_map<UnifiedKey, sf::Keyboard::Key> sfmlKeyMap = {
        {UnifiedKey::A, sf::Keyboard::A}, {UnifiedKey::B, sf::Keyboard::B},
        {UnifiedKey::C, sf::Keyboard::C}, {UnifiedKey::D, sf::Keyboard::D},
        {UnifiedKey::E, sf::Keyboard::E}, {UnifiedKey::F, sf::Keyboard::F},
        {UnifiedKey::G, sf::Keyboard::G}, {UnifiedKey::H, sf::Keyboard::H},
        {UnifiedKey::I, sf::Keyboard::I}, {UnifiedKey::J, sf::Keyboard::J},
        {UnifiedKey::K, sf::Keyboard::K}, {UnifiedKey::L, sf::Keyboard::L},
        {UnifiedKey::M, sf::Keyboard::M}, {UnifiedKey::N, sf::Keyboard::N},
        {UnifiedKey::O, sf::Keyboard::O}, {UnifiedKey::P, sf::Keyboard::P},
        {UnifiedKey::Q, sf::Keyboard::Q}, {UnifiedKey::R, sf::Keyboard::R},
        {UnifiedKey::S, sf::Keyboard::S}, {UnifiedKey::T, sf::Keyboard::T},
        {UnifiedKey::U, sf::Keyboard::U}, {UnifiedKey::V, sf::Keyboard::V},
        {UnifiedKey::W, sf::Keyboard::W}, {UnifiedKey::X, sf::Keyboard::X},
        {UnifiedKey::Y, sf::Keyboard::Y}, {UnifiedKey::Z, sf::Keyboard::Z},
        {UnifiedKey::Num0, sf::Keyboard::Num0}, {UnifiedKey::Num1, sf::Keyboard::Num1},
        {UnifiedKey::Num2, sf::Keyboard::Num2}, {UnifiedKey::Num3, sf::Keyboard::Num3},
        {UnifiedKey::Num4, sf::Keyboard::Num4}, {UnifiedKey::Num5, sf::Keyboard::Num5},
        {UnifiedKey::Num6, sf::Keyboard::Num6}, {UnifiedKey::Num7, sf::Keyboard::Num7},
        {UnifiedKey::Num8, sf::Keyboard::Num8}, {UnifiedKey::Num9, sf::Keyboard::Num9},
        {UnifiedKey::Left, sf::Keyboard::Left}, {UnifiedKey::Right, sf::Keyboard::Right},
        {UnifiedKey::Up, sf::Keyboard::Up}, {UnifiedKey::Down, sf::Keyboard::Down},
        {UnifiedKey::Space, sf::Keyboard::Space}, {UnifiedKey::Enter, sf::Keyboard::Enter},
        {UnifiedKey::Escape, sf::Keyboard::Escape}, {UnifiedKey::Tab, sf::Keyboard::Tab},
        {UnifiedKey::Backspace, sf::Keyboard::Backspace}, {UnifiedKey::Shift, sf::Keyboard::LShift},
        {UnifiedKey::Ctrl, sf::Keyboard::LControl}, {UnifiedKey::Alt, sf::Keyboard::LAlt}
    };
    return sf::Keyboard::isKeyPressed(sfmlKeyMap[key]);

#else
    return false; // Unsupported framework
#endif
}

#endif // KEY_HPP


// //-------------------------------------------example main.cpp usage ----------------------------------------
// #define USE_SDL
// #include <SDL2/SDL.h>
// #include "key.hpp"
// 
// int main(int argc, char* argv[]) {
//     // Initialize SDL
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
//         return -1;
//     }
// 
//     // Create a window
//     SDL_Window* window = SDL_CreateWindow("Unified Input Example",
//                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//                                           800, 600, SDL_WINDOW_SHOWN);
//     if (window == nullptr) {
//         SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
//         SDL_Quit();
//         return -1;
//     }
// 
//     // Create a renderer
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (renderer == nullptr) {
//         SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//         return -1;
//     }
// 
//     // Main loop
//     SDL_Event e;
//     bool quit = false;
// 
//     while (!quit) {
//         // Poll for events
//         while (SDL_PollEvent(&e) != 0) {
//             if (e.type == SDL_QUIT) {
//                 quit = true;
//             }
//         }
// 
//         // Check if the 'A' key is pressed using the key_pressed function from key.hpp
//         if (key_pressed(UnifiedKey::A)) {
//             // Clear screen and render text for 'A' press
//             SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red
//             SDL_RenderClear(renderer);
//         } else {
//             // Clear screen with a different color if 'A' is not pressed
//             SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue
//             SDL_RenderClear(renderer);
//         }
// 
//         // Present the renderer
//         SDL_RenderPresent(renderer);
//     }
// 
//     // Clean up
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();
//     return 0;
// }
