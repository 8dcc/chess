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

#ifndef BOARD_H_
#define BOARD_H_ 1

#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "piece.h"

/*
 * Index value used in 'Board.selection_x' and 'Board.selection_y' to specify
 * that there is no selected cell in the board.
 */
#define BOARD_NONSELECTED_IDX (-1)

/*
 * Structure representing a single cell of a chess board, independently on
 * whether or not it has a piece on it.
 */
typedef struct BoardCell {
    bool has_piece;
    Piece piece;
} BoardCell;

/*
 * Structure representing a chess board, containing the current information
 * about all (alive) pieces.
 */
typedef struct Board {
    /* Width and height of the board, in cells */
    unsigned int width, height;

    /* Array of board cells */
    BoardCell* cells;

    /* Position of the player cursor, in cells */
    unsigned int cursor_x, cursor_y;

    /* Position of the player selection, in cells */
    int selection_x, selection_y;
} Board;

/*----------------------------------------------------------------------------*/

/*
 * Initialize a 'Board' structure with the specified width and height. After
 * successfuly calling this function, the caller is responsible for
 * deinitializing it with 'board_destroy'.
 *
 * This function returns true on success, or false on error.
 */
bool board_init(Board* board, size_t width, size_t height);

/*
 * Deinitialize a 'Board' structure, freeing the relevant members. It doesn't
 * free the argument pointer itself.
 */
void board_destroy(Board* board);

/*
 * Set the initial layout of a chess board.
 */
bool board_set_initial_layout(Board* board);

/*
 * Get the character used to display a cell of a chess board.
 */
static inline char board_cell_get_char(BoardCell* cell) {
    return cell->has_piece ? piece_get_char(&cell->piece) : ' ';
}

/*
 * Assert the integrity of a 'Board' structure, ensuring all of the members are
 * valid and compatible with each other.
 */
static inline void board_assert_integrity(const Board* board) {
    /* The cursor coordinates should not be out of range */
    assert(board->cursor_x < board->width && board->cursor_y < board->height);

    /* The selection coordinates should not be out of range */
    assert((board->selection_x == BOARD_NONSELECTED_IDX ||
            (board->selection_x >= 0 &&
             (unsigned int)board->selection_x < board->width)) &&
           (board->selection_y == BOARD_NONSELECTED_IDX ||
            (board->selection_y >= 0 &&
             (unsigned int)board->selection_y < board->height)));

    /* The selection coordinates should not be partially specified */
    assert((board->selection_x == BOARD_NONSELECTED_IDX &&
            board->selection_y == BOARD_NONSELECTED_IDX) ||
           (board->selection_x != BOARD_NONSELECTED_IDX &&
            board->selection_y != BOARD_NONSELECTED_IDX));
}

#endif /* BOARD_H_ */
