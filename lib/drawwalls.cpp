#include "drawwalls.hpp"

// Draw walls using the raycasting algorithm

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

                // Check for boundary
                std::vector<std::pair<double, double>> p;
                for (int tx = 0; tx < 2; tx++)
                {
                    for (int ty = 0; ty < 2; ty++)
                    {
                        double vy = (double)testY + ty - eyeY * distanceToWall;
                        double vx = (double)testX + tx - eyeX * distanceToWall;
                        double d = sqrt(vx * vx + vy * vy);
                        double dot = (eyeX * vx / d) + (eyeY * vy / d);
                        p.push_back(std::make_pair(d, dot));
                    }
                }

                // Sort pairs from closest to farthest
                std::sort(p.begin(), p.end(), [](const std::pair<double, double> &left, const std::pair<double, double> &right) {
                    return left.first < right.first;
                });

                double bound = 0.01;
                if (acos(p.at(0).second) < bound)
                    hitBoundary = true;
                if (acos(p.at(1).second) < bound)
                    hitBoundary = true;
                if (acos(p.at(2).second) < bound)
                    hitBoundary = true;
            }
        }

        // Calculate distance to ceiling and floor
        int ceiling = (double)(HEIGHT / 2) - HEIGHT / ((double)distanceToWall);
        int floor = HEIGHT - ceiling;

        // Shade walls based on distance
        double b = 1.0 - (distanceToWall / (TILE_SIZE * 5));

        // Choose wall color
        if (distanceToWall <= TILE_SIZE * 2)
            wallType = 0;
        else if (distanceToWall < TILE_SIZE * 3)
            wallType = 1;
        else if (distanceToWall < TILE_SIZE * 4)
            wallType = 2;
        else
            wallType = 3;

        // Draw the columns of the screen
        for (int y = 0; y < HEIGHT; y++)
        {
            if (y < ceiling)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
            else if (y > ceiling && y <= floor)
            {
                int textureX = (int)(256 * (y - ceiling) / (floor - ceiling));
                if (hitBoundary)
                    textureX = 256 - textureX - 1;
                int textureY = (int)(distanceToWall * 256 / TILE_SIZE);
                Uint32 color = getPixel(wallTextures[wallType], textureX, textureY);
                Uint8 r, g, b;
                SDL_GetRGB(color, wallTextures[wallType]->format, &r, &g, &b);
                r *= b;
                g *= b;
                b *= b;
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
            else
            {
                // Shade floor based on distance
                double b = 1.0 - (distanceToWall / (TILE_SIZE * 5));
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

__local__ void initSDL()  
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    loadWallTextures();
}

__local__ void handleEvents()  
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

__local__ void clean()  
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

__local__ void gameLoop()  
{
    while (isRunning)
    {
        handleEvents();
        render();
    }
}

__local__ void main()  
{
    initSDL();
    gameLoop();
    clean();
}

// int main()  
// {  
//     main();  
//     return 0;  
// }