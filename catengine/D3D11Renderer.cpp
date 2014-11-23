#include "D3D11Renderer.h"

using catengine::D3D11Renderer;

void D3D11Renderer::initialize(HWND hwnd)
{
  init_swap_chain_and_device(hwnd);
  init_backbuffer();
  init_viewport();
}

void D3D11Renderer::dispose()
{
  if (swp_chain_ != nullptr) {
    swp_chain_->Release();
    swp_chain_ = nullptr;
  }
  if (backbuffer_ != nullptr) {
    backbuffer_->Release();
    backbuffer_ = nullptr;
  }
  if (dev_ != nullptr) {
    dev_->Release();
    dev_ = nullptr;
  }
  if (dev_context_ != nullptr) {
    dev_context_->Release();
    dev_context_ = nullptr;
  }
}

UINT D3D11Renderer::get_quality_levels()
{
  if (dev_ != nullptr) {
    UINT quality_levels;
    if (dev_->CheckMultisampleQualityLevels(texture_format_, sample_count_, &quality_levels) == S_OK) {
      return quality_levels;
    }
  }
  return 0;
}

void D3D11Renderer::init_swap_chain_and_device(HWND hwnd)
{
  DXGI_SWAP_CHAIN_DESC scd;
  SecureZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
  scd.BufferCount = 1;
  scd.BufferDesc.Format = texture_format_;
  scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scd.OutputWindow = hwnd;
  scd.SampleDesc.Count = sample_count_;
  scd.SampleDesc.Quality = sample_quality_;
  scd.Windowed = (fullscreen_) ? FALSE : TRUE;

  HRESULT r = D3D11CreateDeviceAndSwapChain(NULL,
    D3D_DRIVER_TYPE_HARDWARE,
    NULL,
    (debug_) ? D3D11_CREATE_DEVICE_DEBUG : NULL,
    NULL,
    NULL,
    D3D11_SDK_VERSION,
    &scd,
    &swp_chain_,
    &dev_,
    NULL,
    &dev_context_);

  if (r != S_OK) {
    // todo: log an error
    dispose();
  }
}

void D3D11Renderer::init_backbuffer()
{
  if (swp_chain_ == nullptr || dev_ == nullptr || dev_context_ == nullptr) {
    // todo: log an error
    return;
  }

  ID3D11Texture2D* backbuffer;
  swp_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);

  dev_->CreateRenderTargetView(backbuffer, NULL, &backbuffer_);
  backbuffer->Release();

  dev_context_->OMSetRenderTargets(1, &backbuffer_, NULL);
}

void D3D11Renderer::init_viewport()
{
  if (dev_context_ == nullptr) {
    // todo: log an error
    return;
  }
  SecureZeroMemory(&viewport_, sizeof(D3D11_VIEWPORT));

  viewport_.TopLeftX = 0;
  viewport_.TopLeftY = 0;
  viewport_.Width = 100;
  viewport_.Height = 100;

  dev_context_->RSSetViewports(1, &viewport_);
}