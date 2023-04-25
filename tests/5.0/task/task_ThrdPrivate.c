/*
 *Test case description: 
 * 1) The aim of the test case is to verify the functionality
 * of threadprivate construct when used under the influence of task construct
 * FuncABC() functions tests if thread 0 still has updated GlobalVar variable
 * while other threads observed GlobalVar still see 0
 * 2) In the main there is a check to verify GlobalVar is 0. This is because
 * what ever changes happen inside the task block are not reflected outside
 *
 *
 * */


#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "omp.h"
#include "ompvv.h"
#define THREADS 100

#define INIT_VAL 9999


int IfTstFailed = 0;

int GlobalVar;
#pragma omp threadprivate(GlobalVar)

void FuncABC() {
  int ThrdId = omp_get_thread_num();

  {
    if (ThrdId == 0) {
	    printf("Inside Thrd: 0\n");
      if ((GlobalVar != INIT_VAL)) {
        IfTstFailed++;
        printf("Thrd: 0: Inside foo: GlobalVar: %d\n", GlobalVar);
      }
    } else {
      if (GlobalVar != 0) {
        IfTstFailed++;
      }
    }
  }
}


int main() {
  int errors = 0;
  omp_set_num_threads(THREADS);
  GlobalVar = INIT_VAL;
#pragma omp parallel
  {
    int ThrdId = omp_get_thread_num();
#pragma omp task if(0)
    {
    FuncABC();
#pragma omp taskwait
    GlobalVar = ThrdId;
    }
  }

  if (GlobalVar != 0) {
      IfTstFailed++;
      printf("In main: GlobalVar: %d\n",GlobalVar);
  }
  OMPVV_TEST_AND_SET_VERBOSE(errors, (IfTstFailed != 0));
  OMPVV_REPORT_AND_RETURN(errors);
}
