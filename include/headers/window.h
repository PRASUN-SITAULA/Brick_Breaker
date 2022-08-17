#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>

enum class WindowState {PLAY, EXIT};

class Window{
    public:
    Window();
    ~Window();
    void run();
    void gameLoop();
    void handleEvents();
    void render();
    void input();
    void update();
    void write(std::string text, std::string score, int x, int y);
    void setBricks(int i);
    void resetBricks(); 
    SDL_Window* window;
    int screenWidth ;
    int screenHeight ;
    WindowState windowState;
    public:
    static SDL_Renderer *renderer;
};

