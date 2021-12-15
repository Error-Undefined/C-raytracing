#include <stdio.h>

#include "renderer.h"
#include "fileout.h"
#include "vector3.h"

void render()
{
  color color_data[256*256];

  for (int i = 0; i < 256*256; i++)
  {
    color_data[i].x = 255 - i%128;
    color_data[i].y = i%128;
    color_data[i].z = i%256;   
  }
  int file_result = write_file("testfile.ppm", color_data, 256, 256);
  
  printf("Wrote file, result: %d\n", file_result);
}
