#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include "consts.h"

int wheel_level_i(int i, Wheel wheel, const int level_max);
Error choice_level(SDL_Surface *screen, TTF_Font *font, TTF_Font *font_bold);

#endif // LEVEL_H_INCLUDED
