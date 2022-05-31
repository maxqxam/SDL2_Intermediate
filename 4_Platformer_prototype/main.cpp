#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "definitions.hpp"
#include "declarations.hpp"
#include "structures.hpp"
#include "snippets.hpp"


void Reshape(){
    gridXSize = float(mainWindow.rect.w) / gridCount.x;
    gridYSize = float(mainWindow.rect.h) / gridCount.y; 
    
    for (int i=0;i!=ImagesArray.size();i++){
        ImagesArray[i].texture = loadTexture(
            mainWindow.renderer,std::string(TexturesAddress+TexturesNames[i]),
                        ImagesArray[i].rect
         );
    }  
}

void Init(){
   
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Pos displaySize = {1000,700};
    gridCount = {20,15};
    mainWindow.Create("TITLE",{0,0,displaySize.x,
                                    displaySize.y});


    Reshape();
    

    for (int i=0;i!=gridCount.x;i++){
        TilesArray.push_back({{i,0},0});
        TilesArray.push_back({{i,gridCount.y-1},randint(0,3)});
    }
    for (int i=1;i!=gridCount.y-1;i++){
        TilesArray.push_back({{0,i},randint(0,3)});
        TilesArray.push_back({{gridCount.x-1,i},randint(0,3)});
    }

    for (int i=gridCount.y-2;i>gridCount.y/2;i--){
        TilesArray.push_back({{gridCount.x/3,i},randint(0,3)});
        TilesArray.push_back({{gridCount.x/3,i},randint(3,5)});
    }

    for (int i=gridCount.x/3-1;i!=0;i--){
        TilesArray.push_back({{i,gridCount.y/2+1},2});
        
    }

    TilesArray.push_back({{gridCount.x-5,gridCount.y-2},5});

    Player.pos = {gridCount.x - 7,gridCount.y-2};
    
    
}

void FetchEvents(){
    while (SDL_PollEvent(&event)){
        if (event.type==SDL_QUIT){shouldRun = false;}
        else if(event.type==SDL_KEYDOWN){
            switch (event.key.keysym.sym){
                case SDLK_ESCAPE:   shouldRun=false; break;
                case SDLK_UP:    if(!heldKeys[UP])    heldKeys[UP]=true; break;
                case SDLK_DOWN:  if(!heldKeys[DOWN])  heldKeys[DOWN]=true; break;
                case SDLK_RIGHT: if(!heldKeys[RIGHT]) heldKeys[RIGHT]=true; break;
                case SDLK_LEFT:    if(!heldKeys[LEFT])  heldKeys[LEFT]=true; break;
            }
        }
        else if(event.type==SDL_KEYUP){
            switch (event.key.keysym.sym){
                case SDLK_F2: ToggleFullscreen(mainWindow); Reshape(); break;
                case SDLK_F1: showGrids=!showGrids;

                case SDLK_UP:    if(heldKeys[UP])    heldKeys[UP]=0; break;
                case SDLK_DOWN:  if(heldKeys[DOWN])  heldKeys[DOWN]=0; break;
                case SDLK_RIGHT: if(heldKeys[RIGHT]) heldKeys[RIGHT]=0; break;
                case SDLK_LEFT:  if(heldKeys[LEFT])  heldKeys[LEFT]=0; break;
            }
        }
        else if(event.type==SDL_MOUSEMOTION){
            SDL_GetMouseState(&mousePos.x,&mousePos.y);
            mouseRect  = {mousePos.x-5,mousePos.y-5,10,10};
        }
    }

}

void CheckEvents(){
    for (int i=0;i!=4;i++){
        if (heldKeys[i]) Player.Move(i);
    }
    Player.Fall();   
}

void DrawAndUpdate(){
    SDL_SetRenderDrawColor(mainWindow.renderer,
                220,170,220,255);
    SDL_RenderClear(mainWindow.renderer);
    
    SDL_SetRenderDrawColor(mainWindow.renderer,255,0,0,255);
    SDL_RenderFillRect(mainWindow.renderer,&mouseRect);

    for (int i=0;i!=TilesArray.size();i++){
        TilesArray[i].Draw(mainWindow.renderer,gridXSize,
                        gridYSize);
    }

    Player.Draw(mainWindow.renderer,
                                gridXSize,
                                    gridYSize);
    
    if (showGrids){
        SDL_SetRenderDrawColor(mainWindow.renderer,30,70,160,255);
        

        for (float i=0;i!=gridCount.y;i++){
            
            SDL_RenderDrawLineF(mainWindow.renderer,
                                0.0,
                                i*gridYSize,
                                mainWindow.rect.w,
                                i*gridYSize);
        }

        for (float i=0;i!=gridCount.x;i++){
            
            SDL_RenderDrawLineF(mainWindow.renderer,
                             i*gridXSize,
                             0.0,
                             i*gridXSize,
                             mainWindow.rect.h);
        }
    }


    SDL_RenderPresent(mainWindow.renderer);
}

int main(){
    
    Init();

    while (shouldRun){
        FetchEvents();
        CheckEvents();
        DrawAndUpdate();
        SDL_Delay(50);
    }

    return 0;
}