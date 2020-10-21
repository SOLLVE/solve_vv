//===------ test_allocate_allocators.c ------------------------------------===//
//
// OpenMP API Version 5.0 Nov 2018
//
// Tests the allocate directive with allocator clause, based on the OpenMP
// 5.0 example for allocators. The allocator testing first creates
// an allocator, with 64-byte alignment and the default memory space,
// then checks that 64-byte alignment is correct and that the memory can
// be written to in the target region. The tests checks that the values
// were written correctly, and then frees the memory and deletes the
// allocator.
//
//===----------------------------------------------------------------------===//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"

#define N 1024

int test_requires() {
  int errors = 0;

  int* x;
  omp_memspace_handle_t x_memspace = omp_default_mem_space;
  omp_alloctrait_t x_traits[1] = {omp_atk_alignment, 64};
  omp_allocator_handle_t x_alloc = omp_init_allocator(x_memspace, 1, x_traits);

#pragma omp allocate(x) allocator(x_alloc)

  x = (int *) malloc(N*sizeof(int));

#pragma omp parallel for simd simdlen(16) aligned(x: 64)
  for (int i = 0; i < N; i++) {
    x[i] = i;
  }

  for (int i = 0; i < N; i++) {
    OMPVV_TEST_AND_SET_VERBOSE(errors, x[i] != i);
  }

  free(x);
  omp_destroy_allocator(x_alloc);

  return errors;
}

int main() {
  OMPVV_TEST_OFFLOADING;

  int errors = 0;

  OMPVV_TEST_AND_SET_VERBOSE(errors, test_requires() != 0);

  OMPVV_REPORT_AND_RETURN(errors);
}
