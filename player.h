#pragma once
#include"common.h"

class Bird
{
    bool dead;

    float vertical_speed ;
    int y;
    int x;

    unsigned fitness;

    short score;

    std::uniform_int_distribution<unsigned short> mutation_distribution;

    //This is the range in which the weights can be.
    std::uniform_real_distribution<float> node_distribution;

    //I didn't know how to use arrays here, and matrix to be honest....
    std::vector<std::vector<std::vector<float>>> weights;

public:

    // GAME:
     
    Bird();

    void Draw(SDL_Renderer* rendr);

    void Update(int curr , float dt);
    
    void Reset();

    //AI:
   
    void generate_weights(std::mt19937_64& i_random_engine);

    void crossover(std::mt19937_64& i_random_engine, const std::vector<std::vector<std::vector<float>>>& i_bird_0_weights, const std::vector<std::vector<std::vector<float>>>& i_bird_1_weights);
     
    std::vector<std::vector<std::vector<float>>> get_weights();

    float get_pipe_distance(int curr) const;

    float get_gap_distance(int curr) const;

    unsigned get_fitness();

    bool get_dead();

    short get_score();

    bool operator>(Bird& i_bird);

    bool operator<(Bird& i_bird);
    
    bool think(int curr);
   
    virtual ~Bird();

private:

    int last_scored_pipe = -1;
};
