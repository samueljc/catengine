#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"

#include <windows.h>
#include <Xinput.h>

#include <stdint.h>

#include "Logging.h"
#include "Console.h"

#define internal static
#define local_persist static
#define global static

global bool running;

struct Dimension
{
  int width;
  int height;
};

#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD user_index, XINPUT_STATE* state)
typedef X_INPUT_GET_STATE(XInputGetStateFunc);
global XInputGetStateFunc* x_input_get_state = nullptr;

#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD user_index, XINPUT_VIBRATION* vibration)
typedef X_INPUT_SET_STATE(XInputSetStateFunc);
global XInputSetStateFunc* x_input_set_state = nullptr;

// note: this is loading manually from a dll
internal void win32_load_xinput()
{
  HMODULE x_input_lib = LoadLibrary(L"xinput1_4.dll");
  if (x_input_lib != nullptr) {
    x_input_get_state = (XInputGetStateFunc*)GetProcAddress(x_input_lib, "XInputGetState");
    x_input_set_state = (XInputSetStateFunc*)GetProcAddress(x_input_lib, "XInputSetState");
    if (x_input_get_state != nullptr && x_input_set_state != nullptr) {
      LOG(SUCCESS) << "loaded xinput1_4.dll";
      return;
    }
  }

  x_input_lib = LoadLibrary(L"xinput1_3.dll");
  if (x_input_lib != nullptr) {
    x_input_get_state = (XInputGetStateFunc*)GetProcAddress(x_input_lib, "XInputGetState");
    x_input_set_state = (XInputSetStateFunc*)GetProcAddress(x_input_lib, "XInputSetState");
    if (x_input_get_state != nullptr && x_input_set_state != nullptr) {
      LOG(SUCCESS) << "loaded xinput1_3.dll";
      return;
    }
  }

  LOG(WARNING) << "failed to load xinput";
}

struct win32OffscreenBuffer {
  internal const int bytes_per_pixel = 4;

  BITMAPINFO info;
  void* data;
  int width;
  int height;

  void resize_buffer(int width, int height)
  {
    if (data != nullptr) {
      // todo: mem_release vs mem_commit
      // VirtualProtect: remove access to this memory so that we fail on the line that tries to access it
      // useful for debugging
      VirtualFree(data, 0, MEM_RELEASE);
    }

    this->width = width;
    this->height = height;

    info.bmiHeader.biSize = sizeof(info.bmiHeader);
    info.bmiHeader.biWidth = this->width;
    info.bmiHeader.biHeight = -(this->height);
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;

    int buffer_size = size();
    data = VirtualAlloc(nullptr, buffer_size, MEM_COMMIT, PAGE_READWRITE);
    if (data == nullptr) {
      // TODO: THROW EXCEPTION INSTEAD OF LOGGING
      LOG(FATAL) << "could not allocate backbuffer";
    }
  }

  inline int size() const { return width * height * bytes_per_pixel; }
  inline int pitch() const { return width * bytes_per_pixel; }
};
global win32OffscreenBuffer backbuffer;

internal Dimension
win32_get_window_dimension(HWND window)
{
  Dimension result;

  RECT client_rect;
  GetClientRect(window, &client_rect);
  result.width = client_rect.right - client_rect.left;
  result.height = client_rect.bottom - client_rect.top;
  
  return result;
}

internal void render_something(win32OffscreenBuffer* buffer, int x_offset, int y_offset)
{
  int pitch = buffer->pitch();
  uint8_t* row = (uint8_t*)buffer->data;

  for (int y = 0; y < buffer->height; ++y) {
    uint32_t* pixel = (uint32_t*)row;
    for (int x = 0; x < buffer->width; ++x) {
      uint8_t blue = (uint8_t)(x + x_offset);
      uint8_t green = (uint8_t)(y + y_offset);

      *pixel++ = ((green << 8) | blue);
    }
    row += pitch;
  }
}

internal void win32_resize_DIB_section(win32OffscreenBuffer* buffer, int width, int height)
{
  buffer->resize_buffer(width, height);
  render_something(buffer, 0, 0);
}

internal void win32_update_window(HDC devcon, win32OffscreenBuffer const& buffer, int window_width, int window_height)
{
  StretchDIBits(devcon,
    0, 0, window_width, window_height,
    0, 0, buffer.width, buffer.height,
    buffer.data,
    &buffer.info,
    DIB_RGB_COLORS,
    SRCCOPY);
}

