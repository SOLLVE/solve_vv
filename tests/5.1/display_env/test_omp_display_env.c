//---------test_omp_display_env.c-------------------
//
//OpenMP API Version 5.1 Aug 2021
//
//Tests the behavior of omp_display_env. This test 
//should print out the OpenMP version number and 
//the initial values of the ICV's associated with 
//the environment variables.
//------------------------------------------------

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"
#include <math.h>

int errors;

int test_omp_display_env(){
	check = omp_display_env(1);
}

int main(){
	test_omp_display_env();
}