#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "viewer.h"
#ifdef __linux__
#define M_PI 3.14159265358979323846
#endif

void scale_matrix(matrix_t *matrix, double scale, int count_of_vertex);
// второй аргумент - величина изменения, третий - номер оси, 1 для x, 2 для y и
// т.д.
void move_matrix(matrix_t *matrix, float delta, unsigned int direction,
                 int count_of_vertex);
void rotate_matrix(matrix_t *matrix, double angle, unsigned int direction);

#endif  // TRANSFORMATIONS_H
