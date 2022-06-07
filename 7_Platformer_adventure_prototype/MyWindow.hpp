#pragma once
#include <SDL2/SDL.h>

namespace MyWindow{
    
    class Window{
        private:
        
        public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        
        int width;
        int height;
        int ID;
        
        bool mouseFocus = false;
        bool isClosed = false;
        bool sizeChanged = false;
        Window();
        void Init(std::string,int,int);
        void FetchWindowEvent(SDL_Event&);

    };

    Window::Window(){
        window = NULL;
        renderer = NULL;
    }

    void Window::Init(std::string p_title_text,int p_window_width,
                                    int p_window_height)
    {
        window = SDL_CreateWindow(
                p_title_text.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                p_window_width,
                p_window_height,
                SDL_WINDOW_RESIZABLE);

        if (window!=NULL){
            renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
            if (renderer!=NULL){
                width = p_window_width;
                height = p_window_height;
                ID = SDL_GetWindowID(window);
            }else{
                std::cout<<"Could Not Create Renderer, Closing Window...\n\t";
                std::cout<<"Error: "<<SDL_GetError();
                SDL_DestroyWindow(window);
            }
        }else{

            std::cout<<"Could Not Create Window...\n\t";
            std::cout<<"Error: "<<SDL_GetError();
        }
    
    }

    void Window::FetchWindowEvent(SDL_Event& p_event){
        // Consider: WindowID, event.type
        if (p_event.window.windowID==ID){
            switch (p_event.window.event){
                case SDL_WINDOWEVENT_CLOSE:           
                isClosed=true;
                SDL_HideWindow(window); 
                break;

                case SDL_WINDOWEVENT_SIZE_CHANGED: 
                    width = p_event.window.data1;
                    height = p_event.window.data2;
                    sizeChanged = true;
                    break;   

                case SDL_WINDOWEVENT_ENTER:
                    mouseFocus=true;
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    mouseFocus=false;
                    break;         

            }
        }
    }
};