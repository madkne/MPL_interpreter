//
// Created by amindelavar on 11/30/2018.
//

#include <MPL/system.h>

void __syscall_exit(int32 i) {
  if (is_programmer_debug >= 1) {
    AppStartedClock = clock() - AppStartedClock;
    double time_taken = ((double) AppStartedClock) / CLOCKS_PER_SEC; // in seconds
    //exit state
    String exit_state = 0;
    if (i == 0)str_init(&exit_state, "EXIT_SUCCESS");
    else str_init(&exit_state, "EXIT_FAILURE");
    if (time_taken == 0) {
      time_taken += 0.000001;
      //printf("BAD\n");
    } else
      printf("Process finished during %.6f seconds with exit code %i (%s)\n", time_taken, i, exit_state);
  }
  exit(i);
}

