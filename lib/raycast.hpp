#ifndef __RAYCAST_HPP__
#define __RAYCAST_HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdio.h>

#define MAP_SIZE 256

#define __local__ __attribute__((weak))

// Define constants
const double MOVE_SPEED = 0.0875;
const double ANGLE_SPEED = 0.0025625;
const double FOG = 20.0;
const double VERTICAL_FOV = 1.257;
const double DISTANCE_PROJECTION_PLANE =  1.0;//(MAP_SIZE / 2) / tan(VERTICAL_FOV / 2);
const double FPS = 60.0;

const int WIDTH = 640;
const int HEIGHT = 480;

// Struct
typedef struct fVector2
{
    double x;
    double y;
} fVector2;

// Create player pos
fVector2 playerPos;
double playerA = 0.0;

// define map
int map[MAP_SIZE][MAP_SIZE];


// define texture
SDL_Surface* wall_img_surf;

__local__ fVector2 distanceToWall(fVector2 playerPos, double playerA);
__local__ void readMap(fVector2 *playerPos, double* playerA);
__local__ void playerMove(double dX, double dY);
__local__ bool handleEvents(SDL_Event);
__local__ double veclane(fVector2 vec);
__local__ void drawScreen(SDL_Renderer* renderer, SDL_Texture* screen);


__local__ double veclane(fVector2 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

__local__ void drawScreen(SDL_Renderer* renderer, SDL_Texture* screen)
{
    // Create screen buffer
    Uint32* screenBuffer = new Uint32[WIDTH * HEIGHT];

    // Clear screen buffer
    memset(screenBuffer, 0, WIDTH * HEIGHT * sizeof(Uint32));

    // Draw floor
    for (int y = HEIGHT / 2; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            screenBuffer[y * WIDTH + x] = 0x000000;
        }
    }

    // Draw ceiling
    for (int y = 0; y < HEIGHT / 2; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            screenBuffer[y * WIDTH + x] = 0x000000;
        }
    }

    // Draw walls
    for (int x = 0; x < WIDTH; x++)
    {
        // Calculate ray angle
        double rayAngle = atan2((x - WIDTH / 2), (WIDTH / 2 / tan(VERTICAL_FOV / 2)));

        // Calculate distance to wall
        fVector2 distance = distanceToWall(playerPos, playerA + rayAngle);

        // Calculate wall height
        double wallHeight = (MAP_SIZE / distance.x) * DISTANCE_PROJECTION_PLANE;

        // Calculate wall color
        double wallColor = 255 - std::min(1.0, distance.x / FOG) * 255;

        // Draw wall
        for (int y = 0; y < wallHeight; y++)
        {
            if (y + HEIGHT / 2 - wallHeight / 2 < 0 || y + HEIGHT / 2 - wallHeight / 2 >= HEIGHT)
            {
                continue;
            }
            // screenBuffer[(y + HEIGHT / 2 - wallHeight / 2) * WIDTH + x] = 0x000000;
        }
    }

    // Update screen
    SDL_UpdateTexture(screen, NULL, screenBuffer, WIDTH * sizeof(Uint32));
    SDL_RenderCopy(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Free screen buffer
    delete[] screenBuffer;
}





int Xmain ()
{
    // loop control
    bool running = true;

    // SDL init
    SDL_Init(SDL_INIT_VIDEO);

    // Create window and renderer
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    // Create texture
    SDL_Texture* screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    // Setup Mouse event handler
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_Event event;

    // Read map
    readMap(&playerPos, &playerA);

    // Load wall texture
    wall_img_surf = SDL_LoadBMP("../textures/wall2.bmp");

    // Main loop
    while (running)
    {
        Uint32 frameStart = SDL_GetTicks();
        // Handle events
        running = handleEvents(event);

        // Draw screen
        drawScreen(renderer, screen);

        // Get frame time
        Uint32 frameTime = SDL_GetTicks() - frameStart;

        // Delay to get 60 fps
        if (frameTime < 1000 / FPS)
        {
            SDL_Delay((1000 / FPS) - frameTime);
        }

        // Update screen
        // SDL_RenderPresent(renderer);
    }

    // Free memory
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // SDL_FreeSurface(wall_img_surf);
    // SDL_Quit();

    return 0;
}

