#include "player.h"
#include "pipes.h"
#include "common.h"

Bird::Bird() : dead(false), vertical_speed(0.0f), y(0), x(0), fitness(0), score(0),
               mutation_distribution(0, MUTATION_PROBABILITY - 1),
               node_distribution(-1, std::nextafter(1, 2)),
               weights(2)
{
    Reset();
}


void Bird::Draw(SDL_Renderer * renderer) {
    if (dead) {
        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Set color RED if dead, for debug
        return;
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0); //blu - alive
    }
    SDL_Rect pl;
    pl.x = static_cast<int>(x);
    pl.y = static_cast<int>(y);
    pl.w = BIRD_WIDTH;
    pl.h = BIRD_HEIGHT;

    SDL_RenderFillRect(renderer, &pl);
}

void Bird::Update(int curr ,float dt) {
    
    if (dt >= 1) dt = 1;

    
    float flap_cooldown_time = 0.0f;
    const float flap_interval = 0.25f; // 250ms between flaps

    if (flap_cooldown_time > 0.0f)
        flap_cooldown_time -= dt;

    if (flap_cooldown_time <= 0.0f && think(curr)) {
        vertical_speed = -10.f;
        flap_cooldown_time = flap_interval;
    }
    else {
        vertical_speed += 0.3f * G * dt;
    }


    if (vertical_speed >= 8) vertical_speed = 8;

    y += vertical_speed;

    float dist = get_pipe_distance(curr);
    fitness += 1.0f;

    if (curr != last_scored_pipe && x > Pipe::getPipe(curr).x + Pipe::getPipe(curr).w) {
        fitness += 500;
        score++;
        last_scored_pipe = curr;
    }


    if (y >= SCREEN_HEIGHT || y <= -2 * BIRD_HEIGHT) {
        dead = 1;
        fitness -= (fitness - 100);
    }
    else if (Pipe::isCollide(curr, y)) {
        dead = 1;
        fitness -= 50;
    }
   //std::cout << "Drawing bird at x=" << x << " y=" << y << " dead=" << dead << "\n"; //debug

}

void Bird::Reset()
{
    dead = 0;

    vertical_speed = 0;
    y = 0.5f * (SCREEN_HEIGHT - BIRD_HEIGHT);

    x = 100; // just starting point!

    fitness = 0;

    score = 0;
}

//AI
float Bird::get_pipe_distance(int curr) const {
    const rt& pipe = Pipe::getPipe(curr);
    return pipe.x - x;  
}

float Bird::get_gap_distance(int curr) const {
    const rt& pipe = Pipe::getPipe(curr);
    return (pipe.h + GAP / 2) - y;  
}

unsigned Bird::get_fitness()
{
    return fitness;
}

bool Bird::get_dead() {
    return dead;
}

short Bird::get_score()
{   
    return score;
}

bool Bird::operator>(Bird& i_bird)
{
    return Bird::get_fitness() > i_bird.get_fitness();
}

bool Bird::operator<(Bird& i_bird)
{
    return get_fitness() < i_bird.get_fitness();
}

void Bird::generate_weights(std::mt19937_64& i_random_engine)
{
    weights[0].resize(TOTAL_INPUT_NODES, std::vector<float>(TOTAL_HIDDEN_NODES));
    weights[1].resize(TOTAL_HIDDEN_NODES, std::vector<float>(TOTAL_OUTPUT_NODES));

    //This is how I structured the vector of weights, like shit
    for (std::vector<std::vector<float>>& layer : weights)
    {
        for (std::vector<float>& previous_node : layer)
        {
            for (float& next_node : previous_node)
            {
                //This is the weight of the connection between the previous node and the next.
                next_node = node_distribution(i_random_engine);
            }
        }
    }
}

void Bird::crossover(std::mt19937_64& i_random_engine,
                     const std::vector<std::vector<std::vector<float>>>& i_bird_0_weights,           
                     const std::vector<std::vector<std::vector<float>>>& i_bird_1_weights)           
{
    //I think i used "Uniform crossover".
    //Yeah, I was correct.

    for (unsigned char a = 0; a < weights.size(); a++)
    {
        for (unsigned char b = 0; b < weights[a].size(); b++)
        {
            for (unsigned char c = 0; c < weights[a][b].size(); c++)
            {
                if (0 == rand() % 2)
                {
                    weights[a][b][c] = i_bird_0_weights[a][b][c];
                }
                else
                {
                    weights[a][b][c] = i_bird_1_weights[a][b][c];
                }

                if (0 == mutation_distribution(i_random_engine))
                {
                    weights[a][b][c] = node_distribution(i_random_engine); // yeeeeee ye ey it`s bad i know! Could use matrix! i know but im lazy, dont want spend time, maybe touch some grass
                }
            }
        }
    }

    //std::cout << weights[0][0][0]<<std::endl; //debug
}  

std::vector<std::vector<std::vector<float>>> Bird::get_weights() {
    return weights;
}

bool Bird::think(int curr) {
    std::vector<std::vector<float>> neural_network(3);

    neural_network[0].resize(TOTAL_INPUT_NODES);
    neural_network[1].resize(TOTAL_HIDDEN_NODES);
    neural_network[2].resize(TOTAL_OUTPUT_NODES);

    // Fill input
    neural_network[0][0] = vertical_speed/10.0f;
    neural_network[0][1] = get_pipe_distance(curr)/(SCREEN_WIDTH/2);
    neural_network[0][2] = get_gap_distance(curr) / (SCREEN_HEIGHT / 2);

    // Forward pass, i think
    for (unsigned layer = 0; layer < neural_network.size() - 1; ++layer) {
        for (unsigned neuron_next = 0; neuron_next < neural_network[layer + 1].size(); ++neuron_next) {
            float sum = 0.0f;
            for (unsigned neuron_current = 0; neuron_current < neural_network[layer].size(); ++neuron_current) {
                sum += neural_network[layer][neuron_current] * weights[layer][neuron_current][neuron_next];
            }
            neural_network[layer + 1][neuron_next] = sigmoid(sum);
        }
    }

    float decision = neural_network[2][0];
    std::cout << "Neural output: " << decision << "\n"; //debug

    return decision <= 0.5f;
}


Bird::~Bird() {};