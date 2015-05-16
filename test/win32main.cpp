#define WIN32_LEAN_AND_MEAN


#include <windows.h>
#include <tchar.h>
#include <Xinput.h>
#include <mmreg.h>
#include <dsound.h>

#include "win32Console.hpp"

#include "Types.hpp"
#include "Engine.hpp"
#include "SoundBuffer.hpp"
#include "Logging.hpp"

#define Pi32 3.14159265359f

#define internal static
#define local_persist static
#define global static

global bool running;

///////////////////////////////////////////////////////////////////////////////
// RENDERING
///////////////////////////////////////////////////////////////////////////////
struct Dimension
{
  int width;
  int height;
};

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
    data = VirtualAlloc(nullptr, buffer_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (data == nullptr) {
      // TODO: THROW EXCEPTION INSTEAD OF LOGGING
      LOG(FATAL) << "could not allocate backbuffer";
    }
  }

  inline int size() const { return width * height * bytes_per_pixel; }
  inline int pitch() const { return width * bytes_per_pixel; }
};
global win32OffscreenBuffer backbuffer;

internal Dimension win32_get_window_dimension(HWND window)
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

///////////////////////////////////////////////////////////////////////////////
// SOUND
///////////////////////////////////////////////////////////////////////////////
global LPDIRECTSOUNDBUFFER win32_sound_buffer = nullptr;
global DWORD win32_sound_buffer_size = 0;

internal void win32_clear_sound_buffer() {
  VOID* region_1;
  DWORD region_1_size;
  VOID* region_2;
  DWORD region_2_size;
  DWORD buffer_size = 48000;

  if (!SUCCEEDED(win32_sound_buffer->Lock(0, win32_sound_buffer_size, &region_1, &region_1_size, &region_2, &region_2_size, 0))) {
    return;
  }

  memset(region_1, 0, region_1_size);
  memset(region_2, 0, region_2_size);
}

internal void win32_fill_sound_buffer(ceng::SoundBuffer* buffer, DWORD byte_to_lock, DWORD bytes_to_write)
{
  VOID* region_1;
  DWORD region_1_size;
  VOID* region_2;
  DWORD region_2_size;
  if (!SUCCEEDED(win32_sound_buffer->Lock(byte_to_lock, bytes_to_write, &region_1, &region_1_size, &region_2, &region_2_size, 0))) {
    return;
  }

  DWORD region_1_sample_count = region_1_size / buffer->bytes_per_sample();
  int16* dest = (int16*)region_1;
  int16* src = (int16*)buffer->data();

  for (DWORD sample_index = 0; sample_index < region_1_sample_count; ++sample_index) {
    *(dest++) = *(src++);
    *(dest++) = *(src++);
  }

  DWORD region_2_sample_count = region_2_size / buffer->bytes_per_sample();
  dest = (int16*)region_2;
  for (DWORD sample_index = 0; sample_index < region_2_sample_count; ++sample_index) {
    // real32 t = 2.0f * Pi32 * static_cast<real32>(sound->sample_index++) / static_cast<real32>(sound->period);
    // int16 sample_value = static_cast<int16>(sinf(t) * static_cast<real32>(2000));
    *(dest++) = *(src++);
    *(dest++) = *(src++);
  }

  win32_sound_buffer->Unlock(region_1, region_1_size, region_2, region_2_size);
}

internal void win32_init_d_sound(HWND window, int32 sample_rate, int16 sample_size, int16 channels, int32 buffer_size)
{
  // todo: use a more recent sound api
  HMODULE d_sound_lib = LoadLibrary(L"dsound.dll");
  if (d_sound_lib == nullptr) {
    LOG(WARNING) << "failed to load dsound";
    return;
  }
  typedef HRESULT WINAPI DirectSoundCreateFunc(LPCGUID, LPDIRECTSOUND*, LPUNKNOWN);
  DirectSoundCreateFunc* direct_sound_create = (DirectSoundCreateFunc*)GetProcAddress(d_sound_lib, "DirectSoundCreate");

  if (direct_sound_create == nullptr) {
    LOG(ERROR) << "failed to get proc address of DirectSoundCreate";
    return;
  }

  LPDIRECTSOUND direct_sound;
  if (!SUCCEEDED(direct_sound_create(0, &direct_sound, nullptr))) {
    LOG(ERROR) << "failed to create direct sound";
    return;
  }

  if (!SUCCEEDED(direct_sound->SetCooperativeLevel(window, DSSCL_PRIORITY))) {
    LOG(ERROR) << "failed to set cooperative level";
    direct_sound->Release();
    return;
  }

  WAVEFORMATEX wave_format = {};
  wave_format.wFormatTag = WAVE_FORMAT_PCM;
  wave_format.nChannels = channels;
  wave_format.nSamplesPerSec = sample_rate;
  wave_format.wBitsPerSample = sample_size;
  wave_format.nBlockAlign = (wave_format.nChannels * wave_format.wBitsPerSample) / 8;
  wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;
  
  // primary buffer is never actually called, this just gets a handle to the sound card and tells it the format that
  // we'll be passing to it
  DSBUFFERDESC primary_buffer_desc = {};
  primary_buffer_desc.dwSize = sizeof(primary_buffer_desc);
  primary_buffer_desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
  LPDIRECTSOUNDBUFFER primary_buffer;
  if (SUCCEEDED(direct_sound->CreateSoundBuffer(&primary_buffer_desc, &primary_buffer, nullptr))) {
    if (!SUCCEEDED(primary_buffer->SetFormat(&wave_format))) {
      LOG(ERROR) << "failed to set primary buffer format";
      primary_buffer->Release();
      direct_sound->Release();
      return;
    }
  } else {
    LOG(ERROR) << "failed to create primary buffer";
    direct_sound->Release();
    return;
  }

  DSBUFFERDESC secondary_buffer_desc = {};
  secondary_buffer_desc.dwSize = sizeof(secondary_buffer_desc);
  secondary_buffer_desc.dwFlags = 0;
  secondary_buffer_desc.dwBufferBytes = buffer_size;
  secondary_buffer_desc.lpwfxFormat = &wave_format;
  if (!SUCCEEDED(direct_sound->CreateSoundBuffer(&secondary_buffer_desc, &win32_sound_buffer, nullptr))) {
    LOG(ERROR) << "failed to create secondary buffer";
    primary_buffer->Release();
    direct_sound->Release();
    return;
  } else {
    LOG(SUCCESS) << "successfully created sound buffer";
  }

  win32_sound_buffer_size = buffer_size;
  win32_clear_sound_buffer();
}

