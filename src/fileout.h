#ifndef FILEOUT_H
#define FILEOUT_H

#include "types.h"

int write_file(const char* path, pixel_value* r_channel, pixel_value* g_channel, pixel_value* b_channel, int height, int width);

#endif //FILEOUT_H
