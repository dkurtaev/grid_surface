#ifndef INCLUDE_CAMERA_HPP_
#define INCLUDE_CAMERA_HPP_

class Camera {
 public:
   Camera(float x, float y, float z);

   void Setup(int window_width, int window_height);

 private:
   float x;
   float y;
   float z;
};

#endif  // INCLUDE_CAMERA_HPP_
