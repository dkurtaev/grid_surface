#include "include/surface.hpp"

#include <math.h>
#include <stdlib.h>
#include <string.h>

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

  // Setup normals.
  //   y+dy  _____z6___z5
  //      |***/ | 5 / |
  //      |**/  |  /  |
  //      |*/ 6 | / 4 |
  //    y z1---z0----z4
  //      | 1 / | 3 /*|
  //      |  /  |  /**|
  //      | / 2 | /***|
  // y-dy z2----z3----*
  //     x-dx   x    x+dx
  normals_array = new float[n_nodes_by_x * n_nodes_by_y * 3];
  for (int i = 1; i < n_nodes_by_y - 1; ++i) {
    for (int j = 1; j < n_nodes_by_x - 1; ++j) {
      int base_idx = i * n_nodes_by_x + j;

      float z1 = vertices_array[(base_idx - 1) * 3 + 2];
      float z2 = vertices_array[(base_idx - n_nodes_by_x - 1) * 3 + 2];
      float z3 = vertices_array[(base_idx - n_nodes_by_x) * 3 + 2];
      float z4 = vertices_array[(base_idx + 1) * 3 + 2];
      float z5 = vertices_array[(base_idx + n_nodes_by_x + 1) * 3 + 2];
      float z6 = vertices_array[(base_idx + n_nodes_by_x) * 3 + 2];

      float nx = 1.0f / 6 * step_by_y * (2 * z1 + z2 - z3 - 2 * z4 - z5 + z6);
      float ny = 1.0f / 6 * step_by_x * (-z1 + z2 + 2 * z3 + z4 - z5 - 2 * z6);

      normals_array[base_idx * 3] = nx;
      normals_array[base_idx * 3 + 1] = ny;
      normals_array[base_idx * 3 + 2] = step_by_x * step_by_y;
    }
  }

  // Normals for left and right borders.
  float* left_border = normals_array + n_nodes_by_x * 3;  // From 1st line.
  float* right_border = normals_array + (2 * n_nodes_by_x - 1) * 3;
  for (int i = 1; i < n_nodes_by_y - 1; ++i) {
    memcpy(left_border, left_border + 3, 3 * sizeof(float));
    memcpy(right_border, right_border - 3, 3 * sizeof(float));
    left_border += n_nodes_by_x * 3;
    right_border += n_nodes_by_x * 3;
  }

  // Normals for top and bottom borders.
  memcpy(normals_array, normals_array + n_nodes_by_x * 3,
         n_nodes_by_x * 3 * sizeof(float));
  memcpy(normals_array + (n_nodes_by_y - 1) * n_nodes_by_x * 3,
         normals_array + (n_nodes_by_y - 2) * n_nodes_by_x * 3,
         n_nodes_by_x * 3 * sizeof(float));
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
  float* vertices_offset = vertices_array;
  float* normals_offset = normals_array;
  for (int i = 0; i < n_nodes_by_y - 1; ++i) {
    for (int i = 0; i < n_nodes_by_x - 1; ++i) {
      // Bottom left.
      glNormal3fv(normals_offset);
      glVertex3fv(vertices_offset);
      // Top right.
      glNormal3fv(normals_offset + (n_nodes_by_x + 1) * 3);
      glVertex3fv(vertices_offset + (n_nodes_by_x + 1) * 3);
      // Top left.
      glNormal3fv(normals_offset + n_nodes_by_x * 3);
      glVertex3fv(vertices_offset + n_nodes_by_x * 3);

      glNormal3fv(normals_offset);
      glVertex3fv(vertices_offset);
      // Bottom right.
      glNormal3fv(normals_offset + 3);
      glVertex3fv(vertices_offset + 3);
      // Top right.
      glNormal3fv(normals_offset + (n_nodes_by_x + 1) * 3);
      glVertex3fv(vertices_offset + (n_nodes_by_x + 1) * 3);
      normals_offset += 3;
      vertices_offset += 3;
    }
    // Skip right border nodes.
    normals_offset += 3;
    vertices_offset += 3;
  }
  glEnd();
}
