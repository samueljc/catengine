#include "D3D11GeometryEffect.h"
#include "Matrix4x4.h"

namespace catengine {
namespace D3D11 {

#include "Resources/Shaders/Compiled/Basic_VSBasic.inc"
#include "Resources/Shaders/Compiled/Basic_PSBasic.inc"

const ShaderByteCode GeometryEffect::vertex_shader_byte_codes[] =
{
  { Basic_VSBasic, sizeof(Basic_VSBasic) },
};

const ShaderByteCode GeometryEffect::pixel_shader_byte_codes[] =
{
  { Basic_PSBasic, sizeof(Basic_PSBasic) },
};

RESULTS GeometryEffect::initialize(ID3D11Device* device)
{
  if (device == nullptr) {
    LOG(ERROR) << "cannot initialize shader pipeline: device is null";
    return RESULTS::FAILURE;
  }

  if (create_vertex_shader(device) != RESULTS::SUCCESS ||
    create_pixel_shader(device) != RESULTS::SUCCESS ||
    create_constants_buffer(device) != RESULTS::SUCCESS ||
    create_input_layout(device) != RESULTS::SUCCESS) {
    LOG(ERROR) << "failed to create geometry effect";
    return RESULTS::FAILURE;
  }
  initialized_ = true;
  LOG(SUCCESS) << "successfully initialized geometry effect";
  return RESULTS::SUCCESS;
}

RESULTS GeometryEffect::create_vertex_shader(ID3D11Device* device)
{
  auto const& byte_code = vertex_shader_byte_codes[0];
  auto hr = device->CreateVertexShader(byte_code.code, byte_code.length, nullptr, &vs_);
  if (hr != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error creating vertex shader: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  return RESULTS::SUCCESS;
}

RESULTS GeometryEffect::create_pixel_shader(ID3D11Device* device)
{
  auto const& byte_code = pixel_shader_byte_codes[0];
  auto hr = device->CreatePixelShader(byte_code.code, byte_code.length, nullptr, &ps_);
  if (hr != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error creating pixel shader: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  return RESULTS::SUCCESS;
}

RESULTS GeometryEffect::create_constants_buffer(ID3D11Device* device)
{
  HRESULT hr;
  D3D11_BUFFER_DESC buffer_desc;
  SecureZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
  buffer_desc.ByteWidth = sizeof(ConstantBuffer);
  buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
  buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  ConstantBuffer buffer;
  SecureZeroMemory(&buffer, sizeof(ConstantBuffer));

  /*
  DirectX::XMMATRIX dxworld = DirectX::XMMatrixIdentity();

  // left, right, bottom, top, nearz, farz
  // bottom > top to make the top left corner the origin
  FLOAT left = 0.f;
  FLOAT top = 0.f;
  FLOAT width = 1280.0f;
  FLOAT height = 720.f;
  // http://ezekiel.vancouver.wsu.edu/~cs442/lectures/viewing/viewing.pdf
  DirectX::XMMATRIX dxproj = DirectX::XMMatrixOrthographicOffCenterLH(left, left + width, top + height, top, 0.f, 100.f);
  
  DirectX::XMMATRIX dxview = DirectX::XMMatrixLookAtLH(
    DirectX::XMVectorSet(0.f, 0.f, -10.f, 0.f),       // camera position
    DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f),         // focus position
    DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));        // up direction
  */
  

  Matrix4x4 proj;// = Matrix4x4::create_projection_matrix(1264.f, 661.f);
  Matrix4x4 view;// = Matrix4x4::create_view_matrix();
  Matrix4x4 world;// = Matrix4x4::create_world_matrix(50.f, 50.f);

  // matrices need to be transposed for d3d11 - not for d3d10 or older though
  Matrix4x4 wvp = proj * view * world;
  memcpy_s(&buffer.wvp, sizeof(DirectX::XMMATRIX), &wvp, sizeof(Matrix4x4));

  D3D11_SUBRESOURCE_DATA data;
  SecureZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
  data.pSysMem = &buffer;

  if ((hr = device->CreateBuffer(&buffer_desc, &data, &constants_)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error creating constants buffer: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  return RESULTS::SUCCESS;
}

RESULTS GeometryEffect::create_input_layout(ID3D11Device* device)
{
  auto const& byte_code = vertex_shader_byte_codes[0];
  auto hr = device->CreateInputLayout(InputLayout::input_elements,
      InputLayout::input_element_count,
      byte_code.code,
      byte_code.length,
      &layout_);

  if (hr != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error creating layout: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  return RESULTS::SUCCESS;
}

ShaderByteCode const& GeometryEffect::get_vertex_shader_byte_code() const
{
  return vertex_shader_byte_codes[0];
}
}
}