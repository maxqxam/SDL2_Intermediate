#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GSWE.hpp"
#include "MyWindow.hpp"
#include "LevelEditor.hpp"

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

SDL_Event event;
bool shouldRun = true;
bool shouldCreate = false;
bool shouldDestroy = false;
bool shouldBypass  = false;
MyWindow::Window editorWindow;
MyWindow::Window mainWindow;
LE::LevelEditor  levelEditor;
GSWE::GridSystem mainGrid;

std::string imagesAddress = 
"/home/yolo/Workstation/Graphies/Tiles/Adventure-Game-Jam/";

std::string imagesPath[] = {
    imagesAddress+"Tiles/floor0.png",
    imagesAddress+"Tiles/floor1.png",
    imagesAddress+"Tiles/floor2.png",
    imagesAddress+"Tiles/floor3.png",

    imagesAddress+"Tiles/floor4.png",
    imagesAddress+"Tiles/floor5.png",
    imagesAddress+"Tiles/floor6.png",
    imagesAddress+"Tiles/floor7.png",

    imagesAddress+"Tiles/floor8.png",
    imagesAddress+"Tiles/floor9.png",
    imagesAddress+"Tiles/floor10.png",
    imagesAddress+"Tiles/floor11.png",

    imagesAddress+"Tiles/wall0.png",
    imagesAddress+"Tiles/wall1.png",
    imagesAddress+"Tiles/wall2.png",
    imagesAddress+"Tiles/wall3.png",

    //-----------------------------


    imagesAddress+"Tiles/wall4.png",
    imagesAddress+"Tiles/wall5.png",
    imagesAddress+"Tiles/wall6.png",
    imagesAddress+"Tiles/wall7.png",

    imagesAddress+"Tiles/grass0.png",
    imagesAddress+"Tiles/grass1.png",
    imagesAddress+"Tiles/grass2.png",
    imagesAddress+"Tiles/grass3.png",

    imagesAddress+"Tiles/grass4.png",
    imagesAddress+"Tiles/grass5.png",
    imagesAddress+"Tiles/grass6.png",
    imagesAddress+"Tiles/grass7.png",

    imagesAddress+"Tiles/grass8.png",
    imagesAddress+"Tiles/grass9.png",
    imagesAddress+"Tiles/grass10.png",
    imagesAddress+"Tiles/grass11.png",

    //-------------------------------

    imagesAddress+"Tiles/dirt0.png",
    imagesAddress+"Tiles/dirt1.png",
    imagesAddress+"Tiles/dirt2.png",
    imagesAddress+"Tiles/dirt3.png",

    

    
};

enum{
    UP,DOWN,RIGHT,LEFT,
    RSHIFT,RCTRL
};

bool heldKeys[6];
int mouseX=0;
int mouseY=0;
void Init(){
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);


    mainWindow.Init("MainWindow",1000,750);
    mainGrid.Init(1000,750,25,20);

    editorWindow.Init("LevelEditor",800,600);
    levelEditor.Init(800,600,8,8);
    SDL_SetRenderDrawBlendMode(editorWindow.renderer,
    SDL_BLENDMODE_BLEND);

    SDL_Texture* tempTexture;
    int tempWidth;
    int tempHeight;
    LE::Image tempEditImage;
    GSWE::Image tempMainImage;
    for (int i=0;i!=36;i++){
        tempTexture = loadTexture(editorWindow.renderer,
                        imagesPath[i].c_str(),
                        tempWidth,tempHeight);
        
        tempEditImage = {tempTexture,tempWidth,tempHeight};

        LE::imageArray.push_back(tempEditImage);

        tempTexture = loadTexture(mainWindow.renderer,
                        imagesPath[i].c_str(),
                        tempWidth,tempHeight);
        
        tempMainImage = {tempTexture,tempWidth,tempHeight};

        GSWE::imageArray.push_back(tempMainImage);

    }

    levelEditor.Load("level.data");
}

