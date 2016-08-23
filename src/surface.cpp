#include "include/surface.hpp"

#include <math.h>

#include <GL/freeglut.h>

Surface::Surface(float min_x, float max_x, float min_y, float max_y,
                 int n_nodes_by_x, int n_nodes_by_y) {
  float step_by_x = (max_x - min_x) / (n_nodes_by_x - 1);
  float step_by_y = (max_y - min_y) / (n_nodes_by_y - 1);

  // Setup coordinates: (x, y, z) point.
  coordinates = new float[n_nodes_by_x * n_nodes_by_y * 3];
  int offset = 0;
  for (int i = 0; i < n_nodes_by_y; ++i) {
    for (int j = 0; j < n_nodes_by_x; ++j) {
      float x = min_x + j * step_by_x;
      float y = min_y + i * step_by_y;
      coordinates[offset++] = x;
      coordinates[offset++] = y;
      coordinates[offset++] = cos(x * y / M_PI);;
    }
  }

  // Setup indices of triangles.
  // (i+1, j)  (i+1, j+1)   first triangle on 0-2-3 vertices
  //    3-------2           second triangle on 0-1-2 vertices.
  //    |     / |
  //    |   /   |
  //    | /     |
  //    0-------1
  // (i, j)   (i, j+1)
  n_triangles = (n_nodes_by_x - 1) * (n_nodes_by_y - 1) * 2;
  indices = new unsigned short[n_triangles * 3];
  offset = 0;
  for (int i = 0; i < n_nodes_by_y - 1; ++i) {
    for (int j = 0; j < n_nodes_by_x - 1; ++j) {
      int base_idx = (i * n_nodes_by_x) + j;
      indices[offset++] = base_idx;  // Bottom left vertex.
      indices[offset++] = base_idx + n_nodes_by_x + 1;  // Top right vertex.
      indices[offset++] = base_idx + n_nodes_by_x;  // Top left vertex.

      indices[offset++] = base_idx;
      indices[offset++] = base_idx + 1;  // Right vertex.
      indices[offset++] = base_idx + n_nodes_by_x + 1;  // Top right vertex.
    }
  }
}

Surface::~Surface() {
  delete[] coordinates;
  delete[] indices;
}

void Surface::Draw() {
  glColor3ub(0, 127, 127);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, coordinates);
  glDrawElements(GL_TRIANGLES, 3 * n_triangles, GL_UNSIGNED_SHORT, indices);
}
