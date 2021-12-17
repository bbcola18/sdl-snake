#include <SDL.h>

const int CELL_SIZE = 20;
const int BOARD_SIZE = CELL_SIZE * 10;
enum class Direction {RIGHT=0, LEFT, UP, DOWN};

bool gameOver = false;

int snake_x;
int snake_y;
Direction direction;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, BOARD_SIZE, BOARD_SIZE, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // render grid
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 10);
    for (int i = 0; i < CELL_SIZE; i++)
    {
        for (int j = 0; j < CELL_SIZE; j++)
        {
            SDL_Rect cell;
            cell.x = j*CELL_SIZE;
            cell.y = i*CELL_SIZE;
            cell.w = CELL_SIZE;
            cell.h = CELL_SIZE;
            SDL_RenderDrawRect(renderer, &cell);
        }
    }

    // initialize game variables
    snake_x = BOARD_SIZE / 2;
    snake_y = BOARD_SIZE / 2;
    direction = Direction::LEFT;

    SDL_Rect body;
    body.x = snake_x;
    body.y = snake_y;
    body.w = CELL_SIZE;
    body.h = CELL_SIZE;

    while (!gameOver){
        // Application Quit
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) 
            {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) 
                    {
                        case SDLK_RIGHT: direction = Direction::RIGHT; break;
                        case SDLK_LEFT: direction = Direction::LEFT; break;
                        case SDLK_UP: direction = Direction::UP; break;
                        case SDLK_DOWN: direction = Direction::DOWN; break;
                    
                    }
                    break;
                case SDL_QUIT:
                    gameOver = true;
                    break;
            }
        }

        // Logic
        switch (direction) {
            case Direction::RIGHT:
                snake_x = (snake_x + CELL_SIZE) % BOARD_SIZE;
                break;
            case Direction::LEFT:
                snake_x = (BOARD_SIZE + (snake_x - CELL_SIZE)) % BOARD_SIZE;
                break;
            case Direction::UP:
                snake_y = (BOARD_SIZE + (snake_y - CELL_SIZE)) % BOARD_SIZE;
                break;
            case Direction::DOWN:
                snake_y = (snake_y + CELL_SIZE) % BOARD_SIZE;
                break;
        }

        body.x = snake_x;
        body.y = snake_y;

        // Render

        // render background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // render snake
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &body);
        SDL_RenderPresent(renderer);

        // Wait
        SDL_Delay(100);
    }

    /* cleanup SDL */
    SDL_Quit();

    return 0;
}