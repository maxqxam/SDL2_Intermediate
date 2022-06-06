#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <fstream>
#include "GSWE.hpp"

#pragma once

namespace LE{

    struct Pos{
        int x;
        int y;
    };

    struct Image{
        SDL_Texture* texture;
        int width;
        int height;
    };

    std::vector<Image> imageArray;
    
    struct LevelEditor{

        int maxXGrid=6;
        int maxYGrid=6;
        int windowWidth;
        int windowHeight;
        int selected=0;
        SDL_Color backgroundColor = {180,200,220,255};
        SDL_Color selectedColor   = {0,0,0,125};
        SDL_Color Black           = {0,0,0,255};
        LevelEditor();
        void Init(int,int,int,int);
        void Update(int,int);
        void Draw(SDL_Renderer*);
        void CheckClick(int,int);
        void Save(std::string);
        void Load(std::string);

    };

    LevelEditor::LevelEditor(){};
    
    void LevelEditor::CheckClick(int p_mouse_x,int p_mouse_y)
    {
        
        float tempWidth = float(windowWidth)/maxXGrid;
        float tempHeight = float(windowHeight)/maxYGrid;

        SDL_FRect maskRect = {0,0,tempWidth,tempHeight};
        
        for (int i=0;i!=imageArray.size();i++)
        {
            if ((p_mouse_x>maskRect.x) &&
                (p_mouse_x<maskRect.x+maskRect.w) &&
                (p_mouse_y>maskRect.y) &&
                (p_mouse_y<maskRect.y+maskRect.h))
            {
                selected=i; break;
            }

            maskRect.x+=maskRect.w;
            if ((i+1)%maxXGrid==0){
                maskRect.y+=maskRect.h;
                maskRect.x=0;
            }
            
        }   
    }

    void LevelEditor::Init(int p_window_width,int p_window_height
                    ,int p_max_x_grid,int p_max_y_grid)
    {
        windowWidth = p_window_width;
        windowHeight = p_window_height;
        maxXGrid = p_max_x_grid;
        maxYGrid = p_max_y_grid;
    }

    void LevelEditor::Update(int p_window_width,int p_window_height)
    {
        windowWidth = p_window_width;
        windowHeight = p_window_height;

    }

    void LevelEditor::Draw(SDL_Renderer* p_renderer)
    {
        SDL_SetRenderDrawColor(p_renderer,
            backgroundColor.r,
            backgroundColor.g,
            backgroundColor.b,
            backgroundColor.a
        );
      
        SDL_RenderClear(p_renderer);

        SDL_SetRenderDrawColor(p_renderer,
            Black.r,
            Black.g,
            Black.b,
            Black.a
        );
        
        float tempWidth = float(windowWidth)/maxXGrid;
        float tempHeight = float(windowHeight)/maxYGrid;

        SDL_FRect maskRect = {0,0,tempWidth,tempHeight};
        
        for (int i=0;i!=imageArray.size();i++)
        {

            SDL_RenderCopyF(p_renderer,imageArray[i].texture,
            NULL,
            &maskRect);

            if (i==selected){
                SDL_RenderDrawRectF(p_renderer,
                                &maskRect);
            }
            maskRect.x+=maskRect.w;
            if ((i+1)%maxXGrid==0){
                maskRect.y+=maskRect.h;
                maskRect.x=0;
            }
            
        }
    }
    void LevelEditor::Load(std::string p_path){
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
                if (text[i]=='-'){
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
            counter = 0;
            tempString = "";
        }

        myout(totalLines) myout(" Loaded from ")
        myout(p_path) enter
    }


    void LevelEditor::Save(std::string p_path)
    {
        std::ofstream outputFile(p_path);

        std::string maskString = "";
        for (int i=0;i!=GSWE::StaticTilesArray.size();i++){
            maskString = std::to_string(GSWE::StaticTilesArray[i].imageIndex) +
                        "-"+
                       std::to_string(GSWE::StaticTilesArray[i].pos.x) +
                       "-"+
                       std::to_string(
                           GSWE::StaticTilesArray[i].pos.y);

            outputFile << (maskString+"\n");
            maskString = "";
                       
        }

        outputFile.close();
             
    }
};