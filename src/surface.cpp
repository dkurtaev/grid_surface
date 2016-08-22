#include "include/surface.hpp"

#include <math.h>

#include <GL/freeglut.h>

Surface::Surface(float min_x, float max_x, float min_y, float max_y,
                 int n_nodes_by_x, int n_nodes_by_y) {
  this->min_x = min_x;
  this->min_y = min_y;
  this->n_nodes_by_x = n_nodes_by_x;
  this->n_nodes_by_y = n_nodes_by_y;
  step_by_x = (max_x - min_x) / (n_nodes_by_x - 1);
  step_by_y = (max_y - min_y) / (n_nodes_by_y - 1);

  heights = new float[n_nodes_by_x * n_nodes_by_y];
  for (int i = 0; i < n_nodes_by_y; ++i) {
    for (int j = 0; j < n_nodes_by_x; ++j) {
      float x = min_x + j * step_by_x;
      float y = min_y + i * step_by_y;
      heights[i * n_nodes_by_x + j] = cos(x * y / M_PI);
    }
  }
}

Surface::~Surface() {
  delete[] heights;
}

void Surface::Draw() {
  glColor3ub(0, 127, 127);
  glBegin(GL_TRIANGLES);
  for (int i = 1; i < n_nodes_by_y; ++i) {
    for (int j = 1; j < n_nodes_by_x; ++j) {
      float right = min_x + j * step_by_x;
      float top = min_y + i * step_by_y;
      glVertex3f(right, top, heights[i * n_nodes_by_x + j]);
      glVertex3f(right - step_by_x, top, heights[i * n_nodes_by_x + j - 1]);
      glVertex3f(right - step_by_x, top - step_by_y,
                 heights[(i - 1) * n_nodes_by_x + j - 1]);

      glVertex3f(right, top, heights[i * n_nodes_by_x + j]);
      glVertex3f(right - step_by_x, top - step_by_y,
                 heights[(i - 1) * n_nodes_by_x + j - 1]);
      glVertex3f(right, top - step_by_y,
                 heights[(i - 1) * n_nodes_by_x + j]);
    }
  }
  glEnd();
}
