/***********************************************************************
 * AI.h
 * Header for the AI class used for the play AI game state
 * Peter E R Samuel
 * 8 March 2017
 * ********************************************************************/

#ifndef AI_H
#define AI_H

#include "Board.h"

                    // AI moves used in minmax contains score and column
struct AiMove {
    AiMove() {};
 // Stops proc for def score set in minmax done 1000s of times so needed
    AiMove(int Score) : score(Score) {};
    int column;
    int score;
};

class Ai {
    public:
        void setAiPlayer(int player);
        int MinMax(Board& board);
        int Max(Board& board);
        int Min(Board& board);

    private:
        int _aiPlayer;
        int _humanPlayer;
};

#endif // AI_H
