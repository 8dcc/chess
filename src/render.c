/*
 * Copyright 2025 8dcc
 *
 * This file is part of 8dcc's Chess.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#include <curses.h>

#include "include/board.h"
#include "include/util.h"

#define MARGIN_X 2 /* characters */
#define MARGIN_Y 1 /* characters */

/*
 * Enumeration with all possible render colors. These values will be used as IDs
 * for the ncurses colors.
 */
enum ERenderColors {
    RENDER_COL_INVALID = 0, /* Zero is not a valid index */

    RENDER_COL_DEFAULT,
    RENDER_COL_PIECE,
    RENDER_COL_BORDER,

    NUM_RENDER_COLORS, /* Must be last */
};

/*
 * Structure representing a color configuration. The 'foreground' and
 * 'background' members should be ncurses color indices, not RGB values.
 */
typedef struct {
    bool is_bold;
    bool is_dim;
    int foreground;
    int background;
} ColorInfo;

/*----------------------------------------------------------------------------*/

/*
 * Array with the color configurations for all color categories in the program.
 */
static const ColorInfo g_render_colors[] = {
    [RENDER_COL_DEFAULT] = {
      .is_bold    = false,
      .is_dim     = false,
      .foreground = COLOR_WHITE,
      .background = COLOR_BLACK,
    },
    [RENDER_COL_PIECE] = {
      .is_bold    = true,
      .is_dim     = false,
      .foreground = COLOR_WHITE,
      .background = COLOR_BLACK,
    },
    [RENDER_COL_BORDER] = {
      .is_bold    = false,
      .is_dim     = true,
      .foreground = COLOR_WHITE,
      .background = COLOR_BLACK,
    },
};

/*----------------------------------------------------------------------------*/

/*
 * Initialize the ncurses color configuration for the program. This function
 * returns true on success, or false otherwise.
 *
 * Note that, if the terminal doesn't support colors, this function still
 * returns successfully, and the program should simply not use them.
 */
static bool init_colors(void) {
    /* The terminal doesn't support colors, we won't render them */
    if (!has_colors())
        return true;

    /* Initialize ncurses color support */
    if (start_color() == ERR || use_default_colors() == ERR)
        return false;

    /* Initialize each color pair, ensuring the colors are supported */
    for (size_t i = RENDER_COL_DEFAULT; i < NUM_RENDER_COLORS; i++) {
        if (g_render_colors[i].foreground >= COLORS ||
            g_render_colors[i].background >= COLORS)
            return false;

        init_pair(i,
                  g_render_colors[i].foreground,
                  g_render_colors[i].background);
    }

    return true;
}

/*
 * Change to the specified color ID using ncurses routines.
 *
 * Note that, if the terminal doesn't support colors, this function still
 * returns successfully.
 */
static bool change_color(enum ERenderColors color) {
    /* If the terminal doesn't support colors, ignore successfully */
    if (!has_colors() || !can_change_color())
        return true;

    if (attron(COLOR_PAIR(color)) == ERR)
        return false;
    if (g_render_colors[color].is_bold && attron(A_BOLD) == ERR)
        return false;
    if (g_render_colors[color].is_dim && attron(A_DIM) == ERR)
        return false;

    return true;
}

/*
 * Reset the terminal colors using ncurses routines, after 'old_color' was used.
 *
 * Note that, if the terminal doesn't support colors, this function still
 * returns successfully.
 */
static bool reset_color(enum ERenderColors old_color) {
    /* If the terminal doesn't support colors, ignore successfully */
    if (!has_colors() || !can_change_color())
        return true;

    if (attroff(COLOR_PAIR(old_color)) == ERR)
        return false;
    if (g_render_colors[old_color].is_bold && attroff(A_BOLD) == ERR)
        return false;
    if (g_render_colors[old_color].is_dim && attroff(A_DIM) == ERR)
        return false;

    return true;
}

/*
 * Print the specified formatted text into the screen with the specified color,
 * using ncurses routines.
 */
static bool addfmt_colored(enum ERenderColors color, const char* fmt, ...) {
    va_list va;

    va_start(va, fmt);
    const int data_size = vsnprintf(NULL, 0, fmt, va);
    va_end(va);

    if (data_size < 0)
        return false;

    char* buffer = malloc(data_size + 1);
    if (buffer == NULL)
        return false;

    va_start(va, fmt);
    const int written = vsnprintf(buffer, data_size + 1, fmt, va);
    va_end(va);

    bool result = true;

    result = result && (written >= 0);
    result = result && change_color(color);
    result = result && (addstr(buffer) != ERR);
    result = result && reset_color(color);

    free(buffer);
    return result;
}

/*----------------------------------------------------------------------------*/

bool render_startup(void) {
    return initscr() != NULL && /* Init ncurses */
           raw() != ERR &&      /* Scan input without pressing enter */
           noecho() != ERR &&   /* Don't print when typing */
           keypad(stdscr, true) != ERR && /* Enable keypad (arrow keys) */
           init_colors();                 /* Initialize ncurses color pairs */
}

void render_cleanup(void) {
    endwin();
}

bool render_board(const Board* board) {
    move(MARGIN_Y, MARGIN_X);

    /* Initial border */
    for (size_t x = 0; x < board->width; x++)
        addfmt_colored(RENDER_COL_BORDER, "+---");
    addfmt_colored(RENDER_COL_BORDER, "+");

    for (size_t y = 0; y < board->height; y++) {
        move(MARGIN_Y + 1 + (y * 2), MARGIN_X);
        /* Row pieces */
        for (size_t x = 0; x < board->width; x++)
            if (!addfmt_colored(RENDER_COL_BORDER, "|") ||
                !addfmt_colored(RENDER_COL_PIECE,
                                " %c ",
                                board_cell_get_char(
                                  &board->cells[board->width * y + x])))
                return false;
        addfmt_colored(RENDER_COL_BORDER, "|");

        /* Border after each row */
        move(MARGIN_Y + 1 + (y * 2) + 1, MARGIN_X);
        for (size_t x = 0; x < board->width; x++)
            addfmt_colored(RENDER_COL_BORDER, "+---");
        addfmt_colored(RENDER_COL_BORDER, "+");
    }

    /* After rendering, move terminal cursor to the player cursor */
    move(MARGIN_Y + (STRLEN("+|") * board->cursor_y) + 1,
         MARGIN_X + (STRLEN("+---") * board->cursor_x) + 2);

    refresh();
    return true;
}
