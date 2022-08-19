#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

enum class LevelState {PLAY, EXIT};

class NewLevel{
    public:
    
    NewLevel();
    ~NewLevel();
    void run();
   
    private:
    Mix_Chunk *sound;
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
    LevelState levelState;
    public:
    static SDL_Renderer *renderer;
    

};

