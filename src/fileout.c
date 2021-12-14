#include <stdio.h>

#include "fileout.h"
#include "types.h"

int write_file(const char* path, pixel_value* r_channel, pixel_value* g_channel, pixel_value* b_channel, int height, int width)
{
  FILE* fp;
  fp = fopen(path, "w+");

  if (fp == NULL)
  {
    return 1;
  }
  //Write .ppm file header
  fputs("P3\n", fp);
  fprintf(fp, "%d %d\n255\n", width, height);

  for(int h = 0;  h < height; h++)
  {
    for (int w = 0; w < width; w++)
    {
      fprintf(fp, "%d %d %d\n", r_channel[256*h + w], g_channel[256*h + w], b_channel[256*h + w]);
    }
  }

  fclose(fp);
  return 0;
}
