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

#include <stdio.h>

#include "include/board.h"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    const size_t board_width  = 8;
    const size_t board_height = 8;

    Board board;
    if (!board_init(&board, board_width, board_height) ||
        !board_set_initial_layout(&board)) {
        fprintf(stderr,
                "Failed to initialize %zux%zu board.\n",
                board_width,
                board_height);
        return 1;
    }

    /* TODO: Render board using ncurses, etc. */
    for (size_t x = 0; x < board.width; x++)
        printf("+---");
    printf("+\n");
    for (size_t y = 0; y < board.height; y++) {
        for (size_t x = 0; x < board.width; x++)
            printf("| %c ", board_cell_get_char(&board.cells[board.width * y + x]));
        printf("|\n");
        for (size_t x = 0; x < board.width; x++)
            printf("+---");
        printf("+\n");
    }

    board_destroy(&board);
    return 0;
}
