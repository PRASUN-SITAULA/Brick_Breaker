#include <headers/newlevel.h>
#include <headers/window.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <iostream>
#include <cmath>

//Define all the required terms
#define FONT_SIZE 32
#define _ball_SPEED 10
#define SPEED 10
#define SIZE 16
#define COL 13
#define ROW 8
#define PI 3.14159265358979323846
#define SPACING 16
const int width = 800, height = 600;
//SDL requiremments for rendering

SDL_Renderer *NewLevel::renderer = nullptr;
SDL_Window *_window;
SDL_Renderer *_renderer;
TTF_Font* _font, *_textfont;
SDL_Color _color;


int _framecount, _timerfps, _lastframe, _fps ,_score;
SDL_Rect _paddle, _ball, _lives, _brick, _srect={750,10,25,25}, _buttonrect={0,0,51,43}, _imagerect={0,0,1024,600}, _unbreakrect={130,170,230,40};
float _velY, _velX;
int _livescount;
bool _bricks[ROW*COL];
bool _running;

//resets bricks for play again
void NewLevel::resetBricks() {
    for(int i=0; i<COL*ROW; i++) _bricks[i]=1;
    _livescount = 3;
    _score = 0;
    _paddle.x = (width/2)-(_paddle.w/2);
    _ball.y = _paddle.y-(_paddle.h*4);
    _velY = _ball_SPEED/2;
    _velX = 0;
    _ball.x = width/2-(SIZE/2);
}


//to set bricks
void NewLevel::setBricks(int i) {
    if(i==2 || i==3 || i==18 || i==42 || i==43 || i==67 || i==82 || i==83){
        _brick.w = 0;
        _brick.h = 0;
    }
    else{
        _brick.w = (width-(SPACING*COL))/COL;
        _brick.h = 22;
        _brick.x = (((i%COL)+1)*SPACING)+((i%COL)*_brick.w)-(SPACING/4);
        _brick.y = _brick.h*3+(((i%ROW)+1)*SPACING)+((i%ROW)*_brick.h)-(SPACING/4);
    }
    
}

//to write bricks and _paddle and _ball
void NewLevel::write(std::string text,std::string _score, int x, int y) {
    SDL_Surface *surface,*surface1,*image,*button,*unbreakable;
    SDL_Texture *texture,*texture1,*texturebutton,*textureimg,*textureunbreakable;
    const char* t=text.c_str();
    const char* s=_score.c_str();
    surface = TTF_RenderText_Blended(_font, t, _color);
    //surface1 for _score
    surface1 = TTF_RenderText_Blended(_font, s, _color);
    //surface for button;
    button = IMG_Load("images/back.png");
    //surface for image
    image = IMG_Load("images/brick.jpg");
    //surface for unbreakable brick
    unbreakable = IMG_Load("images/unbreakable.png");
    
    textureimg = SDL_CreateTextureFromSurface(_renderer, image);
    texturebutton = SDL_CreateTextureFromSurface(_renderer, button);
    textureunbreakable = SDL_CreateTextureFromSurface(_renderer, unbreakable);
    texture = SDL_CreateTextureFromSurface(_renderer, surface);
    texture1 = SDL_CreateTextureFromSurface(_renderer, surface1);
    _lives.w = surface->w;
    _lives.h = surface->h;
    _lives.x = x-_lives.w;
    _lives.y = y-_lives.h;

    SDL_FreeSurface(image);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(button);
    SDL_FreeSurface(unbreakable);
    
    SDL_RenderCopy(_renderer, textureimg, NULL, &_imagerect);
    SDL_RenderCopy(_renderer, texturebutton, NULL, &_buttonrect);
    SDL_RenderCopy(_renderer, texture1, NULL, &_srect);
    SDL_RenderCopy(_renderer, textureunbreakable, NULL, &_unbreakrect);
    SDL_RenderCopy(_renderer, texture, NULL, &_lives);
    
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texturebutton);
    SDL_DestroyTexture(textureimg);
    SDL_DestroyTexture(textureunbreakable);
}

