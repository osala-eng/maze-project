/**
 * @file maze.cpp
 * @brief Maze implementation.
 * @author Jashon Osala
 * @version 1.0
 * @date 2023-01-04
 */

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include "lib/quickcg.h"

using namespace QuickCG;

#define SCREEN_WIDTH 1360
#define SCREEN_HEIGHT 720
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

// #define GEN_TEXTURES

// World map
int worldMap[mapWidth][mapHeight] =
    {
        {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 6, 4, 4, 6, 4, 6, 4, 4, 4, 6, 4},
        {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
        {8, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
        {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 6, 6, 6, 0, 6, 4, 6},
        {8, 8, 8, 8, 0, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 4, 6, 0, 0, 0, 0, 0, 6},
        {7, 7, 7, 7, 0, 7, 7, 7, 7, 0, 8, 0, 8, 0, 8, 0, 8, 4, 0, 4, 0, 6, 0, 6},
        {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 0, 0, 0, 0, 0, 6},
        {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 0, 0, 0, 0, 4},
        {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 6, 0, 6, 0, 6},
        {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 4, 6, 0, 6, 6, 6},
        {7, 7, 7, 7, 0, 7, 7, 7, 7, 8, 8, 4, 0, 6, 8, 4, 8, 3, 3, 3, 0, 3, 3, 3},
        {2, 2, 2, 2, 0, 2, 2, 2, 2, 4, 6, 4, 0, 0, 6, 0, 6, 3, 0, 0, 0, 0, 0, 3},
        {2, 2, 0, 0, 0, 0, 0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
        {2, 0, 0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 4, 4, 4, 6, 0, 6, 3, 3, 0, 0, 0, 3, 3},
        {2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2, 6, 6, 0, 0, 5, 0, 5, 0, 5},
        {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
        {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 5, 0, 5, 0, 5},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
        {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 5, 0, 5, 0, 5},
        {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
        {2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5}};

struct Sprite
{
  double x, y;
  int texture;
};

#define NUM_SPRITES 19

Sprite sprite[NUM_SPRITES] =
  {
    /** Section 1*/
    {20.5, 11.5, 10},

    /** Section 2*/
    {18.5,4.5, 10},
    {10.0,4.5, 10},
    {10.0,12.5,10},
    {3.5, 6.5, 10},
    {3.5, 20.5,10},
    {3.5, 14.5,10},
    {14.5,20.5,10},

    /** Section 3*/
    {18.5, 10.5, 9},
    {18.5, 11.5, 9},
    {18.5, 12.5, 9},

    /** Section 4*/
    {21.5, 1.5, 8},
    {15.5, 1.5, 8},
    {16.0, 1.8, 8},
    {16.2, 1.2, 8},
    {3.5,  2.5, 8},
    {9.5, 15.5, 8},
    {10.0, 15.1,8},
    {10.5, 15.8,8},
};

Uint32 buffer[SCREEN_HEIGHT][SCREEN_WIDTH]; /* H ==> W*/

/* 1D Zbuffer*/
double ZBuffer[SCREEN_WIDTH];

/* Arrays for sorting sprites*/
int spriteOrder[NUM_SPRITES];
double spriteDistance[NUM_SPRITES];

/* Functions for sorting the sprites */
void sortSprites(int* order, double *dist, int amount);

int main(int ac, char **av, char **env)
{
  double posX = 22.0, posY = 11.5;    // x and y start position
  double dirX = -1.0, dirY = 0.0;     // initial direction vector
  double planeX = 0.0, planeY = 0.66; // the 2d raycaster version of camera plane

  double time = 0;    // time of current frame
  double oldTime = 0; // time of previous frame

  std::vector<Uint32> texture[11];
  for (int i = 0; i < 11; i++)
    texture[i].resize(texWidth * texHeight);

  screen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, "The Maze 1");

// Generate textures
#ifdef GEN_TEXTURES
  for (int x = 0; x < texWidth; ++x)
  {
    for (int y = 0; y < texHeight; ++y)
    {
      int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
      // int xcolor = x * 256 / texWidth;
      int ycolor = y * 256 / texHeight;
      int xycolor = y * 128 / texHeight + x * 128 / texWidth;
      texture[0][texWidth * y + x] = 65536 * 254 * (x != y && x != texWidth - y);  // flat red texture with black cross
      texture[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor;    // sloped greyscale
      texture[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor;              // sloped yellow gradient
      texture[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; // xor greyscale
      texture[4][texWidth * y + x] = 256 * xorcolor;                               // xor green
      texture[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16);             // red bricks
      texture[6][texWidth * y + x] = 65536 * ycolor;                               // red gradient
      texture[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128;                // flat grey texture

      /* Sprite textures*/
      texture[8][texWidth * y + x] = 65536 * 254 * (x != y && x != texWidth - y);  // flat red texture with black cross
      texture[9][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor;    // sloped greyscale
      texture[10][texWidth * y + x] = 256 * xycolor + 65536 * xycolor;             // sloped yellow gradient
    }
  }
#else
  // generate some textures
  unsigned long tw, th, error = 0;
  error |= loadImage(texture[0], tw, th, "pics/bluestone.png");
  error |= loadImage(texture[1], tw, th, "pics/wood.png");
  error |= loadImage(texture[2], tw, th, "pics/wood.png");
  error |= loadImage(texture[3], tw, th, "pics/wood.png");
  error |= loadImage(texture[4], tw, th, "pics/wood.png");
  error |= loadImage(texture[5], tw, th, "pics/wood.png");
  error |= loadImage(texture[6], tw, th, "pics/wood.png");
  error |= loadImage(texture[7], tw, th, "pics/wood.png");
  if(error) {
    std::cout << "Error loading textures" << std::endl;
    return 1;
  }

  /* Sprite textures*/
  error |= loadImage(texture[8], tw, th, "pics/barrel.png");
  error |= loadImage(texture[9], tw, th, "pics/pillar.png");
  error |= loadImage(texture[10], tw, th, "pics/lights.png");
  if(error) {
    std::cout << "Error loading sprite textures" << std::endl;
    return 1;
  }
#endif

  // Main loop
  while (!done())
  {

    /**
     * Floor Casting
     */
    for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; y++)
    {
      // Current y position compared to the center of the screen (the horizon)
      int p = y - SCREEN_HEIGHT / 2;

      // Vertical position of the camera.
      double posZ = 0.5 * SCREEN_HEIGHT;

      // Horizontal distance from the camera to the floor for the current row.
      double rowDistance = posZ / p;

      // calculate the real world step vector we have to add for each x (parallel to camera plane)
      // adding step by step avoids multiplications with a weight in the inner loop
      double floorStepX = rowDistance * (dirY) / SCREEN_WIDTH;
      double floorStepY = rowDistance * (-dirX) / SCREEN_WIDTH;

      // real world coordinates of the leftmost column. This will be updated as we step to the right.
      double floorX = posX + rowDistance * dirX;
      double floorY = posY + rowDistance * dirY;

      for (int x = 0; x < SCREEN_WIDTH; x++)
      {
        // the cell coord is simply got from the integer parts of floorX and floorY
        int cellX = int(floorX);
        int cellY = int(floorY);

        // get the texture coordinate from the fractional part
        int tx = int(texWidth * (floorX - cellX)) & (texWidth - 1);
        int ty = int(texHeight * (floorY - cellY)) & (texHeight - 1);

        floorX += floorStepX;
        floorY += floorStepY;

        // choose texture and draw the pixel
        int floorTexture = 3;
        int ceilingTexture = 6;
        Uint32 floorColor = texture[floorTexture][texWidth * ty + tx];
        Uint32 ceilingColor = texture[ceilingTexture][texWidth * ty + tx];

        // floor
        buffer[y][x] = floorColor;
        //ceiling (symmetrical!)
        buffer[SCREEN_HEIGHT - y][x] = ceilingColor;
      }
    }

    /**
     * Wall Casting
    */
    for (int x = 0; x < w; x++)
    {
      // Calculate ray position and direction
      double cameraX = 2 * x / double(w) - 1; // x-coordinate in camera space
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;

      // Which box of the map we're in
      int mapX = int(posX);
      int mapY = int(posY);

      // Length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

      // Length of ray from one x or y-side to next x or y-side
      double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
      double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
      double perpWallDist;

      // What direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; // was there a wall hit?
      int side;    // was a NS or a EW wall hit?

      // Calculate step and initial sideDist
      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }

      // Perform DDA
      while (!hit)
      {
        // Jump to next map square, OR in x-direction, OR in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        // Check if ray has hit a wall
        if (worldMap[mapX][mapY] > 0)
          hit = 1;
      }

      // TODO: Fix fisheye effect 191
      //  Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
      if (side == 0)
        perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
      else
        perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

      // Calculate height of line to draw on screen
      int lineHeight = (int)(h / perpWallDist);

      // Calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + h / 2;
      if (drawStart < 0)
        drawStart = 0;
      int drawEnd = lineHeight / 2 + h / 2;
      if (drawEnd >= h)
        drawEnd = h - 1;

      // Texturing calculations
      int texNum = worldMap[mapX][mapY] - 1; // 1 subtracted from it so that texture 0 can be used!

      // Calculate value of wallX
      double wallX; // where exactly the wall was hit
      if (side == 0)
        wallX = posY + perpWallDist * rayDirY;
      else
        wallX = posX + perpWallDist * rayDirX;
      wallX -= floor((wallX));

      // x coordinate on the texture
      int texX = int(wallX * double(texWidth));
      if (side == 0 && rayDirX > 0)
        texX = texWidth - texX - 1;
      if (side == 1 && rayDirY < 0)
        texX = texWidth - texX - 1;

      // How much to increase the texture coordinate per screen pixel
      double step = 1.0 * texHeight / lineHeight;
      // Starting texture coordinate
      double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
      for (int y = drawStart; y < drawEnd; y++)
      {
        // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
        int texY = int(texPos) & (texHeight - 1);
        texPos += step;
        Uint32 color = texture[texNum][texHeight * texY + texX];
        // Make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        if (side == 1)
          color = (color >> 1) & 8355711;
        buffer[y][x] = color;
      }

      /* Set the ZBuffer for casting sprite */
      ZBuffer[x] = perpWallDist; /* perpendicular distance is used */
    }

    /**
     * Sprite Casting
     * Sort sprites from far to close
    */
    for (int i = 0; i < NUM_SPRITES; i++)
    {
      spriteOrder[i] = i;
      spriteDistance[i] = ((posX - sprite[i].x) * (posX - sprite[i].x) + (posY - sprite[i].y) * (posY - sprite[i].y)); // sqrt not taken, unneeded
    }
    sortSprites(spriteOrder, spriteDistance, NUM_SPRITES);

    /* After sorting the sprites, do the projection and draw them */
    for (int i = 0; i < NUM_SPRITES; i++)
    {
      // translate sprite position to relative to camera
      double spriteX = sprite[spriteOrder[i]].x - posX;
      double spriteY = sprite[spriteOrder[i]].y - posY;

      // transform sprite with the inverse camera matrix
      // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
      // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
      // [ planeY   dirY ]                                          [ -planeY  planeX ]

      double invDet = 1.0 / (planeX * dirY - dirX * planeY); // required for correct matrix multiplication

      double transformX = invDet * (dirY * spriteX - dirX * spriteY);
      double transformY = invDet * (-planeY * spriteX + planeX * spriteY); // this is actually the depth inside the screen, that what Z is in 3D

      int spriteScreenX = int((w / 2) * (1 + transformX / transformY));

      // calculate height of the sprite on screen
      int spriteHeight = abs(int(h / (transformY))); // using "transformY" instead of the real distance prevents fisheye
      // calculate lowest and highest pixel to fill in current stripe
      int drawStartY = -spriteHeight / 2 + h / 2;
      if (drawStartY < 0)
        drawStartY = 0;
      int drawEndY = spriteHeight / 2 + h / 2;
      if (drawEndY >= h)
        drawEndY = h - 1;

      // calculate width of the sprite
      int spriteWidth = abs(int(h / (transformY)));
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if (drawStartX < 0)
        drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if (drawEndX >= w)
        drawEndX = w - 1;

      // loop through every vertical stripe of the sprite on screen
      for (int stripe = drawStartX; stripe < drawEndX; stripe++)
      {
        int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
        // the conditions in the if are:
        // 1) it's in front of camera plane so you don't see things behind you
        // 2) it's on the screen (left)
        // 3) it's on the screen (right)
        // 4) ZBuffer, with perpendicular distance
        if (transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe])
          for (int y = drawStartY; y < drawEndY; y++) // for every pixel of the current stripe
          {
            int d = (y) * 256 - h * 128 + spriteHeight * 128; // 256 and 128 factors to avoid floats
            int texY = ((d * texHeight) / spriteHeight) / 256;
            Uint32 color = texture[sprite[spriteOrder[i]].texture][texWidth * texY + texX]; // get current color from the texture
            if ((color & 0x00FFFFFF) != 0)
              buffer[y][stripe] = color;
          }
      }
    }

    drawBuffer(buffer[0]);
    
    /* Timing input for FPS counter */
    oldTime = time;
    time = SDL_GetTicks();
    double frameTime = (time - oldTime) / 1000.0; // frameTime is the time this frame has taken, in seconds
    print(1.0 / frameTime);                       // FPS counter
    redraw();

    // Speed modifiers
    double moveSpeed = frameTime * 5.0; // the constant value is in squares/second
    double rotSpeed = frameTime * 3.0;  // the constant value is in radians/second

    readKeys();
    // Move forward if no wall in front of you
    if (keyDown(SDLK_UP) || keyDown(SDLK_w)) // move using arrow up or w key
    {
      if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false)
        posX += dirX * moveSpeed;
      if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false)
        posY += dirY * moveSpeed;
    }

    // Move backwards if no wall behind you
    if (keyDown(SDLK_DOWN) || keyDown(SDLK_s)) // move using arrow down or s key
    {
      if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false)
        posX -= dirX * moveSpeed;
      if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false)
        posY -= dirY * moveSpeed;
    }

    // Rotate to the right
    if (keyDown(SDLK_RIGHT) || keyDown(SDLK_d)) // rotate to the right using arrow right or d key
    {
      // both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }

    // Rotate to the left
    if (keyDown(SDLK_LEFT) || keyDown(SDLK_a)) // rotate to the left using arrow left or a key
    {
      // both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }

    // Close window if escape key is pressed
    if (keyDown(SDLK_ESCAPE))
      break;
  }
}

/* Sort prites based on distance */
void sortSprites(int *order, double *dist, int amount)
{
  std::vector<std::pair<double, int>> sprites(amount);
  for (int i = 0; i < amount; i++)
  {
    sprites[i].first = dist[i];
    sprites[i].second = order[i];
  }
  std::sort(sprites.begin(), sprites.end());

  /* Restore in reverse to go from futhest to nearest */
  for (int i = 0; i < amount; i++)
  {
    order[i] = sprites[amount - i - 1 ].second;
    dist[i] = sprites[amount - i - 1 ].first;
  }
}
