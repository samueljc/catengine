#include "D3D11Renderer.h"

#include <comdef.h>
#include <vector>
#include <memory>

#include "D3D11VertexTypes.h"

namespace catengine {
namespace D3D11 {

static void release_obj(IUnknown** obj)
{
  if (*obj != nullptr) {
    (*obj)->Release();
    (*obj) = nullptr;
  }
}

const D3D_FEATURE_LEVEL Renderer::feature_levels_[] = 
{
  D3D_FEATURE_LEVEL_11_1,
  D3D_FEATURE_LEVEL_11_0,
  D3D_FEATURE_LEVEL_10_1,
  D3D_FEATURE_LEVEL_10_0,
  D3D_FEATURE_LEVEL_9_3,
  D3D_FEATURE_LEVEL_9_2,
  D3D_FEATURE_LEVEL_9_1,
};

const UINT Renderer::feature_levels_size_ = sizeof(feature_levels_) / sizeof(D3D_FEATURE_LEVEL);

RESULTS Renderer::initialize(HWND hwnd)
{
  if (stored_hwnd_ == hwnd) {
    return RESULTS::SUCCESS;
  } else if (initialized_) {
    dispose();
  }

  stored_hwnd_ = hwnd;
  LOG(INFO) << "initializing d3d11 renderer";

  if (collect_display_info(hwnd) == RESULTS::FAILURE) {
    LOG(WARNING) << "unable to collect video card information: continuing initialization";
  }

  try {
    init_d3d_device();
    init_dxgi_device();
    init_d2d_device();
    init_swap_chain(hwnd);
    init_back_buffer();
    //init_depth_buffer();
    init_rasterizer_state();
    init_sampler_state();
    init_blend_state();
    init_viewport();
    init_basic_shaders();
    init_geometry_buffers();
    init_resources();
  } catch (std::exception e) {
    LOG(ERROR) << e.what();
    dispose();
    return RESULTS::FAILURE;
  }

  initialized_ = true;
  LOG(SUCCESS) << "successfully initialized d3d11 renderer";
  return RESULTS::SUCCESS;
}

void Renderer::dispose()
{
  stored_hwnd_ = nullptr;
  initialized_ = false;

  release_obj(reinterpret_cast<IUnknown**>(&dev_));
  release_obj(reinterpret_cast<IUnknown**>(&devcon_));

  release_obj(reinterpret_cast<IUnknown**>(&dxgi_dev_));
  release_obj(reinterpret_cast<IUnknown**>(&dxgi_adapter_));
  release_obj(reinterpret_cast<IUnknown**>(&dxgi_factory_));

  release_obj(reinterpret_cast<IUnknown**>(&d2d_factory_));
  release_obj(reinterpret_cast<IUnknown**>(&d2d_dev_));
  release_obj(reinterpret_cast<IUnknown**>(&d2d_devcon_));

  if (swap_chain_ != nullptr) {
    BOOL fullscreen;
    swap_chain_->GetFullscreenState(&fullscreen, nullptr);
    if (fullscreen == TRUE) {
      swap_chain_->SetFullscreenState(FALSE, nullptr);
    }

    swap_chain_->Release();
    swap_chain_ = nullptr;
  }

  release_obj(reinterpret_cast<IUnknown**>(&back_buffer_));
  release_obj(reinterpret_cast<IUnknown**>(&target_bitmap_));

  release_obj(reinterpret_cast<IUnknown**>(&depth_stencil_buffer_));
  release_obj(reinterpret_cast<IUnknown**>(&depth_stencil_state_));
  release_obj(reinterpret_cast<IUnknown**>(&depth_stencil_view_));

  release_obj(reinterpret_cast<IUnknown**>(&raster_state_));
  release_obj(reinterpret_cast<IUnknown**>(&sampler_state_));
  release_obj(reinterpret_cast<IUnknown**>(&blend_state_));

  release_obj(reinterpret_cast<IUnknown**>(&geometry_vertex_buffer_));
  release_obj(reinterpret_cast<IUnknown**>(&geometry_index_buffer_));
}

void Renderer::begin_draw()
{
  catengine::Color color(1.0f, 0.0f, 0.0f);
  geometry_vertices_staged_.clear();
  geometry_indices_staged_.clear();

  devcon_->ClearRenderTargetView(back_buffer_, reinterpret_cast<float const*>(&color));
  //devcon_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH, 1.0f, 0);

  //d2d_devcon_->BeginDraw();
}

void Renderer::end_draw()
{
  render_geometry();
  //d2d_devcon_->EndDraw();

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

  HRESULT hr;

  // vertices / indices
  D3D11_MAPPED_SUBRESOURCE v_map;
  SecureZeroMemory(&v_map, sizeof(D3D11_MAPPED_SUBRESOURCE));
  if ((hr = devcon_->Map(geometry_vertex_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &v_map)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "cannot map vertex buffer: " << err.ErrorMessage();
    return;
  }
  memcpy(v_map.pData, geometry_vertices_staged_.data(), max_geometry_vertices_);
  devcon_->Unmap(geometry_vertex_buffer_, 0);

  D3D11_MAPPED_SUBRESOURCE i_map;
  SecureZeroMemory(&i_map, sizeof(D3D11_MAPPED_SUBRESOURCE));
  if ((hr = devcon_->Map(geometry_index_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &i_map)) != S_OK) {
    _com_error err(hr);
    LOG(ERROR) << "cannot map index buffer: " << err.ErrorMessage();
    return;
  }
  memcpy(i_map.pData, geometry_indices_staged_.data(), max_geometry_indices_);
  devcon_->Unmap(geometry_index_buffer_, 0);

  // things that aren't really needed here
  devcon_->OMSetRenderTargets(1, &back_buffer_, nullptr);// depth_stencil_view_);
  devcon_->RSSetViewports(1, &viewport_);

  // states
  devcon_->OMSetBlendState(blend_state_, nullptr, 0xffffffff);
  devcon_->OMSetDepthStencilState(depth_stencil_state_, 1);
  devcon_->RSSetState(raster_state_);

  // effects pipeline
  geometry_effect_.apply(devcon_);

  unsigned vertex_stride = sizeof(GeometryVertex);
  unsigned vertex_offset = 0;

  devcon_->IASetVertexBuffers(0, 1, &geometry_vertex_buffer_, &vertex_stride, &vertex_offset);
  devcon_->IASetIndexBuffer(geometry_index_buffer_, DXGI_FORMAT_R16_UINT, 0);

  devcon_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  devcon_->DrawIndexed(geometry_indices_staged_.size(), 0, 0);
}

void Renderer::draw_line(LineSegment const& line)
{
  ID2D1SolidColorBrush* brush;
  d2d_devcon_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
  d2d_devcon_->DrawLine({line.start.x, line.start.y}, {line.end.x, line.end.y}, brush);
}

void Renderer::draw_rect(Rectangle const& rect)
{
  ID2D1SolidColorBrush* brush;
  d2d_devcon_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
  d2d_devcon_->DrawRectangle({ rect.left(), rect.top(), rect.right(), rect.bottom() }, brush);
}

void Renderer::fill_rect(Rectangle const& rect)
{/*
  ID2D1SolidColorBrush* brush;
  d2d_devcon_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
  d2d_devcon_->FillRectangle({ rect.left(), rect.top(), rect.right(), rect.bottom() }, brush);*/

  short start = static_cast<short>(geometry_vertices_staged_.size());
  geometry_vertices_staged_.emplace_back(Point3d{ rect.left(), rect.top(), 1.0f }, brush_color_);
  geometry_vertices_staged_.emplace_back(Point3d{ rect.right(), rect.top(), 1.0f }, brush_color_);
  geometry_vertices_staged_.emplace_back(Point3d{ rect.left(), rect.bottom(), 1.0f }, brush_color_);
  geometry_vertices_staged_.emplace_back(Point3d{ rect.right(), rect.bottom(), 1.0f }, brush_color_);

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
  swap_chain_->ResizeBuffers(2, 0, 0, display_format_, 0);
  //LOG(ERROR) << "not implemented";
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
    LOG(ERROR) << "unable to get canvas size";
    return RESULTS::FAILURE;
  } else if (window_rect.right - window_rect.left <= 0) {
    LOG(ERROR) << "invalid canvas size: width must be greater than 0";
    return RESULTS::FAILURE;
  } else if (window_rect.bottom - window_rect.top <= 0) {
    LOG(ERROR) << "invalid canvas size: height must be greater than 0";
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
  throw_if_failed(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), 
    "error creating dxgi factory");
  throw_if_failed(factory->EnumAdapters(0, &adapter), "error getting enum adapters");
  throw_if_failed(adapter->EnumOutputs(0, &adapter_output), "error getting enum outputs");

  unsigned num_modes;
  throw_if_failed(adapter_output->GetDisplayModeList(
      display_format_,
      DXGI_ENUM_MODES_INTERLACED,
      &num_modes,
      nullptr),
    "error getting display mode list count");
  display_mode_list = new DXGI_MODE_DESC[num_modes];
  throw_if_failed(adapter_output->GetDisplayModeList(
      display_format_, 
      DXGI_ENUM_MODES_INTERLACED, 
      &num_modes, display_mode_list),
    "error getting display mode list");

  for (unsigned i = 0; i < num_modes; ++i) {
    if (display_mode_list[i].Width == canvas_width_ && display_mode_list[i].Height == canvas_height_) {
      refresh_numerator_ = display_mode_list[i].RefreshRate.Numerator;
      refresh_denominator_ = display_mode_list[i].RefreshRate.Denominator;
    }
  }

  DXGI_ADAPTER_DESC adapter_desc;
  SecureZeroMemory(&adapter_desc, sizeof(DXGI_ADAPTER_DESC));
  throw_if_failed(adapter->GetDesc(&adapter_desc), "error getting adapter description");
  size_t str_len;
  char m_adapter_desc[128];
  wcstombs_s(&str_len, m_adapter_desc, 128, adapter_desc.Description, 128);

  display_desc_.adapter_name = std::string(m_adapter_desc, str_len);
  display_desc_.system_memory = adapter_desc.DedicatedSystemMemory;
  display_desc_.video_memory = adapter_desc.DedicatedVideoMemory;
  display_desc_.refresh_numerator = refresh_numerator_;
  display_desc_.refresh_denominator = refresh_denominator_;

  LOG(INFO) << display_desc_
    << "window width: " << canvas_width_ << std::endl
    << "window height: " << canvas_height_;

  return RESULTS::SUCCESS;
}

void Renderer::init_d3d_device()
{
  UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
  if (debug_) creation_flags |= D3D11_CREATE_DEVICE_DEBUG;

  D3D_FEATURE_LEVEL feature_level;

  throw_if_failed(D3D11CreateDevice(
      nullptr,
      D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
      creation_flags,
      feature_levels_,
      feature_levels_size_,
      D3D11_SDK_VERSION,
      &dev_,
      &feature_level,
      &devcon_),
    "failed to initialized d3d device and context");

  LOG(SUCCESS) << "successfully initialized d3d device";
}

void Renderer::init_dxgi_device()
{
  if (dev_ == nullptr) {
    throw std::exception("could not initialize dxgi device: d3d device is null");
  }

  throw_if_failed(dev_->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgi_dev_),
    "could not query dxgi device from d3d device");

