#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#define out(x) std::cout<<x;
#define enter out('\n')
#define space out(' ')

int winX = 700;
int winY = 700;
int gridX = 25;
int gridY = 25;
SDL_Rect gameGridRect = {2,2,11,21};
SDL_FRect gameGridMask;
float gridXSize;
float gridYSize;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

bool shouldRun = true;
bool showGrid = true;
int tickSpeed = 300;
enum : unsigned char{
    UP,DOWN,
    RIGHT,LEFT,
    ENTER,F1
};

bool heldKeys[6];

struct Pos{
    int x;
    int y;
};

struct Logo{
    std::vector<Pos> array;
    int maxX;
};

SDL_Color colorArray[8] = {
    {255,0,0},
    {0,255,0},
    {0,0,255},
    {255,255,0},
    {255,0,255},
    {0,255,255},
    {0,255,255},
    {0,255,255}
};

Logo logoArray[8];

bool isInCanvas(Logo logo,SDL_FRect rect,SDL_Rect canvas){

    for (int i=0;i!=logo.array.size();i++){
        if (logo.array[i].x + rect.x < canvas.x){return false;}
        if (logo.array[i].y + rect.y < canvas.y){return false;}
        if (logo.array[i].x + rect.x > canvas.x+canvas.w-1){return false;}
        if (logo.array[i].y + rect.y > canvas.y+canvas.h-1){return false;}        
    }

    return true;
};

bool isCollided(Logo logo,SDL_FRect rect,int isHolding);
void makePiece();
struct Piece{
    SDL_FRect rect;
    Logo logo;
    SDL_Color color;

    void rotate(){
        Logo tempLogo = logo;
        for (int i=0;i!=logo.array.size();i++){
            tempLogo.array[i] = {-tempLogo.array[i].y,tempLogo.array[i].x};            
        }
        if (isInCanvas(tempLogo,rect,gameGridRect) &&
            !isCollided(tempLogo,rect,1)){
            logo = tempLogo;
        }
    }


    void move(unsigned char Dirc){
        SDL_FRect tempRect = rect;
        if (Dirc==RIGHT){tempRect.x+=1;}
        else if(Dirc==LEFT){tempRect.x-=1;}
        else if(Dirc==DOWN){tempRect.y+=1;}
        if (isInCanvas(logo,tempRect,gameGridRect) &&
            !isCollided(logo,tempRect,1)){
            rect = tempRect;
        }else if(Dirc==DOWN){
            heldKeys[DOWN]=false;
            makePiece();
        }

    }
};

std::vector<Piece> pieceArray;

bool isCollided(Logo logo,SDL_FRect rect,int isHolding)
{
    

    for (int i=0;i!=logo.array.size();i++)
    {
        for (int c=0;c!=pieceArray.size()-isHolding;c++)
        {
           if (pieceArray.size()==0) break;
           for (int j=0;j!=pieceArray[c].logo.array.size();j++)            
           {

            if ((pieceArray[c].logo.array[j].x +
                pieceArray[c].rect.x == logo.array[i].x+rect.x)&&
                (pieceArray[c].logo.array[j].y +
                pieceArray[c].rect.y == logo.array[i].y+rect.y))
            {return true;}


           }
        }
    }

    return false;
}

void initLogo(){
    logoArray[0].array.push_back({0,0});
    logoArray[0].array.push_back({0,1});
    logoArray[0].array.push_back({1,0});
    logoArray[0].array.push_back({1,1});
    logoArray[0].maxX = 1;

    logoArray[1].array.push_back({0,0});
    logoArray[1].array.push_back({1,0});
    logoArray[1].array.push_back({2,0});
    logoArray[1].array.push_back({2,1});
    logoArray[1].maxX = 2;

    logoArray[2].array.push_back({0,0});
    logoArray[2].array.push_back({1,0});
    logoArray[2].array.push_back({2,0});
    logoArray[2].array.push_back({1,1});
    logoArray[2].maxX = 2;

    logoArray[3].array.push_back({0,0});
    logoArray[3].array.push_back({1,0});
    logoArray[3].array.push_back({2,0});
    logoArray[3].array.push_back({3,0});
    logoArray[3].maxX = 3;

    logoArray[4].array.push_back({0,0});
    logoArray[4].array.push_back({0,1});
    logoArray[4].array.push_back({1,0});
    logoArray[4].array.push_back({1,1});
    logoArray[4].array.push_back({2,1});
    logoArray[4].maxX = 2;

    logoArray[5].array.push_back({0,0});
    logoArray[5].array.push_back({1,0});
    logoArray[5].array.push_back({2,0});
    logoArray[5].array.push_back({0,1});
    logoArray[5].array.push_back({2,1});
    logoArray[5].maxX = 2;

    logoArray[6].array.push_back({0,0});
    logoArray[6].array.push_back({0,1});
    logoArray[6].array.push_back({1,1});
    logoArray[6].maxX = 1;

    logoArray[7].array.push_back({0,0});
    logoArray[7].array.push_back({0,1});
    logoArray[7].array.push_back({1,1});
    logoArray[7].array.push_back({1,2});
    logoArray[7].maxX = 1;


};

