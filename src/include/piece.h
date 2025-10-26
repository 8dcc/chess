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

#ifndef PIECE_H_
#define PIECE_H_ 1

/*
 * Enumeration representing available types of chess pieces.
 */
enum EPieceType {
    PIECE_TYPE_UNKNOWN,
    PIECE_TYPE_PAWN,
    PIECE_TYPE_ROOK,
    PIECE_TYPE_KNIGHT,
    PIECE_TYPE_BISHOP,
    PIECE_TYPE_QUEEN,
    PIECE_TYPE_KING,
};

/*
 * Enumeration representing available colors of chess pieces.
 */
enum EPieceColor {
    PIECE_COL_UNKNOWN,
    PIECE_COL_WHITE,
    PIECE_COL_BLACK,
};

/*
 * Structure representing a single chess piece.
 */
typedef struct Piece {
    enum EPieceType type;
    enum EPieceColor color;
} Piece;

/*----------------------------------------------------------------------------*/

/*
 * Get the character used to display a chess piece.
 */
static inline char piece_get_char(const Piece* piece) {
    char result;

    /* clang-format off */
    switch (piece->type) {
        case PIECE_TYPE_UNKNOWN: result = '?'; break;
        case PIECE_TYPE_PAWN:    result = 'P'; break;
        case PIECE_TYPE_ROOK:    result = 'R'; break;
        case PIECE_TYPE_KNIGHT:  result = 'N'; break;
        case PIECE_TYPE_BISHOP:  result = 'B'; break;
        case PIECE_TYPE_QUEEN:   result = 'Q'; break;
        case PIECE_TYPE_KING:    result = 'K'; break;
    }
    /* clang-format on */

    /* TODO: Perhaps change case depending on color? */
    return result;
}

#endif /* PIECE_H_ */
