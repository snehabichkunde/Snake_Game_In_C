#include <stdio.h>
#include <SDL.h>

#define WIDTH 900
#define HEIGHT 600
#define CELL_SIZE 25
#define ROWS (HEIGHT / CELL_SIZE)
#define COLS (WIDTH / CELL_SIZE)

#define FRAME_RATE 60  // Limit frame rate to 60 FPS
#define FRAME_DELAY (1000 / FRAME_RATE)  // Delay per frame


typedef struct {
    int x, y;
} SnakeSegment;

SnakeSegment snake = {5, 9};    // initialise the snake at position (5,9)

void draw_background(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderClear(renderer);
}

void draw_grid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White grid lines
    
    for (int i = 0; i <= ROWS; i++) {
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, WIDTH, i * CELL_SIZE);
    }
    for (int j = 0; j <= COLS; j++) {
        SDL_RenderDrawLine(renderer, j * CELL_SIZE, 0, j * CELL_SIZE, HEIGHT);
    }
}

void draw_snake(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 0, 225, 0, 255);
    SDL_Rect rect = {snake.x * CELL_SIZE, snake.y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    SDL_RenderFillRect(renderer, &rect);
}

int main() {
    printf("Starting Optimized Snake Game Board...\n");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("\U0001F40D Optimized Snake Board",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT,
                                          SDL_WINDOW_SHOWN);

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

    int running = 1;
    SDL_Event event;

    while (running) {
        Uint32 frame_start = SDL_GetTicks();  // Track frame start time

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Render at 60 FPS
        draw_background(renderer);
        draw_grid(renderer);
        draw_snake(renderer);
        SDL_RenderPresent(renderer);

        // Calculate frame time and delay if needed
        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
