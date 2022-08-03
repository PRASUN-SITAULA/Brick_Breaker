#include <headers/window.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <iostream>
#include <cmath>
//Define all the required terms
#define FONT_SIZE 32
#define BALL_SPEED 11
#define SPEED 10
#define SIZE 16
#define COL 13
#define ROW 8
#define PI 3.14159265358979323846
#define SPACING 16
//constant width and height
const int width = 800, height = 600;
//SDL requiremments for rendering

SDL_Renderer *Window::renderer = nullptr;
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font* font, *textfont;
SDL_Color color;
int frameCount, timerFPS, lastFrame, fps ,score;
SDL_Rect paddle, ball, lives, brick, srect={750,10,25,25}, textrect={0,0,50,50};
float velY, velX;
int livesCount;
bool bricks[ROW*COL];
bool running;

//resets bricks for play again
void Window::resetBricks() {
    for(int i=0; i<COL*ROW; i++) bricks[i]=1;
    livesCount = 3;
    score = 0;
    paddle.x = (width/2)-(paddle.w/2);
    ball.y = paddle.y-(paddle.h*4);
    velY = BALL_SPEED/2;
    velX = 0;
    ball.x = width/2-(SIZE/2);
}


//to set bricks
void Window::setBricks(int i) {
    brick.x = (((i%COL)+1)*SPACING)+((i%COL)*brick.w)-(SPACING/2);
    brick.y = brick.h*3+(((i%ROW)+1)*SPACING)+((i%ROW)*brick.h)-(SPACING/2);
}

//to write bricks and paddle and ball
void Window::write(std::string text,std::string score, int x, int y) {
    SDL_Surface *surface,*surface1,*surfacetext;
    SDL_Texture *texture,*texture1,*texturetext;
    const char* t=text.c_str();
    const char* s=score.c_str();
    surface = TTF_RenderText_Blended(font, t, color);
    //surface 1 for score
    surface1 = TTF_RenderText_Blended(font, s, color);
    //surface for text
    surfacetext = TTF_RenderText_Blended(textfont,"Next Level",{255,255,0});
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    texturetext = SDL_CreateTextureFromSurface(renderer,surfacetext);
    lives.w = surface->w;
    lives.h = surface->h;
    lives.x = x-lives.w;
    lives.y = y-lives.h;
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surfacetext);
    SDL_FreeSurface(surface1);
    SDL_RenderCopy(renderer, texture, NULL, &lives);
    SDL_RenderCopy(renderer, texture1, NULL, &srect);
    SDL_RenderCopy(renderer,texturetext,NULL,&textrect);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texturetext);
}

//Updates the status
//i.e. the game logic
void Window::update() {
    if(livesCount<=0) resetBricks();
    if(SDL_HasIntersection(&ball, &paddle)) {
        double rel = (paddle.x+(paddle.w/2))-(ball.x+(SIZE/2));
        double norm = rel/(paddle.w/3);
        double bounce = norm* (5*PI/12);
        velY = -BALL_SPEED*cos(bounce);
        velX = BALL_SPEED*-sin(bounce);
    }

    if(ball.y<=0) velY = -velY;
    if(ball.y+SIZE>=height) {velY=-velY;livesCount--;}
    if(ball.x<=0 || ball.x+SIZE>=width) velX=-velX;
    ball.x += velX;
    ball.y += velY;
    if(paddle.x<0) paddle.x=0;
    if(paddle.x+paddle.w>width) paddle.x = width-paddle.w;

    bool reset=1;
    for(int i=0; i<COL*ROW; i++) {
    setBricks(i);
    if(SDL_HasIntersection(&ball, &brick) && bricks[i]) {
        score += 1;
        bricks[i]=0;
        if(ball.x >= brick.x) {velX=-velX; ball.x-=20;}
        if(ball.x <= brick.x) {velX=-velX; ball.x+=20;}
        if(ball.y <= brick.y) {velY=-velY; ball.y-=20;}
        if(ball.y >= brick.y) {velY=-velY; ball.y+=20;}
    }
    if(bricks[i]) reset=0;
    }
    if(reset) resetBricks();
}

//Keys Input
void Window::input() {
    SDL_Event e;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&e)) if(e.type==SDL_QUIT) running=false;
    if(keystates[SDL_SCANCODE_ESCAPE]) running=false;
    if(keystates[SDL_SCANCODE_LEFT]) paddle.x-=SPEED;
    if(keystates[SDL_SCANCODE_RIGHT]) paddle.x+=SPEED;
}

//rendering the game for playing
void Window::render() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(renderer);

    frameCount++;
    timerFPS = SDL_GetTicks()-lastFrame;
    if(timerFPS<(1000/60)) {
        SDL_Delay((1000/60)-timerFPS);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle);
    SDL_RenderFillRect(renderer, &ball);
    write(std::to_string(livesCount),std::to_string(score),width/2+FONT_SIZE/2, FONT_SIZE*1.5);

    for(int i=0; i<COL*ROW; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        if(i%2 == 0)SDL_SetRenderDrawColor(renderer, 0,255, 255, 125);
        if(i%3 == 0)SDL_SetRenderDrawColor(renderer,0,255,0,0);
        if(bricks[i]) {
            setBricks(i);
            SDL_RenderFillRect(renderer, &brick);
        }
    }

    SDL_RenderPresent(renderer);
}

Window::Window(){
    window = nullptr;
    screenHeight = height;
    screenWidth = width;
    windowState = WindowState::PLAY;
};
Window::~Window(){};

void Window::run(){
    //init("Bricks Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,width, height ,SDL_WINDOW_SHOWN);
    gameLoop();
}
    

void Window::gameLoop(){
    while(windowState != WindowState::EXIT){
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0) std::cout << "Failed at SDL_Init()" << std::endl;
        if(SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) < 0) std::cout<< "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
        SDL_SetWindowTitle(window, "Brick Breaker");

        TTF_Init();
        font = TTF_OpenFont("font.TTF", FONT_SIZE);
        textfont = TTF_OpenFont("font.TTF",100);
        running = 1;
        static int lastTime=0;
        color.r=color.g=color.b=255;
        paddle.h=12; paddle.w=width/4;
        paddle.y=height-paddle.h-32;
        ball.w = ball.h =SIZE;
        brick.w = (width-(SPACING*COL))/COL;
        brick.h = 22;
        resetBricks();

        while(running) {
            handleEvents();
            lastFrame=SDL_GetTicks();
            if(lastFrame>=lastTime+1000) {
                lastTime=lastFrame;
                fps=frameCount;
                frameCount=0;
            }
            update();
            input();
            render(); 
        
        }
    }
}
void Window::handleEvents(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            windowState = WindowState::EXIT;
            TTF_CloseFont(font);
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            TTF_Quit();
        }
    }
}