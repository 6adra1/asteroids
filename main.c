#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SHIP_SIZE = 20;
const int BULLET_SIZE = 5;

// Game state variables
bool isRunning = true;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Ship position and velocity
int shipX, shipY;
int shipVelX, shipVelY;

// Bullet position and velocity
int bulletX, bulletY;
int bulletVelX, bulletVelY;
bool isBulletActive = false;

// Initialize SDL
bool initialize_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create a window
    window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL)
    {
        SDL_Log("Failed to create SDL window: %s", SDL_GetError());
        return false;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("Failed to create SDL renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        return false;
    }

    return true;
}

void process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                shipVelY = -1;
                break;
            case SDLK_DOWN:
                shipVelY = 1;
                break;
            case SDLK_LEFT:
                shipVelX = -1;
                break;
            case SDLK_RIGHT:
                shipVelX = 1;
                break;
            case SDLK_SPACE:
                if (!isBulletActive)
                {
                    // Fire bullet from the ship's position
                    bulletX = shipX + SHIP_SIZE / 2 - BULLET_SIZE / 2;
                    bulletY = shipY;
                    bulletVelY = -10;
                    isBulletActive = true;
                }
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
            case SDLK_DOWN:
                shipVelY = 0;
                break;
            case SDLK_LEFT:
            case SDLK_RIGHT:
                shipVelX = 0;
                break;
            }
        }
    }
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw ship
    SDL_Rect shipRect = {shipX, shipY, SHIP_SIZE, SHIP_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &shipRect);

    // Draw bullet if active
    if (isBulletActive)
    {
        SDL_Rect bulletRect = {bulletX, bulletY, BULLET_SIZE, BULLET_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &bulletRect);
    }

    SDL_RenderPresent(renderer);
}

void update()
{
    // Update ship position
    if (shipX + shipVelX >= 0 && shipX <= SCREEN_WIDTH - SHIP_SIZE - shipVelX) {
        shipX += shipVelX;
    }
    if (shipY + shipVelY >= 0 && shipY <= SCREEN_HEIGHT - SHIP_SIZE - shipVelY) {
        shipY += shipVelY;
    }

    SDL_Log("shipX: %d, shipY: %d", shipX, shipY);

    // Update bullet position
    if (isBulletActive)
    {
        bulletY += bulletVelY;
        if (bulletY < 0)
        {
            // Bullet is off the screen, deactivate it
            isBulletActive = false;
        }
    }
}

// Clean up resources
void cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    if (!initialize_sdl())
    {
        return 1;
    }

    shipX = SCREEN_WIDTH / 2 - SHIP_SIZE / 2;
    shipY = SCREEN_HEIGHT / 2 - SHIP_SIZE / 2;

    while (isRunning)
    {
        process_input();
        update();
        render();
    }

    cleanup();

    return 0;
}
