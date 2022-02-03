#ifndef PLAY_H_INCLUDED
#define PLAY_H_INCLUDED

#include "consts.h"


void move_case(Sprite *first_square, Sprite *second_square);
Error move_player(Sprite play_board[][NUMBER_BLOCKS_WIDTH], SDL_Rect *pos_mario, Direction dir);

void itot(char stime[2], int t);
void add_time(char stime[23], int t, int bool_end);
void format_time(char stime[], int hour, int minutes, int sec);

Error play(SDL_Surface *screen, TTF_Font *font, TTF_Font *font_bold, char filename[], const int level_max);
Error win(SDL_Surface *screen, TTF_Font *font, TTF_Font *font_bold, int chrono, char filename[], int level_max);

#endif // PLAY_H_INCLUDED
