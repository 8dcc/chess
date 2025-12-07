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

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "include/board.h"
#include "include/piece.h"

static void set_board_cell(Board* board, size_t x, size_t y,
                           enum EPieceType type, enum EPieceColor color) {
    BoardCoordinate coord = { .x = x, .y = y };
    BoardCell* cell       = board_cell_at(board, coord);
    cell->has_piece       = true;
    cell->piece.type      = type;
    cell->piece.color     = color;
}

/*----------------------------------------------------------------------------*/

bool board_init(Board* board, size_t width, size_t height) {
    board->cursor.x    = 0;
    board->cursor.y    = 0;
    board->selection.x = BOARD_COL_NONE;
    board->selection.y = BOARD_ROW_NONE;
    board->width       = width;
    board->height      = height;

    board->cells = malloc(board->width * board->height * sizeof(BoardCell));
    if (board->cells == NULL)
        return false;

    for (int y = 0; y < board->height; y++)
        for (int x = 0; x < board->width; x++)
            board_cell_at(board, (BoardCoordinate){ x, y })->has_piece = false;

    return true;
}

void board_destroy(Board* board) {
    if (board->cells != NULL) {
        free(board->cells);
        board->cells = NULL;
    }
}

bool board_set_initial_layout(Board* board) {
    /* TODO: Support arbitrary board dimensions */
    assert(board->width == 8 && board->height == 8);

    int y;

    /* Black pieces, top of the board */
    y = BOARD_ROW_8;
    set_board_cell(board, BOARD_COL_A, y, PIECE_TYPE_ROOK, PIECE_COL_BLACK);
    set_board_cell(board, BOARD_COL_B, y, PIECE_TYPE_KNIGHT, PIECE_COL_BLACK);
    set_board_cell(board, BOARD_COL_C, y, PIECE_TYPE_BISHOP, PIECE_COL_BLACK);
    set_board_cell(board, BOARD_COL_D, y, PIECE_TYPE_QUEEN, PIECE_COL_BLACK);
    set_board_cell(board, BOARD_COL_E, y, PIECE_TYPE_KING, PIECE_COL_BLACK);
    set_board_cell(board, BOARD_COL_F, y, PIECE_TYPE_BISHOP, PIECE_COL_BLACK);
    set_board_cell(board, BOARD_COL_G, y, PIECE_TYPE_KNIGHT, PIECE_COL_BLACK);
    set_board_cell(board, BOARD_COL_H, y, PIECE_TYPE_ROOK, PIECE_COL_BLACK);
    y = BOARD_ROW_7;
    for (int x = 0; x < board->width; x++)
        set_board_cell(board, x, y, PIECE_TYPE_PAWN, PIECE_COL_BLACK);

    /* White pieces, bottom of the board */
    y = BOARD_ROW_1;
    set_board_cell(board, BOARD_COL_A, y, PIECE_TYPE_ROOK, PIECE_COL_WHITE);
    set_board_cell(board, BOARD_COL_B, y, PIECE_TYPE_KNIGHT, PIECE_COL_WHITE);
    set_board_cell(board, BOARD_COL_C, y, PIECE_TYPE_BISHOP, PIECE_COL_WHITE);
    set_board_cell(board, BOARD_COL_D, y, PIECE_TYPE_QUEEN, PIECE_COL_WHITE);
    set_board_cell(board, BOARD_COL_E, y, PIECE_TYPE_KING, PIECE_COL_WHITE);
    set_board_cell(board, BOARD_COL_F, y, PIECE_TYPE_BISHOP, PIECE_COL_WHITE);
    set_board_cell(board, BOARD_COL_G, y, PIECE_TYPE_KNIGHT, PIECE_COL_WHITE);
    set_board_cell(board, BOARD_COL_H, y, PIECE_TYPE_ROOK, PIECE_COL_WHITE);
    y = BOARD_ROW_2;
    for (int x = 0; x < board->width; x++)
        set_board_cell(board, x, y, PIECE_TYPE_PAWN, PIECE_COL_WHITE);

    return true;
}