internal void win32_play_sound(ceng::SoundBuffer* buffer)
{
  DWORD play_cursor;
  DWORD write_cursor;
  if (!SUCCEEDED(win32_sound_buffer->GetCurrentPosition(&play_cursor, &write_cursor))) {
    return;
  }
  
  DWORD buffer_size = buffer->size_in_bytes();

  DWORD byte_to_lock = 0;//(sound->sample_index * sound->sample_size) % buffer_size;
  DWORD target_cursor = play_cursor;
  DWORD bytes_to_write;
  if (byte_to_lock > play_cursor) {
    bytes_to_write = buffer_size - byte_to_lock;
    bytes_to_write += play_cursor;
  } else {
    bytes_to_write = play_cursor - byte_to_lock;
  }

  win32_fill_sound_buffer(buffer, byte_to_lock, bytes_to_write);
}

///////////////////////////////////////////////////////////////////////////////
// INPUT
///////////////////////////////////////////////////////////////////////////////
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD user_index, XINPUT_STATE* state)
typedef X_INPUT_GET_STATE(XInputGetStateFunc);
global XInputGetStateFunc* x_input_get_state = nullptr;

#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD user_index, XINPUT_VIBRATION* vibration)
typedef X_INPUT_SET_STATE(XInputSetStateFunc);
global XInputSetStateFunc* x_input_set_state = nullptr;

internal void win32_load_x_input()
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

internal void win32_process_keyboard(WPARAM w_param, LPARAM l_param)
{
  uint32_t vk_code = w_param;
  bool was_down = (l_param & (1 << 30)) != 0;
  bool is_down = (l_param & (1 << 31)) == 0;

  bool alt_mask = (l_param & (1 << 29)) != 0;

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
  case VK_F4:
    if (alt_mask) running = false;
    break;
  }
}

global int y_off;
global int x_off;

internal void win32_vibrate_controller(DWORD index, int32_t left = 0xFFFF, int32_t right = 0xFFFF)
{
  if (x_input_set_state == nullptr) return;

  XINPUT_VIBRATION vibration;
  vibration.wLeftMotorSpeed = static_cast<DWORD>(left);
  vibration.wRightMotorSpeed = static_cast<DWORD>(right);

  x_input_set_state(index, &vibration);
}

internal void win32_process_gamepad(DWORD controller_index)
{
  XINPUT_STATE controller_state;
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

  win32Console::open();
  
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
  win32_load_x_input();

  int16 sample_size = sizeof(int16);
  int16 channels = 2;
  DWORD sample_rate = 48000;
  DWORD buffer_size = sample_rate * sample_size * channels;
  win32_init_d_sound(window, sample_rate, sample_size, channels, buffer_size);

  ceng::Engine engine;
  if (win32_sound_buffer != nullptr) {
    engine.initialize_sound_buffer(sample_rate, channels, 1);
  }
  
  MSG message;
  running = true;

  LARGE_INTEGER counter_frequency;
  QueryPerformanceFrequency(&counter_frequency);
  LARGE_INTEGER prev_counter;
  QueryPerformanceCounter(&prev_counter);

  // can use __rdtsc() to get the cycle count - probably won't do this

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
      for (DWORD controller_index = 0; controller_index < XUSER_MAX_COUNT; ++controller_index) {
        win32_process_gamepad(controller_index);
      }
    }


    render_something(&backbuffer, x_off, y_off);

    if (win32_sound_buffer != nullptr) {
      win32_play_sound(engine.get_sound_buffer());
    }

    auto devcon = GetDC(window);
    auto d = win32_get_window_dimension(window);
    win32_update_window(devcon, backbuffer, d.width, d.height);
    ReleaseDC(window, devcon);
    
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    auto counter_elapsed = counter.QuadPart - prev_counter.QuadPart;
    auto ms_per_frame = (counter_elapsed * 1000) / counter_frequency.QuadPart;
    LOG(INFO) << ms_per_frame << "ms";
    LOG(INFO) << counter_frequency.QuadPart / counter_elapsed << "fps";

    prev_counter = counter;
  }
  return 0;
}