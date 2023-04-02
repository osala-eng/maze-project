#include "common.hpp"
#include "constants.hpp"

namespace maze
{
  /**
   * resizeTexture - resize the texture
   * @texture: The texture to resize.
   * @size: The size of the texture.
   * Return: void
  */
  void resizeTexture(std::vector<Uint32>* texture, const int size)
  {
    for (int i = 0; i < size; ++i)
    {
      texture[i].resize(texWidth * texHeight);
    }
  }

}