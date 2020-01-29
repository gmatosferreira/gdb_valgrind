////////////////////////////////////////////////////////////////////////////////////////////////////
//
// code to measure the elapsed time used by a program fragment (an almost copy of elapsed_time.h)
//
// use as follows:
//
//   (void)elapsed_time();
//   // put your code to be time measured here
//   dt = elapsed_time();
//   // put morecode to be time measured here
//   dt = elapsed_time();
//
// elapsed_time() measures the CPU time between consecutive calls
//

#if defined(__linux__) || defined(__APPLE__)

//
// GNU/Linux and MacOS code to measure elapsed time
//

#include <time.h>

static double elapsed_time(void)
{
  static struct timespec last_time, current_time;

  last_time = current_time;
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &current_time) != 0)
    return -1.0; // clock_gettime() failed!!!
  return ((double)current_time.tv_sec - (double)last_time.tv_sec) + 1.0e-9 * ((double)current_time.tv_nsec - (double)last_time.tv_nsec);
}

#endif

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)

//
// Microsoft Windows code to measure elapsed time
//

#include <windows.h>

static double elapsed_time(void)
{
  static LARGE_INTEGER frequency, last_time, current_time;
  static int first_time = 1;

  if (first_time != 0)
  {
    QueryPerformanceFrequency(&frequency);
    first_time = 0;
  }
  last_time = current_time;
  QueryPerformanceCounter(&current_time);
  return (double)(current_time.QuadPart - last_time.QuadPart) / (double)frequency.QuadPart;
}

#endif