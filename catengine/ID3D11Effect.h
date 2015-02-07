#ifndef ID3D11_EFFECT_H
#define ID3D11_EFFECT_H

#include <comdef.h>
#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")

#include "Results.h"
#include "Logging.h"

#include <vector>

namespace catengine {
namespace D3D11 {
struct ShaderByteCode
{
  void const* code;
  size_t length;
};

template <typename T>
class IEffect {
public:
  typedef typename T::InputLayout InputLayout;
  typedef typename T::ConstantBufferType ConstantBuffer;

  IEffect() :
    layout_(nullptr),
    vs_(nullptr),
    ps_(nullptr),
    constants_(nullptr),
    initialized_(false)
  {
  }

  virtual ~IEffect() = 0
  {
    if (layout_ != nullptr) {
      layout_->Release();
      layout_ = nullptr;
    }
    if (vs_ != nullptr) {
      vs_->Release();
      vs_ = nullptr;
    }
    if (ps_ != nullptr) {
      ps_->Release();
      ps_ = nullptr;
    }
    if (constants_ != nullptr) {
      constants_->Release();
      constants_ = nullptr;
    }
  }

  virtual RESULTS initialize(ID3D11Device* device) = 0;
  virtual RESULTS apply(ID3D11DeviceContext* dev_context) const
  {
    if (!initialized_) {
      LOG(ERROR) << "cannot apply effect: effect has not been initialized";
      return RESULTS::FAILURE;
    }
    dev_context->IASetInputLayout(layout_);
    dev_context->VSSetShader(vs_, nullptr, 0);
    dev_context->PSSetShader(ps_, nullptr, 0);
    dev_context->VSSetConstantBuffers(0, 1, &constants_);
    dev_context->PSSetConstantBuffers(0, 1, &constants_);
    return RESULTS::SUCCESS;
  }
  virtual ShaderByteCode const& get_vertex_shader_byte_code() const = 0;

protected:

protected:
  ID3D11InputLayout* layout_;
  ID3D11VertexShader* vs_;
  ID3D11PixelShader* ps_;
  ID3D11Buffer* constants_;

  // boolean to say that we're initialized so we don't have to check each of our pointers to make
  // sure it isn't null
  bool initialized_;
};
/*
RESULTS create_input_layout_from_vertex_shader_signature(ShaderByteCode const& byte_code,
  ID3D11Device* device,
  ID3D11InputLayout** layout)
{
  HRESULT hr;
  ID3D11ShaderReflection* reflection = nullptr;
  if ((hr = D3DReflect(byte_code.code, byte_code.length, IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&reflection))) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "could not perform reflection on vertex shader: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  D3D11_SHADER_DESC shader_desc;
  if ((hr = reflection->GetDesc(&shader_desc)) != S_OK) {
    reflection->Release();
    _com_error err(hr);
    LOG(ERROR) << "could not get shader description: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  D3D11_SIGNATURE_PARAMETER_DESC param_desc;
  std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout_desc;
  for (unsigned i = 0u; i < shader_desc.InputParameters; ++i) {
    reflection->GetInputParameterDesc(i, &param_desc);

    D3D11_INPUT_ELEMENT_DESC element_desc;
    element_desc.SemanticName = param_desc.SemanticName;
    element_desc.SemanticIndex = param_desc.SemanticIndex;
    element_desc.InputSlot = 0u;
    element_desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    element_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    element_desc.InstanceDataStepRate = 0u;

    if (param_desc.Mask == 1) {
      if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        element_desc.Format = DXGI_FORMAT_R32_UINT;
      } else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        element_desc.Format = DXGI_FORMAT_R32_SINT;
      } else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        element_desc.Format = DXGI_FORMAT_R32_FLOAT;
      }
    } else if (param_desc.Mask <= 3) {
      if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        element_desc.Format = DXGI_FORMAT_R32G32_UINT;
      } else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        element_desc.Format = DXGI_FORMAT_R32G32_SINT;
      } else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        element_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
      }
    } else if (param_desc.Mask <= 7) {
      if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        element_desc.Format = DXGI_FORMAT_R32G32B32_UINT;
      } else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        element_desc.Format = DXGI_FORMAT_R32G32B32_SINT;
      } else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        element_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
      }
    } else if (param_desc.Mask <= 15) {
      if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
        element_desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
      } else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
        element_desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
      } else if (param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
        element_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      }
    }

    input_layout_desc.emplace_back(element_desc);
  }

  hr = device->CreateInputLayout(input_layout_desc.data(),
    input_layout_desc.size(),
    byte_code.code,
    byte_code.length,
    layout);
  if (hr != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "could not create input layout from vertex buffer: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  reflection->Release();
  return RESULTS::SUCCESS;
}

RESULTS create_constants_buffer_from_vertex_shader_signature(ShaderByteCode const& byte_code,
  ID3D11Device* device)
{
  HRESULT hr;
  ID3D11ShaderReflection* reflection = nullptr;
  if ((hr = D3DReflect(byte_code.code, byte_code.length, IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&reflection))) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "could not perform reflection on vertex shader: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  ID3D11ShaderReflectionConstantBuffer* buffer = reflection->GetConstantBufferByIndex(0);
  D3D11_SHADER_BUFFER_DESC desc;
  if ((hr = buffer->GetDesc(&desc)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "could not get buffer description: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  // todo: complete this if I feel like it; I have no intention of using it anytime soon, so it doesn't matter too much

  return RESULTS::SUCCESS;
}*/
}
}

#endif