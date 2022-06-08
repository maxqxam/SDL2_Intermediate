#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GSWE.hpp"
#include "MyWindow.hpp"
#include "LevelEditor.hpp"

std::string saveToPath = "./data/level.data";
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
bool mouseMove = false;
std::string imagesAddress = 
"/home/yolo/Workstation/Graphies/Tiles/Adventure-Game-Jam/";

std::string imagesPath[] = {
    imagesAddress+"/Tiles1/stone-tbrl.png",
    imagesAddress+"/Tiles1/stone-tbr.png",
    imagesAddress+"/Tiles1/stone-tbl.png",
    imagesAddress+"/Tiles1/stone-tb.png",
    
    imagesAddress+"/Tiles1/stone-trl.png",
    imagesAddress+"/Tiles1/stone-tr.png",
    imagesAddress+"/Tiles1/stone-tl.png",
    imagesAddress+"/Tiles1/stone-t.png",
    
    imagesAddress+"/Tiles1/stone-brl.png",
    imagesAddress+"/Tiles1/stone-br.png",
    imagesAddress+"/Tiles1/stone-bl.png",
    imagesAddress+"/Tiles1/stone-b.png",
    
    imagesAddress+"/Tiles1/stone-rl.png",
    imagesAddress+"/Tiles1/stone-r.png",
    imagesAddress+"/Tiles1/stone-l.png",
    imagesAddress+"/Tiles1/stone.png",

    // 16

    imagesAddress+"/Tiles1/water0.png",
    imagesAddress+"/Tiles1/flower0.png",
    imagesAddress+"/Tiles1/flower1.png",
    imagesAddress+"/Tiles1/flower2.png",

    imagesAddress+"/Tiles1/flower3.png",
    imagesAddress+"/Tiles1/sign0.png",
    imagesAddress+"/Tiles1/sign1.png",
    imagesAddress+"/Tiles1/sign2.png",

    imagesAddress+"/Tiles1/sign3.png",
    imagesAddress+"/Tiles1/door0.png",
    imagesAddress+"/Tiles1/gate0.png",
    imagesAddress+"/Tiles1/lever0.png",

    imagesAddress+"/Tiles1/ladder0.png",
    imagesAddress+"/Tiles1/person0.png",
    imagesAddress+"/Tiles1/person1.png",
    imagesAddress+"/Tiles1/person2.png",

    // 32

    imagesAddress+"/Tiles1/person3.png", 
    imagesAddress+"/Tiles1/fish0.png",
    imagesAddress+"/Tiles1/bee0.png",
    imagesAddress+"/Characters/hero.png",

    imagesAddress+"/Tiles1/fire0.png",
    imagesAddress+"/Tiles1/chest0.png",
    // imagesAddress+"/Tiles1/trapdoor0.png"
    
    //37

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
    levelEditor.Init(800,600,7,7);
    SDL_SetRenderDrawBlendMode(editorWindow.renderer,
    SDL_BLENDMODE_BLEND);
    
    SDL_Texture* tempTexture;
    int tempWidth;
    int tempHeight;
    LE::Image tempEditImage;
    GSWE::Image tempMainImage;
    for (int i=0;i!=sizeof(imagesPath)/sizeof(imagesPath[i]);i++){
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

    levelEditor.Load(saveToPath);
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
                case SDLK_1:
                SDL_RaiseWindow(mainWindow.window);
                SDL_SetWindowInputFocus(mainWindow.window);
                break;
                case SDLK_2:
                SDL_RaiseWindow(editorWindow.window);
                SDL_SetWindowInputFocus(editorWindow.window);
                break;
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
           
                SDL_SetWindowInputFocus(mainWindow.window);
            }else if(editorWindow.mouseFocus){
                
                SDL_SetWindowInputFocus(editorWindow.window);
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
                
                case SDL_BUTTON_MIDDLE:
                    mouseMove=true;;
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
                case SDL_BUTTON_MIDDLE:
                    mouseMove=false;
                    heldKeys[UP]=0;
                    heldKeys[DOWN]=0;
                    heldKeys[RIGHT]=0;
                    heldKeys[LEFT]=0;  
                    break;
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
    if (mouseMove){
    if ((mouseX<mainWindow.width/15)){
                        heldKeys[LEFT]=true;
    }else{
                        heldKeys[LEFT]=false;
    }

    if ((mouseY<mainWindow.height/15)){
                        heldKeys[UP]=true;
    }else{
                        heldKeys[UP]=false;
    }

    if (mouseX>mainWindow.width-mainWindow.width/15-10
                                    ){
                        heldKeys[RIGHT]=true;
    }else{
                        heldKeys[RIGHT]=false;
    }

    if (mouseY>mainWindow.height-mainWindow.height/15-10
                                    ){
                        heldKeys[DOWN]=true;
    }else{
                        heldKeys[DOWN]=false;
                    }
    }                
    
    // if ((!mainWindow.mouseFocus)||!mouseMove){
    //             heldKeys[UP]=0;
    //             heldKeys[DOWN]=0;
    //             heldKeys[RIGHT]=0;
    //             heldKeys[LEFT]=0;    
    // }


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
        mainGrid.cameraZoom*=1.05;
    }
    if (heldKeys[RCTRL]){
        mainGrid.cameraZoom*=0.95;
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
            levelEditor.Save(saveToPath);
        }
    }

    return 0;
}