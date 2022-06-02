#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GSWE.hpp"

SDL_Texture* loadTexture(SDL_Renderer* p_renderer,
                std::string path,int &p_width,
                                 int &p_height){
    
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface==NULL){
        std::cout<<path<<'\n'<<'\t';
        std::cout<<SDL_GetError()<<std::endl;
        return NULL;
    }
    newTexture = SDL_CreateTextureFromSurface(p_renderer,loadedSurface);
    if (newTexture==NULL){
        std::cout<<path<<'\n'<<'\t';
        std::cout<<SDL_GetError()<<std::endl;
        return NULL;
    }
    p_width = loadedSurface->w;
    p_height = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    
    return newTexture;
}

MyWindow::Window mainWindow;
GSWE::GridSystem mainGrid;

bool shouldRun = true;
SDL_Event event;

void Init(){
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    mainWindow.Init("I Love OOP",1000,750);
    mainGrid.Init(1000,750,10,10); //why Renderer?



    SDL_Texture* tempTexture;
    int tempWidth;
    int tempHeight;
    GSWE::Tile tempTile;
    std::string paths[] = {
        "/home/yolo/Workstation/Graphies/Tiles/oimages/stone0.png",
        "/home/yolo/Workstation/Graphies/Tiles/oimages/water0.png",
        "/home/yolo/Workstation/Graphies/Tiles/oimages/dirt0.png"
    };

    for (int i=0;i!=3;i++){
        tempTexture = loadTexture(mainWindow.renderer,
                        paths[i].c_str(),
                        tempWidth,tempHeight);
        
        tempTile = {tempTexture,tempWidth,tempHeight};

        GSWE::tileArray.push_back(tempTile);

    }

    for (int i=0;i!=mainGrid.gridWidth;i++)
    {
        GSWE::mapTilesArray.push_back({{i,0},0});
    }

}

void FetchEvents(){
    while (SDL_PollEvent(&event)!=0)
    {
        switch (event.type)
        {
            case (SDL_KEYDOWN):
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: shouldRun = false; break;
                }

            case (SDL_WINDOWEVENT):             
                mainWindow.FetchWindowEvent(event);
                break;       
        }

    }
}

void CheckEvents(){
    
    if (mainWindow.isClosed){ shouldRun=false; }
    if (mainWindow.sizeChanged){ 
        mainGrid.Update( mainWindow.width,mainWindow.height);
        mainWindow.sizeChanged=false;
    }
}

void DrawAndUpdate(){
    mainGrid.Draw(mainWindow.renderer);

    SDL_RenderPresent(mainWindow.renderer);
}

int main(){
    Init();

    while (shouldRun){
        FetchEvents();
        CheckEvents();
        DrawAndUpdate();
    }

    return 0;
}