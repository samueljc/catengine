#include "D3D11GeometryEffect.h"

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
  buffer.world = DirectX::XMMatrixIdentity();

  DirectX::XMMATRIX ortho = DirectX::XMMatrixOrthographicOffCenterLH(0.f, 600.f, 0.f, 400.f, 0.1f, 100.f);
  
  DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
    DirectX::XMVectorSet(0.f, 0.f, -10.f, 0.f), 
    DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f),
    DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));

  buffer.world = DirectX::XMMatrixTranspose(buffer.world);
  buffer.view = DirectX::XMMatrixTranspose(view);
  buffer.proj = DirectX::XMMatrixTranspose(ortho);

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