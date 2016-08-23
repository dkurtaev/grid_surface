#ifndef INCLUDE_SURFACE_HPP_
#define INCLUDE_SURFACE_HPP_

class Surface {
 public:
  Surface(float min_x, float max_x, float min_y, float max_y,
          int n_nodes_by_x, int n_nodes_by_y);

  ~Surface();

  void Draw();

 private:
  float* coordinates;
  unsigned short* indices;
  int n_triangles;
};

#endif  // INCLUDE_SURFACE_HPP_
