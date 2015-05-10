#include "Camera.h"

namespace catengine {
  
void Camera::reposition(_decimal x, _decimal y)
{
  if (x == viewport_.x || y == viewport_.y) return;

  viewport_.x = x;
  viewport_.y = y;
  world_ = create_world_matrix(viewport_.x, viewport_.y);
  rebuild_matrices();
}

void Camera::resize(_decimal width, _decimal height)
{
  if (width == viewport_.width && height == viewport_.height ) return;

  viewport_.width = width;
  viewport_.height = height;
  proj_ = create_proj_matrix(viewport_.width, viewport_.height);
  rebuild_matrices();
}

Matrix4x4 Camera::create_world_matrix(_decimal x, _decimal y)
{
  return {
    { 1.f, 0.f, 0.f, -x },
    { 0.f, 1.f, 0.f, -y },
    { 0.f, 0.f, 1.f, 0.f },
    { 0.f, 0.f, 0.f, 1.f }
  };
}

Matrix4x4 Camera::create_view_matrix()
{
  return view_;
}

Matrix4x4 Camera::create_proj_matrix(_decimal width, _decimal height)
{
  return {
    { 2.f / width, 0.f, 0.f, -1.f },
    { 0.f, 2.f / -height, 0.f, 1.f },
    { 0.f, 0.f, -2.f / 100.f, -1.f },
    { 0.f, 0.f, 0.f, 1.f }
  };
}

const Matrix4x4 Camera::view_ = {
  { 1.f, 0.f, 0.f, 0.f },
  { 0.f, 1.f, 0.f, 0.f },
  { 0.f, 0.f, 1.f, 10.f },
  { 0.f, 0.f, 0.f, 1.f }
};
}