fVector2 distanceToWall(fVector2 playerPos, double playerA)
{
    // Define variables
    fVector2 rayPos = playerPos;
    fVector2 rayDir = {cos(playerA), sin(playerA)};
    fVector2 deltaDist = {veclane(rayDir), veclane(rayDir)};
    fVector2 sideDist = {0.0, 0.0};
    fVector2 step = {0.0, 0.0};
    fVector2 mapPos = {0.0, 0.0};
    fVector2 wallHit = {0.0, 0.0};
    fVector2 distance = {0.0, 0.0};

    // Calculate step and initial sideDist
    if (rayDir.x < 0)
    {
        step.x = -1;
        sideDist.x = (rayPos.x - mapPos.x) * deltaDist.x;
    }
    else
    {
        step.x = 1;
        sideDist.x = (mapPos.x + 1.0 - rayPos.x) * deltaDist.x;
    }

    if (rayDir.y < 0)
    {
        step.y = -1;
        sideDist.y = (rayPos.y - mapPos.y) * deltaDist.y;
    }
    else
    {
        step.y = 1;
        sideDist.y = (mapPos.y + 1.0 - rayPos.y) * deltaDist.y;
    }

    // Perform DDA
    bool hitWall = false;
    bool hitBoundary = false;
    while (!hitWall)
    {
        // Jump to next map square, OR in x-direction, OR in y-direction
        if (sideDist.x < sideDist.y)
        {
            sideDist.x += deltaDist.x;
            mapPos.x += step.x;
            wallHit.y = 0;
            wallHit.x = step.x;
        }
        else
        {
            sideDist.y += deltaDist.y;
            mapPos.y += step.y;
            wallHit.x = 0;
            wallHit.y = step.y;
        }

        // Check if ray has hit a wall
        if (map[(int)mapPos.x][(int)mapPos.y] > 0)
        {
            hitWall = true;
        }
    }

    // Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
    if (wallHit.x == -1)
    {
        distance.x = (rayPos.x - mapPos.x);
    }
    else if (wallHit.x == 1)
    {
        distance.x = (mapPos.x + 1.0 - rayPos.x);
    }
    else if (wallHit.y == -1)
    {
        distance.y = (rayPos.y - mapPos.y);
    }
    else if (wallHit.y == 1)
    {
        distance.y = (mapPos.y + 1.0 - rayPos.y);
    }

    return distance;
}

void readMap(fVector2 *playerPos, double* playerA)
{
    // Open file
    FILE* file = fopen("../maps/world.json", "r");

    // Read map
    for (int y = 0; y < MAP_SIZE; y++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            fscanf(file, "%d", &map[x][y]);
            if (map[x][y] == 2)
            {
                playerPos->x = x + 0.5;
                playerPos->y = y + 0.5;
            }
            else if (map[x][y] == 3)
            {
                *playerA = atan2(y - playerPos->y, x - playerPos->x);
            }
        }
    }

    // Close file
    fclose(file);
}

void playerMove(double dX, double dY)
{
    // Check if player can move
    if (map[(int)(playerPos.x + dX)][(int)playerPos.y] == 0)
    {
        playerPos.x += dX;
    }
    if (map[(int)playerPos.x][(int)(playerPos.y + dY)] == 0)
    {
        playerPos.y += dY;
    }
}

void playerRotate(double dA)
{
    playerA += dA;
}

bool handleEvents(SDL_Event event)
{
    // Loop control
    bool running = true;

    // Handle events
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_w:
                        playerMove(0.1 * cos(playerA), 0.1 * sin(playerA));
                        break;
                    case SDLK_s:
                        playerMove(-0.1 * cos(playerA), -0.1 * sin(playerA));
                        break;
                    case SDLK_a:
                        playerMove(0.1 * cos(playerA + M_PI / 2), 0.1 * sin(playerA + M_PI / 2));
                        break;
                    case SDLK_d:
                        playerMove(0.1 * cos(playerA - M_PI / 2), 0.1 * sin(playerA - M_PI / 2));
                        break;
                    case SDLK_q:
                        playerRotate(-0.1);
                        break;
                    case SDLK_e:
                        playerRotate(0.1);
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                playerRotate(event.motion.xrel * 0.001);
                break;
        }
    }

    return running;
}

#define TILE_SIZE 64

