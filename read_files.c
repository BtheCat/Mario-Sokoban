#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "read_files.h"

char itoc(int i) {
    return i + '0';
}

int ctoi(char c) {
    return c - '0';
}

Sprite itosprite(int c) {
    switch (c) {
    case '0':
        return EMPTY;
    case '2':
        return CASE;
    case '3':
        return OBJECTIVE;
    case '4':
        return MARIO;
    case '5':
        return CASE_OK;
    default:
        return WALL;
    }
}

Error incr_array_pos(int *i, int *j, const int n, const int m) {
    if (*j < m - 1) {
        (*j)++;
    } else if (*i <= n - 1) {
        (*i)++;
        *j = 0;
    } else {
        fprintf(stderr, "Echec d'incrementation des indices (i = %d, j = %d, n = %d, m = %d).\n",
                *i, *j, n, m);
        return ERROR_FILE_INCR;
    }
    return NO_ERROR;
}

void set_white_down(SDL_Surface *surf) {
    SDL_SetColorKey(surf, SDL_SRCCOLORKEY, SDL_MapRGB(surf->format, 255, 255, 255));
}

Error read_level(Sprite play_board[][NUMBER_BLOCKS_WIDTH], char filename[], SDL_Rect *pos_mario) {
    FILE *level_file = NULL;
    Error err = NO_ERROR;

    char level_file_name[LENGTH_LEVEL_FILENAME] = LEVEL_FILE_DEFAULT_PATH;
    level_file_name[LEVEL_POS_10] = filename[LEVEL_POS_10];
    level_file_name[LEVEL_POS_1] = filename[LEVEL_POS_1];

    level_file = fopen(level_file_name, "r");

    if (level_file == NULL) {
        fprintf(stderr, "Echec d'ouverture du fichier %s.\n", level_file_name);
        err = ERROR_READ_FILE_OPPENING;
    } else {
        int i = 0, j = 0;
        char actual_char = ' ';
        int objectives_vs_cases = 0;
        int mario_set = 0;
        actual_char = fgetc(level_file);

        while (actual_char != EOF) {
            play_board[i][j] = itosprite(actual_char);
            if (play_board[i][j] == OBJECTIVE) {
                objectives_vs_cases++;
            }
            if (play_board[i][j] == CASE) {
                objectives_vs_cases--;
            }
            if (play_board[i][j] == MARIO) {
                if (pos_mario != NULL) {
                    if (mario_set) {
                        fprintf(stderr, "Mario deja present !\n\
                                \tPosition Mario 1 : %d %d\n\
                                \tPosition Mario 2 : %d %d\n", pos_mario->x, pos_mario->y, i, j);
                        err = ERROR_FILE_MARIOS;
                    } else {
                        pos_mario->x = i;
                        pos_mario->y = j;
                        mario_set = 1;
                    }
                }
            }

            err = incr_array_pos(&i, &j, NUMBER_BLOCKS_HEIGHT, NUMBER_BLOCKS_WIDTH);
            if (err != NO_ERROR)
                return err;
            actual_char = fgetc(level_file);
        }

        if (objectives_vs_cases > 0) {
            fprintf(stderr, "Pas suffisamment de caisse pour les objectifs : il y a %d objectifs en trop.\n", objectives_vs_cases);
            err = ERROR_FILE_GOAL;
        }
        fclose(level_file);
    }

    return err;
}

int number_levels(void) {
    int levels = 1, max_level = 0;

    FILE *max_level_config = NULL;
    max_level_config = fopen(NUMBER_LEVEL_CONFIG_FILE_PATH, "r");

    if (max_level_config == NULL) {
        max_level = 1;
        max_level_config = fopen(NUMBER_LEVEL_CONFIG_FILE_PATH, "w");
        fputc(itoc(max_level / 10), max_level_config);
        fputc(itoc(max_level % 10), max_level_config);
    } else {
        char char_max_level[2] = "  ";
        char_max_level[0] = fgetc(max_level_config);
        char_max_level[1] = fgetc(max_level_config);
        max_level = (ctoi(char_max_level[0]) * 10) + ctoi(char_max_level[1]);
    }

    fclose(max_level_config);
    max_level_config = NULL;

    char filename[LENGTH_LEVEL_FILENAME] = LEVEL_FILE_DEFAULT_PATH;

    FILE *test_file = NULL;
    test_file = fopen(filename, "r");

    while (test_file != NULL && levels <= max_level) {
        fclose(test_file);
        test_file = NULL;
        levels++;
        filename[LEVEL_POS_10] = itoc(levels / 10);
        filename[LEVEL_POS_1] = itoc(levels % 10);
        test_file = fopen(filename, "r");
    }

    return levels;
}

Error update_max_level_file(int new_max_level) {
    Error err = NO_ERROR;

    FILE *max_level_config = NULL;
    max_level_config = fopen(NUMBER_LEVEL_CONFIG_FILE_PATH, "w");

    if (max_level_config != NULL) {
        fputc(itoc(new_max_level / 10), max_level_config);
        fputc(itoc(new_max_level % 10), max_level_config);
    } else {
        err = ERROR_WRITE_FILE_OPPENING;
    }

    fclose(max_level_config);
    max_level_config = NULL;

    return err;
}

