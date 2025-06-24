#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <cstdlib> 
#include <ctime>
#include <cmath>
#include <cassert>
#include <tuple>
#include <functional>
#include <fstream>
#include "SDL.h"

constexpr int    MUTATION_PROBABILITY = 11; // 10%
constexpr int    POPULATION_SIZE = 50;
constexpr int    TOTAL_HIDDEN_NODES = 10;
constexpr int    TOTAL_INPUT_NODES = 3;
constexpr int    TOTAL_OUTPUT_NODES = 2; 
                 
constexpr int    SCREEN_HEIGHT = 600;
constexpr int    SCREEN_WIDTH = 800;
constexpr double G = 10;
                
constexpr int    BIRD_HEIGHT = 50;
constexpr int    BIRD_WIDTH = 75;
constexpr int    GAP = 200;
                        

struct rt //mostly for pipes 
{

    float x;
    float y;
    int w;
    int h;

    int ticks;
};


 inline double sigmoid(int x) {
    return 1.f / (1.f + expf(-x));
}

inline int GetRandom(int min, int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
    return dist6(rng); 
}

inline double GetDT() {
    static Uint64 LAST = SDL_GetPerformanceCounter();  
    Uint64 NOW = SDL_GetPerformanceCounter();

    double dt = (double)(NOW - LAST)*1000/ (double)SDL_GetPerformanceFrequency();  // seconds
    LAST = NOW; 

    return dt;
}


