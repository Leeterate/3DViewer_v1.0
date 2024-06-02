#ifndef SRC_VIEWER_H_
#define SRC_VIEWER_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

typedef struct facets {
  int *vertex;
  int count_vertex_in_facets;
} poligon;

typedef struct model {
  int count_of_vertex;
  int count_of_facets;
  matrix_t matrix_of_vertexes;
  poligon *poligons;
} model;

matrix_t create_matrix(int rows, int columns);
void remove_matrix(matrix_t *A);
model *pars_obj_file(char *file_name);
void get_vertexes_and_facets(FILE *f, model *my_struct);
void fill_matrix_facets(poligon *model, char *str);
void fill_array_vertex(char *str, matrix_t *model, int row);

#endif  // SRC_VIEWER_H_
