#include <SDL2/SDL.h>
#include <vector>
#include "MyWindow.hpp"

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

    struct Tile{
        SDL_Texture* texture;
        int width;
        int height;
    };

    std::vector<Tile> tileArray;

    struct MapTiles{
        Pos pos;
        // float tensionX;
        // float tensionY;
        int tileIndex;
    };

    std::vector<MapTiles> mapTilesArray;
    
    
    class GridSystem{
        private:

        public:
        int windowWidth;
        int windowHeight;
        int gridWidth;
        int gridHeight;

        float gridXGap;
        float gridYGap;
        float cameraZoom;
        float cameraXRel = 50; // Must be between 0-100
        float cameraYRel = 50; // Must be between 0-100
        Pos cameraGridRel;

        bool showGrid = true;
        bool showTiles = true;
        SDL_Color backgroundColor;
        SDL_Color gridColor;

        GridSystem();
        void Init(int,int,int,int);
        void Update(int,int);
        void Draw(SDL_Renderer*);
    };

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
        if (showTiles){
            SDL_FRect maskRect;
            for (int i=0;i!=mapTilesArray.size();i++)
            {
                maskRect = {
                    (float(mapTilesArray[i].pos.x+cameraGridRel.x)*
                        (gridXGap*cameraZoom))+gridXGap*cameraXRel/100,
                    (float(mapTilesArray[i].pos.y+cameraGridRel.y)*
                        (gridYGap*cameraZoom))+gridXGap*cameraYRel/100,
                    gridXGap*cameraZoom,
                    gridYGap*cameraZoom
                };

                SDL_RenderCopyF(p_renderer,
                    tileArray[mapTilesArray[i].tileIndex].texture,
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
                    x*gridXGap*cameraZoom,
                    0,
                    x*gridXGap*cameraZoom,
                    windowHeight*tempZoom);
            }

            for (int y=0;
                 y*gridYGap*cameraZoom < windowHeight*tempZoom;
                 y++)
            {
                SDL_RenderDrawLineF(p_renderer,
                    0,
                    y*gridYGap*cameraZoom,
                    windowWidth*tempZoom,
                    y*gridYGap*cameraZoom);
            }
            
            
        }
    }




}