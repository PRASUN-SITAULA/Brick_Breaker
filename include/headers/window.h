#pragma once
#include <iostream>
#include <SDL2/SDL.h>

enum class WindowState {PLAY, EXIT};
class Window{
    public:
    
    Window();
    ~Window();
    void run();
   
    private:
    //void init(const char *title, int x, int y, int w, int h, Uint32 flags);  // flags for window resizeability, x,y for postion of window
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