void FetchEvents(){
    while (SDL_PollEvent(&event)!=0)
    {
        if (event.type==SDL_WINDOWEVENT)
        {
            editorWindow.FetchWindowEvent(event);
            mainWindow.FetchWindowEvent(event);
        }
        else if(event.type==SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_LSHIFT: if(!shouldBypass
                                )shouldBypass=true;break;                               
                case SDLK_UP: if(!heldKeys[UP]) heldKeys[UP]=true;break;
                case SDLK_DOWN: if(!heldKeys[DOWN]) heldKeys[DOWN]=true;break;
                case SDLK_RIGHT: if(!heldKeys[RIGHT]) heldKeys[RIGHT]=true;break;
                case SDLK_LEFT: if(!heldKeys[LEFT]) heldKeys[LEFT]=true;break;
                case SDLK_RSHIFT: if(!heldKeys[RSHIFT]) heldKeys[RSHIFT]=true;break;
                case SDLK_RCTRL: if(!heldKeys[RCTRL]) heldKeys[RCTRL]=true;break;
            }
        }
        else if(event.type==SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_LSHIFT: if(shouldBypass)shouldBypass=false;break;
                case SDLK_F1: mainGrid.showGrid=!mainGrid.showGrid;break;
                case SDLK_ESCAPE: shouldRun=false; break;
                case SDLK_UP: if(heldKeys[UP]) heldKeys[UP]=0;break;
                case SDLK_DOWN: if(heldKeys[DOWN]) heldKeys[DOWN]=0;break;
                case SDLK_RIGHT: if(heldKeys[RIGHT]) heldKeys[RIGHT]=0;break;
                case SDLK_LEFT: if(heldKeys[LEFT]) heldKeys[LEFT]=0;break;
                case SDLK_RSHIFT: if(heldKeys[RSHIFT]) heldKeys[RSHIFT]=0;break;
                case SDLK_RCTRL: if(heldKeys[RCTRL]) heldKeys[RCTRL]=0;break;
            }
        }
        else if(event.type==SDL_MOUSEMOTION)
        {
            if (mainWindow.mouseFocus){
                SDL_GetMouseState(&mouseX,&mouseY);
                mainGrid.GetMouseGridPos(mouseX,mouseY);
            }
        }
        else if(event.type==SDL_MOUSEBUTTONDOWN)
        {
         switch (event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    SDL_GetMouseState(&mouseX,&mouseY);
                    if (mainWindow.mouseFocus){
                        shouldCreate = true;
                    }else{
                        shouldCreate = false;
                    }
                    break;
                
                case SDL_BUTTON_RIGHT:
                    SDL_GetMouseState(&mouseX,&mouseY);
                    if (mainWindow.mouseFocus){
                        shouldDestroy = true;
                    }else{
                        shouldDestroy = false;
                    }
                    break;

            }   
        }
        else if(event.type==SDL_MOUSEBUTTONUP)
        {
            switch (event.button.button)
            {
                case SDL_BUTTON_LEFT:
                SDL_GetMouseState(&mouseX,&mouseY);
                if (editorWindow.mouseFocus){
                    levelEditor.CheckClick(mouseX,mouseY);
                }
                else if (mainWindow.mouseFocus){
                    shouldCreate = false;
                }else{
                    shouldCreate = false;
                }
                break;
                
                case SDL_BUTTON_RIGHT:
                if (mainWindow.mouseFocus){
                    shouldDestroy=false;
                }
                break;
            }
        }
    }
}

void CheckEvents(){
    if (editorWindow.isClosed || mainWindow.isClosed){shouldRun=false;}
    if (editorWindow.sizeChanged){
        levelEditor.Update(editorWindow.width,editorWindow.height);
    }
    if (mainWindow.sizeChanged){
        mainGrid.Update(mainWindow.width,mainWindow.height);
    }

    if (heldKeys[UP]){
        mainGrid.cameraYRel+=20;
        mainGrid.RelToGridRel();
    }
    if (heldKeys[DOWN]){
        mainGrid.cameraYRel-=20;
        mainGrid.RelToGridRel();
    }
    if (heldKeys[RIGHT]){
        mainGrid.cameraXRel-=20;
        mainGrid.RelToGridRel();
    }
    if (heldKeys[LEFT]){
        mainGrid.cameraXRel+=20;
        mainGrid.RelToGridRel();
    }
    if (heldKeys[RSHIFT]){
        mainGrid.cameraZoom*=1.01;
    }
    if (heldKeys[RCTRL]){
        mainGrid.cameraZoom*=0.99;
    }

    if (shouldCreate){
        bool shouldInsert = true;

        for (int i=0;i!=GSWE::StaticTilesArray.size();i++)
        {
            if ((GSWE::StaticTilesArray[i].pos.x==mainGrid.mouseGridX) &&
                (GSWE::StaticTilesArray[i].pos.y==mainGrid.mouseGridY))
            {
                
                shouldInsert=false;
            }
        }

        if (shouldInsert || shouldBypass)
        {           
            if (!shouldInsert&&shouldBypass){
                myout("Bypassed duplicate insertion | ")
                shouldBypass=false;
            }
            myout("Image number  ") 
            myout(levelEditor.selected)
            myout(" Inserted at : ")
            myout(mainGrid.mouseGridX) space
            myout(mainGrid.mouseGridY) space 
            myout(" | Total Tiles: ")
            myout(GSWE::StaticTilesArray.size())
            enter 

            GSWE::StaticTilesArray.push_back({
                {mainGrid.mouseGridX,mainGrid.mouseGridY},
                levelEditor.selected
            });            
        }
        
    }else if(shouldDestroy){
        for (int i=0;i!=GSWE::StaticTilesArray.size();i++)
        {
            if ((GSWE::StaticTilesArray[i].pos.x==mainGrid.mouseGridX) &&
                (GSWE::StaticTilesArray[i].pos.y==mainGrid.mouseGridY))
            {

                myout("Image number ")
                myout(GSWE::StaticTilesArray[i].imageIndex)
                myout(" Removed from : ")
                myout(GSWE::StaticTilesArray[i].pos.x) space 
                myout(GSWE::StaticTilesArray[i].pos.y) space
                myout(" | Total Tiles: ")
                myout(GSWE::StaticTilesArray.size()) 
                enter

                GSWE::StaticTilesArray.erase(GSWE::StaticTilesArray.begin()
                                + i);
                
                break;
            }
        }

    }


}

void DrawAndUpdate(){
    levelEditor.Draw(editorWindow.renderer);
    mainGrid.Draw(mainWindow.renderer);

    SDL_RenderPresent(mainWindow.renderer);
    SDL_RenderPresent(editorWindow.renderer);
}

int main(){
    Init();

    while (shouldRun){
        FetchEvents();
        CheckEvents();
        DrawAndUpdate();
        SDL_Delay(50);
        if (!shouldRun){
            myout("exiting...\n");
            levelEditor.Save("level.data");
        }
    }

    return 0;
}