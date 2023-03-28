#ifndef __THE_MAZE_PROJECT__
#define __THE_MAZE_PROJECT__

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

// Define constants
const int WIDTH = 800;
const int HEIGHT = 640;
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;
const int TILE_SIZE = 64;
const int FOV_ANGLE = 60;
const int NUM_RAYS = WIDTH;
const int TEXTURE_WIDTH = 64;
const int TEXTURE_HEIGHT = 64;

// Define variables
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Surface *wallTextures[4];

#define __local__ __attribute__((weak))

int map[MAP_HEIGHT][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

// Define helper functions
__local__ int getMapValue(int x, int y)
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
    {
        return 1;
    }
    return map[x][y];
}

__local__ void loadWallTextures()
{
    wallTextures[0] = SDL_LoadBMP("./textures/wall1.bmp");
    wallTextures[1] = SDL_LoadBMP("./textures/wall2.bmp");
    wallTextures[2] = SDL_LoadBMP("./textures/wall3.bmp");
    wallTextures[3] = SDL_LoadBMP("./textures/wall4.bmp");
}

__local__ void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < WIDTH; x++)
    {
        double rayAngle = atan2((x - WIDTH / 2), (WIDTH / 2 / tan(FOV_ANGLE / 2)));
        double distanceToWall = 0;
        int wallType = 0;
        bool hitWall = false;
        bool hitBoundary = false;

        // Calculate steps
        double eyeX = sin(rayAngle);
        double eyeY = cos(rayAngle);

        // Check each vertical ray
        while (!hitWall && distanceToWall < TILE_SIZE * 5)
        {
            distanceToWall += 0.1;
            int testX = (int)(eyeX * distanceToWall);
            int testY = (int)(eyeY * distanceToWall);

            // Check for collision with walls
            if (getMapValue(testX, testY) == 1)
            {
                hitWall = true;

                // Check for boundary between two wall textures
                std::vector<std::pair<double, double>> boundaryPoints;
                for (int tx = 0; tx < TILE_SIZE; tx++)
                {
                    for (int ty = 0; ty < TILE_SIZE; ty++)
                    {
                        double vy = (double)ty / (double)TILE_SIZE - 0.5;
                        double vx = (double)tx / (double)TILE_SIZE - 0.5;
                        double d = sqrt(vx * vx + vy * vy);
                        double dot = (eyeX * vx / d) + (eyeY * vy / d);
                        double angle = acos(dot) * 180 / M_PI;
                        if (angle < 20)
                        {
                            boundaryPoints.push_back(std::make_pair(tx, ty));
                        }
                    }
                }
            }
        }
    }
}

__local__ void initSDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("The Maze Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    loadWallTextures();
    auto windowSurface = SDL_GetWindowSurface(window);

    for (auto texture : wallTextures)
    {
        SDL_BlitSurface(texture, NULL, windowSurface, NULL);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(5000);
    }

    // Get window to stay up
    SDL_Event e;
    bool quit = false;
    while (quit == false)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }
    }
}

#endif