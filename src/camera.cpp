#include "include/camera.hpp"

#include <sstream>
#include <iomanip>

#include <GL/freeglut.h>
#include <opencv2/opencv.hpp>

Camera::Camera()
  : azimuth(0.0f) {}

void Camera::Setup(int window_width, int window_height) {
  static const float near_clip_plane = 0.1f;
  static const float far_clip_plane = 1000.0f;
  static const float view_angle = 30.0f;
  static float ratio = M_PI / 180.0f;
  static float camera_orbit_radius = 20.0f;
  static const float camera_z = 15.0f;

  azimuth = (azimuth != 359.0f ? azimuth + 0.01f : 0.0f);

  const float aspect = (float)window_width / window_height;
  const float camera_x = camera_orbit_radius * cos(azimuth * ratio);
  const float camera_y = camera_orbit_radius * sin(azimuth * ratio);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(view_angle, aspect, near_clip_plane, far_clip_plane);
  gluLookAt(camera_x, camera_y, camera_z,  // Camera position.
            0, 0, 0,   // View point position.
            0, 0, 1);  // Camera axis.
}

void Camera::MakeScreenshot(int window_width, int window_height) {
  std::stringstream ss;
  ss << "./screenshots/" << std::setw(3) << std::setfill('0') << azimuth
     << ".png";

  unsigned char* data = new unsigned char[window_width * window_height * 3];
  glReadPixels(0, 0, window_width, window_height, GL_BGR, GL_UNSIGNED_BYTE,
               data);
  cv::Mat mat(window_height, window_width, CV_8UC3, data);
  cv::flip(mat, mat, 0);
  cv::imwrite(ss.str(), mat);
  delete[] data;
}
