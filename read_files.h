#ifndef READ_FILES_H_INCLUDED
#define READ_FILES_H_INCLUDED

#include "consts.h"

char itoc(int i);
int ctoi (char c);

Error read_level(Sprite play_board[][NUMBER_BLOCKS_WIDTH], char filename[], SDL_Rect *pos_mario);
Error incr_array_pos(int *i, int *j, const int n, const int m);
Sprite itosprite(int c);

void set_white_down(SDL_Surface *surf);
int number_levels(void);
Error update_max_level_file(int new_max_level);

#endif // READ_FILES_H_INCLUDED
