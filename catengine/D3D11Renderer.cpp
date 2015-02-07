#include "D3D11Renderer.h"

#include <comdef.h>
#include <vector>
#include <memory>

#include "D3D11VertexTypes.h"

namespace catengine {
namespace D3D11 {

static void release_com_object(IUnknown** obj)
{
  if (*obj != nullptr) {
    (*obj)->Release();
    (*obj) = nullptr;
  }
}

RESULTS Renderer::initialize(HWND hwnd)
{
  if (stored_hwnd_ == hwnd) {
    return RESULTS::SUCCESS;
  }
  LOG(INFO) << "initializing d3d11 renderer";
  stored_hwnd_ = hwnd;

  if (collect_display_info(hwnd) == RESULTS::FAILURE) {
    LOG(WARNING) << "unable to collect video card information: continuing initialization";
  }

  if (init_swap_chain_and_device(hwnd)    != RESULTS::SUCCESS ||
      init_back_buffer()                  != RESULTS::SUCCESS ||
      init_depth_buffer()                 != RESULTS::SUCCESS ||
      init_rasterizer_state()             != RESULTS::SUCCESS ||
      init_sampler_state()                != RESULTS::SUCCESS ||
      init_blend_state()                  != RESULTS::SUCCESS ||
      init_viewport()                     != RESULTS::SUCCESS ||
      init_basic_shaders()                != RESULTS::SUCCESS ||
      init_geometry_buffers()             != RESULTS::SUCCESS ||
      init_resources()                    != RESULTS::SUCCESS) {
    dispose();
    return RESULTS::FAILURE;
  }

  LOG(SUCCESS) << "successfully initialized d3d11 renderer";
  return RESULTS::SUCCESS;
}

void Renderer::dispose()
{
  stored_hwnd_ = nullptr;
  if (swap_chain_ != nullptr) {
    BOOL fullscreen;
    swap_chain_->GetFullscreenState(&fullscreen, nullptr);
    if (fullscreen == TRUE) {
      swap_chain_->SetFullscreenState(FALSE, nullptr);
    }

    swap_chain_->Release();
    swap_chain_ = nullptr;
  }
  release_com_object(reinterpret_cast<IUnknown**>(&back_buffer_));
  release_com_object(reinterpret_cast<IUnknown**>(&depth_stencil_buffer_));
  release_com_object(reinterpret_cast<IUnknown**>(&depth_stencil_state_));
  release_com_object(reinterpret_cast<IUnknown**>(&depth_stencil_view_));
  release_com_object(reinterpret_cast<IUnknown**>(&raster_state_));
  release_com_object(reinterpret_cast<IUnknown**>(&sampler_state_));
  release_com_object(reinterpret_cast<IUnknown**>(&blend_state_));
  release_com_object(reinterpret_cast<IUnknown**>(&dev_));
  release_com_object(reinterpret_cast<IUnknown**>(&dev_context_));
  release_com_object(reinterpret_cast<IUnknown**>(&geometry_vertex_buffer_));
  release_com_object(reinterpret_cast<IUnknown**>(&geometry_index_buffer_));
}

RESULTS Renderer::swap_shader_state()
{
  if (dev_context_ == nullptr) {
    LOG(ERROR) << "cannot swap shaders: device context is null";
    return RESULTS::FAILURE;
  }

  LOG(ERROR) << "not implemented";

  return RESULTS::SUCCESS;
}

void Renderer::begin_draw()
{
  catengine::Color color(1.0f, 0.0f, 0.0f);
  geometry_vertices_staged_.clear();
  geometry_indices_staged_.clear();

  dev_context_->ClearRenderTargetView(back_buffer_, reinterpret_cast<float const*>(&color));
  dev_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::end_draw()
{
  render_geometry();

  if (vsync_enabled_) {
    swap_chain_->Present(1, 0);
  } else {
    swap_chain_->Present(0, 0);
  }
}

void Renderer::render_geometry()
{
  if (geometry_vertex_buffer_ == nullptr) {
    LOG(ERROR) << "cannot render geometry: vertex buffer is null";
    return;
  } else if (geometry_index_buffer_ == nullptr) {
    LOG(ERROR) << "cannot render geometry: index buffer is null";
    return;
  }

  dev_context_->OMSetRenderTargets(1, &back_buffer_, depth_stencil_view_);
  dev_context_->RSSetViewports(1, &viewport_);

  // states
  dev_context_->OMSetBlendState(blend_state_, nullptr, 0xffffffff);
  dev_context_->OMSetDepthStencilState(depth_stencil_state_, 1);
  dev_context_->RSSetState(raster_state_);

  // effects pipeline
  geometry_effect_.apply(dev_context_);

  // vertices / indices
  D3D11_MAPPED_SUBRESOURCE v_map;
  SecureZeroMemory(&v_map, sizeof(D3D11_MAPPED_SUBRESOURCE));
  dev_context_->Map(geometry_vertex_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &v_map);
  memcpy(v_map.pData, geometry_vertices_staged_.data(), max_geometry_vertices_);
  dev_context_->Unmap(geometry_vertex_buffer_, 0);

  D3D11_MAPPED_SUBRESOURCE i_map;
  SecureZeroMemory(&i_map, sizeof(D3D11_MAPPED_SUBRESOURCE));
  dev_context_->Map(geometry_index_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &i_map);
  memcpy(i_map.pData, geometry_indices_staged_.data(), max_geometry_indices_);
  dev_context_->Unmap(geometry_index_buffer_, 0);

  unsigned vertex_stride = sizeof(GeometryVertex);
  unsigned vertex_offset = 0;

  dev_context_->IASetVertexBuffers(0, 1, &geometry_vertex_buffer_, &vertex_stride, &vertex_offset);
  dev_context_->IASetIndexBuffer(geometry_index_buffer_, DXGI_FORMAT_R16_UINT, 0);

  dev_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  dev_context_->DrawIndexed(geometry_indices_staged_.size(), 0, 0);
}

void Renderer::draw_line(LineSegment const& line)
{
  fill_rect({
    line.start.x, line.start.y,
    line.end.x - line.start.x, line.end.y - line.start.y
  });
}

void Renderer::draw_rect(Rectangle const& rect)
{
  LOG(ERROR) << "not implemented";
  fill_rect(rect);/*
  Point2d top_left{ rect.left(), rect.top() };
  Point2d top_right{ rect.right(), rect.top() };
  Point2d bottom_left{ rect.left(), rect.bottom() };
  Point2d bottom_right{ rect.right(), rect.bottom() };
  draw_line({ top_left, top_right });
  draw_line({ top_right, bottom_right });
  draw_line({ bottom_right, bottom_left });
  draw_line({ bottom_left, top_left });*/
}

void Renderer::fill_rect(Rectangle const& rect)
{
  short start = static_cast<short>(geometry_vertices_staged_.size());
  geometry_vertices_staged_.emplace_back(Point3d{ rect.left(), rect.top(), 0.0f }, brush_color_);
  geometry_vertices_staged_.emplace_back(Point3d{ rect.right(), rect.top(), 0.0f }, brush_color_);
  geometry_vertices_staged_.emplace_back(Point3d{ rect.left(), rect.bottom(), 0.0f }, brush_color_);
  geometry_vertices_staged_.emplace_back(Point3d{ rect.right(), rect.bottom(), 0.0f }, brush_color_);

  geometry_indices_staged_.emplace_back(start);
  geometry_indices_staged_.emplace_back(start + 1);
  geometry_indices_staged_.emplace_back(start + 2);

  geometry_indices_staged_.emplace_back(start + 1);
  geometry_indices_staged_.emplace_back(start + 3);
  geometry_indices_staged_.emplace_back(start + 2);
}

void Renderer::draw_circle(Circle const& circle)
{
  LOG(ERROR) << "not implemented";
}

void Renderer::fill_circle(Circle const& circle)
{
  LOG(ERROR) << "not implemented";
}

void Renderer::resize(_unsigned x, _unsigned y)
{
  LOG(ERROR) << "not implemented";
}


UINT Renderer::get_quality_levels()
{
  if (dev_ != nullptr) {
    UINT quality_levels;
    if (dev_->CheckMultisampleQualityLevels(display_format_, sample_count_, &quality_levels) == S_OK) {
      return quality_levels;
    }
  }
  return 0;
}

RESULTS Renderer::collect_display_info(HWND const& hwnd)
{
  IDXGIFactory* factory = nullptr;
  IDXGIAdapter* adapter = nullptr;
  IDXGIOutput* adapter_output = nullptr;
  DXGI_MODE_DESC* display_mode_list = nullptr;

  RECT window_rect;
  if (GetWindowRect(hwnd, &window_rect) != TRUE) {
    LOG(INFO) << "unable to get canvas size";
    return RESULTS::FAILURE;
  } else if (window_rect.right - window_rect.left <= 0) {
    LOG(INFO) << "invalid canvas size: width must be greater than 0";
    return RESULTS::FAILURE;
  } else if (window_rect.bottom - window_rect.top <= 0) {
    LOG(INFO) << "invalid canvas size: height must be greater than 0";
    return RESULTS::FAILURE;
  }
  canvas_width_ = window_rect.right - window_rect.left;
  canvas_height_ = window_rect.bottom - window_rect.top;

  auto results = collect_display_info(factory, adapter, adapter_output, display_mode_list);

  if (factory != nullptr) {
    factory->Release();
  }
  if (adapter != nullptr) {
    adapter->Release();
  }
  if (adapter_output != nullptr) {
    adapter_output->Release();
  }
  if (display_mode_list != nullptr) {
    delete[] display_mode_list;
  }

  return results;
}

// wrapper for collecting display info so that all of the containers can be safely released by the calling
// function upon completion either by failure or success
RESULTS Renderer::collect_display_info(IDXGIFactory*& factory,
  IDXGIAdapter*& adapter,
  IDXGIOutput*& adapter_output,
  DXGI_MODE_DESC*& display_mode_list)
{
  HRESULT hr;
  if ((hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error creating DXGI factory: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  if ((hr = factory->EnumAdapters(0, &adapter)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error getting enum adapters: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  if ((hr = adapter->EnumOutputs(0, &adapter_output)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error getting enum outputs: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  unsigned num_modes;
  if ((hr = adapter_output->GetDisplayModeList(display_format_, DXGI_ENUM_MODES_INTERLACED, &num_modes, nullptr)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error getting display mode list count: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  display_mode_list = new DXGI_MODE_DESC[num_modes];
  if ((hr = adapter_output->GetDisplayModeList(display_format_, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error getting display mode list: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  for (unsigned i = 0; i < num_modes; ++i) {
    if (display_mode_list[i].Width == canvas_width_ && display_mode_list[i].Height == canvas_height_) {
      refresh_numerator_ = display_mode_list[i].RefreshRate.Numerator;
      refresh_denominator_ = display_mode_list[i].RefreshRate.Denominator;
    }
  }

  DXGI_ADAPTER_DESC adapter_desc;
  SecureZeroMemory(&adapter_desc, sizeof(DXGI_ADAPTER_DESC));
  if ((hr = adapter->GetDesc(&adapter_desc)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "error getting adapter description: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  size_t str_len;
  char m_adapter_desc[128];
  wcstombs_s(&str_len, m_adapter_desc, 128, adapter_desc.Description, 128);

  display_desc_.adapter_name = std::string(m_adapter_desc, str_len);
  display_desc_.system_memory = adapter_desc.DedicatedSystemMemory;
  display_desc_.video_memory = adapter_desc.DedicatedVideoMemory;
  display_desc_.refresh_numerator = refresh_numerator_;
  display_desc_.refresh_denominator = refresh_denominator_;

  LOG(INFO) << display_desc_;

  return RESULTS::SUCCESS;
}

RESULTS Renderer::init_swap_chain_and_device(HWND hwnd)
{
  DXGI_SWAP_CHAIN_DESC scd;
  SecureZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
  scd.BufferCount = 1;
  scd.BufferDesc.Format = display_format_;
  scd.BufferDesc.Width = canvas_width_;
  scd.BufferDesc.Height = canvas_height_;
  scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  scd.BufferDesc.RefreshRate.Numerator = refresh_numerator_;
  scd.BufferDesc.RefreshRate.Denominator = refresh_denominator_;
  scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  scd.OutputWindow = hwnd;
  scd.SampleDesc.Count = sample_count_;
  scd.SampleDesc.Quality = sample_quality_;
  scd.Windowed = windowed_;

  HRESULT r = D3D11CreateDeviceAndSwapChain(NULL,
      D3D_DRIVER_TYPE_HARDWARE,
      NULL,
      (debug_) ? D3D11_CREATE_DEVICE_DEBUG : NULL,
      NULL,
      NULL,
      D3D11_SDK_VERSION,
      &scd,
      &swap_chain_,
      &dev_,
      NULL,
      &dev_context_);

  if (r != S_OK) {
    _com_error err(r);
    LOG(ERROR) << "failed to create swap chain and device: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  LOG(SUCCESS) << "successfully initialized swap chain and device";
  return RESULTS::SUCCESS;
}

RESULTS Renderer::init_back_buffer()
{
  if (swap_chain_ == nullptr) {
    LOG(ERROR) << "cannot create back_buffer: swap chain is null";
    return RESULTS::FAILURE;
  } else if (dev_ == nullptr) {
    LOG(ERROR) << "cannot create back_buffer: device is null"; 
    return RESULTS::FAILURE;
  }

  ID3D11Texture2D* back_buffer;
  swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);

  HRESULT hr = dev_->CreateRenderTargetView(back_buffer, NULL, &back_buffer_);
  back_buffer->Release();

  if (hr != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "failed to create back_buffer: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  LOG(SUCCESS) << "successfully initialized back buffer";
  return RESULTS::SUCCESS;
}

RESULTS Renderer::init_depth_buffer()
{
  if (dev_ == nullptr) {
    LOG(ERROR) << "cannot create depth buffer: device is null";
    return RESULTS::FAILURE;
  }

  HRESULT hr;
  D3D11_TEXTURE2D_DESC db_desc;
  SecureZeroMemory(&db_desc, sizeof(D3D11_TEXTURE2D_DESC));
  db_desc.Width = canvas_width_;
  db_desc.Height = canvas_height_;
  db_desc.MipLevels = 1;
  db_desc.ArraySize = 1;
  db_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  db_desc.SampleDesc.Count = 1;
  db_desc.SampleDesc.Quality = 0;
  db_desc.Usage = D3D11_USAGE_DEFAULT;
  db_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

  if ((hr = dev_->CreateTexture2D(&db_desc, NULL, &depth_stencil_buffer_)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "failed to create depth stencil buffer: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  D3D11_DEPTH_STENCIL_DESC ds_desc;
  SecureZeroMemory(&ds_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
  ds_desc.DepthEnable = false;
  ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  ds_desc.DepthFunc = D3D11_COMPARISON_LESS;

  ds_desc.StencilEnable = true;
  ds_desc.StencilReadMask = 0xff;
  ds_desc.StencilWriteMask = 0xff;

  ds_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  ds_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  ds_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  ds_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  ds_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  ds_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  ds_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  ds_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  if ((hr = dev_->CreateDepthStencilState(&ds_desc, &depth_stencil_state_)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "failed to create depth stencil state state: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
  SecureZeroMemory(&dsv_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
  dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  dsv_desc.Texture2D.MipSlice = 0;

  if ((hr = dev_->CreateDepthStencilView(depth_stencil_buffer_, &dsv_desc, &depth_stencil_view_)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "failed to create depth stencil view: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  LOG(SUCCESS) << "successfully initialized depth stencil";
  return RESULTS::SUCCESS;
}

RESULTS Renderer::init_rasterizer_state()
{
  if (dev_ == nullptr) {
    LOG(ERROR) << "cannot create rasterizer state: device is null";
    return RESULTS::FAILURE;
  }

  HRESULT hr;
  D3D11_RASTERIZER_DESC raster_desc;
  SecureZeroMemory(&raster_desc, sizeof(D3D11_RASTERIZER_DESC));
  raster_desc.AntialiasedLineEnable = false;
  raster_desc.FillMode = D3D11_FILL_SOLID;
  raster_desc.CullMode = D3D11_CULL_BACK;
  raster_desc.DepthBias = 0;
  raster_desc.DepthBiasClamp = 0.0f;
  raster_desc.DepthClipEnable = true;
  raster_desc.FrontCounterClockwise = false;
  raster_desc.MultisampleEnable = false;
  raster_desc.ScissorEnable = false;
  raster_desc.SlopeScaledDepthBias = 0.0f;

  if ((hr = dev_->CreateRasterizerState(&raster_desc, &raster_state_)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "cannot create rasterizer state: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  LOG(SUCCESS) << "successfully initialized rasterizer state";
  return RESULTS::SUCCESS;
}

RESULTS Renderer::init_sampler_state()
{
  if (dev_ == nullptr) {
    LOG(ERROR) << "cannot create sampler state: device is null";
    return RESULTS::FAILURE;
  }

  HRESULT hr;
  D3D11_SAMPLER_DESC desc;
  SecureZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
  desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.MaxAnisotropy = (dev_->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;
  desc.MinLOD = 0.0f;
  desc.MaxLOD = FLT_MAX;
  desc.MipLODBias = 0.0f;
  desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

  if ((hr = dev_->CreateSamplerState(&desc, &sampler_state_)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "cannot create sampler state: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  LOG(SUCCESS) << "successfully initialized sampler state";
  return RESULTS::SUCCESS;
}

RESULTS Renderer::init_blend_state()
{
  return create_blend_state(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, &blend_state_);
}

RESULTS Renderer::create_blend_state(D3D11_BLEND src_blend,
  D3D11_BLEND dest_blend,
  D3D11_BLEND_OP blend_op,
  ID3D11BlendState** blend_state)
{
  if (dev_ == nullptr) {
    LOG(ERROR) << "cannot create blend state: device is null";
    return RESULTS::FAILURE;
  }

  HRESULT hr;
  D3D11_BLEND_DESC blend_desc;
  SecureZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
  blend_desc.RenderTarget[0].BlendEnable = true;
  blend_desc.RenderTarget[0].SrcBlend = blend_desc.RenderTarget[0].SrcBlendAlpha = src_blend;
  blend_desc.RenderTarget[0].DestBlend = blend_desc.RenderTarget[0].DestBlendAlpha = dest_blend;
  blend_desc.RenderTarget[0].BlendOp = blend_desc.RenderTarget[0].BlendOpAlpha = blend_op;

  blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  if ((hr = dev_->CreateBlendState(&blend_desc, blend_state)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "cannot create blend state: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }
  LOG(SUCCESS) << "successfully created blend state";
  return RESULTS::SUCCESS;
}

RESULTS Renderer::init_viewport()
{
  SecureZeroMemory(&viewport_, sizeof(D3D11_VIEWPORT));

  viewport_.TopLeftX = 0.f;
  viewport_.TopLeftY = 0.f;
  viewport_.Width = static_cast<FLOAT>(canvas_width_);
  viewport_.Height = static_cast<FLOAT>(canvas_height_);
  viewport_.MinDepth = 0.f;
  viewport_.MaxDepth = 0.f;

  LOG(SUCCESS) << "successfully initialized viewport";
  return RESULTS::SUCCESS;
}

RESULTS Renderer::init_basic_shaders()
{
  return geometry_effect_.initialize(dev_);
}

RESULTS Renderer::init_resources()
{
  return RESULTS::SUCCESS;
}

RESULTS Renderer::init_geometry_buffers()
{
  D3D11_BUFFER_DESC vbd;
  SecureZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
  vbd.Usage = D3D11_USAGE_DYNAMIC;
  vbd.ByteWidth = sizeof(GeometryVertex) * max_geometry_vertices_;
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  auto hr = dev_->CreateBuffer(&vbd, nullptr, &geometry_vertex_buffer_);
  if (hr != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "failed to create geometry vertex buffer: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  geometry_vertices_staged_.reserve(max_geometry_vertices_);

  // sizing the index buffer in relation to a rectangle or line
  max_geometry_indices_ = max_geometry_vertices_ * 6 / 4;
  D3D11_BUFFER_DESC ibd;
  SecureZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
  ibd.Usage = D3D11_USAGE_DYNAMIC;
  ibd.ByteWidth = sizeof(short) * max_geometry_indices_;
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  hr = dev_->CreateBuffer(&ibd, nullptr, &geometry_index_buffer_);
  if (hr != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "failed to create geometry index buffer: " << err.ErrorMessage();
    return RESULTS::FAILURE;
  }

  geometry_indices_staged_.reserve(max_geometry_indices_);
  LOG(SUCCESS) << "successfully initialized geometry buffers";
  return RESULTS::SUCCESS;
}
}
}