  throw_if_failed(dxgi_dev_->GetAdapter(&dxgi_adapter_),
    "could not get dxgi adapter");

  throw_if_failed(dxgi_adapter_->GetParent(IID_PPV_ARGS(&dxgi_factory_)),
    "could not get dxgi factory");

  LOG(SUCCESS) << "successfully initialized dxgi device, adapter, and factory";
}

void Renderer::init_d2d_device()
{
  if (dxgi_dev_ == nullptr) {
    throw new std::exception("could not initialize d2d device: dxgi device is null");
  }

  D2D1_FACTORY_OPTIONS options;
  SecureZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
  options.debugLevel = (debug_) ? D2D1_DEBUG_LEVEL_WARNING : D2D1_DEBUG_LEVEL_NONE;

  throw_if_failed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
      __uuidof(ID2D1Factory1),
      &options,
      (void**)&d2d_factory_),
    "failed to create d2d factory");

  throw_if_failed(d2d_factory_->CreateDevice(dxgi_dev_, &d2d_dev_),
    "could not create d2d device");

  throw_if_failed(d2d_dev_->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2d_devcon_),
    "could not create d2d device context");

  LOG(SUCCESS) << "successfully initialized d2d device, context, and factory";
}

void Renderer::initialize_size_dependent_resources(HWND hwnd)
{
  if (swap_chain_ != nullptr) {
    auto hr = swap_chain_->ResizeBuffers(2, canvas_width_, canvas_height_, display_format_, 0);

    if (hr == DXGI_ERROR_DEVICE_REMOVED) {
      dispose();
      initialize(hwnd);
      return;
    } else {
        throw_if_failed(hr, "could not resize buffers");
    }
  } else {
    init_swap_chain(hwnd);
  }

  init_back_buffer();
  init_depth_buffer();
  init_viewport();
}

