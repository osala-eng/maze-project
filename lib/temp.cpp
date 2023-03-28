#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

// Define constants
const int WIDTH = 640;
const int HEIGHT = 480;
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;
const int TILE_SIZE = 64;
const int FOV_ANGLE = 60;
const int NUM_RAYS = WIDTH;
const int TEXTURE_WIDTH = 64;
const int TEXTURE_HEIGHT = 64;

// Define variables
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_Surface* wallTextures[4];
int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,0,0,0,1},
    {1,0,0,0,1,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

// Define helper functions
int getMapValue(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return 1;
    }
    return map[x][y];
}

void loadWallTextures() {
    wallTextures[0] = SDL_LoadBMP("textures/wall1.bmp");
    wallTextures[1] = SDL_LoadBMP("textures/wall2.bmp");
    wallTextures[2] = SDL_LoadBMP("textures/wall3.bmp");
    wallTextures[3] = SDL_LoadBMP("textures/wall4.bmp");
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < WIDTH; x++) {
        double rayAngle = atan2((x - WIDTH / 2), (WIDTH / 2 / tan(FOV_ANGLE / 2)));
        double distanceToWall = 0;
        int wallType = 0;
        bool hitWall = false;
        bool hitBoundary = false;

        // Calculate steps
        double eyeX = sin(rayAngle);
        double eyeY = cos(rayAngle);

        // Check each vertical ray
        while (!hitWall && distanceToWall < TILE_SIZE * 5) {
            distanceToWall += 0.1;
            int testX = (int)(eyeX * distanceToWall);
            int testY = (int)(eyeY * distanceToWall);

            // Check for collision with walls
            if (getMapValue(testX, testY) == 1) {
                hitWall = true;

                // Check for boundary between two wall textures
                std::vector<std::pair<double, double>> boundaryPoints;
                for (int tx = 0; tx < TILE_SIZE; tx++){
                    for (int ty = 0; ty < TILE_SIZE; ty++){
                        double vy = (double)ty / (double)TILE_SIZE - 0.5;
                        double vx = (double)tx / (double)TILE_SIZE - 0.5;
                        double d = sqrt(vx * vx + vy * vy);
                        double dot = (eyeX * vx / d) + (eyeY * vy / d);
                        double angle = acos(dot) * 180 / M_PI;
                        if (angle < 20) {
                            boundaryPoints.push_back(std::make_pair(tx, ty));
                        }
                    }
                }
            }
        }
    }
}
