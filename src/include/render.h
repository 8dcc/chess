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

#ifndef RENDER_H_
#define RENDER_H_ 1

#include <stdbool.h>

#include "board.h"


/*
 * Start rendering data through the "ncurses" library.
 */
bool render_startup(void);

/*
 * Stop rendering data.
 */
void render_cleanup(void);

/*
 * Render the specified board with the "ncurses" library.
 */
bool render_board(const Board* board);

#endif /* RENDER_H_ */
