#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#include "src/renderer.h"

#ifdef PROFILE_CODE
#include <gperftools/profiler.h>
#endif //PROFILE_CODE

int main(int argc, char** argv)
{
  #ifdef PROFILE_CODE
  #include <gperftools/profiler.h>
  ProfilerStart("test.log");
  #endif //PROFILE_CODE

  int t0 = time(NULL);
  if (argc < 3)
  { 
    //Render for example at 16:9 ratio, 800x450 now
    render(450, 800, NULL);
    int t1 = time(NULL);
    printf("Rendering took %d seconds\n", t1-t0);
    exit(0);
  }
  int h = atoi(argv[1]);
  int w = atoi(argv[2]);
  if(h == 0 || w == 0)
  {
    printf("Could not parse command line args\n");
    exit(1);
  }
  render(h, w, NULL);
  int t1 = time(NULL);
  printf("Rendering took %d seconds\n", t1-t0);

  #ifdef PROFILE_CODE
  #include <gperftools/profiler.h>
  ProfilerStop();
  #endif //PROFILE_CODE
}
