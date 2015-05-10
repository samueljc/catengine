#ifndef D3D11_GEOMETRY_EFFECT_H
#define D3D11_GEOMETRY_EFFECT_H

#include <DirectXMath.h>

#include "ID3D11Effect.h"
#include "D3D11VertexTypes.h"
#include "Camera.h"

namespace catengine {
namespace D3D11 {

struct GeometryTraits {
  typedef VertexTypes::PositionColor InputLayout;
  typedef struct ConstantBufferType {
    DirectX::XMMATRIX wvp;
  } ConstantBufferType;
};

class GeometryEffect : public IEffect<GeometryTraits> {
public:
  GeometryEffect() :
    IEffect()
  {
  }

  RESULTS initialize(ID3D11Device* device);
  ShaderByteCode const& get_vertex_shader_byte_code() const override;
  void update_camera(Camera const& camera);

private:
  RESULTS create_vertex_shader(ID3D11Device* device);
  RESULTS create_pixel_shader(ID3D11Device* device);
  RESULTS create_constants_buffer(ID3D11Device* device);
  RESULTS create_input_layout(ID3D11Device* device);

private:
  static const ShaderByteCode vertex_shader_byte_codes[];
  static const ShaderByteCode pixel_shader_byte_codes[];
};
}
}

#endif