void drawMap(SDL_Renderer* renderer)
{
    // Draw map
    for (int y = 0; y < MAP_SIZE; y++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            if (map[x][y] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void drawPlayer(SDL_Renderer* renderer)
{
    // Draw player
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = {(int)(playerPos.x * TILE_SIZE), (int)(playerPos.y * TILE_SIZE), TILE_SIZE, TILE_SIZE};
    SDL_RenderFillRect(renderer, &rect);
}

void drawRays(SDL_Renderer* renderer)
{
    // Draw rays
    for (int x = 0; x < WIDTH; x++)
    {
        // Calculate ray position and direction
        double cameraX = 2 * x / (double)WIDTH - 1; // x-coordinate in camera space
        double rayDirX = cos(playerA) + playerPos.x * cameraX;
        double rayDirY = sin(playerA) + playerPos.y * cameraX;

        // Which box of the map we're in
        fVector2 mapPos = {playerPos.x, playerPos.y};

        // Length of ray from current position to next x or y-side
        fVector2 deltaDist = {fabs(1 / rayDirX), fabs(1 / rayDirY)};

        // Length of ray from one x or y-side to next x or y-side
        fVector2 sideDist;

        // What direction to step in x or y-direction (either +1 or -1)
        fVector2 step;

        // Calculate step and initial sideDist
        if (rayDirX < 0)
        {
            step.x = -1;
            sideDist.x = (playerPos.x - mapPos.x) * deltaDist.x;
        }
        else
        {
            step.x = 1;
            sideDist.x = (mapPos.x + 1.0 - playerPos.x) * deltaDist.x;
        }

        if (rayDirY < 0)
        {
            step.y = -1;
            sideDist.y = (playerPos.y - mapPos.y) * deltaDist.y;
        }
        else
        {
            step.y = 1;
            sideDist.y = (mapPos.y + 1.0 - playerPos.y) * deltaDist.y;
        }

        // Perform DDA
        bool hitWall = false;
        while (!hitWall)
        {
            // Jump to next map square, OR in x-direction, OR in y-direction
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapPos.x += step.x;
            }
            else
            {
                sideDist.y += deltaDist.y;
                mapPos.y += step.y;
            }

            // Check if ray has hit a wall
            if (map[(int)mapPos.x][( int)mapPos.y] > 0)
            {
                hitWall = true;
            }
        }

        // Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
        double perpWallDist;
        if (sideDist.x < sideDist.y)
        {
            perpWallDist = (mapPos.x - playerPos.x + (1 - step.x) / 2) / rayDirX;
        }
        else
        {
            perpWallDist = (mapPos.y - playerPos.y + (1 - step.y) / 2) / rayDirY;
        }

        // Calculate height of line to draw on screen
        int lineHeight = (int)(HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0)
        {
            drawStart = 0;
        }
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT)
        {
            drawEnd = HEIGHT - 1;
        }

        // Choose wall color
        SDL_Color color;
        switch (map[(int)mapPos.x][(int)mapPos.y])
        {
            case 1:
                color = {255, 255, 255, 255};
                break;
            case 2:
                color = {255, 0, 0, 255};
                break;
            case 3:
                color = {0, 255, 0, 255};
                break;
            case 4:
                color = {0, 0, 255, 255};
                break;
        }
        
        // Draw the pixels of the stripe as a vertical line
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}

void draw(SDL_Renderer* renderer)
{
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawMap(renderer);
    drawPlayer(renderer);
    drawRays(renderer);

    // Update screen
    SDL_RenderPresent(renderer);
}

int Ymain(int argc, char* args[])
{
    SDL_Window* window;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s \r  ", SDL_GetError());     
    }
    else
    {
        // Create window
         window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s \r  ", SDL_GetError());
        }
        else
        {
            // Create renderer for window
            SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s \r  ", SDL_GetError());
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                // Initialize renderer clear color
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                // Main loop flag
                bool running = true;

                // Event handler
                SDL_Event e;

                // While application is running
                while (running)
                {
                    // Handle events on queue
                    while (SDL_PollEvent(&e) != 0)
                    {
                        // User requests quit
                        if (e.type == SDL_QUIT)
                        {
                            running = false;
                        }

                        running = handleEvents(e);
                    }

                    draw(renderer);
                }
            }
        }
    }
    
    // Destroy window   
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}


#endif // __RAYCAST_HPP__