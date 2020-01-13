/***********************************************************************
 * Board.h
 * Implementation file for the board class
 * Peter E R Samuel
 * 8 March 2017
 * ********************************************************************/

#include "Board.h"
#include <iostream>

Board::Board() {

    _markerColumn = 0;
    _piecesCount = 0;
}

int Board::getPiecesCount() {

    return _piecesCount;
}

int Board::getDepthLimit() {

    return _depthLimit;
}

int Board::getColumnHeight(int column) {

    return _heights[column];
}

int Board::getBoardValue(int row, int column) {

    return _board[row][column];
}

void Board::placeMove(int column, int player) {
                        // Inc the column height counter and piece count
    _heights[column]++;
    _piecesCount++;
                  // Select which piece to place depending on the player
    if(player == PLAYER1) {
        _board[HEIGHT - _heights[column]][column] = PLAYER1;
    }
    else {
        _board[HEIGHT - _heights[column]][column] = PLAYER2;
    }
}

void Board::placeMarker(int column) {

    if(_heights[column] < HEIGHT) {
        // Remove the old piece only if it doesn't access invalid bounds
        if(_heights[_markerColumn] != HEIGHT) {
            _board[HEIGHT - _heights[_markerColumn] - 1][_markerColumn]
                = EMPTY;
        }
        _markerColumn = column;
                // - 1 to place the marker just above the existing piece
        _board[HEIGHT - _heights[column] - 1][column] = MARKER;
    }
}

void Board::removeMove(int column) {
    _piecesCount--;
                       // Set the board position at that column to empty
    _board[HEIGHT - _heights[column]][column] = EMPTY;
                                  // decrement the column height counter
    _heights[column]--;
}

void Board::incDepthLimit() {

    _depthLimit++;
}

void Board::clearBoard() {
                            // For each piece on board set back to empty
    for(int x = 0; x < WIDTH; x++) {
        _heights[x] = 0;
		for(int y = 0; y < HEIGHT; y++)	{
            _board[y][x] = EMPTY;
		}
    }
    _piecesCount = 0;
}

void Board::setDifficulty(int diff) {

    switch(diff) {
        case DIFF1:
            _heuristic = 0;
            _depthLimit = 4;
            break;
        case DIFF2:
            _heuristic = 0;
            _depthLimit = 6;
            break;
        case DIFF3:
            _heuristic = 1;
            _depthLimit = 4;
            break;
        case DIFF4:
            _heuristic = 1;
            _depthLimit = 6;
            break;
        case DIFF5:
            _heuristic = 1;
            _depthLimit = 8;
            break;
    }
}

int Board::checkWin() {

	int player1Count;
	int player2Count;
                                                           // HORIZONTAL
                                  // For each piece in a horizontal line
	for(int y = 0; y < HEIGHT; y++)	{
                                           // Reset player colour counts
		player1Count = 0;
		player2Count = 0;

		for(int x = 0; x < WIDTH; x++) {
               // Count the number of pieces of the same colour in a row
			if(_board[y][x] == PLAYER1) {
				player1Count++;
				player2Count = 0;
				if(player1Count >= 4) {
					return PLAYER1;
				}
			}
			else if(_board[y][x] == PLAYER2) {
				player2Count++;
				player1Count = 0;
				if(player2Count >= 4) {
					return PLAYER2;
				}
			}
			// Interrupt of diff colour/empty reset the in-a row counter
			else {
				player2Count = 0;
				player1Count = 0;
			}
		}
	}
                                                             // VERTICAL
	for(int x = 0; x < WIDTH; x++) {

		player1Count = 0;
		player2Count = 0;

		for(int y = 0; y < HEIGHT; y++)	{
			if(_board[y][x] == PLAYER1)	{
				player1Count++;
				player2Count = 0;
				if(player1Count >= 4) {
					return PLAYER1;
				}
			}
			else if(_board[y][x] == PLAYER2) {
				player2Count++;
				player1Count = 0;
				if(player2Count >= 4) {
					return PLAYER2;
				}
			}
			else {
				player2Count = 0;
				player1Count = 0;
			}
		}
	}
                                                     // DIAGONALLY RIGHT
	for(int x = 0; x < WIDTH; x++) {
		for(int y = 0; y < HEIGHT; y++)	{

			player2Count = 0;
			player1Count = 0;

			for(int i = 0; i < 5; i++) {
                         // Chk out of bounds array access since + delta
				if((y + i) >= HEIGHT || (x + i) >= WIDTH) {
					break;
				}
				if(_board[y + i][x + i] == PLAYER1)	{
					player1Count++;
					player2Count = 0;
					if(player1Count >= 4) {
						return PLAYER1;
					}
				}
				else if(_board[y + i][x + i] == PLAYER2)	{
					player2Count++;
					player1Count = 0;
					if(player2Count >= 4) {
						return PLAYER2;
					}
				}
				else {
					player2Count = 0;
					player1Count = 0;
				}
			}
		}
	}
                                                      // DIAGONALLY LEFT
	for(int x = (WIDTH - 1); x >= 0; x--) {
		for(int y = 0; y < HEIGHT; y++)	{

			player2Count = 0;
			player1Count = 0;

			for(int i = 0; i < 5; i++) {
				if((y + i) >= HEIGHT || (x - i) < 0) {
					break;
				}
				if(_board[y + i][x - i] == PLAYER1)	{
					player1Count++;
					player2Count = 0;
					if(player1Count >= 4) {
						return PLAYER1;
					}
				}
				else if(_board[y + i][x - i] == PLAYER2)	{
					player2Count++;
					player1Count = 0;
					if(player2Count >= 4) {
						return PLAYER2;
					}
				}
				else {
					player2Count = 0;
					player1Count = 0;
				}
			}
		}
	}
                                                                  // TIE
                               // If board is full and there's no winner
	for(int i = 0; i < WIDTH; i++) {
		if(_heights[i] < 6)	{
			return EMPTY;
		}
	}

	return TIE;
}

