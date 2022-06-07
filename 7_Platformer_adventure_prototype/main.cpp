#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "fstream"

#include "MyWindow.hpp"
#include "GSWE.hpp"


SDL_Event event;
bool shouldRun = true;

MyWindow::Window mainWindow;
GSWE::GridSystem mainGrid;

std::string imagesAddress =
"/home/yolo/Workstation/Graphies/Tiles/Adventure-Game-Jam/";

enum {
    UP,DOWN,
    RIGHT,LEFT,
    RETURN,
    NUMBER1,NUMBER2,
    NUMBER3
};

bool heldKeys[8];

std::string imagesPath[] = {
    imagesAddress+"/Tiles1/stone-mass0.png",

    imagesAddress+"/Tiles1/stone-floor0.png",
    imagesAddress+"/Tiles1/stone-ceiling0.png",
    imagesAddress+"/Tiles1/stone-wall-left0.png",
    imagesAddress+"/Tiles1/stone-wall-right0.png",


    imagesAddress+"/Tiles1/stone-edge-top-left0.png",
    imagesAddress+"/Tiles1/stone-edge-top-right0.png",
    imagesAddress+"/Tiles1/stone-edge-bottom-left0.png",
    imagesAddress+"/Tiles1/stone-edge-bottom-right0.png",

    imagesAddress+"/Tiles1/bee0.png", // 9
    imagesAddress+"/Tiles1/water0.png", // 10
    imagesAddress+"/Tiles1/fire0.png",
    imagesAddress+"/Tiles1/ladder0.png", //12

    imagesAddress+"/Tiles1/flower0.png",
    imagesAddress+"/Tiles1/flower1.png",
    imagesAddress+"/Tiles1/flower2.png",
    imagesAddress+"/Tiles1/flower3.png",

    imagesAddress+"/Tiles1/person0.png",
    imagesAddress+"/Tiles1/person1.png",
    imagesAddress+"/Tiles1/person2.png",
    imagesAddress+"/Tiles1/person3.png",

    imagesAddress+"/Tiles1/door0.png",
    imagesAddress+"/Tiles1/door1.png",
    imagesAddress+"/Tiles1/door2.png",
    imagesAddress+"/Tiles1/door3.png",

    imagesAddress+"/Tiles1/chest0.png",
    imagesAddress+"/Tiles1/chest1.png",
    imagesAddress+"/Tiles1/chest2.png",
    imagesAddress+"/Tiles1/chest3.png",

    imagesAddress+"/Tiles1/sign0.png",//29
    imagesAddress+"/Tiles1/sign1.png",
    imagesAddress+"/Tiles1/sign2.png",
    imagesAddress+"/Tiles1/sign3.png",

    imagesAddress+"/Tiles1/lever0.png",//33
    imagesAddress+"/Tiles1/lever1.png",
    imagesAddress+"/Tiles1/lever2.png",
    imagesAddress+"/Tiles1/lever3.png",

    imagesAddress+"/Tiles1/gate0.png",//37
    imagesAddress+"/Tiles1/gate1.png",
    imagesAddress+"/Tiles1/gate2.png",
    imagesAddress+"/Tiles1/gate3.png",

    imagesAddress+"/Tiles1/fish0.png", //41
    imagesAddress+"/Characters/hero.png" //42


};

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

int heroIndex=0;

void LoadLevel(std::string p_path){
        std::ifstream inputFile(p_path);

        int posx  = 0;
        int posy  = 0;
        int index = 0;

        int counter = 0;
        std::string text="";
        std::string tempString = "";
        int totalLines = 0;
        while (std::getline(inputFile,text))
        {
            totalLines ++;
            for (int i=0;i!=text.length();i++){
                if (text[i]=='_'){
                    // myout("tempString: ") myout(tempString) enter
                    if (counter==0){
                        index = std::stoi(tempString);
                    }else if(counter==1){
                        posx = std::stoi(tempString);
                    }
                    counter++;
                    tempString="";
                }else{
                    tempString+=text[i];
                }
            }
            posy=std::stoi(tempString);
            // myout(text) indent
            // myout(index) space
            // myout(posx) space
            // myout(posy) enter

            GSWE::StaticTilesArray.push_back({{posx,posy},index});
            if (index==42){heroIndex=GSWE::StaticTilesArray.size()-1;}
            counter = 0;
            tempString = "";
        }

        myout(totalLines) myout(" Loaded from ")
        myout(p_path) enter
}


void Init(){
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    mainWindow.Init("MainWindow",1000,750);
    mainGrid.Init(1000,750,10,7);

    SDL_Texture* tempTexture;
    int tempWidth;
    int tempHeight;

    for (int i=0;i!=sizeof(imagesPath)/sizeof(imagesPath[i]);i++){
        tempTexture = loadTexture(mainWindow.renderer,
                        imagesPath[i].c_str(),
                        tempWidth,tempHeight);

        GSWE::imageArray.push_back({tempTexture,tempWidth,tempHeight});

    }

    LoadLevel("data/tutorial.map");
    mainGrid.SetZoomFocus({8,13},0,0);
}

