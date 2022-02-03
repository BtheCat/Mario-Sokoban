#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "consts.h"
#include "play.h"
#include "editor.h"
#include "read_files.h"
#include "level.h"

int main(int argc, char* argv[]) {
    SDL_Surface *screen = NULL, *menu = NULL, *icon = NULL, *txt_menu = NULL;
    SDL_Rect pos_menu;
    SDL_Event event;
    Error err = NO_ERROR;
    int pause = 1;

    SDL_Init(SDL_INIT_VIDEO);
    pos_menu.x = 0;
    pos_menu.y = 0;

    screen = SDL_SetVideoMode(WIDTH_SCREEN, HEIGHT_SCREEN, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (screen == NULL) {
        fprintf(stderr, "Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return ERROR_LOAD_SDL;
    }
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return ERROR_LOAD_TTF;
    }

    TTF_Font *font = NULL;
    TTF_Font *font_bold = NULL;

    font = TTF_OpenFont(FONT_TTF, FONT_SIZE);
    font_bold = TTF_OpenFont(FONT_TTF_BOLD, FONT_SIZE);

    if (font == NULL || font_bold == NULL) {
        fprintf(stderr, "Erreur d'initialisation d'une police\n");
        return ERROR_LOAD_FONT;
    }

    menu = IMG_Load(MENU_BMP_PATH);
    icon = IMG_Load(CASE_BMP_PATH);

    SDL_WM_SetCaption(APP_NAME, NULL);
    SDL_WM_SetIcon(icon, NULL);
    SDL_FillRect(screen, NULL, BACKGROUND_COLOR_WHITE_SCREEN);
    SDL_BlitSurface(menu, NULL, screen, &pos_menu);

    SDL_Color black_color = {0, 0, 0};

    txt_menu = TTF_RenderText_Blended(font, "Menu", black_color);

    while(pause) {
        SDL_WaitEvent(&event);
        switch(event.type) {
        case SDL_QUIT:
            pause = 0;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_1:
            case SDLK_p:
            case SDLK_j:
            case SDLK_KP1:
                err = choice_level(screen, font, font_bold);
                break;
            case SDLK_e:
            case SDLK_2:
            case SDLK_KP2:
                err = edit_level(screen, font, font_bold);
                break;
            case SDLK_q:
            case SDLK_KP3:
            case SDLK_3:
                pause = 0;
                break;
            default:
                break;
            }
        default:
            break;
        }
        SDL_FillRect(screen, NULL, BACKGROUND_COLOR_WHITE_SCREEN);

        pos_menu.x = 0;
        pos_menu.y = 0;
        SDL_BlitSurface(menu, NULL, screen, &pos_menu);

        pos_menu.x = TXT_SCREEN_POS_BOTTOM_X;
        pos_menu.y = TXT_POS_BOTTOM_Y;
        SDL_BlitSurface(txt_menu, NULL, screen, &pos_menu);

        SDL_Flip(screen);

        if (err != NO_ERROR) {
            pause = 0;
        }
    }

    SDL_FreeSurface(menu);
    SDL_FreeSurface(icon);
    SDL_FreeSurface(txt_menu);
    TTF_CloseFont(font);
    TTF_CloseFont(font_bold);
    TTF_Quit();
    SDL_Quit();

    return err;
}
