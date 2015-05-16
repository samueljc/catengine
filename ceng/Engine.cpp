#include "Engine.hpp"

namespace ceng {
  
Engine::~Engine() 
{
  if (sound_buffer_ != nullptr) {
    delete sound_buffer_;
    sound_buffer_ = nullptr;
  }
}
}