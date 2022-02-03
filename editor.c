#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "editor.h"
#include "read_files.h"

Sprite sprite_wheel(Sprite s, Wheel wheel) {
    if (wheel == WHEEL_UP) {
        return ((s % 5) + 1);
    } else {
        if (s > 1)
            return (s - 1);
        return 5;
    }
}

int spritetoi(Sprite s) {
    switch(s) {
    case WALL:
        return '1';
        break;
    case CASE:
        return '2';
        break;
    case OBJECTIVE:
        return '3';
        break;
    case MARIO:
        return '4';
        break;
    case CASE_OK:
        return '5';
        break;
    default:
        return '0';
        break;
    }
}

Error check_edit(Sprite edit_board[][NUMBER_BLOCKS_WIDTH]) {
    return NO_ERROR;
}

Error save_edit(Sprite edit_board[][NUMBER_BLOCKS_WIDTH]) {
    FILE* save_file = NULL;
    Error err = NO_ERROR;

    char filename[] = LEVEL_PLAYER_FILE_PATH;

    save_file = fopen(filename, "w");

    if (save_file == NULL) {
        fprintf(stderr, "Echec d'ouverture du fichier %s.\n", filename);
        err = ERROR_WRITE_FILE_OPPENING;
    } else {
        int i = 0, j = 0;

        for (i = 0 ; i < NUMBER_BLOCKS_HEIGHT ; i++) {
            for (j = 0 ; j < NUMBER_BLOCKS_WIDTH ; j++) {
                fputc(spritetoi(edit_board[i][j]), save_file);
            }
        }
    }

    fclose(save_file);
    return err;
}

