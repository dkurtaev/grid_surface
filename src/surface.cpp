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
  int offset = 0;
  for (int i = 0; i < n_nodes_by_y; ++i) {
    for (int j = 0; j < n_nodes_by_x; ++j) {
      float x = min_x + j * step_by_x;
      float y = min_y + i * step_by_y;
      vertices_array[offset++] = x;
      vertices_array[offset++] = y;
      vertices_array[offset++] = sin(x) * sin(y) + 1;
    }
  }
}

Surface::~Surface() {
  delete[] vertices_array;
}

void Surface::Draw() {
  glColor3ub(0, 127, 127);
  for (int i = 0; i < n_nodes_by_y; ++i) {
    for (int j = 0; j < n_nodes_by_x; ++j) {
      int offset = (i * n_nodes_by_x + j) * 3;
      glPushMatrix();
        glTranslatef(vertices_array[offset],
                     vertices_array[offset + 1],
                     vertices_array[offset + 2]);
        glutSolidSphere(0.05, 10, 10);
      glPopMatrix();
    }
  }
}
