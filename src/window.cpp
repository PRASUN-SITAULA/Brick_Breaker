#include <headers/window.h>
#include <headers/games.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Renderer *Window::renderer = nullptr;

Window::Window(){
    win = nullptr;
    screenHeight = 600;
    screenWidth = 1024;
    WindowState = WindowState::PLAY;

};
Window::~Window(){};

void Window::run(){
    init("Bricks Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,screenWidth, screenHeight ,SDL_WINDOW_SHOWN);
    gameLoop();
}


 //rectangle for menu
SDL_Rect rect_win = { 870, 0, 86, 86};

void Window::init(const char *title, int x, int y, int w, int h, Uint32 flags){
    SDL_Init(SDL_INIT_EVERYTHING); //initialize sdl

    //create a window
    win = SDL_CreateWindow(title, x, y, w, h,flags);
    if(win==NULL){
        std::cout<<"There was error initialzing the window"<<std::endl;
        std::cout<<SDL_GetError()<<std::endl;
    }

    //render window
    SDL_Renderer *renderer = nullptr;
    renderer = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    
    // //initialize image
    // int imgFlags = IMG_INIT_PNG;
    // int initstatus = IMG_Init(imgFlags);
    // if((initstatus & flags)!=flags){
    //     std::cout<<"Failed to initialize SDL_image"<<std::endl;
    // }

    // //load image
    // SDL_Surface *image;
    // image = IMG_Load("./unnamed.png");
    // if(!image){
    //     std::cout<<"Image not loaded"<<std::endl;
    // }

    
    // SDL_Rect rectangle;
    // rectangle.x = 0;
    // rectangle.y = 0;
    // rectangle.w = 900;
    // rectangle.h = 600;


    // //render iamge
    // SDL_Texture * ourPNG = SDL_CreateTextureFromSurface(ren,image);
    // SDL_RenderCopy(ren, ourPNG, NULL,&rectangle);

    //initialize text
    if(TTF_Init() == -1){
        std::cout<<"Failed to intialize SDL_TTF"<<std::endl;
    }

    //load text
    TTF_Font * ourFont = TTF_OpenFont("ALGER.TTF",70);
    if(ourFont == nullptr){
        std::cout<<"Font cannot be loaded"<<std::endl;
    }
    SDL_Surface *surfacetext = TTF_RenderText_Blended(ourFont,"Go back",{255,255,0});
    SDL_Texture *texturetext = SDL_CreateTextureFromSurface(renderer,surfacetext);
    SDL_FreeSurface(surfacetext);


   

    // //rectangle for menu background
    // SDL_Rect rect1;
    // rect1.x = 800;
    // rect1.y = 0;
    // rect1.h = 900;
    // rect1.w = 240;

    // //rendering menu background
    // SDL_SetRenderDrawColor(ren, 0, 128, 128, 128);
    // SDL_RenderFillRect(ren, &rect1);

    //background rectangle
   

    //adding color to screen
    // SDL_Surface *screen = SDL_GetWindowSurface(win);
    // Uint32 color = SDL_MapRGB( screen->format,127,247,255 );
    // SDL_FillRect( screen ,NULL, color);
    // SDL_UpdateWindowSurface( win );

    SDL_RenderCopy(renderer,texturetext,NULL,&rect_win);
    SDL_RenderPresent(renderer);
    
}

void Window::gameLoop(){

    while(WindowState != WindowState::EXIT){
        handleEvents();
    }
    
}

void Window::handleEvents(){
    SDL_Event evnt;
    SDL_PollEvent(&evnt);
    switch (evnt.type){
        case SDL_QUIT:
        WindowState = WindowState::EXIT;
        break;
    }

    // if(SDL_MOUSEBUTTONDOWN == evnt.type)
    // {
    //     SDL_Point mousePosition;
    //     // Mouse click coords from event handler
    //     mousePosition.x = evnt.motion.x; 
    //     mousePosition.y = evnt.motion.y;

    //     if (SDL_PointInRect(&mousePosition, &rect_win)) {
    //         std::cout<<"mouse is pressed"<<std::endl;
    //         SDL_DestroyWindow(win);
    //         Game::run();

    //     }
    // }

}