#include "include/surface.hpp"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <GL/freeglut.h>

Surface::Surface(float min_x, float max_x, float min_y, float max_y,
                 int n_nodes_by_x, int n_nodes_by_y) {
  float step_by_x = (max_x - min_x) / (n_nodes_by_x - 1);
  float step_by_y = (max_y - min_y) / (n_nodes_by_y - 1);

  // Setup coordinates: (x, y, z) point.
  vertices_array = new float[n_nodes_by_x * n_nodes_by_y * 3];
  float* vertices_offset = vertices_array;
  for (int i = 0; i < n_nodes_by_y; ++i) {
    for (int j = 0; j < n_nodes_by_x; ++j) {
      float x = min_x + j * step_by_x;
      float y = min_y + i * step_by_y;
      vertices_offset[0] = x;
      vertices_offset[1] = y;
      vertices_offset[2] = sin(x) * sin(y) + 1;
      vertices_offset += 3;
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
  indices_array = new unsigned short[n_triangles * 3];
  unsigned short* indices_offset = indices_array;
  for (int i = 0; i < n_nodes_by_y - 1; ++i) {
    for (int j = 0; j < n_nodes_by_x - 1; ++j) {
      int base_idx = (i * n_nodes_by_x) + j;
      indices_offset[0] = base_idx;  // Bottom left vertex.
      indices_offset[1] = base_idx + n_nodes_by_x + 1;  // Top right vertex.
      indices_offset[2] = base_idx + n_nodes_by_x;  // Top left vertex.
      indices_offset += 3;

      indices_offset[0] = base_idx;
      indices_offset[1] = base_idx + 1;  // Right vertex.
      indices_offset[2] = base_idx + n_nodes_by_x + 1;  // Top right vertex.
      indices_offset += 3;
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
  delete[] normals_array;
  delete[] indices_array;
}

void Surface::Draw() {
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, normals_array);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices_array);
  glDrawElements(GL_TRIANGLES, 3 * n_triangles, GL_UNSIGNED_SHORT,
                 indices_array);
}
