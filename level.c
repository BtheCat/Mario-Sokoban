#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "level.h"
#include "read_files.h"
#include "play.h"

int wheel_level_i(int level_i, Wheel wheel, const int level_max) {
    if (wheel == WHEEL_UP) {
        return ((level_i + 1) % level_max);
    } else {
        if (!level_i) {
            return level_max - 1;
        } else {
            return (level_i - 1);
        }
    }
}

Error choice_level(SDL_Surface *screen, TTF_Font *font, TTF_Font *font_bold) {
    SDL_Surface *level_img = NULL, *txt_level = NULL, *txt_incr = NULL, *case_ok_img = NULL;
    SDL_Rect pos_level, pos_txt, pos_incr, pos_case_ok;
    SDL_Event event;
    SDL_Color black_color = {0, 0, 0}, white_color = {255, 255, 255};

    TTF_Font *font_incr = NULL;
    font_incr = TTF_OpenFont(FONT_TTF, 30);

    int level_i = 1;
    char level_s[3] = "";
    char level_file_name[LENGTH_LEVEL_FILENAME] = LEVEL_FILE_DEFAULT_PATH;
    const int level_max = number_levels();
    //const int level_max = NUMBER_LEVEL;

    level_s[0] = ((level_i / 10 ) + '0');
    level_s[1] = ((level_i % 10) + '0');
    level_s[2] = ' ';

    pos_level.x = 0;
    pos_level.y = 0;
    pos_txt.x = TXT_SCREEN_POS_BOTTOM_X;
    pos_txt.y = TXT_POS_BOTTOM_Y;

    pos_incr.x = TXT_LEVEL_POS_BOTTOM_X;
    pos_incr.y = TXT_LEVEL_POS_BOTTOM_Y;

    pos_case_ok.x = TXT_CASE_POS_BOTTOM_X;
    pos_case_ok.y = TXT_CASE_POS_BOTTOM_Y;

    Error err = NO_ERROR;
    level_img = IMG_Load(LEVEL_BMP_PATH);
    case_ok_img = IMG_Load(CASE_OK_BMP_PATH);
    txt_level = TTF_RenderText_Blended(font, "Choix niveau", black_color);

    txt_incr = TTF_RenderText_Shaded(font_incr, level_s, white_color, black_color);

    int pause = 1, play_again = 0;

    SDL_EnableKeyRepeat(100, 100);

    while(pause) {
        SDL_WaitEvent(&event);
        switch(event.type) {
        case SDL_QUIT:
            pause = 0;
            break;

        case SDL_MOUSEBUTTONUP:
            switch (event.button.button) {

            case SDL_BUTTON_WHEELUP:
                level_i = wheel_level_i(level_i, WHEEL_UP, level_max);
                break;

            case SDL_BUTTON_WHEELDOWN:
                level_i = wheel_level_i(level_i, WHEEL_DOWN, level_max);
                break;

            default:
                break;
            }

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_p:
            case SDLK_KP_ENTER:
            case SDLK_RETURN:
                play_again = 1;
                pause = 0;
                break;

            case SDLK_ESCAPE:
                pause = 0;
                break;

            case SDLK_KP_PLUS:
            case SDLK_PLUS:
            case SDLK_UP:
                level_i = wheel_level_i(level_i, WHEEL_UP, level_max);
                break;

            case SDLK_KP_MINUS:
            case SDLK_MINUS:
            case SDLK_DOWN:
                level_i = wheel_level_i(level_i, WHEEL_DOWN, level_max);

            default:
                break;
            }
        }

        SDL_FillRect(screen, NULL, BACKGROUND_COLOR_WHITE_SCREEN);
        SDL_BlitSurface(txt_level, NULL, screen, &pos_txt);
        SDL_BlitSurface(case_ok_img, NULL, screen, &pos_case_ok);
        set_white_down(level_img);


        level_s[0] = ((level_i / 10 ) + '0');
        level_s[1] = ((level_i % 10) + '0');

        level_file_name[LEVEL_POS_10] = level_s[0];
        level_file_name[LEVEL_POS_1] = level_s[1];

        txt_incr = TTF_RenderText_Shaded(font_incr, level_s, white_color, black_color);
        SDL_SetColorKey(txt_incr, SDL_SRCCOLORKEY, SDL_MapRGB(txt_incr->format, 0, 0, 0));
        SDL_BlitSurface(txt_incr, NULL, screen, &pos_incr);
        SDL_BlitSurface(level_img, NULL, screen, &pos_level);

        SDL_Flip(screen);
    }

    SDL_FreeSurface(level_img);
    SDL_FreeSurface(txt_level);
    SDL_FreeSurface(txt_incr);
    SDL_FreeSurface(case_ok_img);
    TTF_CloseFont(font_incr);

    if (play_again)
        err = play(screen, font, font_bold, level_file_name, level_max);

    return err;
}
