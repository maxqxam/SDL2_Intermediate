int randint(int a,int b){
    return (rand()%(b-a)) + a; 
}

void ToggleFullscreen(Window& p_Window) {
    
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool IsFullscreen = SDL_GetWindowFlags(p_Window.window) & FullscreenFlag;
    int x,y;

    SDL_SetWindowFullscreen(p_Window.window, ~IsFullscreen);
    SDL_GetWindowSize(p_Window.window,&p_Window.rect.w,&p_Window.rect.h);
    SDL_ShowCursor(IsFullscreen);
    SDL_DestroyRenderer(p_Window.renderer);
    p_Window.renderer = SDL_CreateRenderer(
        p_Window.window,-1,SDL_RENDERER_ACCELERATED);  
}

SDL_Texture* loadTexture(SDL_Renderer* p_renderer,
                std::string path,SDL_Rect& _rect){
    
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
    _rect.w = loadedSurface->w;
    _rect.h = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    
    return newTexture;
}
// Pos GetDisplaySize(){ //It only works AFTER the window is initialized
//     SDL_DisplayMode DM;
//     SDL_GetDesktopDisplayMode(0, &DM);
//     auto Width = DM.w;
//     auto Height = DM.h;
//     return {Width,Height};
// }