Error edit_level(SDL_Surface *screen, TTF_Font *font, TTF_Font *font_bold) {
    SDL_Surface *case_img = NULL, *case_ok_img = NULL, *wall_img = NULL,
        *objective_img = NULL, *mario_img = NULL, *img_focused = NULL, *grid;
    SDL_Event event;
    SDL_Rect pos_cursor, pos_mouse, track_mouse, pos_grid;

    Sprite edit_board[NUMBER_BLOCKS_HEIGHT][NUMBER_BLOCKS_WIDTH] = {{EMPTY}};

    int pause = 1;
    int save = 0;
    Sprite sprite_focused = WALL;

    char filename[LENGTH_LEVEL_FILENAME] = LEVEL_PLAYER_FILE_PATH;

    Error err = read_level(edit_board, filename, NULL);

    if(err == ERROR_READ_FILE_OPPENING) {
        return err;
    }

    pos_cursor.x = 0;
    pos_cursor.y = 0;
    pos_grid.x = 0;
    pos_grid.y = 0;

    case_img = IMG_Load(CASE_BMP_PATH);
    case_ok_img = IMG_Load(CASE_OK_BMP_PATH);
    wall_img = IMG_Load(WALL_BMP_PATH);
    objective_img = IMG_Load(OBJECTIVE_BMP_PATH);
    mario_img = IMG_Load(MARIO_DOWN_BMP_PATH);
    grid = IMG_Load(GRID_BMP_PATH);
    set_white_down(grid);
    set_white_down(mario_img);
    set_white_down(objective_img);

    img_focused = wall_img;

    int i = 0, j = 0;

    SDL_EnableKeyRepeat(100, 10);

    while (pause) {
        SDL_WaitEvent(&event);
        switch(event.type) {
        case SDL_QUIT:
            pause = 0;
            break;

        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
            case SDL_BUTTON_LEFT:
                pos_mouse.x = event.button.y / SIZE_BLOCK;
                pos_mouse.y = event.button.x / SIZE_BLOCK;

                if (pos_mouse.y < NUMBER_BLOCKS_WIDTH)
                    edit_board[pos_mouse.x][pos_mouse.y] = sprite_focused;

                break;

            case SDL_BUTTON_RIGHT:
            case SDL_BUTTON_MIDDLE:
                pos_mouse.x = event.button.y / SIZE_BLOCK;
                pos_mouse.y = event.button.x / SIZE_BLOCK;

                edit_board[pos_mouse.x][pos_mouse.y] = EMPTY;
                break;

            case SDL_BUTTON_WHEELUP:
                sprite_focused = sprite_wheel(sprite_focused, WHEEL_UP);
                break;

            case SDL_BUTTON_WHEELDOWN:
                sprite_focused = sprite_wheel(sprite_focused, WHEEL_DOWN);
                break;

            default:
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_KP1:
            case SDLK_1:
            case SDLK_w:
                sprite_focused = WALL;
                break;

            case SDLK_KP2:
            case SDLK_2:
            case SDLK_c:
                sprite_focused = CASE;
                break;

            case SDLK_KP3:
            case SDLK_3:
            case SDLK_o:
                sprite_focused = OBJECTIVE;
                break;

            case SDLK_KP4:
            case SDLK_4:
            case SDLK_m:
                sprite_focused = MARIO;
                break;

            case SDLK_KP5:
            case SDLK_5:
            case SDLK_k:
                sprite_focused = CASE_OK;
                break;

            case SDLK_ESCAPE:
                pause = 0;
                break;

            case SDLK_e:
            case SDLK_r:
                for (i = 0 ; i < NUMBER_BLOCKS_HEIGHT ; i++) {
                    for (j = 0 ; j < NUMBER_BLOCKS_WIDTH ; j++) {
                        edit_board[i][j] = EMPTY;
                    }
                }
                break;

            case SDLK_s:
                pause = 0;
                save = 1;
                break;

            default:
                break;
            }
            break;
        }

        switch (sprite_focused) {
        case CASE:
            img_focused = case_img;
            break;
        case OBJECTIVE:
            img_focused = objective_img;
            break;
        case MARIO:
            img_focused = mario_img;
            break;
        case CASE_OK:
            img_focused = case_ok_img;
            break;
        default:
            img_focused = wall_img;
            break;
        }

        SDL_FillRect(screen, NULL, BACKGROUND_COLOR_WHITE_SCREEN);
        for (i = 0 ; i < NUMBER_BLOCKS_HEIGHT ; i++) {
            pos_cursor.y = i * SIZE_BLOCK;
            for (j = 0 ; j < NUMBER_BLOCKS_WIDTH ; j++) {
                pos_cursor.x = j * SIZE_BLOCK;
                switch (edit_board[i][j]) {
                case WALL:
                    SDL_BlitSurface(wall_img, NULL, screen, &pos_cursor);
                    break;

                case CASE:
                    SDL_BlitSurface(case_img, NULL, screen, &pos_cursor);
                    break;

                case OBJECTIVE:
                    SDL_BlitSurface(objective_img, NULL, screen, &pos_cursor);
                    break;

                case CASE_OK:
                    SDL_BlitSurface(case_ok_img, NULL, screen, &pos_cursor);
                    break;

                case MARIO:
                    SDL_BlitSurface(mario_img, NULL, screen, &pos_cursor);
                    break;

                default:
                    break;
                }
            }
        }

        track_mouse.x = event.button.x - (SIZE_BLOCK / 2);
        track_mouse.y = event.button.y - (SIZE_BLOCK / 2);

        SDL_BlitSurface(grid, NULL, screen, &pos_grid);
        SDL_BlitSurface(img_focused, NULL, screen, &track_mouse);

        SDL_Flip(screen);
    }

    SDL_FreeSurface(case_img);
    SDL_FreeSurface(case_ok_img);
    SDL_FreeSurface(wall_img);
    SDL_FreeSurface(objective_img);
    SDL_FreeSurface(mario_img);

    err = check_edit(edit_board);

    if (err != NO_ERROR)
        return err;

    if (save)
        return save_edit(edit_board);
    else
        return NO_ERROR;
}
