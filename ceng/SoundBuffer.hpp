#ifndef SOUND_BUFFER_HPP
#define SOUND_BUFFER_HPP

#include <vector>

#include "Types.hpp"

namespace ceng {
// todo: make this useful
// not a circular buffer, just put things in it
class SoundBuffer {
public:
  using value_type = int16;//typename decltype(samples_)::value_type;
  using size_type = std::size_t;

  SoundBuffer() = delete;
  SoundBuffer(SoundBuffer const&) = delete;
  SoundBuffer(SoundBuffer&&) = delete;

  SoundBuffer(int32 sample_rate, int16 channels, int16 duration)
    : sample_rate_(sample_rate)
    , channels_(channels)
    , samples_(static_cast<size_type>(sample_rate * channels * duration))
  {
  }

  inline value_type& operator[](size_type index) { return samples_[index]; }
  inline value_type const& operator[](size_type index) const { return samples_[index]; }

  inline value_type* data() { return samples_.data(); }
  inline value_type const* data() const { return samples_.data(); }

  inline void clear() { std::memset(samples_.data(), 0, samples_.size() * sizeof(value_type)); }
  
  inline int16 bytes_per_sample() const { return sizeof(value_type) * channels_; }

  inline size_type size() const { return samples_.size(); }
  inline size_type size_in_bytes() const { return samples_.size() * sizeof(value_type); }

private:
  int32 sample_rate_;
  int16 channels_;
  std::vector<value_type> samples_;
};
}

#endif // SOUND_BUFFER_HPP