#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include "consts.h"

Sprite sprite_wheel(Sprite s, Wheel wheel);
Error check_edit(Sprite edit_board[][NUMBER_BLOCKS_WIDTH]);
Error save_edit(Sprite edit_board[][NUMBER_BLOCKS_WIDTH]);
Error edit_level(SDL_Surface *screen, TTF_Font *font, TTF_Font *font_bold);

#endif // EDITOR_H_INCLUDED
