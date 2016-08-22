#ifndef INCLUDE_SURFACE_HPP_
#define INCLUDE_SURFACE_HPP_

class Surface {
 public:
  Surface(float min_x, float max_x, float min_y, float max_y,
          int n_nodes_by_x, int n_nodes_by_y);

  ~Surface();

  void Draw();

 private:
  float** heights;
  float min_x;
  float min_y;
  int n_nodes_by_x;
  int n_nodes_by_y;
  float step_by_x;
  float step_by_y;
};

#endif  // INCLUDE_SURFACE_HPP_
