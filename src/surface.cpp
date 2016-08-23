#include "include/surface.hpp"

#include <math.h>

#include <GL/freeglut.h>

Surface::Surface(float min_x, float max_x, float min_y, float max_y,
                 int n_nodes_by_x, int n_nodes_by_y) {
  this->n_nodes_by_x = n_nodes_by_x;
  this->n_nodes_by_y = n_nodes_by_y;
  float step_by_x = (max_x - min_x) / (n_nodes_by_x - 1);
  float step_by_y = (max_y - min_y) / (n_nodes_by_y - 1);

  // Setup coordinates: (x, y, z) point.
  vertices_array = new float[n_nodes_by_x * n_nodes_by_y * 3];
  float* offset = vertices_array;
  for (int i = 0; i < n_nodes_by_y; ++i) {
    for (int j = 0; j < n_nodes_by_x; ++j) {
      float x = min_x + j * step_by_x;
      float y = min_y + i * step_by_y;
      offset[0] = x;
      offset[1] = y;
      offset[2] = sin(x) * sin(y) + 1;
      offset += 3;
    }
  }
}

Surface::~Surface() {
  delete[] vertices_array;
}

void Surface::Draw() {
  glColor3ub(0, 127, 127);

  glBegin(GL_TRIANGLES);
  // Every cell of grid looks like:
  // (i+1, j)  (i+1, j+1)   first triangle on 0-2-3 vertices
  //    3-------2           second triangle on 0-1-2 vertices.
  //    |     / |
  //    |   /   |
  //    | /     |
  //    0-------1
  // (i, j)   (i, j+1)
  float* offset = vertices_array;
  for (int i = 0; i < n_nodes_by_y - 1; ++i) {
    for (int j = 0; j < n_nodes_by_x - 1; ++j) {
      glVertex3fv(offset);  // Bottom left.
      glVertex3fv(offset + (n_nodes_by_x + 1) * 3);  // Top right.
      glVertex3fv(offset + n_nodes_by_x * 3);  // Top left.

      glVertex3fv(offset);
      glVertex3fv(offset + 3);  // Bottom right.
      glVertex3fv(offset + (n_nodes_by_x + 1) * 3);  // Top right.
      offset += 3;
    }
  }
  glEnd();
}
