/***********************************************************************
 * MainGame.h
 * Header for the aggregated game class, containing the board, AI etc
 * Peter E R Samuel
 * 8 March 2017
 * ********************************************************************/

#ifndef MAINGAME_H
#define MAINGAME_H

#include "Board.h"
#include "AI.h"
#include "Texture.h"
#include "Button.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

enum Gamestate {                              // Game States Enumeration
    PLAYAI = 40,
    SELECT,
    PLAYFRIEND,
    TITLE,
    OPTIONS,
    EXIT
};

enum Background { // Background Enumeration used at start of draw functs
    TURQ = 91,
    BLUE,
    BROWN
};

class MainGame {
    public:
        MainGame();
        ~MainGame();
        void run();

    private:
        bool initSys();
        bool loadMedia();

        void selectLoop();
        void drawSelect();

        void gameLoopAi();
        void drawAiGame();

        void gameLoop2();
        void draw2Game();

        void titleLoop();
        void drawTitle();

        void optLoop();
        void drawOpts();

        SDL_Window* _window;
        SDL_Renderer* _renderer;

        int _victory;
        int _gameState;
        int _difficulty;

        int _screenWidth;
        int _screenHeight;

        SDL_Rect _gameClips[9];
        SDL_Rect _txtClips[11];

        Button _titleButtons[4];
        Button _optionsButtons[17];
        Button _selectButtons[2];
        SDL_Rect _buttonClips[46];

        Texture _gameSpriteSheet;
        Texture _gameAlphaSheet;
        Texture _buttonSpriteSheet;

        BoardStates _currentTurn;
        BoardStates _humanTurn;
        BoardStates _aiTurn;

        bool _soundBool;

        int _backColour;
        int _pieceOffset;
        SDL_Rect _backGround;

        Mix_Music* _music;
        Mix_Chunk* _click;
		Mix_Chunk* _winSound;

        Board _board;
        Ai _ai;
};

#endif // MAINGAME_H
