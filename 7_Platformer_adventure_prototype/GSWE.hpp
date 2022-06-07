#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "MyWindow.hpp"

#define myout(x) std::cout<<x;
#define space myout(' ')
#define enter myout('\n')
#define indent myout('\t')

namespace GSWE{

    float getPercent(float All,float Part){
        float mask;
        // std::cout<<All<<' '<<Part<<'\n';
        float A = 100.0/All;
        // std::cout<<A <<'\n';

        mask = float(Part*A);
        return mask;
    };


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

    struct StaticTiles{
        Pos pos;
        int imageIndex;
    };

    struct DynamicTiles{
        Pos pos;
        int imageIndex;
        float XRel=0;
        float YRel=0;

        void RelToPos(){// incomplete
            if (XRel<0){pos.x--;XRel=100+XRel;}
            if (XRel>=100){pos.x++;XRel=XRel-100;}
            if (YRel<0){pos.y--;YRel=100+YRel;}
            if (YRel>=100){pos.y++;YRel=YRel-100;}
        }
    };

    std::vector<StaticTiles> StaticTilesArray;
    std::vector<DynamicTiles> DynamicTilesArray;


    class GridSystem{
        private:

        public:

        int mouseGridX;
        int mouseGridY;

        int windowWidth;
        int windowHeight;
        int gridWidth;
        int gridHeight;

        float gridXGap;
        float gridYGap;
        float cameraZoom;
        float cameraXRel = 0; // Must be between 0-100
        float cameraYRel = 0; // Must be between 0-100
        Pos cameraGridRel;

        bool showGrid = true;
        bool showStaticTiles = true;
        bool showDynamicTiles = true;

        SDL_Color backgroundColor;
        SDL_Color gridColor;

        GridSystem();
        void Init(int,int,int,int);
        void Update(int,int);
        void Draw(SDL_Renderer*);
        void RelToGridRel();
        void SetZoomFocus(Pos,float,float);
        void GetMouseGridPos(int,int);
    };

    void GridSystem::GetMouseGridPos(int p_mouse_x,int p_mouse_y)
    {
        

        mouseGridX = int(
            (p_mouse_x-((cameraXRel/100)*(gridXGap*cameraZoom)))
            /(gridXGap*cameraZoom))
                        -cameraGridRel.x;
        mouseGridY = int(
            (p_mouse_y-((cameraYRel/100)*(gridYGap*cameraZoom)))
            /(gridYGap*cameraZoom))
                        -cameraGridRel.y;

        // out(mouseGridX) space out(mouseGridY) space

        // enter
    }
    void GridSystem::SetZoomFocus(Pos p_focus_pos,
                    float p_x_rel,
                    float p_y_rel){

        float tempZoom = cameraZoom;
        float totalTilesX,totalTilesY;
        if (tempZoom>1){
            totalTilesX = windowWidth/(gridXGap*cameraZoom);
            totalTilesY = windowHeight/(gridYGap*cameraZoom);
        }else{
            totalTilesX = (windowWidth/cameraZoom)/gridXGap;
            totalTilesY = (windowHeight/cameraZoom)/gridYGap;
        }

            int intPointX = int(totalTilesX);
            int intPointY = int(totalTilesY);

            float floatPointX = totalTilesX - intPointX;
            float floatPointY = totalTilesY - intPointY;

            float tempXRel=floatPointX*100 + p_x_rel;
            float tempYRel=floatPointY*100 + p_y_rel;

            cameraGridRel.x = -p_focus_pos.x+intPointX/2;
            cameraGridRel.y = -p_focus_pos.y+intPointY/2;

            cameraXRel = -p_x_rel;
            cameraYRel = -p_y_rel;
    }

    void GridSystem::RelToGridRel(){

        if (cameraXRel<0){
            cameraXRel=100+cameraXRel;
            cameraGridRel.x-=1;
        }
        if (cameraYRel<0){
            cameraYRel=100+cameraYRel;
            cameraGridRel.y-=1;
        }
        if (cameraXRel>=100){
            cameraXRel=cameraXRel-100;
            cameraGridRel.x+=1;
        }
        if (cameraYRel>=100){
            cameraYRel=cameraYRel-100;
            cameraGridRel.y+=1;
        }
    }

    GridSystem::GridSystem(){
        backgroundColor = {150,150,200,255};
        gridColor = {80,30,30,255};
    }

    void GridSystem::Init(
                            int p_window_width,
                            int p_window_height,
                            int p_grid_width,
                            int p_grid_height)
    {
        windowWidth = p_window_width;
        windowHeight = p_window_height;
        gridWidth = p_grid_width;
        gridHeight = p_grid_height;

        gridXGap = float(windowWidth)/gridWidth;
        gridYGap = float(windowHeight)/gridHeight;
        cameraGridRel = {0,0};
        cameraZoom = 1.0;
    }

