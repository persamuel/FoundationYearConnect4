/***********************************************************************
 * AI.cpp
 * Implementation file for the AI class used for the play AI game state
 * Peter E R Samuel
 * 8 March 2017
 * ********************************************************************/

#include "AI.h"

void Ai::setAiPlayer(int player) {
                                        // Initialise which player AI is
    if(player == PLAYER1) {
        _aiPlayer = PLAYER1;
        _humanPlayer = PLAYER2;
    }
    else {
        _aiPlayer = PLAYER2;
        _humanPlayer = PLAYER1;
    }
}

int Ai::MinMax(Board& board) {
                // Set best move -9999 so will get initially overwritten
    AiMove bestMove(-9999);
                                                // Apply all legal moves
    for(int col = 0; col < WIDTH; col++) {
        if(board.getColumnHeight(col) < (HEIGHT)) {
            AiMove curMove;
            curMove.column = col;
                                    // Place current valid move on board
            board.placeMove(curMove.column, _aiPlayer);
                             // Call recursive minmax for move selection
            curMove.score = Min(board);
                 // Check if current valid move better than current best
            if(curMove.score > bestMove.score) {
                bestMove.column = curMove.column;
                bestMove.score = curMove.score;
            }
                  // Remove the placed move so the board doesn't fill up
            board.removeMove(curMove.column);
        }
    }
                                        // Return the best selected move
    return bestMove.column;
}

int Ai::Max(Board& board) {
                                           //Base case victory selection
    int victory = board.checkWin();
    if(victory == _aiPlayer) {
        return 2000;
    }
    if(victory == _humanPlayer) {
        return -2000;
    }
    if(victory == TIE) {
        return 0;
    }
                            // Depth limit base case to limit leaf depth
	if(board.getPiecesCount() == board.getDepthLimit()) {
        return board.evalBoard(_aiPlayer, _humanPlayer);
    }

    else {
        AiMove bestMove(-9999);
        for(int col = 0; col < WIDTH; col++) {
            if(board.getColumnHeight(col) < (HEIGHT)) {
                AiMove curMove;
                curMove.column = col;

                board.placeMove(curMove.column, _aiPlayer);
                      //Call the sister function for best move selection
                curMove.score = Min(board);

                if(curMove.score > bestMove.score) {
                    bestMove.column = curMove.column;
                    bestMove.score = curMove.score;
                }

                board.removeMove(curMove.column);
            }
        }
        return bestMove.score;
    }
}

int Ai::Min(Board& board) {

    int victory = board.checkWin();
    if(victory == _humanPlayer) {
        return -2000;
    }
    if(victory == _aiPlayer) {
        return 2000;
    }
    if(victory == TIE) {
        return 0;
    }

	if(board.getPiecesCount() == board.getDepthLimit()) {
        return board.evalBoard(_aiPlayer, _humanPlayer);
    }

    else {
        AiMove bestMove(9999);
        for(int col = 0; col < WIDTH; col++) {
            if(board.getColumnHeight(col) < (HEIGHT)) {
                AiMove curMove;
                curMove.column = col;

                board.placeMove(curMove.column, _humanPlayer);
                     //Call the brother function for best move selection
				curMove.score = Max(board);

                if(curMove.score < bestMove.score) {
                    bestMove.column = curMove.column;
                    bestMove.score = curMove.score;
                }

                board.removeMove(curMove.column);
            }
        }
        return bestMove.score;
    }
}



