void Init();
void FetchEvents();
void CheckEvents();
void DrawAndUpdate();
void Reshape();
int randint(int,int);
SDL_Event event;

std::string TexturesAddress = "/home/yolo/Workstation/Graphies/Tiles/";
std::string TexturesNames[] = {
    "stone0.png",
    "stone1.png",
    "stone2.png",
    "ladder0.png",
    "ladder1.png",
    "barrel0.png",
    "oimages/person0.png",
    "oimages/person1.png",
    "oimages/person2.png",
    "oimages/person3.png"
};

bool shouldRun = true;
bool showGrids = true;

struct Window;
struct Pos;
struct Images;
struct Person;
struct Tile;
float gridXSize,gridYSize;
enum {
    UP,DOWN,
    RIGHT,LEFT
};
bool heldKeys[4];
SDL_Rect mouseRect;

void ToggleFullscreen(Window&);
SDL_Texture* loadTexture(SDL_Renderer*,
                std::string,SDL_FRect&);