void Renderer::init_swap_chain(HWND hwnd)
{
  if (dxgi_factory_ == nullptr) {
    throw std::exception("could not initialize swap chain: dxgi factory is null");
  } else if (dev_ == nullptr) {
    throw std::exception("could not initialize swap chain: d3d device is null");
  }

  DXGI_SWAP_CHAIN_DESC sc_desc;
  SecureZeroMemory(&sc_desc, sizeof(DXGI_SWAP_CHAIN_DESC));
  sc_desc.BufferDesc.Width = canvas_width_;
  sc_desc.BufferDesc.Height = canvas_height_;
  sc_desc.BufferDesc.RefreshRate.Numerator = refresh_numerator_;
  sc_desc.BufferDesc.RefreshRate.Denominator = refresh_denominator_;
  sc_desc.BufferDesc.Format = display_format_;
  sc_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  sc_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
  sc_desc.SampleDesc.Count = sample_count_;
  sc_desc.SampleDesc.Quality = sample_quality_;
  sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sc_desc.BufferCount = 2;
  sc_desc.OutputWindow = hwnd;
  sc_desc.Windowed = windowed_;
  sc_desc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
  sc_desc.Flags = 0;

  throw_if_failed(dxgi_factory_->CreateSwapChain(dev_, &sc_desc, &swap_chain_),
    "could not create swap chain");
  dxgi_factory_->MakeWindowAssociation(hwnd, 0);

  throw_if_failed(dxgi_dev_->SetMaximumFrameLatency(1), "could not set max frame latency");

  LOG(SUCCESS) << "successfully initialized swap chain and device";
}

