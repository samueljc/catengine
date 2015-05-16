#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Types.hpp"
#include "SoundBuffer.hpp"
#include "Logging.hpp"

namespace ceng {

class Engine {
public:
  Engine()
    : sound_enabled_(false)
    , sound_buffer_(nullptr)
  {
  }
  Engine(Engine const& engine) = delete;
  Engine(Engine&& engine) = delete;

  ~Engine();

  inline void initialize_sound_buffer(int32 sample_rate, int16 channels, int16 duration);

  inline bool sound_enabled() const { return sound_enabled_; }
  inline SoundBuffer* get_sound_buffer() { return sound_buffer_; }
  inline SoundBuffer const* get_sound_buffer() const { return sound_buffer_; }

private:
  bool sound_enabled_;
  SoundBuffer* sound_buffer_;
};

inline void Engine::initialize_sound_buffer(int32 sample_rate, int16 channels, int16 duration) 
{
  if (sound_enabled_) {
    return;
    LOG(WARNING) << "trying to initialize sound buffer when sound buffer already initialized";
  }

  sound_buffer_ = new SoundBuffer(sample_rate, channels, duration);
  if (sound_buffer_ != nullptr) {
    sound_enabled_ = true;
  }
}
}

#endif // ENGINE_HPP