void FetchEvents(){
    while (SDL_PollEvent(&event))
    {
        if (event.type==SDL_WINDOWEVENT)
        {
            mainWindow.FetchWindowEvent(event);
        }
        else if (event.type==SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE: shouldRun=false; break;
                case SDLK_UP: if(!heldKeys[UP]) heldKeys[UP]=true;break;
                case SDLK_DOWN: if(!heldKeys[DOWN]) heldKeys[DOWN]=true;break;
                case SDLK_RIGHT: if(!heldKeys[RIGHT]) heldKeys[RIGHT]=true;break;
                case SDLK_LEFT: if(!heldKeys[LEFT]) heldKeys[LEFT]=true;break;
                case SDLK_1: if(
                    !heldKeys[NUMBER1]) heldKeys[NUMBER1]=true;break;
                case SDLK_2: if(
                    !heldKeys[NUMBER2]) heldKeys[NUMBER2]=true;break;
                case SDLK_3: if(
                    !heldKeys[NUMBER3]) heldKeys[NUMBER3]=true;break;

            }
        }
        else if(event.type==SDL_KEYUP){
            switch (event.key.keysym.sym)
            {
                case SDLK_F1: mainGrid.showGrid=!mainGrid.showGrid;break;
                case SDLK_UP: if(heldKeys[UP]) heldKeys[UP]=0;break;
                case SDLK_DOWN: if(heldKeys[DOWN]) heldKeys[DOWN]=0;break;
                case SDLK_RIGHT: if(heldKeys[RIGHT]) heldKeys[RIGHT]=0;break;
                case SDLK_LEFT: if(heldKeys[LEFT]) heldKeys[LEFT]=0;break;
                case SDLK_1: if(heldKeys[NUMBER1]
                            ) heldKeys[NUMBER1]=0;break;
                case SDLK_2: if(heldKeys[NUMBER2]
                            ) heldKeys[NUMBER2]=0;break;
                case SDLK_3: if(heldKeys[NUMBER3]
                            ) heldKeys[NUMBER3]=0;break;

            }
        }

    }

}
enum {
    LADDER,
    WATER
};
bool ValidMove(GSWE::Pos p_pos,int xto,int yto){
    bool cond[] = {
        0,
        0,
        0,
        0,

        0,
        0,
        0
    };

    if (yto==-1)
    {cond[0]=1;}
    if (GSWE::StaticTilesArray[heroIndex].imageIndex==9){
        cond[4]=1;
    }
    if (GSWE::StaticTilesArray[heroIndex].imageIndex==41){
        cond[5]=1;
    }

    for (int i=0;i!=GSWE::StaticTilesArray.size();i++)
    {
        if (((p_pos.x+xto)==GSWE::StaticTilesArray[i].pos.x)&&
            ((p_pos.y+yto)==GSWE::StaticTilesArray[i].pos.y))
        {
            switch (GSWE::StaticTilesArray[i].imageIndex){
                case 12: cond[1]=1; break;
                case 10: cond[2]=1; break;
                case 29: cond[3]=0; break;
                case 33: cond[3]=0; break;
                default: cond[3]=1; break;
            }
        }
    }

    if (cond[3]){return false;}
    if (cond[0]&&!cond[1]&&!cond[4]&&!cond[2]){return false;}
    if (cond[5]&&!cond[2]){return false;}
    if ((cond[4])&&(cond[1]||cond[2])){return false;}
    if ((!cond[4]&&!cond[5])&&cond[2]){return false;}
    
    std::cout<<p_pos.x<<' '<<p_pos.y<<'\n';
    return true;
}

void PlayerAction(int p_dirc){
    // std::cout<<GSWE::StaticTilesArray.size()<<"\n"<<
    // p_dirc<<"\n"; 
    
    if (p_dirc==UP){
        if (ValidMove(GSWE::StaticTilesArray[heroIndex].pos
                        ,0,-1)){
            GSWE::StaticTilesArray[heroIndex].pos.y--;
            mainGrid.cameraGridRel.y++;
        }
    }
  
    if (p_dirc==DOWN){
        if (ValidMove(GSWE::StaticTilesArray[heroIndex].pos
                        ,0,1)){
            GSWE::StaticTilesArray[heroIndex].pos.y++;
            mainGrid.cameraGridRel.y--;
        }
    }
  
    if (p_dirc==RIGHT){
        if (ValidMove(GSWE::StaticTilesArray[heroIndex].pos
                    ,1,0)){
            GSWE::StaticTilesArray[heroIndex].pos.x++;
            mainGrid.cameraGridRel.x--;
        }
    }

    if (p_dirc==LEFT){
        if (ValidMove(GSWE::StaticTilesArray[heroIndex].pos
                    ,-1,0)){
            GSWE::StaticTilesArray[heroIndex].pos.x--;
            mainGrid.cameraGridRel.x++;
        }
    }
     
    if (p_dirc==NUMBER1){
        GSWE::StaticTilesArray[heroIndex].imageIndex=42;
    }

    if (p_dirc==NUMBER2){
        GSWE::StaticTilesArray[heroIndex].imageIndex=9;
    }

    if (p_dirc==NUMBER3){
        GSWE::StaticTilesArray[heroIndex].imageIndex=41;
    }
}


void CheckEvents(){
    if (mainWindow.isClosed){shouldRun=false;}
    if (mainWindow.sizeChanged){mainGrid.Update(
        mainWindow.width,
        mainWindow.height
    );}

    for (int i=0;i!=8;i++){
        if (heldKeys[i]){
            PlayerAction(i);
        }
    }


}

void DrawAndUpdate(){
    mainGrid.Draw(mainWindow.renderer);


    SDL_RenderPresent(mainWindow.renderer);
}


int main(){
    Init();

    while(shouldRun){
        FetchEvents();
        CheckEvents();
        DrawAndUpdate();
        SDL_Delay(50);
    }



    return 0;
}