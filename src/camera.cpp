#include "include/camera.hpp"

#include <GL/freeglut.h>

Camera::Camera(float x, float y, float z)
  : x(x), y(y), z(z) {}

void Camera::Setup(int window_width, int window_height) {
  static const float near_clip_plane = 0.1f;
  static const float far_clip_plane = 1000.0f;
  static const float view_angle = 30.0f;

  const float aspect = (float)window_width / window_height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(view_angle, aspect, near_clip_plane, far_clip_plane);
  gluLookAt(x, y, z,  // Camera position.
            0, 0, 0,   // View point position.
            0, 0, 1);  // Camera axis.
}
