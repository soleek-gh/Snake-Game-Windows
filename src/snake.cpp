
#include "iostream"
#include "Windows.h"
#include <vector>
#include <random>

#define MIN 1
#define MAX_X  18
#define MAX_Y  48

enum DIRECTION
{
    UP,
    DOWN,
    RIGHT,
    LEFT
};

struct Vec2
{
    int x; 
    int y;    
};

enum class GameState
{
    RUNNING,
    GAME_OVER
};

struct Snake
{
    Vec2 head_position;
    DIRECTION dir;
    std::vector<Vec2> tails;
};

//snake tails are following the head
void move_tails(std::vector<Vec2>& tails, Vec2 snake_head_position)
{
    Vec2 position_next_tail;

    for(size_t i = 0; i < tails.size(); i++)
    {
        if(i == 0)
        {
            position_next_tail.x = snake_head_position.x;
            position_next_tail.y = snake_head_position.y;
            continue;
        }
        Vec2 current_pos_tail;
        current_pos_tail.x = tails[i].x;
        current_pos_tail.y = tails[i].y;
        
        tails[i].x = position_next_tail.x;    
        tails[i].y = position_next_tail.y;    

        //position for the next tail
        position_next_tail.x = current_pos_tail.x; 
        position_next_tail.y = current_pos_tail.y;
    }

}

bool is_snake_colliding(Vec2 head_position,Vec2 current_tail_position)
{
    if(head_position.x == current_tail_position.x && head_position.y == current_tail_position.y)
    {
        return true;
    }
    return false;
}


bool is_food_eaten(Vec2 snake_head, Vec2 food_pos)
{
    if(snake_head.x == food_pos.x && snake_head.y == food_pos.y)
    {
        return true;
    }
    return false;
}

void randomize_food_spawn(Vec2& food_pos)
{
   std::random_device rd;  
   std::mt19937 gen(rd());

   std::uniform_int_distribution<> distr_X(MIN,MAX_X);
   std::uniform_int_distribution<> distr_Y(MIN,MAX_Y);

   const int random_X = distr_X(gen);
   const int random_Y = distr_Y(gen);

   food_pos.x = random_X; food_pos.y = random_Y;

}

//add + 1 tail to snake

void out_of_bounce_position(Vec2 &snake_head,int COLS, int ROWS)
{
    if(snake_head.x >= COLS){snake_head.x = 1;}
    if(snake_head.x <= 0){snake_head.x = COLS - 1;}

    if(snake_head.y >= ROWS){snake_head.y = 1;}
    if(snake_head.y <= 0){snake_head.y = ROWS - 1;}
}

void add_tail(std::vector<Vec2>& tails, int COLS , int ROWS)
{
    //get last tail
    int last_x =  tails.back().x;

    tails.push_back({(COLS / 2 ) + last_x + 1, (ROWS / 2)});
}


void update_snake(Snake& snake)
{
    // move snake 
    switch (snake.dir)
    {
    case DIRECTION::UP:
        snake.tails[0].x--;    
        break;
 
    case DIRECTION::DOWN:
        snake.tails[0].x++;
        break;
        
    case DIRECTION::LEFT:
        snake.tails[0].y--;
        break;
        
    case DIRECTION::RIGHT:
        snake.tails[0].y++;
        break;
        
    default:
        break;
    }

}; 

Snake spawn_snake(int COLS , int ROWS)
{
   Vec2 spawn_point = {COLS / 2, ROWS / 2};

    //snake creation 
    //head
    Snake snake_spawn = {spawn_point};
    snake_spawn.tails.push_back(spawn_point);

    //body
    snake_spawn.tails.push_back({(COLS / 2 ) + 1, (ROWS / 2)});
    return snake_spawn;
}



