//===--- test_target_device.c ----------------------------------------------===//
//
// OpenMP API Version 5.0 Nov 2018
//
// This test checks the target construct with device clause where device-
// modifier is either ancestor or device_num. If no device_modifier is 
// present, the behavior is the same as if device_num were present.
//
////===---------------------------------------------------------------------===//

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "ompvv.h"

#define N 1028

// Required for 'device(ancestor: 1)
#pragma omp requires reverse_offload

int test_target_device_ancestor() {

    int i, which_device;
    int a[N];
    int errors = 0; 

    for (int i = 0; i < N; i++) {
        a[i] = i;
    }

    OMPVV_TEST_AND_SET(errors, omp_get_num_devices() <= 0);
    OMPVV_ERROR_IF(omp_get_num_devices() <= 0, "Since no target devices were found, this test"
                                                 "will be skipped.");

    if (omp_get_num_devices() > 0) {

        #pragma omp target device(ancestor: 1) map(tofrom: a, which_device) 
	{
	    for (int i = 0; i < N; i++) {
                a[i] = a[i] + 2;
	    }
	    // We like to use omp_is_initial_device(), but for ancestor:
	    // "No OpenMP constructs or calls to OpenMP API runtime routines are allowed"
	    // Thus, settle on the following. Note: With unified-shared memory, it is
	    // not actually testing that this target region is executed on the initial device.
	    which_device = 42;
	}
    }

    OMPVV_TEST_AND_SET(errors, which_device != 42);
    OMPVV_ERROR_IF(which_device != 1, "Target region was executed on device. Due to ancestor device-modifier,"
                                         "this region should execute on host");

    return errors;

}

int test_target_device_device_num() {
    
    int i, target_device_num, host_device_num, first_device_num;
    int b[N];
    int errors = 0; 

    for (int i = 0; i < N; i++) {
        b[i] = i;
    }

    host_device_num = omp_get_device_num(); 

    
    OMPVV_TEST_AND_SET(errors, omp_get_num_devices() <= 0);
    OMPVV_ERROR_IF(omp_get_num_devices() <= 0, "Since no target devices were found, this test"
                                                 "will be skipped");
	
    if (omp_get_num_devices() > 0) {
         
        first_device_num = omp_get_num_devices() - 1;
        #pragma omp target device(device_num: first_device_num) map(tofrom: b, target_device_num) 
        {
            for (int i = 0; i < N; i++) {
                b[i] = b[i] + 2;
            }
 
            target_device_num = omp_get_device_num();
        }
    }

    OMPVV_TEST_AND_SET(errors, target_device_num == host_device_num);
    OMPVV_ERROR_IF(target_device_num == host_device_num, "Target region was executed on host," 
                   "this region should execute on specified target device number");   

    return errors;

}

int main() {

    int errors = 0;
   
    OMPVV_TEST_OFFLOADING;

    OMPVV_TEST_AND_SET_VERBOSE(errors, test_target_device_ancestor());
    OMPVV_TEST_AND_SET_VERBOSE(errors, test_target_device_device_num());

    OMPVV_REPORT_AND_RETURN(errors);
}
