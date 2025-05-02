#pragma once
#include"common.h"

class Pipe
{
public:
    Pipe() {};

    static void Draw(int curr, SDL_Renderer* renderer);
    static void Update(int curr, float dt);
    static bool isCollide(int curr, int y);
    static void MapPipes(int amount);
    static void Reset();
    static int getX(int curr) {
        return PIPES[curr].x;
    }
    static rt getPipe(int curr) {
        return PIPES[curr];
    }

    virtual ~Pipe() {};
    
private:
    static std::vector<rt> PIPES;// dunno how to make this better than that! 16 yo young and stupid :^|
};