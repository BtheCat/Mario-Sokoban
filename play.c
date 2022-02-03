#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "play.h"
#include "read_files.h"
#include "level.h"

void move_case(Sprite *first_square, Sprite *second_square) {
    if (*first_square == CASE || *first_square == CASE_OK) {
        if (*second_square == OBJECTIVE) {
            *second_square = CASE_OK;
        } else {
            *second_square = CASE;
        }

        if (*first_square == CASE_OK) {
            *first_square = OBJECTIVE;
        } else {
            *first_square = EMPTY;
        }
    }
}

Error move_player(Sprite play_board[][NUMBER_BLOCKS_WIDTH], SDL_Rect *pos, Direction dir) {
    switch (dir) {
    case UP:
        if (pos->x - 1 < 0)
            break;
        if (play_board[pos->x - 1][pos->y] == WALL)
            break;
        if ((play_board[pos->x - 1][pos->y] == CASE || play_board[pos->x - 1][pos->y] == CASE_OK) &&
            ((pos->x - 2 < 0) || (play_board[pos->x - 2][pos->y] == WALL
            || play_board[pos->x - 2][pos->y] == CASE || play_board[pos->x - 2][pos->y] == CASE_OK)))
            break;

        move_case(&play_board[pos->x - 1][pos->y], &play_board[pos->x - 2][pos->y]);
        pos->x--;
        break;

    case RIGHT:
        if (pos->y + 1 > NUMBER_BLOCKS_WIDTH - 1)
            break;
        if (play_board[pos->x][pos->y + 1] == WALL)
            break;
        if ((play_board[pos->x][pos->y + 1] == CASE || play_board[pos->x][pos->y + 1] == CASE_OK) &&
            ((pos->y + 2 > NUMBER_BLOCKS_WIDTH - 1) || (play_board[pos->x][pos->y + 2] == WALL
            || play_board[pos->x][pos->y + 2] == CASE || play_board[pos->x][pos->y + 2] == CASE_OK)))
            break;

        move_case(&play_board[pos->x][pos->y + 1], &play_board[pos->x][pos->y + 2]);
        pos->y++;
        break;

    case DOWN:
        if (pos->x + 1 > NUMBER_BLOCKS_HEIGHT - 1)
            break;
        if (play_board[pos->x + 1][pos->y] == WALL)
            break;
        if ((play_board[pos->x + 1][pos->y] == CASE || play_board[pos->x + 1][pos->y] == CASE_OK) &&
            ((pos->x + 2 > NUMBER_BLOCKS_HEIGHT - 1) || (play_board[pos->x + 2][pos->y] == WALL
            || play_board[pos->x + 2][pos->y] == CASE || play_board[pos->x + 2][pos->y] == CASE_OK)))
            break;

        move_case(&play_board[pos->x + 1][pos->y], &play_board[pos->x + 2][pos->y]);
        pos->x++;
        break;

    case LEFT:
        if (pos->y - 1 < 0)
            break;
        if (play_board[pos->x][pos->y - 1] == WALL)
            break;
        if ((play_board[pos->x][pos->y - 1] == CASE || play_board[pos->x][pos->y - 1] == CASE_OK) &&
            ((pos->y - 2 < 0) || (play_board[pos->x][pos->y - 2] == WALL
            || play_board[pos->x][pos->y - 2] == CASE || play_board[pos->x][pos->y - 2] == CASE_OK)))
            break;

        move_case(&play_board[pos->x][pos->y - 1], &play_board[pos->x][pos->y - 2]);
        pos->y--;
        break;

    default:
        return ERROR_MOVE_MARIO;
        break;
    }
    return NO_ERROR;
}

void mstohms(int delay_time, int *hour, int *minutes, int *sec) {
    if (delay_time > MAX_DELAY_TIME) {
        *sec = 59;
        *minutes = 59;
        *hour = 99;
    } else {
        *sec = delay_time / 1000;
        *hour = *sec / 3600;
        *sec %= 3600;
        *minutes = *sec / 60;
        *sec %= 60;
    }
}

void itot(char stime[2], int t) {
    stime[0] = itoc(t / 10);
    stime[1] = itoc(t % 10);
}

void add_time(char stime[23], int t, int bool_end) {
    char scurrent[2];
    itot(scurrent, t);
    strcat(stime, scurrent);
    if (!bool_end)
        strcat(stime, ":");
}

void format_time(char stime[], int hour, int minutes, int sec) {
    strcpy(stime, "Temps ecoule : ");
    add_time(stime, hour, 0);
    add_time(stime, minutes, 0);
    add_time(stime, sec, 1);
}

