#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GSWE.hpp"

int randint(int a,int b){
    return (std::rand() % (b-a+1)) + a;
}

MyWindow::Window mainWindow;
GSWE::GridSystem mainGrid;

void GenerateChunk(){//Simple Chunk
    GSWE::mapTilesArray.clear();
    int index = 0;
    
    int maxWidth =  randint(mainGrid.gridWidth/3,
                        mainGrid.gridWidth/1.5);
    int maxHeight = randint(mainGrid.gridHeight/3,
                        mainGrid.gridHeight/1.5);
    
    int x = randint(0,mainGrid.gridWidth-maxWidth);
    int y = randint(0,mainGrid.gridHeight-maxHeight);
    
    bool ifDo = true;

    for (int i=0;i!=maxWidth;i++)
    {
        for (int c=-1;c!=maxHeight;c++){
            if (c==-1){index=randint(4,7);}
            else if (c==0){index=randint(0,1);}
            else if (c>0){index=randint(2,3);}

            // std::cout<<index<<' '<<i<<' '<<c<<' '<<'\n';
            if (c==-1){if (randint(1,10)!=1) ifDo=false;}
            if (c>0){if (randint(1,10-c)==1) break;}
            
            if (ifDo){
                GSWE::mapTilesArray.push_back({GSWE::Pos({i+x,c+y}),index});
            }
            ifDo = true;
        }
    }
}
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



bool shouldRun = true;
SDL_Event event;

void Init(){
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    mainWindow.Init("I Love OOP",1000,750);
    mainGrid.Init(1000,750,30,30); 
    std::srand(time(0));


    SDL_Texture* tempTexture;
    int tempWidth;
    int tempHeight;
    GSWE::Tile tempTile;
    std::string paths[] = {
        "/home/yolo/Workstation/Graphies/Tiles/oimages/dirt0.png",
        "/home/yolo/Workstation/Graphies/Tiles/oimages/dirt1.png",
        "/home/yolo/Workstation/Graphies/Tiles/oimages/dirt2.png",
        "/home/yolo/Workstation/Graphies/Tiles/oimages/dirt3.png",
        "/home/yolo/Workstation/Graphies/Tiles/oimages/flower0.png",
        "/home/yolo/Workstation/Graphies/Tiles/oimages/flower1.png",
        "/home/yolo/Workstation/Graphies/Tiles/oimages/fire0.png",
        
    };

    for (int i=0;i!=7;i++){
        tempTexture = loadTexture(mainWindow.renderer,
                        paths[i].c_str(),
                        tempWidth,tempHeight);
        
        tempTile = {tempTexture,tempWidth,tempHeight};

        GSWE::tileArray.push_back(tempTile);

    }

    GenerateChunk();
    // for (int i=0;i!=mainGrid.gridWidth;i++)
    // {
    //     GSWE::mapTilesArray.push_back({{i,0},0});
    // }

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
                    case SDLK_RETURN: GenerateChunk(); break;
                    case SDLK_F1: mainGrid.showGrid=!mainGrid.showGrid; break;
                    
                    case SDLK_1: mainGrid.cameraZoom=1; break;
                    case SDLK_2: mainGrid.cameraZoom=0.5; break;
                    case SDLK_3: mainGrid.cameraZoom=0.25; break;
                    case SDLK_4: mainGrid.cameraZoom=2; break;
                    case SDLK_5: mainGrid.cameraZoom=3; break;
                    case SDLK_6: mainGrid.cameraZoom*=1.01; break;
                    case SDLK_7: mainGrid.cameraZoom*=0.99; break;
                    
                    case SDLK_RIGHT: mainGrid.cameraGridRel.x-=1; break;
                    case SDLK_LEFT:  mainGrid.cameraGridRel.x+=1; break;
                    case SDLK_DOWN: mainGrid.cameraGridRel.y-=1; break;
                    case SDLK_UP: mainGrid.cameraGridRel.y+=1; break;
                    
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