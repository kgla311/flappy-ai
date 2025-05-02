#include <iostream>
#include<array>
#include"common.h"
#include"pipes.h"
#include"player.h"

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("ai", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    Pipe::MapPipes(1000); // it can be bigger but why would i need that (??0)

    std::array<Bird, POPULATION_SIZE> birds;
    int curr_pipe = 0;
    std::mt19937_64 random_engine(time(0));


    for (Bird& bird : birds)
    {
        bird.generate_weights(random_engine);
    }

    int generation = 0;

    while (1) {

        float dt = GetDT();
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // or add textures psssss... month later - nothing

        for (Bird& bird : birds)
        {
            bird.Draw(renderer);
        }
        Pipe::Draw(curr_pipe, renderer);


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderPresent(renderer);

        for (Bird& bird : birds)
        {
            bird.Update(curr_pipe, dt);
            
        }
        Pipe::Update(curr_pipe, dt);
        

        if (Pipe::getX(curr_pipe) <= -100) curr_pipe++; // 50 and 50 is scailing and width of the pipe so -100 for smother picture!!! 

        bool is_all_dead = true;
        for (Bird& bird : birds) {
            if (!bird.get_dead()) {
                is_all_dead = false;
                break;
            }
        }
        if (is_all_dead) {
            
            generation++;
            
            std::cout << "Generation: " << generation
                << " | Best Score: " << birds[0].get_score()
                << " | Fitness: " << birds[0].get_fitness() << "\n";

            
            std::sort(birds.begin(), birds.end(), [](Bird& a, Bird& b) {
                return a.get_fitness() > b.get_fitness(); // descending
                });

            
            auto parent0_weights = birds[0].get_weights();
            auto parent1_weights = birds[1].get_weights();

            
            for (auto it = birds.begin() + 2; it != birds.end(); ++it) {
                it->crossover(random_engine, parent0_weights, parent1_weights);
            }

           
            for (Bird& bird : birds) {
                bird.Reset();
            }

       
            Pipe::Reset();
            curr_pipe = 0;
        }
    }
    return 0;
}