Error play(SDL_Surface *screen, TTF_Font *font, TTF_Font *font_bold, char filename[], const int level_max) {
    SDL_Surface *mario_img[4];
    SDL_Surface *case_img = NULL, *case_ok_img = NULL, *wall_img = NULL,
        *objective_img = NULL, *actual_mario_img = NULL, *txt_play = NULL, *txt_time = NULL;
    SDL_Event event;
    SDL_Rect pos_cursor, pos_mario, pos;
    SDL_Color black_color = {0, 0, 0}, white_color = {255, 255, 255};

    txt_play = TTF_RenderText_Blended(font, "Jouer", black_color);

    Sprite play_board[NUMBER_BLOCKS_HEIGHT][NUMBER_BLOCKS_WIDTH] = {{EMPTY}};

    int goal = 0;
    int pause = 1;
    int actual_time = 0, hour = 0, minutes = 0, sec = 0;
    int start_time = SDL_GetTicks();
    char stime[LENGTH_TIME] = {0};
    char level_file_name[LENGTH_LEVEL_FILENAME] = LEVEL_FILE_DEFAULT_PATH;
    level_file_name[LEVEL_POS_10] = filename[LEVEL_POS_10];
    level_file_name[LEVEL_POS_1] = filename[LEVEL_POS_1];

    Error err = read_level(play_board, level_file_name, &pos);

    if(err != NO_ERROR) {
        return err;
    }

    pos_cursor.x = 0;
    pos_cursor.y = 0;

    pos_mario.x = pos.y * SIZE_BLOCK;
    pos_mario.y = pos.x * SIZE_BLOCK;

    int i = 0, j = 0;

    case_img = IMG_Load(CASE_BMP_PATH);
    case_ok_img = IMG_Load(CASE_OK_BMP_PATH);
    wall_img = IMG_Load(WALL_BMP_PATH);
    objective_img = IMG_Load(OBJECTIVE_BMP_PATH);

    mario_img[UP] = IMG_Load(MARIO_UP_BMP_PATH);
    mario_img[RIGHT] = IMG_Load(MARIO_RIGHT_BMP_PATH);
    mario_img[DOWN] = IMG_Load(MARIO_DOWN_BMP_PATH);
    mario_img[LEFT] = IMG_Load(MARIO_LEFT_BMP_PATH);

    for (i = 0 ; i < 4 ; i++)
        set_white_down(mario_img[i]);
    set_white_down(objective_img);

    actual_mario_img = mario_img[DOWN];

    SDL_EnableKeyRepeat(10, 10);

    while (pause && !goal) {
        SDL_PollEvent(&event);
        switch(event.type) {
        case SDL_QUIT:
            pause = 0;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                err = move_player(play_board, &pos, UP);
                if (err != NO_ERROR) {
                    fprintf(stderr, "Probleme lors du deplacement UP de Mario (position mario : %d %d)\n", pos.x, pos.y);
                    pause = 0;
                }  else {
                    actual_mario_img = mario_img[UP];
                }
                break;

            case SDLK_RIGHT:
                err = move_player(play_board, &pos, RIGHT);
                if (err != NO_ERROR) {
                    fprintf(stderr, "Probleme lors du deplacement RIGHT de Mario (position mario : %d %d)\n", pos.x, pos.y);
                    pause = 0;
                }  else {
                    actual_mario_img = mario_img[RIGHT];
                }
                break;

            case SDLK_DOWN:
                err = move_player(play_board, &pos, DOWN);
                if (err != NO_ERROR) {
                    fprintf(stderr, "Probleme lors du deplacement DOWN de Mario (position mario : %d %d)\n", pos.x, pos.y);
                    pause = 0;
                }  else {
                    actual_mario_img = mario_img[DOWN];
                }
                break;

            case SDLK_LEFT:
                err = move_player(play_board, &pos, LEFT);
                if (err != NO_ERROR) {
                    fprintf(stderr, "Probleme lors du deplacement LEFT de Mario (position mario : %d %d)\n", pos.x, pos.y);
                    pause = 0;
                }  else {
                    actual_mario_img = mario_img[LEFT];
                }
                break;

            case SDLK_ESCAPE:
                pause = 0;
                break;
            case SDLK_r:
                err = read_level(play_board, filename, &pos);
                if (err != NO_ERROR) {
                    pause = 0;
                } else {
                    start_time = SDL_GetTicks();
                }
                break;
            default:
                break;
            }
            break;
        }

        pos_mario.x = pos.y * SIZE_BLOCK;
        pos_mario.y = pos.x * SIZE_BLOCK;

        SDL_FillRect(screen, NULL, BACKGROUND_COLOR_WHITE_SCREEN);
        goal = 1;
        for (i = 0 ; i < NUMBER_BLOCKS_HEIGHT ; i++) {
            pos_cursor.y = i * SIZE_BLOCK;
            for (j = 0 ; j < NUMBER_BLOCKS_WIDTH ; j++) {
                pos_cursor.x = j * SIZE_BLOCK;
                switch (play_board[i][j]) {
                case WALL:
                    SDL_BlitSurface(wall_img, NULL, screen, &pos_cursor);
                    break;

                case CASE:
                    SDL_BlitSurface(case_img, NULL, screen, &pos_cursor);
                    break;

                case OBJECTIVE:
                    goal = 0;
                    SDL_BlitSurface(objective_img, NULL, screen, &pos_cursor);
                    break;

                case CASE_OK:
                    SDL_BlitSurface(case_ok_img, NULL, screen, &pos_cursor);
                    break;

                default:
                    break;
                }
            }
        }

        set_white_down(actual_mario_img);
        SDL_BlitSurface(actual_mario_img, NULL, screen, &pos_mario);

        pos_cursor.x = TXT_SCREEN_POS_BOTTOM_X;
        pos_cursor.y = TXT_POS_BOTTOM_Y;
        SDL_BlitSurface(txt_play, NULL, screen, &pos_cursor);

        if (!goal)
            actual_time = SDL_GetTicks();

        mstohms(actual_time - start_time, &hour, &minutes, &sec);
        format_time(stime, hour, minutes, sec);
        txt_time = TTF_RenderText_Shaded(font, stime, black_color, white_color);

        pos_cursor.x = TXT_TIMER_POS_BOTTOM_X;
        pos_cursor.y = TXT_POS_BOTTOM_Y;
        SDL_BlitSurface(txt_time, NULL, screen, &pos_cursor);

        SDL_Flip(screen);
        SDL_Delay(100);
    }

    SDL_FreeSurface(case_img);
    SDL_FreeSurface(case_ok_img);
    SDL_FreeSurface(wall_img);
    SDL_FreeSurface(objective_img);

    for (i = 0 ; i < 4 ; i++)
        SDL_FreeSurface(mario_img[i]);

    if (err != NO_ERROR)
        return err;

    if (goal)
        return win(screen, font, font_bold, actual_time - start_time, filename, level_max);

    return NO_ERROR;
}

