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
#include "include/render.h"
#include "include/input.h"

int main(void) {
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

    if (!render_startup()) {
        fprintf(stderr, "Failed to start rendering.\n");
        goto cleanup;
    }

    /* TODO: Render board using ncurses, etc. */
    bool should_quit = false;
    while (!should_quit) {
        if (!render_board(&board)) {
            fprintf(stderr, "Failed to render board. Aborting...\n");
            break;
        }

        /* Handle user input */
        switch (input_get_key()) {
            case INPUT_KEY_QUIT:
                should_quit = true;
                break;

            case INPUT_KEY_UNKNOWN:
                break;
        }
    }

cleanup:
    render_cleanup();
    board_destroy(&board);
    return 0;
}