internal void win32_process_keyboard(WPARAM w_param, LPARAM l_param)
{
  uint32_t vk_code = w_param;
  bool was_down = (l_param & (1 << 30)) != 0;
  bool is_down = (l_param & (1 << 31)) == 0;

  switch(vk_code) {
  case 'W':
  case VK_UP:
    break;
  case 'S':
  case VK_DOWN:
    break;
  case 'A':
  case VK_LEFT:
    break;
  case 'D':
  case VK_RIGHT:
    break;
  case VK_ESCAPE:
    if (is_down) running = false;
    break;
  }
}

global int y_off;
global int x_off;

internal void win32_vibrate_controller(DWORD index, int32_t left = 0xFFFF, int32_t right = 0xFFFF)
{
  if (x_input_set_state == nullptr) return;

  XINPUT_VIBRATION vibration;
  vibration.wLeftMotorSpeed = left;
  vibration.wRightMotorSpeed = right;

  x_input_set_state(index, &vibration);
}

internal void win32_process_gamepad()
{
  XINPUT_STATE controller_state;
  for (DWORD controller_index = 0; controller_index < XUSER_MAX_COUNT; ++controller_index)
  {
    if (x_input_get_state(controller_index, &controller_state) == ERROR_SUCCESS) {
      auto& pad = controller_state.Gamepad;

      bool up = pad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
      bool down = pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
      bool left = pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
      bool right = pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
      bool back = pad.wButtons & XINPUT_GAMEPAD_BACK;
      bool start = pad.wButtons & XINPUT_GAMEPAD_START;
      bool a = pad.wButtons & XINPUT_GAMEPAD_A;
      bool b = pad.wButtons & XINPUT_GAMEPAD_B;
      bool x = pad.wButtons & XINPUT_GAMEPAD_X;
      bool y = pad.wButtons & XINPUT_GAMEPAD_Y;
      bool left_shoulder = pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
      bool right_shoulder = pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

      int8_t trigger_left = pad.bLeftTrigger;
      int8_t trigger_right = pad.bRightTrigger;

      int16_t stick_x = pad.sThumbLX;
      int16_t stick_y = pad.sThumbLY;

      if (up) ++y_off;
      else if (down) --y_off;

      if (left) --x_off;
      else if (right) ++x_off;
          
      win32_vibrate_controller(controller_index, static_cast<WORD>(trigger_left) * 4, static_cast<WORD>(trigger_right) * 4);
    }
  }
}

LRESULT CALLBACK win32_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
  LRESULT result(0);
  
  switch (message) {
  case WM_SIZE:
    break;
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  case WM_KEYDOWN:
  case WM_KEYUP:
    win32_process_keyboard(w_param, l_param);
    break;
  case WM_DESTROY:
    running = false;
    break;
  case WM_CLOSE:
    running = false;
    break;
  case WM_ACTIVATEAPP:
    LOG(INFO) << "WM_ACTIVATEAPP";
    break;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    auto devcon = BeginPaint(window, &ps);
    win32_update_window(devcon, 
      backbuffer,
      ps.rcPaint.right - ps.rcPaint.left, 
      ps.rcPaint.bottom - ps.rcPaint.top);
    EndPaint(window, &ps);
  } break;
  default:
    result = DefWindowProc(window, message, w_param, l_param);
  }

  return result;
}

int APIENTRY _tWinMain(HINSTANCE instance,
  HINSTANCE prev_instance,
  LPTSTR command_line,
  int show_command)
{
  WNDCLASS window_class = {};

  window_class.style = CS_HREDRAW | CS_VREDRAW;
  window_class.lpfnWndProc = win32_callback;
  window_class.hInstance = instance;
  window_class.lpszClassName = L"test";

  catengine::Console::open();
  
  if (!RegisterClass(&window_class)) {
    // TODO: get last error, blah blah blah
    LOG(FATAL) << "could not register window class";
  }

  HWND window = CreateWindowEx(
    0,
    window_class.lpszClassName,
    L"CATch",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    nullptr,
    nullptr,
    instance,
    0);
  if (window == nullptr) {
    // TODO: get last error, blah blah blah
    LOG(FATAL) << "could not create window";
  }

  win32_resize_DIB_section(&backbuffer, 800, 600);
  win32_load_xinput();
  
  MSG message;
  running = true;
  while (running) {
    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
      if (message.message == WM_QUIT) {
        running = false;
      }

      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    // poll input
    if (x_input_get_state != nullptr) {
      win32_process_gamepad();
    }

    render_something(&backbuffer, x_off, y_off);

    auto devcon = GetDC(window);
    auto d = win32_get_window_dimension(window);
    win32_update_window(devcon, backbuffer, d.width, d.height);
    ReleaseDC(window, devcon);
  }
  return 0;
}