Error win(SDL_Surface *screen, TTF_Font *font, TTF_Font *font_bold, int chrono, char filename[], int level_max) {
    SDL_Surface *win = NULL, *txt_time = NULL;
    SDL_Rect pos_win, pos_txt;
    SDL_Event event;

    char level_file_name[LENGTH_LEVEL_FILENAME] = LEVEL_FILE_DEFAULT_PATH;
    level_file_name[LEVEL_POS_10] = filename[LEVEL_POS_10];
    level_file_name[LEVEL_POS_1] = filename[LEVEL_POS_1];

    SDL_Color white_color = {255, 255, 255};
    TTF_Font *font_time = NULL;

    font_time = TTF_OpenFont(FONT_TTF, 26);

    Error err = update_max_level_file(level_max);

    if (err != NO_ERROR) {
        return err;
    }

    level_max++;
    int pause = 1, next_level = 0, try_again_level = 0, go_level = 0;
    int hour = 0, minutes = 0, sec = 0;
    char stime[LENGTH_TIME] = "";
    int level_i = (ctoi(level_file_name[LEVEL_POS_10]) * 10) + ctoi(level_file_name[LEVEL_POS_1]);

    win = IMG_Load(WIN_BMP_PATH);

    pos_win.x = 0;
    pos_win.y = 0;
    pos_txt.x = TXT_CHRONO_POS_X;
    pos_txt.y = TXT_CHRONO_POS_Y;

    while(pause) {
        SDL_WaitEvent(&event);
        switch(event.type) {
        case SDL_QUIT:
            pause = 0;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_1:
            case SDLK_s:
            case SDLK_j:
            case SDLK_KP1:
                next_level = 1;
                pause = 0;
                break;

            case SDLK_r:
            case SDLK_2:
            case SDLK_KP2:
                try_again_level = 1;
                pause = 0;
                break;

            case SDLK_l:
            case SDLK_KP3:
            case SDLK_3:
                go_level = 1;
                pause = 0;
                break;

            case SDLK_q:
            case SDLK_KP4:
            case SDLK_4:
            case SDLK_ESCAPE:
                SDL_Delay(100);
                pause = 0;
                break;

            default:
                break;
            }
        default:
            break;
        }
        SDL_FillRect(screen, NULL, BACKGROUND_COLOR_WHITE_SCREEN);
        SDL_BlitSurface(win, NULL, screen, &pos_win);


        mstohms(chrono, &hour, &minutes, &sec);
        format_time(stime, hour, minutes, sec);
        txt_time = TTF_RenderText_Blended(font_time, stime, white_color);

        SDL_BlitSurface(txt_time, NULL, screen, &pos_txt);

        SDL_Flip(screen);

        if (err != NO_ERROR) {
            pause = 0;
        }
    }

    SDL_FreeSurface(win);
    SDL_FreeSurface(txt_time);
    TTF_CloseFont(font_time);

    if (next_level && (level_i < level_max)) {
        level_i = wheel_level_i(level_i, WHEEL_UP, level_max);

        level_file_name[LEVEL_POS_10] = itoc(level_i / 10);
        level_file_name[LEVEL_POS_1] = itoc(level_i % 10);

        err = play(screen, font, font_bold, level_file_name, level_max);
    }
    if (try_again_level) {
        err = play(screen, font, font_bold, level_file_name, level_max);
    }
    if (go_level) {
        err = choice_level(screen, font, font_bold);
    }

    return err;
}

