#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstring>
#include <vector>

#define out(x) std::cout<<x;
#define enter out('\n')
#define space out(' ')

std::string imagesPath = "/home/yolo/Workstation/C++/SDL/oimages/";
std::string imagesArray[] = {"dirt0.png",
                             "dirt3.png",
                             "dirt2.png",
                             "fire0.png",
                             "flower0.png"};


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

float winX = 1000;
float winY = 800;
int gridX = 48;
int gridY = 39;
float gridXSize;
float gridYSize;

bool shouldRun = true;
bool showGrid  = true;

enum : unsigned char{
    SPACE,F1
};

enum : unsigned char{
    DIRT,STONE,
    WATER,FIRE,
    FLOWER
};

struct chunkTiles{
    unsigned char id = 0;
    SDL_FRect rect;
};






std::vector<chunkTiles> chunkArray;

int randint(int a,int b){
    return (std::rand() % (b-a+1)) + a;
}

void GenerateChunk(){//Simple Chunk
    chunkArray.clear();
    unsigned char index = 0;
    
    int maxWidth = randint(10,40);
    int maxHeight = randint(4,15);
    
    int x = randint(0,gridX-maxWidth);
    int y = randint(0,gridY-maxHeight);
    
    bool ifDo = true;

    for (int i=0;i!=maxWidth;i++)
    {
        for (int c=-1;c!=maxHeight;c++){
            if (c==-1){index=randint(3,4);}
            else if (c==0){index=0;}
            else if (c>0){index=randint(1,2);}

            if (c==-1){if (randint(1,10)!=10) ifDo=false;}
            if (c>0){if (randint(1,10-c)==1) break;}
            
            if (ifDo){
                chunkArray.push_back({index,
                           {float(x+i),float(y+c),gridXSize,gridYSize}});
            }
            ifDo = true;
        }
    }
    // out(chunkArray.size()) enter

}



SDL_Texture* loadTexture(std::string path,SDL_FRect& _rect){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface==NULL){
        std::cout<<path<<'\n'<<'\t';
        std::cout<<SDL_GetError()<<std::endl;
        return NULL;
    }
    newTexture = SDL_CreateTextureFromSurface(renderer,loadedSurface);
    if (newTexture==NULL){
        std::cout<<path<<'\n'<<'\t';
        std::cout<<SDL_GetError()<<std::endl;
        return NULL;
    }
    _rect.w = loadedSurface->w;
    _rect.h = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    
    return newTexture;
}

struct Tile{
    private:
    std::string path;
    SDL_Texture* texture;
    SDL_FRect rect = {0,0,0,0};

    public:

    void Define(std::string _path){
        texture = loadTexture(_path,rect);
    }
    SDL_Texture* GetTexture(){return texture;};

};
Tile testTile;

Tile tilesArray[5];


void gridDraw(){
    SDL_SetRenderDrawColor(renderer,180,180,180,255);
    
    for (int i=1;i!=gridX;i++)
    {
        SDL_RenderDrawLineF(renderer,i*gridXSize,0,i*gridXSize,winY);
    }

    for (int i=1;i!=gridY;i++)
    {
        SDL_RenderDrawLineF(renderer,0,i*gridYSize,winX,i*gridYSize);
    }

}

bool heldKeys[2];

void Init(){
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
 
    window = SDL_CreateWindow("Chunk Generator",SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,winX,winY,
                                                    SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    gridXSize = winX / gridX;
    gridYSize = winY / gridY;


    
    for (int i=0;i!=gridX;i++){
        chunkArray.push_back({DIRT,{float(i),
                        float(gridY-1),gridXSize,gridYSize}});
    }
    

    tilesArray[0].Define(imagesPath+imagesArray[0]);
    tilesArray[1].Define(imagesPath+imagesArray[1]);
    tilesArray[2].Define(imagesPath+imagesArray[2]);
    tilesArray[3].Define(imagesPath+imagesArray[3]);
    tilesArray[4].Define(imagesPath+imagesArray[4]);

    
    SDL_FRect rect;
    
}

void DrawChunk(){
    SDL_FRect maskRect = {0,0,0,0};
    for (int i=0;i!=chunkArray.size();i++)
    {
        maskRect = chunkArray[i].rect;
        maskRect.x *= gridXSize;
        maskRect.y *= gridYSize;
        
        SDL_RenderCopyF(renderer,tilesArray[chunkArray[i].id].GetTexture()
                                ,NULL,&maskRect);



    }
}

void FetchEvents(){
    while (SDL_PollEvent(&event)!=0){
        if(event.type==SDL_QUIT){shouldRun=false;}
        else if(event.type==SDL_KEYDOWN){
            switch (event.key.keysym.sym){
                case SDLK_ESCAPE: shouldRun = false; break;
                case SDLK_SPACE: heldKeys[SPACE] = true;break;
                case SDLK_F1: heldKeys[F1] = true;break;
            }
        }
        else if(event.type==SDL_KEYUP){
            switch (event.key.keysym.sym){
                case SDLK_SPACE: if(heldKeys[SPACE]) heldKeys[SPACE] = false;
                break;
                case SDLK_F1: if(heldKeys[F1]) heldKeys[F1] = false;  
                break;
            }
        }
    }

}

void CheckEvents(){
    if (heldKeys[F1]){heldKeys[F1]=false;showGrid=!showGrid;}
    if (heldKeys[SPACE]){heldKeys[SPACE]=false;GenerateChunk();}

}

void DrawAndUpdate(){
    SDL_SetRenderDrawColor(renderer,239,255,255,255);
    SDL_RenderClear(renderer);

    if (showGrid){
        gridDraw();
    }
    DrawChunk();

    SDL_RenderPresent(renderer);
    SDL_Delay(50);
}



int main(){
    Init();

    while (shouldRun){
        FetchEvents();
        CheckEvents();
        DrawAndUpdate();
    }
}
