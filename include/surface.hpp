#ifndef INCLUDE_SURFACE_HPP_
#define INCLUDE_SURFACE_HPP_

class Surface {
 public:
  Surface(float min_x, float max_x, float min_y, float max_y,
          int n_nodes_by_x, int n_nodes_by_y);

  ~Surface();

  void Draw();

 private:
  float* vertices_array;
  float* normals_array;
  unsigned short* indices_array;
  int n_indices;
};

#endif  // INCLUDE_SURFACE_HPP_
