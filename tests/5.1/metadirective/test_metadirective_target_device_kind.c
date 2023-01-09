//===--------------------- test_metadirective_target_device_kind.c ---------------------===//
//
// OpenMP API Version 5.1 Nov 2020
// 
// Tests that target_device is recognized & will work properly when matching an nvidia
// GPU. More specifically, uses the kind clause to see if gpu or nohost is recognized
// as the target device's "kind" implementation. 
//
////===---------------------------------------------------------------------===//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"

#define N 1024

int test_metadirective_target_device() {
   int errors = 0;
   int A[N];

   for (int i = 0; i < N; i++) {
      A[i] = 1;
   }

   #pragma omp target map(tofrom: A)
   {
      #pragma omp parallel num_threads(4)
      {
      // Expect that one of these are true, so the array should be set to masked thread num (0)
      #pragma omp metadirective \
         when( target_device={kind(gpu)}: masked ) \
         when( target_device={kind(nohost)}: masked ) \
         default( for)
            for (int i = 0; i < N; i++) {
               A[i] = omp_get_thread_num();
            }
      }
   }

   for (int i = 0; i < N; i++) {
      OMPVV_TEST_AND_SET(errors, A[i] != 0);
   }

   OMPVV_INFOMSG("Test ran with a number of available devices greater than 0");

   return errors;
}

int main () {
  int errors = 0;
  OMPVV_TEST_OFFLOADING;

  if (omp_get_num_devices() > 0) {
    errors = test_metadirective_target_device();
  } else {
    OMPVV_INFOMSG("Cannot test target_device without a target device!")
  }

  OMPVV_REPORT_AND_RETURN(errors);

  return 0;
}