int Board::evalBoard(int aiPlayer, int humanPlayer) {

    int score = 0;
                // Heuristic must be enabled in difficulty to enhance AI
    if(_heuristic) {
                            // Apply check for all empty pieces on board
        for(int y = 0; y < HEIGHT; y++) {
            for(int x = 0; x < WIDTH; x++) {
                if(_board[y][x] == EMPTY) {
                    score += checkThreeRow(y, x, aiPlayer, humanPlayer);
                }
            }
        }
    }

    return score;
}

int Board::checkThreeRow(int row, int column,
    int aiPlayer, int humanPlayer) {
           // Same as check win function except terminates at 3 in a row
            // and skips the piece in the middle as it knows it is empty

    int score = 0;
    int humanCount = 0;
    int aiCount = 0;
                                                     // DIAGONALLY RIGHT
    for(int i = 3; i > -4; i--) {
                                              // Check for out of bounds
        if(row - i < 0 || row - i >= HEIGHT) {
            continue;
        }
        if(column + i < 0 || column + i >= WIDTH) {
            continue;
        }
        if(i == 0) {
            continue;
        }

        if(_board[row - i][column + i] == humanPlayer) {
            humanCount++;
            aiCount = 0;
            if(humanCount == 3) {
                score -= 50;
            }
        }
        else if(_board[row - i][column + i] == aiPlayer) {
            aiCount++;
            humanCount = 0;
            if(aiCount == 3) {
                score += 50;
            }
        }
        else {
            humanCount = 0;
            aiCount = 0;
        }
    }
                                                      // DIAGONALLY LEFT
    humanCount = 0;
    aiCount = 0;

    for(int i = -3; i < 4; i++) {

        if(row + i < 0 || row + i >= HEIGHT) {
            continue;
        }
        if(column + i < 0 || column + i >= WIDTH) {
            continue;
        }
        if(i == 0) {
            continue;
        }

        if(_board[row + i][column + i] == humanPlayer) {
            humanCount++;
            aiCount = 0;
            if(humanCount == 3) {
                score -= 50;
            }
        }
        else if(_board[row + i][column + i] == aiPlayer) {
            aiCount++;
            humanCount = 0;
            if(aiCount == 3) {
                score += 50;
            }
        }
        else {
            humanCount = 0;
            aiCount = 0;
        }
    }
                                                            //HORIZONTAL
    humanCount = 0;
    aiCount = 0;

    for(int i = -3; i < 4; i++) {
        if(column + i < 0 || column + i >= WIDTH) {
            continue;
        }
        if(i == 0) {
            continue;
        }

        if(_board[row][column + i] == humanPlayer) {
            humanCount++;
            aiCount = 0;
            if(humanCount == 3) {
                score -= 50;
            }
        }
        else if(_board[row][column + i] == aiPlayer) {
            aiCount++;
            humanCount = 0;
            if(aiCount == 3) {
                score += 50;
            }
        }
        else {
            humanCount = 0;
            aiCount = 0;
        }
    }
                                                             // VERTICAL
    if(row < 3) {
        if(_board[row + 1][column] == aiPlayer &&
           _board[row + 2][column] == aiPlayer &&
           _board[row + 3][column] == aiPlayer) {
            score += 50;
        }
        if(_board[row + 1][column] == humanPlayer &&
           _board[row + 2][column] == humanPlayer &&
           _board[row + 3][column] == humanPlayer) {
            score -= 50;
        }
    }

    return score;
}

