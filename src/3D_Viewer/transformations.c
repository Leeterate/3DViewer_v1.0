#include "transformations.h"

#include "viewer.h"

void move_matrix(matrix_t *matrix, float delta, unsigned int direction,
                 int count_of_vertex) {
  for (int i = 1; i <= count_of_vertex; i++)
    matrix->matrix[i][(direction - 1)] += delta;
}

void rotate_matrix(matrix_t *matrix, double angle, unsigned int direction) {
  angle = (angle * M_PI) / 180;
  double x, y, z;
  if (direction == 1) {
    for (int i = 1; i < matrix->rows; i++) {
      y = matrix->matrix[i][1];
      z = matrix->matrix[i][2];
      matrix->matrix[i][1] = cos(angle) * y + sin(angle) * z;
      matrix->matrix[i][2] = -sin(angle) * y + cos(angle) * z;
    }
  } else if (direction == 2) {
    for (int i = 1; i < matrix->rows; i++) {
      x = matrix->matrix[i][0];
      z = matrix->matrix[i][2];
      matrix->matrix[i][0] = cos(angle) * x - sin(angle) * z;
      matrix->matrix[i][2] = sin(angle) * x + cos(angle) * z;
    }
  } else if (direction == 3) {
    for (int i = 1; i < matrix->rows; i++) {
      x = matrix->matrix[i][0];
      y = matrix->matrix[i][1];
      matrix->matrix[i][0] = cos(angle) * x + sin(angle) * y;
      matrix->matrix[i][1] = -sin(angle) * x + cos(angle) * y;
    }
  }
}

void scale_matrix(matrix_t *matrix, double scale, int count_of_vertex) {
  for (int i = 1; i <= count_of_vertex; i++) {
    for (int j = 0; j < 3; j++) {
      matrix->matrix[i][j] *= scale;
    }
  }
}
