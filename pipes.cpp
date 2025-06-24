#include"pipes.h"

std::vector<rt> Pipe::PIPES;

void Pipe::Draw(int curr, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  
    SDL_Rect upper_pipe;
    upper_pipe.x = static_cast<int>(PIPES[curr].x);
    upper_pipe.y = static_cast<int>(PIPES[curr].y);
    upper_pipe.w = PIPES[curr].w;
    upper_pipe.h = PIPES[curr].h;

    SDL_Rect lower_pipe;
    lower_pipe.x = static_cast<int>(PIPES[curr].x);
    lower_pipe.y = PIPES[curr].h + GAP;
    lower_pipe.w = PIPES[curr].w;
    lower_pipe.h = SCREEN_HEIGHT - lower_pipe.y;

    SDL_RenderCopyEx(renderer, texture, nullptr, &upper_pipe, 180.0, nullptr, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, texture, nullptr, &lower_pipe, 180.0, nullptr, SDL_FLIP_VERTICAL);
}


void Pipe::Update(int curr, float dt) {
    PIPES[curr].x -= 1*dt;
}

bool Pipe::isCollide(int curr, int y) {
    if ((PIPES[curr].x >= 100 && PIPES[curr].x <= 100 + BIRD_WIDTH) && (PIPES[curr].h >= y || y >= PIPES[curr].h + GAP)) return true; 
    // 100 is default, 175 with bird hitbox length, 200 - GAP
    else
    {
        return false;
    }
}

void Pipe::MapPipes(int amount) {
    for (int i = 0; i <= amount; i++) {
        rt temp;

        temp.h = GetRandom(50, 400); // height - gap, 25 so there will be some pipe just for beauty! 
        temp.w = 50;
        temp.x = SCREEN_WIDTH - temp.w;
        temp.y = 0;

        PIPES.push_back(temp);
    }

}

void Pipe::Reset() {
    Pipe::PIPES.clear();  
    Pipe::MapPipes(1000);
}