#include "viewer.h"

matrix_t create_matrix(int rows, int columns) {
  matrix_t result;
  result.rows = rows;
  result.columns = columns;
  result.matrix = (double **)calloc(rows, sizeof(double *));
  for (int i = 0; i < rows; i++) {
    result.matrix[i] = (double *)calloc(columns, sizeof(double));
  }
  return result;
}

void remove_matrix(matrix_t *A) {
  int iRow = 0;
  for (iRow = 0; iRow < A->rows; iRow++) {
    free(A->matrix[iRow]);
  }
  free(A->matrix);
  A->columns = 0;
  A->rows = 0;
}

model *pars_obj_file(char *file_name) {
  FILE *file = NULL;
  char str[255] = {0};
  int poly = 1, row = 1;
  model *struct_mt = malloc(sizeof *struct_mt);
  if ((file = fopen(file_name, "r")) == NULL) {
    struct_mt->count_of_vertex = 0;
    struct_mt->count_of_facets = 0;
    struct_mt->matrix_of_vertexes =
        create_matrix(struct_mt->count_of_vertex + 1, 3);
    struct_mt->poligons = (struct facets *)calloc(
        struct_mt->count_of_facets + 1, sizeof(struct facets));
  } else {
    get_vertexes_and_facets(file, struct_mt);
    fseek(file, 0, SEEK_SET);
    struct_mt->matrix_of_vertexes =
        create_matrix(struct_mt->count_of_vertex + 1, 3);
    struct_mt->poligons = (struct facets *)calloc(
        struct_mt->count_of_facets + 1, sizeof(struct facets));
    while (!feof(file)) {
      fgets(str, 100, file);
      if (str[0] == 'v' && str[1] == ' ') {
        fill_array_vertex(str, &struct_mt->matrix_of_vertexes, row);
        row++;
      } else if (str[0] == 'f' && str[1] == ' ') {
        fill_matrix_facets(&struct_mt->poligons[poly], str);
        poly++;
      }
    }
    fclose(file);
  }
  return struct_mt;
}

void get_vertexes_and_facets(FILE *f, model *my_struct) {
  my_struct->count_of_vertex = 0;
  my_struct->count_of_facets = 0;
  while (!feof(f)) {
    char str[200] = {0};
    fgets(str, 200, f);
    if (str[0] == 'v' && str[1] == ' ') {
      my_struct->count_of_vertex++;
    } else if (str[0] == 'f' && str[1] == ' ') {
      my_struct->count_of_facets++;
    }
  }
}

void fill_array_vertex(char *str, matrix_t *model, int row) {
  int j = 0;
  double sign = 1;
  char tmp[255] = "\0";
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] >= '0' && str[i] <= '9') {
      while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
        if (str[i] == '.') {
          strcat(tmp, ",");
        } else {
          strncat(tmp, str + i, 1);
        }
        i++;
      }
      double vershina = atof(tmp);
      vershina *= sign;
      model->matrix[row][j] = vershina;
      j++;
      memset(tmp, 0, 255);
      sign = 1;
    } else {
      if (str[i] == '-') {
        sign = -1;
      }
    }
  }
}
void fill_matrix_facets(poligon *model, char *str) {
  int ind_facets = 0, k = 0;
  char tmp[255] = {0}, *buf;
  model->count_vertex_in_facets = 0;
  while (str[k] != '\0') {
    if (str[k] >= '0' && str[k] <= '9') {
      model->count_vertex_in_facets += 1;
      while (str[k] != '\0' && str[k] != ' ' && str[k] != '\n') {
        k++;
      }
    } else {
      k++;
    }
  }
  if (model->count_vertex_in_facets) {
    model->vertex = (int *)malloc(model->count_vertex_in_facets * sizeof(int));
    for (int i = 0; str[i] != '\0'; i++) {
      if (str[i] >= '0' && str[i] <= '9') {
        while (str[i] >= '0' && str[i] <= '9') {
          strncat(tmp, str + i, 1);
          i++;
        }
        model->vertex[ind_facets] = strtod(tmp, &buf);
        memset(tmp, 0, 255);
        if (str[i] == ' ' || str[i] == '\0' || str[i] == '\n') {
          i--;
        } else {
          while (str[i] != '\0' && str[i] != ' ' && str[i] != '\n') {
            i++;
          }
          i--;
        }
        ind_facets++;
      }
    }
  }
}
