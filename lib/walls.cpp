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

int map[MAP_HEIGHT][MAP_HEIGHT] = {
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

void render () {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < WIDTH; x++) {
        double rayAngle = atan2((x - WIDTH / 2), (WIDTH / 2 / tan(FOV_ANGLE / 2)));
        double distanceToWall = 0;
        int wallType = 0;
        bool hitWall = false;
        bool hitBoundary = false;

        // Calculate steps
        double xStep = cos(rayAngle);
        double yStep = sin(rayAngle);

        // Calculate initial ray position
        double xRay = 5;
        double yRay = 5;

        // Calculate initial ray grid position
        int xGrid = (int)xRay;
        int yGrid = (int)yRay;

        // Calculate distance to next grid
        double xDistanceToNextGrid = 0;
        double yDistanceToNextGrid = 0;

        if (xStep != 0) {
            xDistanceToNextGrid = xStep > 0 ? 1 - (xRay - xGrid) : xRay - xGrid;
        }
        if (yStep != 0) {
            yDistanceToNextGrid = yStep > 0 ? 1 - (yRay - yGrid) : yRay - yGrid;
        }

        // Calculate distance to next grid
        double xDistanceToNextGrid = 0;
        double yDistanceToNextGrid = 0;

        if (xStep != 0) {
            xDistanceToNextGrid = xStep > 0 ? 1 - (xRay - xGrid) : xRay - xGrid;
        }
        if (yStep != 0) {
            yDistanceToNextGrid = yStep > 0 ? 1 - (yRay - yGrid) : yRay - yGrid;
        }

        // Calculate distance to next grid
        double xDistanceToNextGrid = 0;
        double yDistanceToNextGrid = 0;

        if (xStep != 0) {
            xDistanceToNextGrid = xStep > 0 ? 1 - (xRay - xGrid) : xRay - xGrid;
        }
        if (yStep != 0) {
            yDistanceToNextGrid = yStep > 0 ? 1 - (yRay - yGrid) : yRay - yGrid;
        }
    }
}