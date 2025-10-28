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

static bool addfmt(const char* fmt, ...) {
    va_list va;

    va_start(va, fmt);
    const int data_size = vsnprintf(NULL, 0, fmt, va);
    va_end(va);

    if (data_size < 0)
        return false;

    char* buffer = malloc(data_size);
    if (buffer == NULL)
        return false;

    va_start(va, fmt);
    const int written = vsnprintf(buffer, data_size + 1, fmt, va);
    va_end(va);

    if (written < 0) {
        free(buffer);
        return false;
    }

    const int result = addstr(buffer);
    free(buffer);

    return result != ERR;
}

/*----------------------------------------------------------------------------*/

bool render_startup(void) {
    return initscr() != NULL &&         /* Init ncurses */
           raw() != ERR &&              /* Scan input without pressing enter */
           noecho() != ERR &&           /* Don't print when typing */
           keypad(stdscr, true) != ERR; /* Enable keypad (arrow keys) */
}

void render_cleanup(void) {
    endwin();
}

bool render_board(const Board* board) {
    move(MARGIN_Y, MARGIN_X);

    /* Initial border */
    for (size_t x = 0; x < board->width; x++)
        addstr("+---");
    addch('+');

    for (size_t y = 0; y < board->height; y++) {
        move(MARGIN_Y + 1 + (y * 2), MARGIN_X);
        /* Row pieces */
        for (size_t x = 0; x < board->width; x++)
            if (!addfmt("| %c ",
                        board_cell_get_char(
                          &board->cells[board->width * y + x])))
                return false;
        addch('|');

        /* Border after each row */
        move(MARGIN_Y + 1 + (y * 2) + 1, MARGIN_X);
        for (size_t x = 0; x < board->width; x++)
            addstr("+---");
        addch('+');
    }

    /* After rendering, move terminal cursor to the player cursor */
    move(MARGIN_Y + (STRLEN("+|") * board->cursor_y) + 1,
         MARGIN_X + (STRLEN("+---") * board->cursor_x) + 2);

    refresh();
    return true;
}
