#include <SDL2/SDL.h>
#include <vector>
#include "MyWindow.hpp"

namespace GSWE{
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
                    float(mapTilesArray[i].pos.x)*gridXGap,
                    float(mapTilesArray[i].pos.y)*gridYGap,
                    gridXGap,
                    gridYGap
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
            
            for (float i=0;i!=gridWidth;i++)
            {
                SDL_RenderDrawLineF(p_renderer,
                        0.0,
                        i*gridYGap,
                        windowWidth,
                        i*gridYGap);
            }

            for (float i=0;i!=gridHeight;i++)
            {
                SDL_RenderDrawLineF(p_renderer,
                        i*gridXGap,
                        0.0,
                        i*gridXGap,
                        windowHeight);
            }
        }
    }




}