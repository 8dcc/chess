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

#include <ctype.h>
#include <stdbool.h>

#include <curses.h>

#include "include/input.h"
#include "include/board.h"

/*
 * Key received by 'getch' when the user presses Ctrl+C.
 */
#define KEY_CTRLC 3

/*----------------------------------------------------------------------------*/

/*
 * Get a key input from the user, and return it.
 *
 * This function could be modified (even at compile-time) to support multiple
 * input methods.
 */
static inline int get_user_char(void) {
    return getch();
}

/*----------------------------------------------------------------------------*/

enum EInputKey input_get_key(void) {
    switch (tolower(get_user_char())) {
        case 'q':
        case KEY_CTRLC:
            return INPUT_KEY_QUIT;

        case 'k':
        case KEY_UP:
            return INPUT_KEY_UP;

        case 'j':
        case KEY_DOWN:
            return INPUT_KEY_DOWN;

        case 'h':
        case KEY_LEFT:
            return INPUT_KEY_LEFT;

        case 'l':
        case KEY_RIGHT:
            return INPUT_KEY_RIGHT;

        case ' ':
        case KEY_ENTER:
            return INPUT_KEY_SELECT;

        default:
            return INPUT_KEY_UNKNOWN;
    }
}

/*
 * TODO: Perhaps this function should be moved to a separate module, and keep
 * the processing outside of the 'input' module.
 */
bool input_process_game_key(Board* board, enum EInputKey input_key) {
    switch (input_key) {
        case INPUT_KEY_UP:
            if (board->cursor.y > 0)
                board->cursor.y--;
            break;

        case INPUT_KEY_DOWN:
            if (board->cursor.y < board->height - 1)
                board->cursor.y++;
            break;

        case INPUT_KEY_LEFT:
            if (board->cursor.x > 0)
                board->cursor.x--;
            break;

        case INPUT_KEY_RIGHT:
            if (board->cursor.x < board->width - 1)
                board->cursor.x++;
            break;

        case INPUT_KEY_SELECT:
            if (board->selection.x == BOARD_COL_NONE &&
                board->selection.y == BOARD_ROW_NONE) {
                board->selection.x = board->cursor.x;
                board->selection.y = board->cursor.y;
            } else {
                board->selection.x = BOARD_COL_NONE;
                board->selection.y = BOARD_ROW_NONE;
            }
            break;

        default:
            return false;
    }

    return true;
}
