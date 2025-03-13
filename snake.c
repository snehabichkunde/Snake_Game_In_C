#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <time.h>

#define WIDTH 900
#define HEIGHT 600
#define CELL_SIZE 25
#define ROWS (HEIGHT / CELL_SIZE)
#define COLS (WIDTH / CELL_SIZE)

#define FRAME_RATE 60
#define FRAME_DELAY (1000 / FRAME_RATE)
#define SPEED 100  // Move every 100ms

typedef struct SnakeSegment {
    int x, y;
    struct SnakeSegment* next;
} SnakeSegment;

typedef struct {
    int up, down, left, right;
} Velocity;

typedef struct {
    int x, y;
} Apple;

SnakeSegment* create_snake() {
    SnakeSegment* head = (SnakeSegment*)malloc(sizeof(SnakeSegment));
    head->x = COLS / 2;
    head->y = ROWS / 2;
    head->next = NULL;
    return head;
}

void spawn_apple(Apple* apple) {
    apple->x = rand() % COLS;
    apple->y = rand() % ROWS;
}

void draw_background(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderClear(renderer);
}

void draw_grid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  // Gray grid
    SDL_Rect rect;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            rect.x = j * CELL_SIZE;
            rect.y = i * CELL_SIZE;
            rect.w = CELL_SIZE - 1;
            rect.h = CELL_SIZE - 1;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void draw_snake(SDL_Renderer* renderer, SnakeSegment* snake) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green snake
    SnakeSegment* temp = snake;
    while (temp) {
        SDL_Rect rect = {temp->x * CELL_SIZE, temp->y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
        temp = temp->next;
    }
}

void draw_apple(SDL_Renderer* renderer, Apple* apple) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red apple
    SDL_Rect rect = {apple->x * CELL_SIZE, apple->y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    SDL_RenderFillRect(renderer, &rect);
}

void move_snake(SnakeSegment* snake, Velocity vel, int* grow) {
    if (!snake) return;

    int new_x = snake->x + (vel.right - vel.left);
    int new_y = snake->y + (vel.down - vel.up);

    SnakeSegment* temp = snake;
    int prev_x = temp->x, prev_y = temp->y;
    temp->x = new_x;
    temp->y = new_y;

    while (temp->next) {
        temp = temp->next;
        int swap_x = temp->x, swap_y = temp->y;
        temp->x = prev_x;
        temp->y = prev_y;
        prev_x = swap_x;
        prev_y = swap_y;
    }

    if (*grow) {
        SnakeSegment* new_segment = (SnakeSegment*)malloc(sizeof(SnakeSegment));
        new_segment->x = prev_x;
        new_segment->y = prev_y;
        new_segment->next = NULL;

        temp->next = new_segment;
        *grow = 0;
    }
}

int check_collision(SnakeSegment* snake) {
    if (snake->x < 0 || snake->x >= COLS || snake->y < 0 || snake->y >= ROWS) {
        return 1;
    }

    SnakeSegment* temp = snake->next;
    while (temp) {
        if (temp->x == snake->x && temp->y == snake->y) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int check_eat_apple(SnakeSegment* snake, Apple* apple) {
    return (snake->x == apple->x && snake->y == apple->y);
}

int main() {
    srand(time(NULL));

    Velocity vel = {0, 1, 0, 0};  // Start moving down
    printf("Starting Snake Game with Apple...\n");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("🐍 Snake Game with Apple",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1, grow = 0;
    SDL_Event event;
    Uint32 lastUpdate = SDL_GetTicks();

    SnakeSegment* snake = create_snake();
    Apple apple;
    spawn_apple(&apple);

    while (running) {
        Uint32 frame_start = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP && !vel.down) {
                    vel.up = 1, vel.down = vel.left = vel.right = 0;
                }
                else if (event.key.keysym.sym == SDLK_DOWN && !vel.up) {
                    vel.down = 1, vel.up = vel.left = vel.right = 0;
                }
                else if (event.key.keysym.sym == SDLK_LEFT && !vel.right) {
                    vel.left = 1, vel.right = vel.up = vel.down = 0;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT && !vel.left) {
                    vel.right = 1, vel.left = vel.up = vel.down = 0;
                }
            }
        }

        if (SDL_GetTicks() - lastUpdate > SPEED) {
            move_snake(snake, vel, &grow);
            if (check_collision(snake)) {
                printf("Game Over! You collided.\n");
                running = 0;
            }
            if (check_eat_apple(snake, &apple)) {
                grow = 1;
                spawn_apple(&apple);
            }
            lastUpdate = SDL_GetTicks();
        }

        draw_background(renderer);
        draw_grid(renderer);
        draw_snake(renderer, snake);
        draw_apple(renderer, &apple);
        SDL_RenderPresent(renderer);

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    SnakeSegment* temp;
    while (snake) {
        temp = snake;
        snake = snake->next;
        free(temp);
    }

    return 0;
}