    void GridSystem::Update(
                            int p_window_width,
                            int p_window_height

                            )
    {
        windowWidth = p_window_width;
        windowHeight = p_window_height;

        gridXGap = float(windowWidth)/gridWidth;
        gridYGap = float(windowHeight)/gridHeight;
    }

    void GridSystem::Draw(SDL_Renderer* p_renderer)
    {
        SDL_SetRenderDrawColor(
            p_renderer,
            backgroundColor.r,
            backgroundColor.g,
            backgroundColor.b,
            backgroundColor.a);

        SDL_RenderClear(p_renderer);
        if (showStaticTiles){
            SDL_FRect maskRect;
            for (int i=0;i!=StaticTilesArray.size();i++)
            {
                maskRect = {
                    (float(StaticTilesArray[i].pos.x+cameraGridRel.x)*
                        (gridXGap*cameraZoom))+gridXGap*cameraZoom*cameraXRel/100,
                    (float(StaticTilesArray[i].pos.y+cameraGridRel.y)*
                        (gridYGap*cameraZoom))+gridYGap*cameraZoom*cameraYRel/100,
                    gridXGap*cameraZoom,
                    gridYGap*cameraZoom
                };
                SDL_RenderCopyF(p_renderer,
                    imageArray[StaticTilesArray[i].imageIndex].texture,
                    NULL,
                    &maskRect);
            }
        }
        if (showDynamicTiles){
            SDL_FRect maskRect;
            SDL_FRect maskRect0;
            SDL_SetRenderDrawColor(p_renderer,230,100,100,255);

            // maskRect0={
            //         (float(mouseGridX+cameraGridRel.x)*
            //             (gridXGap*cameraZoom))+gridXGap*cameraZoom*cameraXRel/100
            //             ,
            //         (float(mouseGridY+cameraGridRel.y)*
            //             (gridYGap*cameraZoom))+gridYGap*cameraZoom*cameraYRel/100,
            //         gridXGap*cameraZoom,
            //         gridYGap*cameraZoom
            // };
            // SDL_RenderFillRectF(p_renderer,
            //         &maskRect0);

            for (int i=0;i!=DynamicTilesArray.size();i++)
            {
                if (showGrid)
                {maskRect0={
                    (float(DynamicTilesArray[i].pos.x+cameraGridRel.x)*
                        (gridXGap*cameraZoom))+gridXGap*cameraZoom*cameraXRel/100
                        ,
                    (float(DynamicTilesArray[i].pos.y+cameraGridRel.y)*
                        (gridYGap*cameraZoom))+gridYGap*cameraZoom*cameraYRel/100,
                    gridXGap*cameraZoom,
                    gridYGap*cameraZoom
                };
                SDL_RenderFillRectF(p_renderer,
                    &maskRect0);
                }
                maskRect = {
                    (float(DynamicTilesArray[i].pos.x+cameraGridRel.x)*
                        (gridXGap*cameraZoom))+gridXGap*cameraZoom*cameraXRel/100+
                        gridXGap*DynamicTilesArray[i].XRel/100*cameraZoom,
                    (float(DynamicTilesArray[i].pos.y+cameraGridRel.y)*
                        (gridYGap*cameraZoom))+gridYGap*cameraZoom*cameraYRel/100+
                        gridYGap*DynamicTilesArray[i].YRel/100*cameraZoom,
                    gridXGap*cameraZoom,
                    gridYGap*cameraZoom
                };


                SDL_RenderCopyF(p_renderer,
                    imageArray[DynamicTilesArray[i].imageIndex].texture,
                    NULL,
                    &maskRect);

            }
        }
        if (showGrid)
        {
            SDL_SetRenderDrawColor(
                p_renderer,
                gridColor.r,
                gridColor.g,
                gridColor.b,
                gridColor.a
            );
            int tempZoom = (1/tempZoom);

            if (tempZoom<1) tempZoom=1;
            for (int x=0;
                 x*gridXGap*cameraZoom < windowWidth*tempZoom;
                 x++)
            {
                SDL_RenderDrawLineF(p_renderer,
                    x*gridXGap*cameraZoom+gridXGap*cameraZoom*cameraXRel/100,
                    0,
                    x*gridXGap*cameraZoom+gridXGap*cameraZoom*cameraXRel/100,
                    windowHeight*tempZoom);
            }

            for (int y=0;
                 y*gridYGap*cameraZoom < windowHeight*tempZoom;
                 y++)
            {
                SDL_RenderDrawLineF(p_renderer,
                    0,
                    y*gridYGap*cameraZoom+gridYGap*cameraZoom*cameraYRel/100,
                    windowWidth*tempZoom,
                    y*gridYGap*cameraZoom+gridYGap*cameraZoom*cameraYRel/100);
            }


        }
    }




}