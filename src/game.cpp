#include <headers/games.h>

Game::Game(){
    window = nullptr;
    screenHeight = 600;
    screenWidth = 1024;
    gameState = GameState::PLAY;

};
Game::~Game(){};

void Game::run(){
    init("Bricks Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,screenWidth, screenHeight ,SDL_WINDOW_SHOWN);
    gameLoop();
}
void Game::init(const char *title, int x, int y, int w, int h, Uint32 flags){
    SDL_Init(SDL_INIT_EVERYTHING); //initialize sdl

    //create a window
    window = SDL_CreateWindow(title, x, y, w, h,flags);
    if(window==NULL){
        std::cout<<"There was error initialzing the window"<<std::endl;
        // SDL_GetError()<<std::endl;
    }


    //adding color to screen
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    Uint32 color = SDL_MapRGB( screen->format,255,256,255 );
    SDL_FillRect( screen ,NULL, color);
    SDL_UpdateWindowSurface( window );

}

void Game::gameLoop(){
    while(gameState != GameState::EXIT){
        handleEvents();
    }
    
}

void Game::handleEvents(){
    SDL_Event evnt;
    SDL_PollEvent(&evnt);
    switch (evnt.type){
        case SDL_QUIT:
        gameState = GameState::EXIT;
        break;
    }

}