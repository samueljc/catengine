#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

#include "Types.h"
#include "Rectangle.h"
#include "Color.h"
#include "Texture.h"

namespace catengine {
class SpriteBatch {
public:
  struct SpriteInfo {
    Rectangle source;
    Rectangle destination;
    Color color;
    Texture* texture;
  };

private:
  void create_index_buffer(ID3D11Device* device);

private:
  static const _unsigned max_batch_size = 2048;
  static const _unsigned min_batch_size = 128;
  static const _unsigned vertices_per_sprite = 4;
  static const _unsigned indeces_per_sprite = 6;

  SpriteBatch(SpriteBatch const&) = delete;
  SpriteBatch& operator=(SpriteBatch const&) = delete;
};
}

#endif