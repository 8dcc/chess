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

#ifndef INPUT_H_
#define INPUT_H_ 1

#include <stdbool.h>

#include "board.h"

/*
 * Enumeration representing possible user inputs.
 */
enum EInputKey {
    INPUT_KEY_UNKNOWN,
    INPUT_KEY_QUIT,
    INPUT_KEY_UP,
    INPUT_KEY_DOWN,
    INPUT_KEY_LEFT,
    INPUT_KEY_RIGHT,
    INPUT_KEY_SELECT,
};

/*----------------------------------------------------------------------------*/

/*
 * Read an input key from the user, and return it.
 */
enum EInputKey input_get_key(void);

/*
 * Process a game key, altering the specified chess board if needed. Keys
 * unrelated to the game are not processed, so the caller is responsible for
 * the other application-level keys.
 *
 * This function returns true if the key was successfully processed, or false in
 * case of error (e.g. it's not a game key, or there was an error processing a
 * valid key).
 */
bool input_process_game_key(Board* board, enum EInputKey input_key);

#endif /* INPUT_H_ */