void Renderer::init_back_buffer()
{
  if (swap_chain_ == nullptr) {
    throw std::exception("cannot create back buffer: swap chain is null");
  } else if (dev_ == nullptr) {
    throw std::exception("canont create back buffer: d3d device is null");
  }

  // create the d3d backbuffer
  ID3D11Texture2D* back_buffer;
  throw_if_failed(swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer),
    "could not get buffer from swap chain");

  throw_if_failed(dev_->CreateRenderTargetView(back_buffer, nullptr, &back_buffer_),
    "could not create render target view");

  // create the d2d back buffer
  IDXGISurface1* dxgi_back_buffer;
  throw_if_failed(swap_chain_->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&dxgi_back_buffer),
    "could not get dxgi buffer from swap chain");

  D2D1_BITMAP_PROPERTIES1 bitmap_properties;
  SecureZeroMemory(&bitmap_properties, sizeof(D2D1_BITMAP_PROPERTIES1));
  bitmap_properties.pixelFormat.format = display_format_;
  bitmap_properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
  bitmap_properties.dpiX = dpi_x_;
  bitmap_properties.dpiY = dpi_y_;
  bitmap_properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

  if (d2d_devcon_->CreateBitmapFromDxgiSurface(dxgi_back_buffer, &bitmap_properties, &target_bitmap_) != S_OK) {
    throw std::exception("could not create bitmap from dxgi surface");
  }
  d2d_devcon_->SetTarget(target_bitmap_);

  dxgi_back_buffer->Release(); // todo: make sure this is alright
  back_buffer->Release();

  LOG(SUCCESS) << "successfully initialized back buffer";
}

