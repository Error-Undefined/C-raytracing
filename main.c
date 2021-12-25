#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "src/renderer.h"
#include "camera.h"

#ifdef PROFILE_CODE
#include <gperftools/profiler.h>
#endif //PROFILE_CODE

int main(int argc, char** argv)
{
  #ifdef PROFILE_CODE
  #include <gperftools/profiler.h>
  ProfilerStart("test.log");
  #endif //PROFILE_CODE

  struct camera c;
  c.focal_length = 1.5;
  c.camera_center = (vec3) {2, 0, -2};
  vec3 camera_to = {1,0,-0.4};
  c.view_dir = vec3_sub_new(&camera_to, &c.camera_center);
  c.camera_up = (vec3) {0,-1,0};
  c.aperture = 0.1;
  c.focus_distance = 1;

  int t0 = time(NULL);
  if (argc < 3)
  { 
    //Render for example at 16:9 ratio, 800x450 now
    render(450, 800, &c);
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
  render(h, w, &c);
  int t1 = time(NULL);
  printf("Rendering took %d seconds\n", t1-t0);

  #ifdef PROFILE_CODE
  #include <gperftools/profiler.h>
  ProfilerStop();
  #endif //PROFILE_CODE
}
