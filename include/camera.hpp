#ifndef INCLUDE_CAMERA_HPP_
#define INCLUDE_CAMERA_HPP_

#include <string>

class Camera {
 public:
   Camera();

   void Setup(int window_width, int window_height);

   void MakeScreenshot(int window_width, int window_height);

 private:
   float azimuth;
};

#endif  // INCLUDE_CAMERA_HPP_
