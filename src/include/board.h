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
 * Structure representing a coordinate in the board. The enumerations for the X
 * and Y members contain the standard names for rows and columns in a chess
 * board, which will be used for accesing the 'cells' member of the 'Board'
 * structure.
 *
 * The 'NONE' values are used in 'Board.selection' to specify that there is no
 * selected cell in the board.
 */
typedef struct BoardCoordinate {
    enum {
        BOARD_ROW_NONE = -1,
        BOARD_ROW_A    = 0,
        BOARD_ROW_B    = 1,
        BOARD_ROW_C    = 2,
        BOARD_ROW_D    = 3,
        BOARD_ROW_E    = 4,
        BOARD_ROW_F    = 5,
        BOARD_ROW_G    = 6,
        BOARD_ROW_H    = 7,
    } x;
    enum {
        BOARD_COL_NONE = -1,
        BOARD_COL_1    = 0,
        BOARD_COL_2    = 1,
        BOARD_COL_3    = 2,
        BOARD_COL_4    = 3,
        BOARD_COL_5    = 4,
        BOARD_COL_6    = 5,
        BOARD_COL_7    = 6,
        BOARD_COL_8    = 7,
    } y;
} BoardCoordinate;

/*
 * Structure representing a single cell of a chess board, independently on
 * whether or not it has a piece on it.
 */
typedef struct BoardCell {
    bool has_piece;

    /*
     * TODO: Perhaps use a pointer, which is moved around when the pieces move.
     */
    Piece piece;
} BoardCell;

/*
 * Structure representing a chess board, containing the current information
 * about all (alive) pieces.
 */
typedef struct Board {
    /* Width and height of the board, in cells */
    int width, height;

    /*
     * 2D array of board cells. The array size is determined by the 'width' and
     * 'height' members. The stored orientation always haves the white pieces
     * (rows 1-2) on top, and the black pieces (rows 7-8) on the bottom; the
     * board will be rotated when rendering, if needed.
     */
    BoardCell* cells;

    /* Position of the player cursor, in cells */
    BoardCoordinate cursor;

    /* Position of the player selection, in cells */
    BoardCoordinate selection;
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
 * Return a pointer to the piece at the specified position in the specified
 * board.
 */
static inline BoardCell* board_cell_at(const Board* board,
                                       BoardCoordinate coord) {
    return &board->cells[board->width * coord.y + coord.x];
}

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
    assert(board->cursor.x < board->width && board->cursor.y < board->height);

    /* The selection coordinates should not be out of range */
    assert((board->selection.x == BOARD_ROW_NONE ||
            (board->selection.x >= 0 && board->selection.x < board->width)) &&
           (board->selection.y == BOARD_COL_NONE ||
            (board->selection.y >= 0 && board->selection.y < board->height)));

    /* The selection coordinates should not be partially specified */
    assert((board->selection.x == BOARD_ROW_NONE &&
            board->selection.y == BOARD_COL_NONE) ||
           (board->selection.x != BOARD_ROW_NONE &&
            board->selection.y != BOARD_COL_NONE));
}

#endif /* BOARD_H_ */