int main()
{
    int input = 0;
    const int COLS = 20; //x           
    const int ROWS = 50; //y
    const char* map_symbol = " "; //add "." for debugging

    const char* borders_symbol[2];
    borders_symbol[0] = "|"; //side
    borders_symbol[1] = "-"; // top - bottom
    
    GameState state = GameState::RUNNING;

    bool isQuit = false;
    int best_score = 0;
    int score = 0;

    std::string snake_symbol = "▀";
    std::string food_symbol = "●";
    std::vector<std::string> snake_body_symbols;

    snake_body_symbols.push_back(snake_symbol);
    
    Vec2 spawn_point = {COLS / 2, ROWS / 2};

    //food spawn location
    Vec2 food = {15,17};

    //snake creation 
    //head
    Snake snake_spawn = spawn_snake(COLS, ROWS);

    //game loop
    while(!isQuit)
    {

        SetConsoleOutputCP(CP_UTF8);

        for(size_t i = 0; i < COLS; i++)
        {
            for(size_t j = 0; j < ROWS; j++)  
            {
                //border1
                if(i == 0 || i == COLS - 1)
                {
                    std::cout << borders_symbol[1];      
                    continue;   
                }  

                //if head is there (should always be there)
                else if(i == snake_spawn.tails[0].x && j == snake_spawn.tails[0].y)
                {
                    // display head
                        std::cout << snake_symbol;
                        continue;
                }
                //display body
                bool tail_found = false;
                if(snake_spawn.tails.size() > 1)
                {
                    for(size_t k = 1; k < snake_spawn.tails.size(); k++)
                        {
                            if(is_snake_colliding(snake_spawn.tails[0], snake_spawn.tails[k]))
                            {
                                state = GameState::GAME_OVER;
                            }

                            if(i == snake_spawn.tails[k].x && j == snake_spawn.tails[k].y)
                            {
                                tail_found = true;
                                std::cout << snake_symbol;
                            }
                        
                        }
                    if(tail_found)
                    {
                        continue;
                    }
                }
                //Display food
                if(i == food.x && j == food.y)
                {
                    std::cout << food_symbol;
                    continue;
                }

                if(is_food_eaten(snake_spawn.tails[0], food))    
                {
                    randomize_food_spawn(food);
                    add_tail(snake_spawn.tails,COLS,ROWS);
                    score++;
                }


                //border2
                if(j == 0 || j == ROWS - 1)
                {
                    std::cout << borders_symbol[0];       
                    continue;  
                }

               printf(map_symbol);            
            }
            printf("\n");
        }

        //head position before update
        Vec2 snake_head_initial_position = snake_spawn.tails[0];    

        //inputs
        if(snake_spawn.dir != DIRECTION::DOWN)
        {
            if(GetKeyState(VK_UP) & 0x8000)
            {
                snake_spawn.dir = DIRECTION::UP;
            }
        }
        if(snake_spawn.dir != DIRECTION::UP)
        {
            if(GetKeyState(VK_DOWN) & 0x8000)
            {
                snake_spawn.dir = DIRECTION::DOWN;
            }
        }
        if(snake_spawn.dir != DIRECTION::RIGHT)
        {
            if(GetKeyState(VK_LEFT) & 0x8000)
            {
                snake_spawn.dir = DIRECTION::LEFT;
            }
         }
        if(snake_spawn.dir != DIRECTION::LEFT)
        {
            if(GetKeyState(VK_RIGHT) & 0x8000)
            {
                snake_spawn.dir = DIRECTION::RIGHT;
            }
        }
        //update snake
        update_snake(snake_spawn);
        out_of_bounce_position(snake_spawn.tails[0],COLS,ROWS);
        move_tails(snake_spawn.tails,snake_head_initial_position);

        Sleep(45);

        printf("SCORE : %d   BEST SCORE  : %d \n", score, best_score);

        std::cout << "\n"; 

        switch(state)
        {
            case GameState::RUNNING:
                printf("GAME IS RUNNING             ");
                break;
            case GameState::GAME_OVER:
                printf("GAME OVER                   ");
                break;
        }

        if(state == GameState::GAME_OVER)
        {
            std::cout << "\n"; 
            if(score > best_score)
            {
                best_score = score;
            }
            system("pause");
            //resets snake and food
            score = 0;
            snake_spawn.tails.clear();
            snake_spawn = spawn_snake(COLS, ROWS);
            randomize_food_spawn(food);
            state = GameState::RUNNING;
            Sleep(100);
        }

        //reset cursor
        printf("\033[%d;%dH", (0), (0));
        fputs("\033[?25l", stdout);

    }


    return 0;   
}