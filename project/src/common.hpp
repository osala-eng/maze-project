/**
 * @file common.hpp
 * @brief Common definitions for the maze project.
 * @author Jashon Osala
 * @version 1.0
 */

#ifndef __THE_MAZE_COMMON_H__
#define __THE_MAZE_COMMON_H__

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

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
  std::string valtostr(const T &value, const int &length)
  {
    std::string stream;
    stream << std::setw(length) << std::setfill('0') << value;
    return stream.str();
  }

  /**
   * Usage: strtoval<T>(stream, length);
   * example: int val = strtoval<int>("123", 5);
   */
  template <typename T>
  T strtoval(const std::string &stream, const int &length)
  {
    T value;
    std::istringstream sstream(stream);
    sstream >> std::setw(length) >> std::setfill('0') >> value;
    return value;
  }

  /**
   * Usage: valtostr(value, length, fill);
   * example: std::string str = valtostr(123, 5, '0');
   */
  template <typename T>
  std::string valtostr(const T &value, const int &length, const char &fill)
  {
    std::string stream;
    stream << std::setw(length) << std::setfill(fill) << value;
    return stream.str();
  }

  /**
   * Usage: strtoval<T>(stream, length, fill);
   * example: int val = strtoval<int>("123", 5, '0');
   */
  template <typename T>
  T strtoval(const std::string &stream, const int &length, const char &fill)
  {
    T value;
    std::istringstream sstream(stream);
    sstream >> std::setw(length) >> std::setfill(fill) >> value;
    return value;
  }

  /**
   * Keyboard functions
  */
  bool KeyDown(SDL_Scancode key);
  bool KeyPressed(SDL_Scancode key);

  /**
   * Screen functions
  */
  void lock();
  void unlock();
  void redraw();
  void cls(const SDL_Color &color = {0, 0, 0, 255});
  void pset(const int &x, const int &y, const SDL_Color &color = {255, 255, 255, 255});
  void drawBuffer(Uint32* buffer);
  void onScreen(int x, int y);

  /**
   * Helper functions
  */
 void sleep();
 void sleep(double seconds);
 void waitFrame (double oldTime, double frameDuration);
 bool done(bool quit_esc = true, bool delay = true);
 void end();
 void readKeys();
 void getMouseState(int& mouseX, int& mouseY);
 void getMouseState(int& mouseX, int& mouseY, bool& mouseLeft, bool& mouseRight);
 unsigned long getTicks();
 void resizeTexture(std::vector<Uint32> &texture, const int size);
 void generateStructures();

 __local__ double getTime() {
    return getTicks() / 1000.0;
 }

 int loadImage(std::vector<Uint32> &image, unsigned long &width, unsigned long &height, const std::string &filename);
 struct Sprite
{
  double x, y;
  int texture;
};

} // namespace maze

#endif // __THE_MAZE_COMMON_H__