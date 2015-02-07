#ifndef DISPLAY_DESC_H
#define DISPLAY_DESC_H

#include <string>
#include <iostream>

namespace catengine
{
struct DisplayDesc
{
  std::string adapter_name;
  size_t video_memory;
  size_t system_memory;
  size_t refresh_numerator;
  size_t refresh_denominator;

  friend std::ostream& DisplayDesc::operator<<(std::ostream& os, DisplayDesc const& desc)
  {
    os << "video card: " << desc.adapter_name << std::endl;
    os << "video memory: " << (desc.video_memory / 1024 / 1024) << "MB" << std::endl;
    os << "system memory: " << (desc.system_memory / 1024 / 1024) << "MB" << std::endl;
    os << "refresh rate: " << desc.refresh_numerator << " / " << desc.refresh_denominator;
    os << " (" << (desc.refresh_numerator / desc.refresh_denominator) << "Hz)" << std::endl;
    return os;
  }
};
}

#endif