void makePiece(){

    // out(pieceArray.size());
    int num;     
    num = rand()%8;
    //out(num) enter
    
    SDL_FRect  r1 = {float((gameGridRect.x+gameGridRect.w+1)/2)
                            -logoArray[num].maxX/2
                    ,float(gameGridRect.y),gridXSize,gridYSize};
    
    if (!isCollided(logoArray[num],r1,0)){
        pieceArray.push_back({r1,logoArray[num],colorArray[num]});
    }
}


void setColor(SDL_Color cr){
    SDL_SetRenderDrawColor(renderer,cr.r,cr.g,cr.b,255);
}

void piecesDraw(){
    SDL_FRect mask={0,0,0,0};
    for (int i=0;i!=pieceArray.size();i++){
        for (int c=0;c!=pieceArray[i].logo.array.size();c++){
            mask.w = pieceArray[i].rect.w;
            mask.h = pieceArray[i].rect.h;
            // out(c) space out(mask.w) space out(mask.h) space
            mask.x = pieceArray[i].rect.x * mask.w + 
            pieceArray[i].logo.array[c].x * mask.w;
            mask.y = pieceArray[i].rect.y * mask.h + 
            pieceArray[i].logo.array[c].y * mask.h;
            // out(mask.x) space out(mask.y) enter
            setColor(pieceArray[i].color);
            SDL_RenderFillRectF(renderer,&mask);
            
        }

    }
}


void Init(){
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("TETRIS",SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,winX,winY,SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    initLogo();
    srand(time(0));


    gridXSize = float(winX) / gridX;
    gridYSize = float(winY) / gridY;
    
    makePiece();

    gameGridMask.x = gameGridRect.x * gridXSize;
    gameGridMask.y = gameGridRect.y * gridXSize;
    gameGridMask.w = gameGridRect.w * gridXSize;
    gameGridMask.h = gameGridRect.h * gridXSize;
};

void FetchEvents(){
    while (SDL_PollEvent(&event)!=0){
        if (event.type==SDL_QUIT){
            shouldRun = false;
        }else if(event.type==SDL_KEYDOWN){

            switch (event.key.keysym.sym){
                case SDLK_ESCAPE: shouldRun = false;break;
                case SDLK_UP: heldKeys[UP] = true; break;
                case SDLK_DOWN: heldKeys[DOWN] = true; break;
                case SDLK_RIGHT: heldKeys[RIGHT] = true; break;
                case SDLK_LEFT: heldKeys[LEFT] = true; break;
                // case SDLK_RETURN: heldKeys[ENTER] = true; break;
                case SDLK_F1: heldKeys[F1] = true; break;
            }
        }
        else if(event.type==SDL_KEYUP){

            switch (event.key.keysym.sym){

                case SDLK_UP:    if(heldKeys[UP]) heldKeys[UP] = false; break;
                // case SDLK_DOWN:  if(heldKeys[DOWN])  heldKeys[DOWN] = false; break;
                case SDLK_RIGHT: if(heldKeys[RIGHT])  heldKeys[RIGHT] = false; break;
                case SDLK_LEFT:  if(heldKeys[LEFT])  heldKeys[LEFT] = false; break;
                // case SDLK_RETURN: if(heldKeys[ENTER]) heldKeys[ENTER] = false;break;
                case SDLK_F1:    if(heldKeys[F1])  heldKeys[F1] = false; break;
            }
        }
    }
};

int lastFrame = 0;
int currentFrame = 0;
bool shouldFall = false;
void CheckEvents(){
    currentFrame = SDL_GetTicks();

    if (currentFrame-lastFrame>tickSpeed){
        shouldFall=true;
        lastFrame = SDL_GetTicks();
    }

    if (heldKeys[F1]){heldKeys[F1]=false; showGrid=!showGrid;}
    // if (heldKeys[ENTER]){heldKeys[ENTER]=false;makePiece();}

    if (heldKeys[UP])
    {heldKeys[UP]=false; pieceArray[pieceArray.size()-1].rotate();}
    if (heldKeys[RIGHT])
    {heldKeys[RIGHT]=false; pieceArray[pieceArray.size()-1].move(RIGHT);}
    if (heldKeys[LEFT])
    {heldKeys[LEFT]=false; pieceArray[pieceArray.size()-1].move(LEFT);}
    if (heldKeys[DOWN] || shouldFall)
    { pieceArray[pieceArray.size()-1].move(DOWN);shouldFall=false;}


};

void DrawAndUpdate(){
    SDL_SetRenderDrawColor(renderer,200,200,200,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,40,40,40,255);
    SDL_RenderFillRectF(renderer,&gameGridMask);
    
    piecesDraw();

    if (showGrid){
        SDL_SetRenderDrawColor(renderer,120,120,120,255);

        
        for (float i=1;i!=gridX;i++){
            SDL_RenderDrawLineF(renderer,i*gridXSize,0,i*gridXSize,winY);
        }
        for (float i=1;i!=gridY;i++){
            SDL_RenderDrawLineF(renderer,0,i*gridYSize,winX,i*gridYSize);
        }
    }

    

    SDL_RenderPresent(renderer);
    SDL_Delay(50);
};

int main(){
    Init();

    while (shouldRun){
        FetchEvents();
        CheckEvents();
        DrawAndUpdate();
    }


    return 0;
}