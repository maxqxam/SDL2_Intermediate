#include <iostream>
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// #include <vector>
#include <string>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;
TTF_Font* gFont = NULL;
SDL_Texture* Payam1Texture = NULL;
SDL_Texture* Payam2Texture = NULL;
bool shouldRun = true;

enum : unsigned char{
    UP1,DOWN1,
    UP2,DOWN2,
    SPACE
};

bool heldKeys[5];
const int winX = 1000;
const int winY = 750;


struct Objects{
    SDL_Rect  rect  = {0,0,0,0};
    SDL_Color color = {0,0,0};

    void Draw(){
        SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,
                                                    255);
        SDL_RenderDrawRect(renderer,&rect);

    }

};

bool colliderect(SDL_Rect rect1, SDL_Rect rect2){
    return !((rect1.x>rect2.x+rect2.w) ||
             (rect2.x>rect1.x+rect1.w) ||
             (rect1.y>rect2.y+rect2.h) ||
             (rect2.y>rect1.y+rect1.h));
}

Objects Pad1;
Objects Pad2;
Objects Ball;

int Pad1Score = 0;
int Pad2Score = 0;
std::string Payam1 = "Player 1 :";
std::string Payam2 = "Player 2 :";

SDL_Rect Payam1Rect = {0,0,0,0};
SDL_Rect Payam2Rect = {0,0,0,0};

int padX = 10;
int padY = 100;
int padSpeed = 10;
int BallSpeed = 10;

bool pause = false;
int ballDirc[2] = {1,1};
SDL_Color PayamColor = {250,250,235};
void PayamNegari();
void ballMove(){

   
    Ball.rect.y+=BallSpeed*ballDirc[1];
    Ball.rect.x+=BallSpeed*ballDirc[0];
    
    if (Ball.rect.x<0)
    {
        Ball.rect.x = 0;
        ballDirc[0] = 1;
        Pad1Score++;
        PayamNegari();
        pause = true;
    }
    if (Ball.rect.x>(winX-Ball.rect.w))
    {
        Ball.rect.x = winX-Ball.rect.w;
        ballDirc[0] = -1;
        Pad2Score++;
        PayamNegari();
        pause = true;
    }
    if (Ball.rect.y<0)
    { 
        Ball.rect.y = 0;
        ballDirc[1] = 1;
    }

    if (Ball.rect.y>(winY-Ball.rect.h))
    {
        Ball.rect.y = winY-Ball.rect.h;
        ballDirc[1] = -1;
    }

    if (colliderect(Ball.rect,Pad1.rect))
    {
        ballDirc[0] = -1;
    }

    if (colliderect(Ball.rect,Pad2.rect))
    {
        ballDirc[0] = 1;
    }

}

void Rematch(){
    Pad2.rect = {padX*4,(winY/2)-(padY/2),padX,padY};
    Pad1.rect = {winX-padX*4,(winY/2)-(padY/2),padX,padY};
    Ball.rect = {(winX/2)-(padX/2),(winY/2)-(padX/2),padX,padX};
}

void PayamNegari(){
    std::string tempStr1 = Payam1 + std::to_string(Pad1Score);
    std::string tempStr2 = Payam2 + std::to_string(Pad2Score);

    SDL_Surface* tempSurf1 = TTF_RenderText_Solid(gFont,tempStr1.c_str(),PayamColor);
    SDL_Surface* tempSurf2 = TTF_RenderText_Solid(gFont,tempStr2.c_str(),PayamColor);

    Payam1Texture = SDL_CreateTextureFromSurface(renderer,tempSurf1);
    Payam2Texture = SDL_CreateTextureFromSurface(renderer,tempSurf2);

    Payam1Rect.w = tempSurf1->w;
    Payam1Rect.h = tempSurf1->h;

    Payam2Rect.w = tempSurf2->w;
    Payam2Rect.h = tempSurf2->h;

    Payam1Rect.x = winX - Payam1Rect.w;
}
void Init(){
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Pong",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                    winX,winY,SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    gFont = TTF_OpenFont( "./lazy.ttf", 38 );
    if (gFont == NULL){
		std::cout<<SDL_GetError()<<'\n';
	}

    PayamNegari();


    Pad2.rect = {padX*4,(winY/2)-(padY/2),padX,padY};
    Pad1.rect = {winX-padX*4,(winY/2)-(padY/2),padX,padY};

    Pad1.color = {255,255,255};
    Pad2.color = Pad1.color;

    Ball.rect = {(winX/2)-(padX/2),(winY/2)-(padX/2),padX,padX};
    Ball.color = {255,255,255};

    
}

void FetchEvents(){
    while (SDL_PollEvent(&event)!=0){
        if (event.type==SDL_QUIT){shouldRun=false;}
        else if(event.type==SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE: shouldRun=false; break;
                case SDLK_UP: if(!heldKeys[UP1])     heldKeys[UP1] = true; break;
                case SDLK_DOWN: if(!heldKeys[DOWN1])  heldKeys[DOWN1] = true; break;
                case SDLK_w: if(!heldKeys[UP2])  heldKeys[UP2] = true; break;
                case SDLK_s: if(!heldKeys[DOWN2])  heldKeys[DOWN2] = true; break;
                case SDLK_SPACE: if(!heldKeys[SPACE]) heldKeys[SPACE] = true; break;
            }
        }
        else if(event.type==SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                
                case SDLK_UP: if(heldKeys[UP1])     heldKeys[UP1] = 0; break;
                case SDLK_DOWN: if(heldKeys[DOWN1])  heldKeys[DOWN1] = 0; break;
                case SDLK_w: if(heldKeys[UP2])  heldKeys[UP2] = 0; break;
                case SDLK_s: if(heldKeys[DOWN2])  heldKeys[DOWN2] = 0; break;
                case SDLK_SPACE: if(heldKeys[SPACE]) heldKeys[SPACE] = false; break;
            }
        }
    }

}

void CheckEvents(){
    if (pause)
    {
        if (heldKeys[SPACE])
        {     
        pause=false;
        Rematch();
        }
    }
    else{
        if (heldKeys[UP1]) Pad1.rect.y -= padSpeed;
        if (heldKeys[DOWN1]) Pad1.rect.y += padSpeed;
        if (heldKeys[UP2]) Pad2.rect.y -= padSpeed;
        if (heldKeys[DOWN2]) Pad2.rect.y += padSpeed;

        if (Pad1.rect.y<0) Pad1.rect.y=0;
        if (Pad1.rect.y>winY-Pad1.rect.h) Pad1.rect.y=winY-Pad1.rect.h;
        if (Pad2.rect.y<0) Pad2.rect.y=0;
        if (Pad2.rect.y>winY-Pad2.rect.h) Pad2.rect.y=winY-Pad2.rect.h;

        ballMove();


    }

}

void DrawAndUpdate(){
    
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    Pad1.Draw();
    Pad2.Draw();
    Ball.Draw();
    SDL_RenderCopy(renderer,Payam1Texture,NULL,&Payam1Rect);
    SDL_RenderCopy(renderer,Payam2Texture,NULL,&Payam2Rect);
    

    SDL_RenderPresent(renderer);
    SDL_Delay(30);
}


int main(){
    Init();

    while (shouldRun){
        FetchEvents();
        CheckEvents();
        DrawAndUpdate();
    }
}


