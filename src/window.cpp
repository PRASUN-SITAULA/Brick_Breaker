#include <headers/window.h>
#include <headers/newlevel.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <iostream>
#include <cmath>

//Define all the required terms
#define FONT_SIZE 32
#define BALL_SPEED 8
#define SPEED 10
#define SIZE 16
#define COL 10
#define ROW 7
#define PI 3.14159265358979323846
#define SPACING 20
//constant width and height
const int width = 800, height = 600;
//SDL requiremments for rendering

SDL_Renderer *Window::renderer = nullptr;
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font* font, *textfont;
SDL_Color color;

int frameCount, timerFPS, lastFrame, fps ,score;
SDL_Rect paddle, ball, lives, brick, srect={750,10,25,25}, buttonrect={0,0,51,37}, nextrect={55,0,51,37}, imagerect={0,0,1024,600};
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
    velY = BALL_SPEED/3;
    velX = 0;
    ball.x = width/2-(SIZE/2);
}


//to set bricks
void Window::setBricks(int i) {
    if(i==1||i==5||i==7||i==9||i==12||i==15||i==21||i==27||i==30){
        brick.w = 0;
        brick.h = 0;
    }
    else{
        brick.w =  (width-(SPACING*COL))/COL;
        brick.h = 22;
        brick.x = (((i%COL)+1)*SPACING)+((i%COL)*brick.w)-(SPACING/3);
        brick.y = brick.h*3+(((i%ROW)+1)*SPACING)+((i%ROW)*brick.h)-(SPACING/2);
    }
}

//to write bricks and paddle and ball
void Window::write(std::string text,std::string score, int x, int y) {
    SDL_Surface *surface,*surface1,*image,*resetbutton,*nextbutton;
    SDL_Texture *texture,*texture1,*textureresetbutton,*textureimg, *texturenextbutton;
    const char* t=text.c_str();
    const char* s=score.c_str();
    surface = TTF_RenderText_Blended(font, t, color);
    //surface1 for score
    surface1 = TTF_RenderText_Blended(textfont, s, color);
    //
    //surface for reset button;
    resetbutton = IMG_Load("images/reset.png");
    //surface for image
    image = IMG_Load("images/brick.jpg");
    nextbutton = IMG_Load("images/next.png");
    
    textureimg = SDL_CreateTextureFromSurface(renderer, image);
    textureresetbutton = SDL_CreateTextureFromSurface(renderer, resetbutton);
    texturenextbutton = SDL_CreateTextureFromSurface(renderer, nextbutton);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    lives.w = surface->w;
    lives.h = surface->h;
    lives.x = x-lives.w;
    lives.y = y-lives.h;

    SDL_FreeSurface(image);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(resetbutton);
    SDL_FreeSurface(nextbutton);
    
    SDL_RenderCopy(renderer, textureimg, NULL, &imagerect);
    SDL_RenderCopy(renderer, textureresetbutton, NULL, &buttonrect);
    SDL_RenderCopy(renderer, texturenextbutton, NULL, &nextrect);
    SDL_RenderCopy(renderer, texture, NULL, &lives);
    SDL_RenderCopy(renderer, texture1, NULL, &srect);
    
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(textureresetbutton);
    SDL_DestroyTexture(texturenextbutton);
    SDL_DestroyTexture(textureimg);
}

//Updates the status
//i.e. the game logic
void Window::update() {
    if(livesCount<=0) resetBricks();
    if(SDL_HasIntersection(&ball, &paddle)) {
        double rel = (paddle.x+(paddle.w/2))-(ball.x+(SIZE/2));
        double norm = rel/(paddle.w/3);
        double bounce = norm* (4*PI/12);
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
    for(int i=0 ; i<COL*ROW; i++) {
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
    write(std::to_string(livesCount),std::to_string(score),width/2+FONT_SIZE/2, FONT_SIZE*1.5);
    frameCount++;
    timerFPS = SDL_GetTicks()-lastFrame;
    if(timerFPS<(1000/60)) {
        SDL_Delay((1000/60)-timerFPS);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle);
    SDL_RenderFillRect(renderer, &ball);
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
    gameLoop();
}
    
void Window::gameLoop(){
    while(windowState != WindowState::EXIT){
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0) std::cout << "Failed at SDL_Init()" << std::endl;
        if(SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) < 0) std::cout<< "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
        SDL_SetWindowTitle(window, "Brick Breaker");

        int imgflags = IMG_INIT_JPG;
        TTF_Init();
        IMG_Init(imgflags);
        font = TTF_OpenFont("ALGER.TTF", FONT_SIZE);
        textfont = TTF_OpenFont("ALGER.TTF",45);
        
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
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            windowState = WindowState::EXIT;
            TTF_CloseFont(font);
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            TTF_Quit();
        }

        if(SDL_MOUSEBUTTONDOWN == event.type)
        {
            SDL_Point mousePosition;
            // Mouse click coords from event handler
            mousePosition.x = event.motion.x; 
            mousePosition.y = event.motion.y;

            if (SDL_PointInRect(&mousePosition, &buttonrect)) {
                std::cout<<"mouse is pressed"<<std::endl;
                resetBricks();
            }
        }

        if(SDL_MOUSEBUTTONDOWN == event.type)
        {
            SDL_Point mousePosition;
            // Mouse click coords from event handler
            mousePosition.x = event.motion.x; 
            mousePosition.y = event.motion.y;

            if (SDL_PointInRect(&mousePosition, &nextrect)) {
                std::cout<<"mouse is pressed"<<std::endl;
                NewLevel nl;
                nl.run();
            }
        }
    }
}