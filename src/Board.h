/***********************************************************************
 * Board.h
 * Header for the board class
 * Peter E R Samuel
 * 8 March 2017
 * ********************************************************************/

#ifndef BOARD_H
#define BOARD_H
                                            // Board dimension constants
const int HEIGHT = 6;
const int WIDTH = 7;

                                             // Victory and piece states
enum BoardStates {
    TIE = -1,
    EMPTY,
    PLAYER1,
    PLAYER2,
    MARKER
};
                                                // Set difficulty values
enum Difficulty {
    DIFF1 = 130,
    DIFF2,
    DIFF3,
    DIFF4,
    DIFF5
};

class Board
{
    public:
        Board();
                                                              // Getters
        int getPiecesCount();
        int getDepthLimit();
        int getColumnHeight(int column);
        int getBoardValue(int row, int column);
                                                              // Setters
        void toggleHeuristic(bool toggle);
        void placeMove(int column, int player);
        void placeMarker(int column);
        void removeMove(int column);
        void incDepthLimit();
        void clearBoard();
        void setDifficulty(int diff);
                                                           // Heuristics
        int checkWin();
        int evalBoard(int aiPlayer, int humanPlayer);
        int checkThreeRow(int row, int column,
            int aiPlayer, int humanPlayer);

    private:
        int _board[HEIGHT][WIDTH]{};
        int _heights[WIDTH]{};
        int _piecesCount;
        bool _heuristic;
        int _depthLimit;
        int _markerColumn;
};

#endif // BOARD_H
