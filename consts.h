#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED

#define SIZE_BLOCK 34
#define NUMBER_BLOCKS_WIDTH 12
#define NUMBER_BLOCKS_HEIGHT 12
#define BOTTOM_MARGIN 25
#define MAX_DELAY_TIME 1000*59*59*99

#define WIDTH_SCREEN SIZE_BLOCK * NUMBER_BLOCKS_WIDTH
#define HEIGHT_SCREEN SIZE_BLOCK * NUMBER_BLOCKS_HEIGHT + BOTTOM_MARGIN

#define CASE_BMP_PATH "img/case.bmp"
#define CASE_OK_BMP_PATH "img/case_ok.bmp"
#define WALL_BMP_PATH "img/wall.bmp"
#define OBJECTIVE_BMP_PATH "img/objective.bmp"
#define MARIO_UP_BMP_PATH "img/mario_up.bmp"
#define MARIO_DOWN_BMP_PATH "img/mario_down.bmp"
#define MARIO_LEFT_BMP_PATH "img/mario_left.bmp"
#define MARIO_RIGHT_BMP_PATH "img/mario_right.bmp"

#define MENU_BMP_PATH "img/menu.bmp"
#define WIN_BMP_PATH "img/win.bmp"
#define GRID_BMP_PATH "img/grid_1pt.bmp"
#define LEVEL_BMP_PATH "img/level.bmp"

#define FONT_TTF "font/font.ttf"
#define FONT_TTF_BOLD "font/fontbd.ttf"
#define FONT_SIZE 11
#define TXT_POS_BOTTOM_Y HEIGHT_SCREEN - 20
#define TXT_SCREEN_POS_BOTTOM_X 5
#define TXT_TIMER_POS_BOTTOM_X HEIGHT_SCREEN - 130 - BOTTOM_MARGIN
#define TXT_LEVEL_POS_BOTTOM_X 221
#define TXT_LEVEL_POS_BOTTOM_Y HEIGHT_SCREEN - BOTTOM_MARGIN - 62
#define TXT_CASE_POS_BOTTOM_X WIDTH_SCREEN - (5 * SIZE_BLOCK + 17)
#define TXT_CASE_POS_BOTTOM_Y HEIGHT_SCREEN - BOTTOM_MARGIN - (2 * 34 - 11)
#define TXT_CHRONO_POS_X 50
#define TXT_CHRONO_POS_Y 130
#define LENGTH_TIME 23

#define APP_NAME "Mario Sokoban"
#define BACKGROUND_COLOR_WHITE_SCREEN SDL_MapRGB(screen->format, 255, 255, 255)
#define BACKGROUND_COLOR_BLACK_SCREEN SDL_MapRGB(screen->format, 0, 0, 0)

#define LEVEL_PLAYER_FILE_PATH "lvl/level00.lvl"
#define LEVEL_FILE_DEFAULT_PATH "lvl/level01.lvl"
#define NUMBER_LEVEL_CONFIG_FILE_PATH "config/max_level.config"
#define LENGTH_LEVEL_FILENAME 15

#define LEVEL_POS_10 9
#define LEVEL_POS_1 10

typedef enum Sprite Sprite;
enum Sprite {
    EMPTY = 0,
    WALL = 1,
    CASE = 2,
    OBJECTIVE = 3,
    MARIO = 4,
    CASE_OK = 5
};

typedef enum Direction Direction;
enum Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

typedef enum Error Error;
enum Error {
    NO_ERROR = 0,
    ERROR_READ_FILE_OPPENING = 1,
    ERROR_FILE_MARIOS = 2,
    ERROR_FILE_INCR = 3,
    ERROR_FILE_GOAL = 4,
    ERROR_MOVE_MARIO = 5,
    ERROR_WRITE_FILE_OPPENING = 6,
    ERROR_LOAD_SDL = 7,
    ERROR_LOAD_TTF = 8,
    ERROR_LOAD_FONT = 9
};

typedef enum Wheel Wheel;
enum Wheel {
    WHEEL_UP = 0,
    WHEEL_DOWN = 1
};
#endif // CONSTS_H_INCLUDED
