#include <stdio.h>

#include "src/fileout.h"
#include "src/types.h"

int main()
{
  pixel_value r_chan[256*256];
  pixel_value g_chan[256*256]; 
  pixel_value b_chan[256*256];

  for (int i = 0; i < 256*256; i++)
  {
    r_chan[i] = 255 - i%256;
    b_chan[i] = i%256;
    g_chan[i] = i%256;   
  }
  int file_result = write_file("testfile.ppm", r_chan, g_chan, b_chan, 256, 256);
  
  printf("Wrote file, result: %d\n", file_result);
}
