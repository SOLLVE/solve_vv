//===---- test_declare_target_device_type_host.c  ----------------------------------===//
// 
// OpenMP API Version 5.0 
//
// The declare target directive specifies that variables, functions(C,C++ and Fortran),
// and subroutines (Fortran) are mapped to a device. If a device_type
// clause is present on the contained declare target directive, then its argument 
// determines which versions are made available. If device_type(host) is present 
// only a host version of the procedure is made available. 
// 
//===-------------------------------------------------------------------------------===//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"

#define N 1024

int a[N], b[N], c[N];  
int errors = 0;
int i = 0;

#pragma omp begin declare target
#pragma omp declare target device_type(host)
int update() { 
  #pragma omp parallel for 
  for (i = 0; i < N; i++) {
    a[i] += 1;
    b[i] += 2;
    c[i] += 3;
  }
}
#pragma omp end declare target

int test_declare_target_device_type_host() { 

  #pragma omp target map(tofrom: a,b,c) 
  {
    for (i = 0; i < N; i++) {
      a[i] += i;
      b[i] += 2 * i;
      c[i] += 3 * i;
    }
  }
  
  //on host
  update();

  for (i = 0; i < N; i++) { //check array values on host
    if ( a[i] != 2 * i + 1 || b[i] != 4 * i + 2 || c[i] != 6 * i + 3 ) {
      errors++;
    }
  }
  
  return errors;
}

int main () {
  
  //initalize arrays on host
  for (i = 0; i < N; i++) {
    a[i] = i;
    b[i] = 2 * i;
    c[i] = 3 * i;
  }

  OMPVV_TEST_AND_SET_VERBOSE(errors, test_declare_target_device_type_host());
  OMPVV_REPORT_AND_RETURN(errors);
}  
