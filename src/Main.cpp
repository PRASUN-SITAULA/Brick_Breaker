#include <iostream>
#include <SDL2/SDL.h>
#include <headers/games.h>
#include <headers/window.h>
#include <SDL2/SDL_image.h>


const int width = 800, height = 600;

int main(int argc, char *argv[])
{
    Game game;
    game.run();

    return 0;
}