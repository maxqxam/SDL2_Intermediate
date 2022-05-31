struct Pos{
    int x;
    int y;
};

Pos mousePos {0,0};
Pos gridCount;

struct Images{
    SDL_Texture* texture = NULL;
    SDL_Rect rect;
    Images(){};

    
};

Images Mask;
std::vector<Images> ImagesArray(10,Mask);

struct Tile{
    Pos pos = {0,0};
    int imageIndex;
    // Tile(){

    // };
    
    void Draw(SDL_Renderer* p_renderer,
                float p_width,float p_height){
        
        SDL_FRect mask = {float(pos.x)*gridXSize,float(pos.y)*gridYSize,
                                p_width,p_height};

        SDL_RenderCopyF(p_renderer
                ,ImagesArray[imageIndex].texture,NULL,&mask);
        
    }
};
std::vector<Tile> TilesArray;

bool isInTiles(Pos p_pos){
    for (int i=0;i!=TilesArray.size();i++)
    {
        if (p_pos.x==TilesArray[i].pos.x &&
            p_pos.y==TilesArray[i].pos.y) return true;

    }
    return false;
}
bool isInLadders(Pos p_pos){
    for (int i=0;i!=TilesArray.size();i++)
    {   

        if (p_pos.x==TilesArray[i].pos.x &&
            p_pos.y==TilesArray[i].pos.y &&(
            TilesArray[i].imageIndex==3 || 
            TilesArray[i].imageIndex==4)
            ) return true;

    }
    return false;
}

struct Person{
    public:
    Pos pos = {0,0};
    int frame = 0;
    Person(){}
    void Draw(SDL_Renderer* p_renderer,
                float p_width,float p_height){
        
        SDL_FRect mask = {float(pos.x)*gridXSize,float(pos.y)*gridYSize,
                                p_width,p_height};

        SDL_RenderCopyF(p_renderer
                ,ImagesArray[imageIndex+frame].texture,NULL,&mask);
        
    }

    void Fall(){
        Pos maskPos = {pos.x,pos.y+1};
        if (!isInTiles(maskPos)){
            pos = maskPos;
        }
    }

    void Move(int p_dirc){
        Pos maskPos = pos;
             if(p_dirc==UP)     maskPos.y--;
        else if(p_dirc==DOWN)   maskPos.y++;
        else if(p_dirc==RIGHT)  maskPos.x++;
        else if(p_dirc==LEFT)   maskPos.x--;
        
        bool conditions[] = {
            isInTiles(pos),
            isInLadders(pos),
            isInTiles(maskPos),
            isInLadders(maskPos),
            (p_dirc==UP),
            (p_dirc==DOWN)
        };
        
        if (!(conditions[2]&&!conditions[3]))
        {
            if (conditions[4]||conditions[5]){
                if (conditions[3] || conditions[1]){
                    pos = maskPos;
                }
            }else{
                pos = maskPos;
            }
        }
    }
    private:
    int imageIndex = 6;

};

Person Player;

struct Window{
    public:

    SDL_Rect rect;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    void Create(const char*,SDL_Rect,bool);
    private:


};

Window mainWindow;

void Window::Create(const char* p_title_str, SDL_Rect p_rect,bool p_no_pos=true){
    rect = p_rect;

    if (p_no_pos){
        window = SDL_CreateWindow(p_title_str,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    rect.w,
                                    rect.h,
                                    SDL_WINDOW_SHOWN);    
    }else{
        window = SDL_CreateWindow(p_title_str,
                                    rect.x,
                                    rect.y,
                                    rect.w,
                                    rect.h,
                                    SDL_WINDOW_SHOWN);
    }

    if (window==NULL){
        out("ERROR: \t Could not create Window : \n\t")
        out(SDL_GetError()) enter
    }else{
        renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
        if (renderer==NULL){
            out("ERROR: \t Could not create Renderer : \n\t")
            out(SDL_GetError()) enter
        }
    }
}

