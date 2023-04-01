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

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

#define GEN_TEXTURES

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
    {12.0, 4.0, 0},

    /** Section 2*/
    {12.0, 10.0, 1},
    {12.0, 14.0, 2},
    {12.0, 18.0, 3},
    {16.0, 4.0, 4},
    {16.0, 10.0, 5},
    {16.0, 14.0, 6},
    {16.0, 18.0, 7},

    /** Section 3*/
    {20.0, 4.0, 8},
    {20.0, 10.0, 9},
    {20.0, 14.0, 10},

    /** Section 4*/
    {20.0, 18.0, 11},
    {24.0, 4.0, 12},
    {24.0, 10.0, 13},
    {24.0, 14.0, 14},
    {24.0, 18.0, 15},
    {28.0, 4.0, 16},
    {28.0, 10.0, 17},
    {28.0, 14.0, 18}
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
  error |= loadImage(texture[0], tw, th, "pics/eagle.png");
  error |= loadImage(texture[1], tw, th, "pics/redbrick.png");
  error |= loadImage(texture[2], tw, th, "pics/purplestone.png");
  error |= loadImage(texture[3], tw, th, "pics/greystone.png");
  error |= loadImage(texture[4], tw, th, "pics/bluestone.png");
  error |= loadImage(texture[5], tw, th, "pics/mossy.png");
  error |= loadImage(texture[6], tw, th, "pics/wood.png");
  error |= loadImage(texture[7], tw, th, "pics/colorstone.png");
  if(error) {
    std::cout << "Error loading textures" << std::endl;
    return 1;
  }

  /* Sprite textures*/
  error |= loadImage(texture[8], tw, th, "pics/barrel.png");
  error |= loadImage(texture[9], tw, th, "pics/pillar.png");
  error |= loadImage(texture[10], tw, th, "pics/greenlight.png");
  if(error) {
    std::cout << "Error loading sprite textures" << std::endl;
    return 1;
  }
#endif

  // Main loop
  while (!done())
  {
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

      int pitch = 100;

      // Calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + h / 2 + pitch;
      if (drawStart < 0)
        drawStart = 0;
      int drawEnd = lineHeight / 2 + h / 2 + pitch;
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
      double texPos = (drawStart - pitch - h / 2 + lineHeight / 2) * step;
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
    }

    drawBuffer(buffer[0]);
    for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
      {
        buffer[y][x] = 0; // Clear the buffer instead of cls()
      }
    }

    // Timing for input and FPS counter
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