void Renderer::init_depth_buffer()
{
  if (dev_ == nullptr) {
    throw std::exception("cannot create depth buffer: d3d device is null");
  }

  LOG(FATAL) << "depth buffer is broken and stops any rendering";

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

  throw_if_failed(dev_->CreateTexture2D(&db_desc, nullptr, &depth_stencil_buffer_),
    "failed to create depth stencil buffer");

  D3D11_DEPTH_STENCIL_DESC ds_desc;
  SecureZeroMemory(&ds_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
  ds_desc.DepthEnable = false;
  ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  ds_desc.DepthFunc = D3D11_COMPARISON_LESS;

  ds_desc.StencilEnable = false;
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

  throw_if_failed(dev_->CreateDepthStencilState(&ds_desc, &depth_stencil_state_),
    "failed to create depth stencil state state");

  D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
  SecureZeroMemory(&dsv_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
  dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  dsv_desc.Texture2D.MipSlice = 0;

  throw_if_failed(dev_->CreateDepthStencilView(depth_stencil_buffer_, &dsv_desc, &depth_stencil_view_),
    "failed to create depth stencil view");

  LOG(SUCCESS) << "successfully initialized depth stencil";
}

void Renderer::init_viewport()
{
  SecureZeroMemory(&viewport_, sizeof(D3D11_VIEWPORT));

  viewport_.TopLeftX = 0.f;
  viewport_.TopLeftY = 0.f;
  viewport_.Width = static_cast<FLOAT>(canvas_width_);
  viewport_.Height = static_cast<FLOAT>(canvas_height_);
  viewport_.MinDepth = 0.f;
  viewport_.MaxDepth = 1.f;

  LOG(SUCCESS) << "successfully initialized viewport";
}

void Renderer::init_rasterizer_state()
{
  if (dev_ == nullptr) {
    throw std::exception("cannot create rasterizer state: d3d device is null");
  }

  D3D11_RASTERIZER_DESC raster_desc;
  SecureZeroMemory(&raster_desc, sizeof(D3D11_RASTERIZER_DESC));
  raster_desc.AntialiasedLineEnable = false;
  raster_desc.FillMode = D3D11_FILL_SOLID;
  raster_desc.CullMode = D3D11_CULL_NONE;
  raster_desc.DepthBias = 0;
  raster_desc.DepthBiasClamp = 0.0f;
  // Turning off depth clip as I'm having issues with it. Probably something to do with my matrices; I'll look at it again later.
  raster_desc.DepthClipEnable = false;
  raster_desc.FrontCounterClockwise = false;
  raster_desc.MultisampleEnable = false;
  raster_desc.ScissorEnable = false;
  raster_desc.SlopeScaledDepthBias = 0.0f;

  throw_if_failed(dev_->CreateRasterizerState(&raster_desc, &raster_state_),
    "cannot create rasterizer state");

  LOG(SUCCESS) << "successfully initialized rasterizer state";
}

void Renderer::init_sampler_state()
{
  if (dev_ == nullptr) {
    throw std::exception("cannot create sampler state: d3d device is null");
  }

  D3D11_SAMPLER_DESC desc;
  SecureZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
  desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.MaxAnisotropy = (dev_->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;
  desc.MinLOD = 0.0f;
  desc.MaxLOD = FLT_MAX;
  desc.MipLODBias = 0.0f;
  desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

  throw_if_failed(dev_->CreateSamplerState(&desc, &sampler_state_), 
    "cannot create sampler state");

  LOG(SUCCESS) << "successfully initialized sampler state";
}

void Renderer::init_blend_state()
{
  return create_blend_state(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, &blend_state_);
}

void Renderer::create_blend_state(D3D11_BLEND src_blend,
  D3D11_BLEND dest_blend,
  D3D11_BLEND_OP blend_op,
  ID3D11BlendState** blend_state)
{
  if (dev_ == nullptr) {
    throw std::exception("cannot create blend state: d3d device is null");
  }

  D3D11_BLEND_DESC blend_desc;
  SecureZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
  blend_desc.RenderTarget[0].BlendEnable = true;
  blend_desc.RenderTarget[0].SrcBlend = blend_desc.RenderTarget[0].SrcBlendAlpha = src_blend;
  blend_desc.RenderTarget[0].DestBlend = blend_desc.RenderTarget[0].DestBlendAlpha = dest_blend;
  blend_desc.RenderTarget[0].BlendOp = blend_desc.RenderTarget[0].BlendOpAlpha = blend_op;

  blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  throw_if_failed(dev_->CreateBlendState(&blend_desc, blend_state),
    "cannot create blend state");

  LOG(SUCCESS) << "successfully created blend state";
}

void Renderer::init_basic_shaders()
{
  geometry_effect_.initialize(dev_);
}

void Renderer::init_resources()
{
}

void Renderer::init_geometry_buffers()
{
  if (dev_ == nullptr) {
    throw std::exception("could not create geometry buffers: d3d device is null");
  }

  D3D11_BUFFER_DESC vbd;
  SecureZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
  vbd.Usage = D3D11_USAGE_DYNAMIC;
  vbd.ByteWidth = sizeof(GeometryVertex) * max_geometry_vertices_;
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  throw_if_failed(dev_->CreateBuffer(&vbd, nullptr, &geometry_vertex_buffer_),
    "failed to create geometry vertex buffer");

  geometry_vertices_staged_.reserve(max_geometry_vertices_);

  // sizing the index buffer in relation to a rectangle or line
  max_geometry_indices_ = max_geometry_vertices_ * 6 / 4;
  D3D11_BUFFER_DESC ibd;
  SecureZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
  ibd.Usage = D3D11_USAGE_DYNAMIC;
  ibd.ByteWidth = sizeof(short) * max_geometry_indices_;
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  throw_if_failed(dev_->CreateBuffer(&ibd, nullptr, &geometry_index_buffer_),
    "failed to create geometry index buffer");

  geometry_indices_staged_.reserve(max_geometry_indices_);
  LOG(SUCCESS) << "successfully initialized geometry buffers";
}
}
}