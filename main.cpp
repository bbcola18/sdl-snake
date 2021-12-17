#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <stack>

using namespace std;

const int CELL_SIZE = 20;
const int CELL_AMOUNT = 50;
const int BOARD_SIZE = CELL_SIZE * CELL_AMOUNT;
enum class Direction {RIGHT=0, LEFT, UP, DOWN};

bool gameOver = false;

class Segment {
public:
    int x;
    int y;
    Segment(int myx, int myy) {
        x = myx;
        y = myy;
    }
};

stack<Segment> snake;
int snake_x, snake_y, fruit_x, fruit_y, growth;
Direction direction;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, BOARD_SIZE, BOARD_SIZE, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    srand(time(0));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // initialize game variables
    snake_x = BOARD_SIZE / 2;
    snake_y = BOARD_SIZE / 2;
    fruit_x = (rand() % CELL_AMOUNT) * CELL_SIZE;
    fruit_y = (rand() % CELL_AMOUNT) * CELL_SIZE;
    direction = Direction::LEFT;
    snake.push(Segment(snake_x, snake_y));
    snake.push(Segment(snake_x-CELL_SIZE, snake_y));
    growth = snake.size();

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
                        case SDLK_RIGHT: if (direction != Direction::LEFT) direction = Direction::RIGHT; break;
                        case SDLK_LEFT: if (direction != Direction::RIGHT) direction = Direction::LEFT; break;
                        case SDLK_UP: if (direction != Direction::DOWN) direction = Direction::UP; break;
                        case SDLK_DOWN: if (direction != Direction::UP) direction = Direction::DOWN; break;
                    }
                    break;
                case SDL_QUIT:
                    gameOver = true;
                    break;
            }
        }

        // Render

        // render background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // render grid
        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 10);
        for (int i = 0; i < CELL_AMOUNT; i++)
        {
            for (int j = 0; j < CELL_AMOUNT; j++)
            {
                SDL_Rect cell;
                cell.x = j * CELL_SIZE;
                cell.y = i * CELL_SIZE;
                cell.w = CELL_SIZE;
                cell.h = CELL_SIZE;
                SDL_RenderDrawRect(renderer, &cell);
            }
        }

        // render fruit
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect fruit;
        fruit.x = fruit_x;
        fruit.y = fruit_y;
        fruit.w = CELL_SIZE;
        fruit.h = CELL_SIZE;
        SDL_RenderFillRect(renderer, &fruit);

        // render snake
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        stack<Segment> hold_snake;
        for (int i = 0; i < growth; i++)
        {
            body.x = snake.top().x;
            body.y = snake.top().y;
            hold_snake.push(snake.top());
            SDL_RenderFillRect(renderer, &body);
            snake.pop();
        }
        snake = hold_snake;
        SDL_RenderPresent(renderer);

        // Wait
        SDL_Delay(CELL_AMOUNT * (50/CELL_AMOUNT));

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
        
        // fruit collision
        if (snake_x == fruit_x && snake_y == fruit_y) {
            fruit_x = (rand() % CELL_AMOUNT) * CELL_SIZE;
            fruit_y = (rand() % CELL_AMOUNT) * CELL_SIZE;
            snake.push(snake.top());
            growth++;
        }

        // push the new segment onto the snake
        snake.push(Segment(snake_x, snake_y));

        // create new snake from old segments and check snake collision
        stack<Segment> new_snake;
        for (int i = 0; i < growth; i++)
        {
            if (snake.top().x == snake_x && snake.top().y == snake_y && i != 0) {
                gameOver = true;
            }
            new_snake.push(snake.top());
            snake.pop();
        }
        snake = new_snake;
    }

    /* cleanup SDL */
    SDL_Quit();

    return 0;
}