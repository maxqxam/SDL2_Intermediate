#include <iostream>
#include <SDL2/SDL.h>
#include <vector>


#define out(x) std::cout<<x;
#define enter std::cout<<'\n';
#define space std::cout<<' ';

const int winDims[2] = {1200,750};
int gridDims[2];
float gridSize[2];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

bool shouldRun = true;
bool failed = false;
enum : unsigned char{
    UP,DOWN,
    RIGHT,LEFT,
    SPACE,ESCAPE
};

char Dirc = 'n';
bool heldKeys[6];

struct Color{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;
};

Color bgColor = {150,150,215,255};
Color gridsColor = {70,30,30,255};
Color headColor = {100,40,40,255};
void SetColor(SDL_Renderer& _renderer, Color _color){
    SDL_SetRenderDrawColor(&_renderer,_color.r,_color.g,_color.b,_color.a);
}


struct Pos{
    float x = 0;
    float y = 0;
};

Pos Food = {0,0};
void makeFood()
{
    Food.x = rand()%gridDims[0];
    Food.y = rand()%gridDims[1];
}

Color foodColor = {120,30,200,255};


void DrawFood()
{
    SDL_FRect tempRect = {Food.x * gridSize[0],Food.y * gridSize[1],
                        gridSize[0],gridSize[1]};

    // out(Food.x) space out(Food.y) enter

    SetColor(*renderer,foodColor);
    SDL_RenderFillRectF(renderer,&tempRect);
}


struct Snake{
    SDL_FRect rect;
    Color color;
    std::vector<Pos> body;
    
    Snake(SDL_FRect _rect, Color _color)
    {

        rect = _rect;
        color = _color;
        Pos pos = {rect.x,rect.y};
        body.push_back(pos);
    };

    void remake(){
        Pos pos = {rect.x,rect.y}; 
        body.clear();
        body.push_back(pos);
        makeFood();
        
    }

    void Draw(SDL_Renderer &_renderer)
    {
        SDL_FRect mask = {0,0,rect.w,rect.h};
        SetColor(_renderer,color);
        for (int i=0; i!=body.size();i++)
        {
            if (i==body.size()-1)
            {
                SetColor(_renderer,headColor);
            }

            mask.x = body[i].x * gridSize[0] ;
            mask.y = body[i].y * gridSize[1] ;

            SDL_RenderFillRectF(&_renderer,&mask);
            
        }
    }

    void Move(unsigned char Direction)
    {
        Pos newPos = body[body.size()-1];
        
        switch (Direction)
        {
            case UP:    newPos.y -= 1; break;
            case DOWN:  newPos.y += 1; break;
            case RIGHT: newPos.x += 1; break;
            case LEFT:  newPos.x -= 1; break;
        }

        if (newPos.y<0){newPos.y=gridDims[1]-1;};
        if (newPos.x<0){newPos.x=gridDims[0]-1;};
        if (newPos.y>gridDims[1]-1){newPos.y=0;};
        if (newPos.x>gridDims[0]-1){newPos.x=0;};

        
        body.push_back(newPos);
        if ((newPos.x == Food.x) && (newPos.y == Food.y)){makeFood();}
        else{body.erase(body.begin());}

        for (int i=0;i!=body.size()-1;i++)
        {
            if ((body[i].x == newPos.x)&&(body[i].y == newPos.y))
            {
                Dirc = 'n';
                failed = true;
            }
        }
        
    }

    
    
};
Snake Player = {{0,0,0,0},{0,0,0,0}};
void Init(){
    gridDims[0] = 65; gridDims[1] = 40;
    gridSize[0] = float(winDims[0])/gridDims[0];
    gridSize[1] = float(winDims[1])/gridDims[1];

    Player = { { float(gridDims[0]/2),float(gridDims[1]/2)
                         ,gridSize[0] ,gridSize[1]-0}
                     , {50,140,50,255}};


    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake",0,0,winDims[0],winDims[1],SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    
}

void FetchEvents(){
    while (SDL_PollEvent(&event)!=0)
    {
        if (event.type==SDL_QUIT)
        {
            shouldRun = false;
        }
        else if(event.type==SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    shouldRun = false; break;

                case SDLK_UP:   if (!heldKeys[UP]) heldKeys[UP] = true; break;
                case SDLK_DOWN: if (!heldKeys[DOWN]) heldKeys[DOWN] = true; break;
                case SDLK_RIGHT: if (!heldKeys[RIGHT]) heldKeys[RIGHT] = true; break;
                case SDLK_LEFT: if (!heldKeys[LEFT]) heldKeys[LEFT] = true; break;
                case SDLK_SPACE: if (!heldKeys[SPACE]) heldKeys[SPACE] = true; break;
                
            }
        }
        else if(event.type==SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                
                case SDLK_UP: if (heldKeys[UP]) heldKeys[UP] = false; break;
                case SDLK_DOWN: if (heldKeys[DOWN]) heldKeys[DOWN] = false; break;
                case SDLK_RIGHT: if (heldKeys[RIGHT]) heldKeys[RIGHT] = false; break;
                case SDLK_LEFT: if (heldKeys[LEFT]) heldKeys[LEFT] = false; break;
                case SDLK_SPACE: if (heldKeys[SPACE]) heldKeys[SPACE] = false; break;
                
            }
        }

    }

}


void CheckEvents(){
    
    if (heldKeys[UP]&&Dirc!='d'){       Dirc = 'u'; heldKeys[UP]    = false;}
    else if (heldKeys[DOWN]&&Dirc!='u'){   Dirc = 'd'; heldKeys[DOWN]  = false;}
    else if (heldKeys[RIGHT]&&Dirc!='l'){    Dirc = 'r'; heldKeys[RIGHT] = false;}
    else if (heldKeys[LEFT]&&Dirc!='r'){     Dirc = 'l'; heldKeys[LEFT]  = false;}
    if (heldKeys[SPACE] && failed){ failed = false; Player.remake(); Dirc = 'n';}
    
    if (!failed)
    {
    switch (Dirc)
    {
        case 'u': Player.Move(UP); break;
        case 'd': Player.Move(DOWN); break;
        case 'r': Player.Move(RIGHT); break;
        case 'l': Player.Move(LEFT); break;
    }
    }
}

void DrawAndUpdate(){
    SetColor(*renderer,bgColor);
    SDL_RenderClear(renderer);

    SetColor(*renderer,gridsColor);
    for (int i=1; i!=gridDims[0]; i++)
    {
        SDL_RenderDrawLine(renderer,i*gridSize[0],0,i*gridSize[0],winDims[1]);
    }

    for (int i=1; i!=gridDims[1]; i++)
    {
        SDL_RenderDrawLine(renderer,0,i*gridSize[1],winDims[0],i*gridSize[1]);
    }

    Player.Draw(*renderer);
    DrawFood();
    SDL_RenderPresent(renderer);
    SDL_Delay(50);
}



int main(){

    Init();

    makeFood();


    while (shouldRun){
        FetchEvents();
        CheckEvents();
        DrawAndUpdate();

    }


    return 0;
}