#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "transformations.h"
#include "viewer.h"

#define START                        \
  model *struct_mt;                  \
  char file_name[200] = "test1.obj"; \
  struct_mt = pars_obj_file(file_name);

#define NO_ERRORS 0

START_TEST(test_count_vertex_and_facets) {
  START;
  int vertex_orig = 8;
  int facets_orig = 12;
  ck_assert_int_eq(vertex_orig, struct_mt->count_of_vertex);
  ck_assert_int_eq(facets_orig, struct_mt->count_of_facets);
}
END_TEST

START_TEST(test_poligon_count_vertex_in_facets) {
  START;
  poligon orig[13];
  orig[1].count_vertex_in_facets = 6;
  orig[2].count_vertex_in_facets = 4;
  orig[3].count_vertex_in_facets = 3;
  orig[4].count_vertex_in_facets = 3;
  orig[5].count_vertex_in_facets = 3;
  orig[6].count_vertex_in_facets = 3;
  orig[7].count_vertex_in_facets = 3;
  orig[8].count_vertex_in_facets = 3;
  orig[9].count_vertex_in_facets = 3;
  orig[10].count_vertex_in_facets = 3;
  orig[11].count_vertex_in_facets = 6;
  orig[12].count_vertex_in_facets = 3;

  int check_count_vertex_in_facets = 0;
  for (int i = 1; i < 13; ++i) {
    if (orig[i].count_vertex_in_facets !=
        struct_mt->poligons[i].count_vertex_in_facets) {
      check_count_vertex_in_facets = 1;
    }
  }
  ck_assert_int_eq(check_count_vertex_in_facets, NO_ERRORS);
}
END_TEST

START_TEST(test_affin) {
  START;

  double xGradus = 0, yGradus = 0, zGradus = 0;

  float val = 10.0;
  move_matrix(&struct_mt->matrix_of_vertexes, val, 1,
              struct_mt->count_of_vertex);
  move_matrix(&struct_mt->matrix_of_vertexes, val, 2,
              struct_mt->count_of_vertex);
  move_matrix(&struct_mt->matrix_of_vertexes, val, 3,
              struct_mt->count_of_vertex);

  rotate_matrix(&struct_mt->matrix_of_vertexes, xGradus, 1);
  rotate_matrix(&struct_mt->matrix_of_vertexes, yGradus, 2);
  rotate_matrix(&struct_mt->matrix_of_vertexes, zGradus, 3);

  scale_matrix(&struct_mt->matrix_of_vertexes, 2, struct_mt->count_of_vertex);

  float array_of_vertex_orig[27] = {40, 40, 20, 20, 40, 20, 20, 40, 40,
                                    40, 40, 40, 40, 20, 20, 40, 20, 40,
                                    20, 20, 20, 20, 20, 40, 20, 20, 40};
  int check_array = 0;
  int k = 0;
  for (int i = 1; i <= struct_mt->count_of_vertex; i++) {
    for (int j = 0; j < 3; j++) {
      if (array_of_vertex_orig[k] !=
          struct_mt->matrix_of_vertexes.matrix[i][j]) {
        check_array = 1;
      }
      k++;
    }
  }
  ck_assert_int_eq(check_array, NO_ERRORS);
}
END_TEST

START_TEST(test_array_of_vertex) {
  START;
  float array_of_vertex_orig[27] = {10, 10, 0,  0,  10, 0,  0,  10, 10,
                                    10, 10, 10, 10, 0,  0,  10, 0,  10,
                                    0,  0,  0,  0,  0,  10, 0,  0,  0};
  int check_array = 0;
  int k = 0;
  for (int i = 1; i <= struct_mt->count_of_vertex; i++) {
    for (int j = 0; j < 3; j++) {
      if (array_of_vertex_orig[k] != struct_mt->matrix_of_vertexes.matrix[i][j])
        check_array = 1;
      k++;
    }
  }
  ck_assert_int_eq(check_array, NO_ERRORS);
}
END_TEST

START_TEST(test_no_file) {
  model *struct_mt;
  struct_mt = pars_obj_file("123.obj");
  remove_matrix(&struct_mt->matrix_of_vertexes);
  for (int i = 1; i <= struct_mt->count_of_facets + 1; i++) {
    free(struct_mt->poligons[i].vertex);
  }
  free(struct_mt->poligons);
  free(struct_mt);
}

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, test_count_vertex_and_facets);
  tcase_add_test(tc1_1, test_array_of_vertex);
  tcase_add_test(tc1_1, test_poligon_count_vertex_in_facets);
  tcase_add_test(tc1_1, test_affin);
  tcase_add_test(tc1_1, test_no_file);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
