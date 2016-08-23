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
  coordinates = new float[n_nodes_by_x * n_nodes_by_y * 3];
  int offset = 0;
  for (int i = 0; i < n_nodes_by_y; ++i) {
    for (int j = 0; j < n_nodes_by_x; ++j) {
      float x = min_x + j * step_by_x;
      float y = min_y + i * step_by_y;
      coordinates[offset++] = x;
      coordinates[offset++] = y;
      coordinates[offset++] = sin(x) * sin(y) + 1;
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
  normals = new float[n_nodes_by_x * n_nodes_by_y * 3];
  memset(normals, 0, sizeof(float) * n_nodes_by_x * n_nodes_by_y * 3);
  for (int i = 1; i < n_nodes_by_y - 1; ++i) {
    for (int j = 1; j < n_nodes_by_x - 1; ++j) {
      int base_idx = i * n_nodes_by_x + j;

      float z1 = coordinates[(base_idx - 1) * 3 + 2];
      float z2 = coordinates[(base_idx - n_nodes_by_x - 1) * 3 + 2];
      float z3 = coordinates[(base_idx - n_nodes_by_x) * 3 + 2];
      float z4 = coordinates[(base_idx + 1) * 3 + 2];
      float z5 = coordinates[(base_idx + n_nodes_by_x + 1) * 3 + 2];
      float z6 = coordinates[(base_idx + n_nodes_by_x) * 3 + 2];

      float nx = 1.0f / 6.0f * step_by_y * (2 * z1 + z2 - z3 - 2 * z4 - z5 + z6);
      float ny = 1.0f / 6.0f * step_by_x * (-z1 + z2 + 2 * z3 + z4 - z5 - 2 * z6);
      float nz = step_by_x * step_by_y;

      float norm = sqrt(nx * nx + ny * ny + nz * nz);
      normals[base_idx * 3] = nx / norm;
      normals[base_idx * 3 + 1] = ny / norm;
      normals[base_idx * 3 + 2] = nz / norm;
    }
  }

  // Normals for left and right borders.
  for (int i = 0; i < n_nodes_by_y; ++i) {
    int base_idx = (i * n_nodes_by_x) * 3;
    memcpy(normals + (i * n_nodes_by_x) * 3,
           normals + (i * n_nodes_by_x + 1) * 3,
           3 * sizeof(float));
    memcpy(normals + ((i + 1) * n_nodes_by_x - 1) * 3,
           normals + ((i + 1) * n_nodes_by_x - 2) * 3,
           3 * sizeof(float));
  }

  // Normals for top and bottom borders.
  memcpy(normals, normals + n_nodes_by_x * 3, n_nodes_by_x * 3 * sizeof(float));
  memcpy(normals + (n_nodes_by_y - 1) * n_nodes_by_x * 3,
         normals + (n_nodes_by_y - 2) * n_nodes_by_x * 3,
         n_nodes_by_x * 3 * sizeof(float));
}

Surface::~Surface() {
  delete[] coordinates;
  delete[] indices;
}

void Surface::Draw() {
  glColor3ub(0, 127, 127);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, normals);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, coordinates);
  glDrawElements(GL_TRIANGLES, 3 * n_triangles, GL_UNSIGNED_SHORT, indices);
}
