#include <stdio.h>
#include <stdlib.h>

#include "src/renderer.h"

int main(int argc, char** argv)
{
  if (argc < 3)
  { 
    //Render for example at 16:9 ratio, 800x450 now
    render(450, 800);
    exit(0);
  }
  int h = atoi(argv[1]);
  int w = atoi(argv[2]);
  if(h == 0 || w == 0)
  {
    printf("Could not parse command line args\n");
    exit(1);
  }
  render(h, w);
}
