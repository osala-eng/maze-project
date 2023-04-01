/**
 * The maze project is a simple maze game that uses raycasting to render the
 * walls.
 * This project is built on top of SDL2 and uses C++17.
 *
 * GitHub: https://github.com/osala-eng/maze-project
 * Author: Jashon Osala
 * c 2023
 */

#ifndef __THEMAZE_HPP__
#define __THEMAZE_HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
#include <map>

#define __local__ __attribute__((weak))

namespace maze
{
  /**
   * Quick templates to get everything up and running.
   */

  template <typename T>
  const T template_abs(const T &value)
  {
    return (value < 0) ? -value : value;
  }

  /**
   * Usage: valtostr(value);
   * example: std::string str = valtostr(123);
   */
  template <typename T>
  std::string valtostr(const T &value)
  {
    std::string stream;
    stream << value;
    return stream.str();
  }

  /**
   * Usage: strtoval<T>(stream);
   * example: int val = strtoval<int>("123");
   */
  template <typename T>
  T strtoval(const std::string &stream)
  {
    T value;
    std::istringstream sstream(stream);
    sstream >> value;
    return value;
  }

  /**
   * Usage: valtostr(value, length);
   * example: std::string str = valtostr(123, 5);
   */
  template <typename T>
  std::string valtostr(const T &value, int length, bool fixed = true)
  {
    std::ostringstream sstream;
    if (fixed)
      sstream << std::fixed;
    sstream << std::setprecision(length) << value;
    return sstream.str();
  }

  /////////////////////////////////////////////////////////////////////////////
  /**
   * COLOR STRUCTS
   */

  struct ColorRGB8bit;
  struct ColorRGB
  {
    int r;
    int g;
    int b;
    ColorRGB(Uint8 r, Uint8 g, Uint8 b);
    ColorRGB(const ColorRGB8bit &color);
    ColorRGB();
  };

  ColorRGB operator+(const ColorRGB &color1, const ColorRGB &color2);
  ColorRGB operator-(const ColorRGB &color1, const ColorRGB &color2);
  ColorRGB operator*(const ColorRGB &color1, int a);
  ColorRGB operator*(int a, const ColorRGB &color1);
  ColorRGB operator/(const ColorRGB &color1, int a);
  bool operator==(const ColorRGB &color1, const ColorRGB &color2);
  bool operator!=(const ColorRGB &color1, const ColorRGB &color2);

  static const ColorRGB RGB_Black = ColorRGB(0, 0, 0);
  static const ColorRGB RGB_White = ColorRGB(255, 255, 255);
  static const ColorRGB RGB_Red = ColorRGB(255, 0, 0);
  static const ColorRGB RGB_Green = ColorRGB(0, 255, 0);
  static const ColorRGB RGB_Blue = ColorRGB(0, 0, 255);
  static const ColorRGB RGB_Yellow = ColorRGB(255, 255, 0);
  static const ColorRGB RGB_Cyan = ColorRGB(0, 255, 255);
  static const ColorRGB RGB_Magenta = ColorRGB(255, 0, 255);
  static const ColorRGB RGB_Gray = ColorRGB(128, 128, 128);
  static const ColorRGB RGB_Gray50 = ColorRGB(128, 128, 128);
  static const ColorRGB RGB_Gray25 = ColorRGB(64, 64, 64);
  static const ColorRGB RGB_Maroon = ColorRGB(128, 0, 0);
  static const ColorRGB RGB_DarkGreen = ColorRGB(0, 128, 0);
  static const ColorRGB RGB_Navy = ColorRGB(0, 0, 128);
  static const ColorRGB RGB_Olive = ColorRGB(128, 128, 0);
  static const ColorRGB RGB_Teal = ColorRGB(0, 128, 128);
  static const ColorRGB RGB_Purple = ColorRGB(128, 0, 128);
  static const ColorRGB RGB_Silver = ColorRGB(192, 192, 192);
  static const ColorRGB RGB_Lime = ColorRGB(0, 255, 0);
  static const ColorRGB RGB_Orange = ColorRGB(255, 165, 0);
  static const ColorRGB RGB_Aqua = ColorRGB(0, 255, 255);
  static const ColorRGB RGB_Rose = ColorRGB(255, 0, 255);
  static const ColorRGB RGB_Amber = ColorRGB(255, 191, 0);
  static const ColorRGB RGB_Turquoise = ColorRGB(64, 224, 208);
  static const ColorRGB RGB_Violet = ColorRGB(238, 130, 238);
  static const ColorRGB RGB_Azure = ColorRGB(240, 255, 255);
  static const ColorRGB RGB_Beige = ColorRGB(245, 245, 220);
  static const ColorRGB RGB_Brown = ColorRGB(165, 42, 42);

  /**
   * Color with 8 bit per channel.
   * RGB stands for Red, Green, Blue.
   */
  struct ColorRGB8bit
  {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    ColorRGB8bit(Uint8 r, Uint8 g, Uint8 b);
    ColorRGB8bit(const ColorRGB &color);
    ColorRGB8bit();
  };

  /**
   * Color with 8 bit per channel.
   * HSL stands for Hue, Saturation, Lightness.
   */
  struct ColorHSL
  {
    int h;
    int s;
    int l;
    ColorHSL(Uint8 h, Uint8 s, Uint8 l);
    ColorHSL();
  };

  /**
   * Color with 8 bit per channel.
   * HSV stands for Hue, Saturation, Value.
   */
  struct ColorHSV
  {
    int h;
    int s;
    int v;
    ColorHSV(Uint8 h, Uint8 s, Uint8 v);
    ColorHSV();
  };

  //////////////////////////////////////////////////////////////////////////////////

  /**
   * Global variables.
   */

  extern int w;
  extern int h;

  std::map<int, bool> key_pressed; // Key pressed -- Detect key press only once.
  SDL_Window *scr;                 // Screen surface.
  Uint8 *inkeys = 0;
  SDL_Event event = {0};

  /////////////////////////////////////////////////////////////////////////////

  /**
   * Keyboard functions.
   */

  /**
   * KeyPressed - Checks if the key is **just** pressed.
   * Return: true only once until the key is released.
   */
  __local__ bool KeyPressed(int key)
  {
    if (!inkeys)
      return false;
    if (key_pressed.find(key) == key_pressed.end())
      key_pressed[key] = false;

    if (inkeys[key])
    {
      if (!key_pressed[key])
      {
        key_pressed[key] = true;
        return true;
      }
    }
    else
    {
      key_pressed[key] = false;
    }
    return false;
  }

  /**
   * KeyDown - Checks if the key is held down.
   * Return: true if the key is held down until its released.
   */
  __local__ bool KeyDown(int key)
  {
    if (!inkeys)
      return false;
    return (inkeys[key] != 0);
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
   * Basic Screen functions.
   */

  /**
   * lock - Lock the screen.
   */
  __local__ void lock()
  {
    // SDL_LockSurface(scr);
  }

  /**
   * screen - Initialize the screen.
   */
  __local__ void screen(int width, int height, bool fullscreen, const std::string &title)
  {
    int colorDepth = 32;
    w = width;
    h = height;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
      SDL_Quit();
      std::exit(1);
    }
    std::atexit(SDL_Quit);
    if (fullscreen)
    {
      scr = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FULLSCREEN_DESKTOP);
      lock();
    }
    else
    {
      scr = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    }

    if (scr == NULL)
    {
      std::cerr << "SDL_SetVideoMode failed: " << SDL_GetError() << std::endl;
      SDL_Quit();
      std::exit(1);
    }
  }
}


#endif // __THEMAZE_HPP__