//Updates the status
//i.e. the game logic
void NewLevel::update() {
    if(_livescount<=0) resetBricks();
    if(SDL_HasIntersection(&_ball, &_paddle)) {
        double rel = (_paddle.x+(_paddle.w/2))-(_ball.x+(SIZE/2));
        double norm = rel/(_paddle.w/3);
        double bounce = norm* (5*PI/12);
        _velY = -_ball_SPEED*cos(bounce);
        _velX = _ball_SPEED*-sin(bounce);
    }

    if(_ball.y<=0) _velY = -_velY;
    if(_ball.y+SIZE>=height) {_velY=-_velY;_livescount--;}
    if(_ball.x<=0 || _ball.x+SIZE>=width) _velX=-_velX;
    _ball.x += _velX;
    _ball.y += _velY;
    if(_paddle.x<0) _paddle.x=0;
    if(_paddle.x+_paddle.w>width) _paddle.x = width-_paddle.w;

    bool reset = 1;
    int count =0;
    for(int i=0; i<COL*ROW; i++)
    {
        setBricks(i);
        if(SDL_HasIntersection(&_ball, &_brick) && _bricks[i]) {
            _score += 1;
            _bricks[i] = 0;
            if(_ball.x >= _brick.x) {_velX=-_velX; _ball.x-=20;}
            if(_ball.x <= _brick.x) {_velX=-_velX; _ball.x+=20;}
            if(_ball.y <= _brick.y) {_velY=-_velY; _ball.y-=20;}
            if(_ball.y >= _brick.y) {_velY=-_velY; _ball.y+=20;}
        }
        if(SDL_HasIntersection(&_ball, &_unbreakrect)) {
            if(_ball.x >= _brick.x) {_velX=-_velX; _ball.x-=20;}
            if(_ball.x <= _brick.x) {_velX=-_velX; _ball.x+=20;}
            if(_ball.y <= _brick.y) {_velY=-_velY; _ball.y-=20;}
            if(_ball.y >= _brick.y) {_velY=-_velY; _ball.y+=20;}
        }
        if(_bricks[i]) reset=0;
    }
    if(reset) resetBricks();
}

//Keys Input
void NewLevel::input() {
    SDL_Event e;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&e)) if(e.type==SDL_QUIT) _running=false;
    if(keystates[SDL_SCANCODE_ESCAPE]) _running=false;
    if(keystates[SDL_SCANCODE_LEFT]) _paddle.x-=SPEED;
    if(keystates[SDL_SCANCODE_RIGHT]) _paddle.x+=SPEED;
}

//rendering the game for playing
void NewLevel::render() {
    SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(_renderer);
    write(std::to_string(_livescount),std::to_string(_score),width/2+FONT_SIZE/2, FONT_SIZE*1.5);
    _framecount++;
    _timerfps = SDL_GetTicks()-_lastframe;
    if(_timerfps<(1000/60)) {
        SDL_Delay((1000/60)-_timerfps);
    }
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(_renderer, &_paddle);
    SDL_RenderFillRect(_renderer, &_ball);
    

    for(int i=0; i<COL*ROW; i++) {
        SDL_SetRenderDrawColor(_renderer, 250, 95, 85, 255);
        if(i%2 == 0)SDL_SetRenderDrawColor(_renderer, 255,234, 0, 255);
        if(i%3 == 0)SDL_SetRenderDrawColor(_renderer,127, 0, 255,255);
        if(_bricks[i]) {
            setBricks(i);
            SDL_RenderFillRect(_renderer, &_brick);
        }
    }
    
    SDL_RenderPresent(_renderer);
}

NewLevel::NewLevel(){
    _window = nullptr;
    screenHeight = height;
    screenWidth = width;
    levelState = LevelState::PLAY;
};
NewLevel::~NewLevel(){};

void NewLevel::run(){
    //init("Bricks Breaker", SDL__windowPOS_CENTERED, SDL__windowPOS_CENTERED,width, height ,SDL__window_SHOWN);
    gameLoop();
}
    

void NewLevel::gameLoop(){
    while(levelState != LevelState::EXIT){
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0) std::cout << "Failed at SDL_Init()" << std::endl;
        if(SDL_CreateWindowAndRenderer(width, height, 0, &_window, &_renderer) < 0) std::cout<< "Failed at SDL_CreateWindowAnd_renderer()" << std::endl;
        SDL_SetWindowTitle(_window, "Brick Breaker");

        int imgflags = IMG_INIT_JPG||IMG_INIT_PNG;
        TTF_Init();
        IMG_Init(imgflags);
        _font = TTF_OpenFont("ALGER.TTF", FONT_SIZE);
        _textfont = TTF_OpenFont("ALGER.TTF",100);
        
        _running = 1;
        static int lastTime=0;
        _color.r=_color.g=_color.b=255;
        _paddle.h=12; _paddle.w=width/4;
        _paddle.y=height-_paddle.h-32;
        _ball.w = _ball.h =SIZE;
        _brick.w = (width-(SPACING*COL))/COL;
        _brick.h = 22;
        resetBricks();

        while(_running) {
            handleEvents();
            _lastframe=SDL_GetTicks();
            if(_lastframe>=lastTime+1000) {
                lastTime=_lastframe;
                _fps=_framecount;
                _framecount=0;
            }
            update();
            input();
            render(); 
        
        }
    }
}

void NewLevel::handleEvents(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            levelState = LevelState::EXIT;
            TTF_CloseFont(_font);
            SDL_DestroyWindow(_window);
            SDL_DestroyRenderer(_renderer);
            SDL_Quit();
            TTF_Quit();
        }
    }
}