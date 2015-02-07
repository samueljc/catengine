#include "D3D11VertexTypes.h"

namespace catengine {
namespace D3D11 {
namespace VertexTypes {

// Vertex struct holding position and color information.
const D3D11_INPUT_ELEMENT_DESC PositionColor::input_elements[] =
{
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(PositionColor) == 28, "Vertex struct/layout mismatch");


// Vertex struct holding position and texture information.
const D3D11_INPUT_ELEMENT_DESC PositionTexture::input_elements[] =
{
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(PositionTexture) == 20, "Vertex struct/layout mismatch");


// Vertex struct holding position, texture, and color information.
const D3D11_INPUT_ELEMENT_DESC PositionTextureColor::input_elements[] =
{
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(PositionTextureColor) == 36, "Vertex struct/layout mismatch");
}
}
}