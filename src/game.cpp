#include <headers/games.h>
#include <headers/window.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

SDL_Renderer *Game::ren = nullptr;
TTF_Font * ourFont;
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

SDL_Rect rect = { 838, 320, 125, 75}, quitrect ={838, 450 ,125, 75};

void Game::init(const char *title, int x, int y, int w, int h, Uint32 flags){
    SDL_Init(SDL_INIT_EVERYTHING); //initialize sdl

    //create a window
    window = SDL_CreateWindow(title, x, y, w, h,flags);
    if(window==NULL){
        std::cout<<"There was error initialzing the window"<<std::endl;
        std::cout<<SDL_GetError()<<std::endl;
    }

    //render window
    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    
    //initialize image
    int imgFlags = IMG_INIT_PNG ||IMG_INIT_JPG;
    int initstatus = IMG_Init(imgFlags);
    if((initstatus & flags)!=flags){
        std::cout<<"Failed to initialize SDL_image"<<std::endl;
    }

    //load image
    SDL_Surface *image,*button, *quit, *box;
    image = IMG_Load("images/background.jpg");
    if(!image){
        std::cout<<"Image not loaded"<<std::endl;
    }
    button = IMG_Load("images/playbutton.png");
    if(!button){
        std::cout<<"button not loaded"<<std::endl;
    }
    quit = IMG_Load("images/quit.png");
    box = IMG_Load("images/ui.png");

    //rectangle image background
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = 1024;
    rectangle.h = 600;

    //rectangle for menu background
    SDL_Rect rect1;
    rect1.x = 800;
    rect1.y = 290;
    rect1.h = 270;
    rect1.w = 210;


    //render iamge
    SDL_Texture *ourPNG = SDL_CreateTextureFromSurface(ren,image);
    SDL_Texture *textureui = SDL_CreateTextureFromSurface(ren,box);
    SDL_Texture *play = SDL_CreateTextureFromSurface(ren,button);
    SDL_Texture *texturequit = SDL_CreateTextureFromSurface(ren,quit);
    
    // SDL_SetRenderDrawColor(ren, 0, 130, 127, 127);
    // SDL_RenderFillRect(ren, &rect1);

    SDL_RenderCopy(ren, ourPNG, NULL,&rectangle);
    SDL_RenderCopy(ren,textureui, NULL, &rect1);
    SDL_RenderCopy(ren, play, NULL, &rect);
    SDL_RenderCopy(ren,texturequit, NULL, &quitrect);
    

    SDL_RenderPresent(ren);
}

void Game::gameLoop(){

    while(gameState != GameState::EXIT){
        handleEvents();
    }
    
}

void Game::handleEvents(){
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt))
    {
        if (evnt.type == SDL_QUIT)
        { 
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(ren);
            SDL_Quit();
        }
    

        if(SDL_MOUSEBUTTONUP == evnt.type)
        {
            SDL_Point mousePosition;
            // Mouse click coords from event handler
            mousePosition.x = evnt.motion.x; 
            mousePosition.y = evnt.motion.y;

            if (SDL_PointInRect(&mousePosition, &rect)) {
                std::cout<<"mouse is pressed"<<std::endl;
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(ren);
                Window window;
                window.run();
            }

            if(SDL_PointInRect(&mousePosition ,&quitrect)){
                std::cout<<"quit button is pressed"<<std::endl;
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(ren);
                SDL_Quit();
            }
        }
    }

}