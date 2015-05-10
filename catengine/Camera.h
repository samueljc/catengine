// todo: I don't want this to be dependant upon DirectX ultimately. For now it's fine as I only have 
// one renderer type, but hopefully I'll get it fixed.

#ifndef CAMERA_H
#define CAMERA_H

#include "Types.h"
#include "Rectangle.h"
#include "Matrix4x4.h"

#include <DirectXMath.h>

namespace catengine {
class Camera {
public:
  Camera() = delete;
  
  inline Camera(_decimal width, _decimal height) :
    viewport_(0.f, 0.f, width, height),
    world_(create_world_matrix(0.f, 0.f)),
    proj_(create_proj_matrix(width, height)),
    vp_(proj_ * view_),
    wvp_(vp_ * world_)
  {
  }

  inline Camera(_decimal x, _decimal y, _decimal width, _decimal height) :
    viewport_(x, y, width, height),
    world_(create_world_matrix(x, y)),
    proj_(create_proj_matrix(width, height)),
    vp_(proj_ * view_),
    wvp_(vp_ * world_)
  {
  }

  inline Rectangle const& get_viewport() const { return viewport_; }

  void reposition(_decimal x, _decimal y);
  void resize(_decimal width, _decimal height);

  inline Matrix4x4 const& world_matrix() const { return world_; }
  inline Matrix4x4 const& view_matrix() const { return view_; }
  inline Matrix4x4 const& proj_matrix() const { return proj_; }
  
  inline Matrix4x4 const& vp_matrix() const { return vp_; }
  inline Matrix4x4 const& wvp_matrix() const { return wvp_; }

private:
  static Matrix4x4 create_world_matrix(_decimal x, _decimal y);
  static Matrix4x4 create_view_matrix();
  static Matrix4x4 create_proj_matrix(_decimal width, _decimal height);

  inline void rebuild_matrices()
  {
    vp_ = proj_ * view_;
    wvp_ = vp_ * world_;
  }

private:
  Rectangle viewport_;

  Matrix4x4 world_;
  static const Matrix4x4 view_;
  Matrix4x4 proj_;
  Matrix4x4 vp_;
  Matrix4x4 wvp_;

};
}

#endif