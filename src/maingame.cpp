/***********************************************************************
 * MainGame.cpp
 * Implementation file for the main game class
 * Peter E R Samuel
 * 8 March 2017
 * ********************************************************************/

#include "MainGame.h"

#include <iostream>
using std::cout;
using std::endl;

#include <stdio.h>
#include <string>
#include <random>
using std::mt19937;
using std::uniform_real_distribution;

#include <ctime>

MainGame::MainGame() {
                                              // Set SDL classes to null
    _window = nullptr;
    _renderer = nullptr;
    _music = nullptr;
    _click = nullptr;
	_winSound = nullptr;
                                                   // Define screen size
    _screenHeight = 520;
    _screenWidth = 605;
                            // Start game in the title screen with sound
    _gameState = TITLE;
    _soundBool = 1;
                                               // Set background as TURQ
    _backColour = TURQ;
    _backGround = {0, 0, _screenWidth, _screenWidth};
                                       // Set difficulty and turn values
    _currentTurn = PLAYER1;
    _pieceOffset = 0;
    _difficulty = DIFF3;
}

void MainGame::run() {
                                               // Initialise SDL systems
    if(!initSys()) {
        cout << "Failed to initialize!" << endl;
    }
    else {
                          // Load all media needed, and set sprite clips
        if(!loadMedia()) {
            cout << "Failed to load media!" << endl;
        }
        else {
            Mix_PlayMusic(_music, -1);
                         // Run game loops while game hasn't been closed
			while(_gameState != EXIT) {
                switch(_gameState) {
                    case TITLE:
                    titleLoop();
                    break;
                    case SELECT:
                    selectLoop();
                    break;
                    case PLAYAI:
                    gameLoopAi();
                    break;
                    case PLAYFRIEND:
                    gameLoop2();
                    break;
                    case OPTIONS:
                    optLoop();
                    break;
                                 // Default to EXIT if game loop problem
                    default:
                    cout << "Failed to run game loops!" << endl;
                    _gameState = EXIT;
                }
            }
        }
    }
}

/* initSys function for starting SDL, code adapted from lazy foo's
 * tutorials: http://www.lazyfoo.net/tutorials/SDL/index.php
 * Link to specific tutorial:
 * http://lazyfoo.net/tutorials/SDL/
 * 07_texture_loading_and_rendering/index.php
 */

bool MainGame::initSys() {

    bool success = true;                          // initialisation flag
                                  // initialise SDL video else error out
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
                    // printf used over cout due to easier error getting
        printf("SDL could not initialize! SDL Error: %s\n"
               , SDL_GetError());
        success = false;
    }
    else {
                                    // Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}
                                  // Create window with tag of Connect 4
		_window = SDL_CreateWindow("Connect 4", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, _screenWidth,
            _screenHeight, SDL_WINDOW_SHOWN);
                                  // Return error if can't create window
		if(_window == nullptr) {
			printf("Window could not be created! SDL Error: %s\n",
                SDL_GetError());
			success = false;
		}
		else {
			  // Create v-sync renderer for limiting fps to refresh rate
			_renderer = SDL_CreateRenderer(_window, -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                                // Return error if can't create renderer
			if(_renderer == nullptr) {
				printf("Renderer could not be created! SDL Error: %s\n",
                    SDL_GetError());
				success = false;
			}
			else {
                              //Initialize renderer draw colour to white
				SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF,
                    0xFF, 0xFF);
                                          //Initialize PNG image loading
				int imgFlags = IMG_INIT_PNG;
                     // Pass in PNG loading flags returns them if loaded
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image"
                           "Error: %s\n", IMG_GetError());
					success = false;
				}
                // Initialize SDL_mixer for sounds returns -1 if failed
                if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT,
                                 2, 2048) < 0) {
                    printf("SDL_mixer could not initialize!"
                           "SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
			}
		}
	}

	return success;
}

/* Load media function for starting SDL, code adapted from lazy foo's
 * tutorials: http://www.lazyfoo.net/tutorials/SDL/index.php
 * Link to specific tutorial:
 * http://www.lazyfoo.net/tutorials/SDL/
 * 11_clip_rendering_and_sprite_sheets/index.php
 */

bool MainGame::loadMedia() {
                                                         // Success flag
	bool success = true;

                     // Load piece sprite sheet else return unsuccessful
	if(!_gameSpriteSheet.loadFromFile("pieceSheet.png", _renderer)) {
		cout << "Failed to load game sprite sheet texture!\n"  << endl;
		success = false;
	}

	else {
         // Set sprite sheet clips for the game pieces and alpha markers
                                                       // RED AND YELLOW
        _gameClips[0].x = 0;
		_gameClips[0].y = 0;
        _gameClips[0].w = 75;
		_gameClips[0].h = 75;

		_gameClips[1].x = 75;
		_gameClips[1].y = 0;
        _gameClips[1].w = 75;
		_gameClips[1].h = 75;
                                                                // WHITE
		_gameClips[2].x = 150;
		_gameClips[2].y = 0;
        _gameClips[2].w = 75;
		_gameClips[2].h = 75;
                                                        // RED AND BLACK
        _gameClips[3].x = 0;
		_gameClips[3].y = 75;
        _gameClips[3].w = 75;
		_gameClips[3].h = 75;

		_gameClips[4].x = 75;
		_gameClips[4].y = 75;
        _gameClips[4].w = 75;
		_gameClips[4].h = 75;
                                                     // GREEN AND PURPLE
		_gameClips[5].x = 0;
		_gameClips[5].y = 150;
        _gameClips[5].w = 75;
		_gameClips[5].h = 75;

		_gameClips[6].x = 75;
		_gameClips[6].y = 150;
        _gameClips[6].w = 75;
		_gameClips[6].h = 75;
                                                     // BROWN AND ORANGE
		_gameClips[7].x = 0;
		_gameClips[7].y = 225;
        _gameClips[7].w = 75;
		_gameClips[7].h = 75;

		_gameClips[8].x = 75;
		_gameClips[8].y = 225;
        _gameClips[8].w = 75;
		_gameClips[8].h = 75;
    }
                       // Load piece sheet again for alpha Marker pieces
    if(!_gameAlphaSheet.loadFromFile("alphaSheet.png", _renderer)) {
		cout << "Failed to load game sprite sheet texture!\n"  << endl;
		success = false;
	}
                                    // Set alpha to half 255 for Markers
    _gameAlphaSheet.setAlpha(120);
                             // Load sprite sheet for buttons and titles
    if(!_buttonSpriteSheet.loadFromFile("buttonSheet.png", _renderer)) {
		printf( "Failed to load button sprite sheet texture!\n" );
		success = false;
	}
                    // Set sprite sheet clips for the buttons and titles
	else {
                                                                // TITLE
        _txtClips[0].x = 0;
        _txtClips[0].y = 0;
        _txtClips[0].w = 227;
        _txtClips[0].h = 38;
                                                               // PLAYER
        _txtClips[1].x = 233;
        _txtClips[1].y = 0;
        _txtClips[1].w = 132;
        _txtClips[1].h = 49;
                                                           // DIFFICULTY
        _txtClips[2].x = 378;
        _txtClips[2].y = 0;
        _txtClips[2].w = 192;
        _txtClips[2].h = 48;
                                                               // COLOUR
        _txtClips[3].x = 378;
        _txtClips[3].y = 53;
        _txtClips[3].w = 162;
        _txtClips[3].h = 32;
                                                           // BACKGROUND
        _txtClips[4].x = 378;
        _txtClips[4].y = 98;
        _txtClips[4].w = 236;
        _txtClips[4].h = 41;
                                                         // SOUND SYMBOL
        _txtClips[5].x = 568;
        _txtClips[5].y = 241;
        _txtClips[5].w = 70;
        _txtClips[5].h = 50;
                                                         // MUSIC SYMBOL
        _txtClips[6].x = 567;
        _txtClips[6].y = 183;
        _txtClips[6].w = 63;
        _txtClips[6].h = 50;
                                                             // / SYMBOL
        _txtClips[7].x = 554;
        _txtClips[7].y = 148;
        _txtClips[7].w = 17;
        _txtClips[7].h = 30;
                                                                  // TIE
        _txtClips[8].x = 10;
        _txtClips[8].y = 404;
        _txtClips[8].w = 65;
        _txtClips[8].h = 37;
                                                                // 1 Won
        _txtClips[9].x = 125;
        _txtClips[9].y = 404;
        _txtClips[9].w = 296;
        _txtClips[9].h = 47;
                                                                // 2 Won
        _txtClips[10].x = 125;
        _txtClips[10].y = 452;
        _txtClips[10].w = 296;
        _txtClips[10].h = 47;
                                                               //PLAY AI
        _buttonClips[0].x = 0;
        _buttonClips[0].y = 38;
        _buttonClips[0].w = 122;
        _buttonClips[0].h = 41;

        _buttonClips[1].x = 0;
        _buttonClips[1].y = 82;
        _buttonClips[1].w = 124;
        _buttonClips[1].h = 43;
                       // Button declaration for Play AI on title screen
        Button playAi(0, static_cast<int>(SELECT), 124, 43, 236, 140);
        _titleButtons[0] = playAi;
                                                          // PLAY FRIEND
        _buttonClips[2].x = 0;
        _buttonClips[2].y = 126;
        _buttonClips[2].w = 195;
        _buttonClips[2].h = 39;

        _buttonClips[3].x = 0;
        _buttonClips[3].y = 166;
        _buttonClips[3].w = 198;
        _buttonClips[3].h = 43;
                   // Button declaration for Play Friend on title screen
        Button playFriend(2, static_cast<int>(PLAYFRIEND),
                          198, 43, 203, 200);
        _titleButtons[1] = playFriend;
                                                              // OPTIONS
        _buttonClips[4].x = 0;
        _buttonClips[4].y = 211;
        _buttonClips[4].w = 146;
        _buttonClips[4].h = 39;

        _buttonClips[5].x = 0;
        _buttonClips[5].y = 251;
        _buttonClips[5].w = 147;
        _buttonClips[5].h = 43;
                       // Button declaration for Options on title screen
        Button options(4, static_cast<int>(OPTIONS), 147, 39, 229, 260);
        _titleButtons[2] = options;
                                                                 // EXIT
        _buttonClips[6].x = 0;
        _buttonClips[6].y = 303;
        _buttonClips[6].w = 72;
        _buttonClips[6].h = 30;

        _buttonClips[7].x = 0;
        _buttonClips[7].y = 338;
        _buttonClips[7].w = 74;
        _buttonClips[7].h = 35;
                          // Button declaration for Exit on title screen
        Button exit (6, static_cast<int>(EXIT), 74, 35, 265, 320);
        _titleButtons[3] = exit;
                                              // PLAYER 1 / DIFFICULTY 1
        _buttonClips[8].x = 123;
        _buttonClips[8].y = 38;
        _buttonClips[8].w = 43;
        _buttonClips[8].h = 42;

        _buttonClips[9].x = 170;
        _buttonClips[9].y = 38;
        _buttonClips[9].w = 45;
        _buttonClips[9].h = 44;
                     // Button declaration for player 1 on select screen
        Button play1 (8, static_cast<int>(PLAYER1), 43, 44, 150, 220);
        _selectButtons[0] = play1;
                // Button declaration for difficulty 1 on options screen
        Button diff1 (8, static_cast<int>(DIFF1), 43, 44, 222, 130);
        _optionsButtons[0] = diff1;
                                               // PLAYER2 / DIFFICULTY 2
        _buttonClips[10].x = 123;
        _buttonClips[10].y = 82;
        _buttonClips[10].w = 43;
        _buttonClips[10].h = 42;

        _buttonClips[11].x = 170;
        _buttonClips[11].y = 82;
        _buttonClips[11].w = 45;
        _buttonClips[11].h = 44;
                     // Button declaration for player 2 on select screen
        Button play2 (10, static_cast<int>(PLAYER2), 43, 44, 400, 220);
        _selectButtons[1] = play2;
                // Button declaration for difficulty 2 on options screen
        Button diff2 (10, static_cast<int>(DIFF2), 43, 44, 272, 130);
        _optionsButtons[1] = diff2;
                                                         // DIFFICULTY 3
        _buttonClips[12].x = 79;
        _buttonClips[12].y = 298;
        _buttonClips[12].w = 43;
        _buttonClips[12].h = 42;

        _buttonClips[13].x = 122;
        _buttonClips[13].y = 296;
        _buttonClips[13].w = 44;
        _buttonClips[13].h = 44;
                // Button declaration for difficulty 3 on options screen
         Button diff3 (12, static_cast<int>(DIFF3), 43, 44, 322, 130);
        _optionsButtons[2] = diff3;
                                                          //DIFFICULTY 4
        _buttonClips[14].x = 79;
        _buttonClips[14].y = 343;
        _buttonClips[14].w = 43;
        _buttonClips[14].h = 42;

        _buttonClips[15].x = 122;
        _buttonClips[15].y = 342;
        _buttonClips[15].w = 44;
        _buttonClips[15].h = 44;
                // Button declaration for difficulty 4 on options screen
        Button diff4 (14, static_cast<int>(DIFF4), 43, 44, 372, 130);
        _optionsButtons[3] = diff4;
                                                          //DIFFICULTY 5
        _buttonClips[16].x = 170;
        _buttonClips[16].y = 297;
        _buttonClips[16].w = 43;
        _buttonClips[16].h = 42;

        _buttonClips[17].x = 214;
        _buttonClips[17].y = 297;
        _buttonClips[17].w = 43;
        _buttonClips[17].h = 44;
                // Button declaration for difficulty 5 on options screen
        Button diff5 (16, static_cast<int>(DIFF5), 43, 44, 422, 130);
        _optionsButtons[4] = diff5;
                                                       // COLOUR BLOCK 1
        _buttonClips[18].x = 220;
        _buttonClips[18].y = 82;
        _buttonClips[18].w = 45;
        _buttonClips[18].h = 44;

        _buttonClips[19].x = 204;
        _buttonClips[19].y = 128;
        _buttonClips[19].w = 54;
        _buttonClips[19].h = 54;
                // Button declaration for background 1 on options screen
        Button bblock1 (18, static_cast<int>(TURQ), 54, 54, 276, 340);
        _optionsButtons[5] = bblock1;
                                                       // COLOUR BLOCK 2
        _buttonClips[20].x = 266;
        _buttonClips[20].y = 82;
        _buttonClips[20].w = 45;
        _buttonClips[20].h = 44;

        _buttonClips[21].x = 260;
        _buttonClips[21].y = 128;
        _buttonClips[21].w = 54;
        _buttonClips[21].h = 54;
                // Button declaration for background 2 on options screen
        Button bblock2 (20, static_cast<int>(BLUE), 54, 54, 336, 340);
        _optionsButtons[6] = bblock2;
                                                       // COLOUR BLOCK 3
        _buttonClips[22].x = 311;
        _buttonClips[22].y = 82;
        _buttonClips[22].w = 45;
        _buttonClips[22].h = 44;

        _buttonClips[23].x = 316;
        _buttonClips[23].y = 128;
        _buttonClips[23].w = 54;
        _buttonClips[23].h = 54;
                // Button declaration for background 3 on options screen
        Button bblock3 (22, static_cast<int>(BROWN), 54, 54, 396, 340);
        _optionsButtons[7] = bblock3;
                                                                   // ON
        _buttonClips[24].x = 373;
        _buttonClips[24].y = 137;
        _buttonClips[24].w = 74;
        _buttonClips[24].h = 52;

        _buttonClips[25].x = 464;
        _buttonClips[25].y = 144;
        _buttonClips[25].w = 76;
        _buttonClips[25].h = 56;
                   // Button declaration for music on, on options screen
        Button onMusic (24, 1, 74, 54, 115, 190);
        _optionsButtons[8] = onMusic;
                   // Button declaration for sound on, on options screen
        Button onSound (24, 1, 74, 54, 115, 260);
        _optionsButtons[9] = onSound;
                                                                  // OFF
        _buttonClips[26].x = 385;
        _buttonClips[26].y = 199;
        _buttonClips[26].w = 76;
        _buttonClips[26].h = 56;

        _buttonClips[27].x = 465;
        _buttonClips[27].y = 202;
        _buttonClips[27].w = 76;
        _buttonClips[27].h = 56;
                  // Button declaration for music off, on options screen
        Button offMusic (26, 0, 74, 54, 225, 190);
        _optionsButtons[10] = offMusic;
                  // Button declaration for sound off, on options screen
        Button offSound (26, 0, 74, 54, 225, 260);
        _optionsButtons[11] = offSound;
                                                             // COLOUR 1
        _buttonClips[28].x = 200;
        _buttonClips[28].y = 193;
        _buttonClips[28].w = 44;
        _buttonClips[28].h = 90;

        _buttonClips[29].x = 269;
        _buttonClips[29].y = 295;
        _buttonClips[29].w = 54;
        _buttonClips[29].h = 99;
           // Button declaration for 1st set of pieces on options screen
        Button pieces1 (28, 7, 54, 99, 202, 410);
        _optionsButtons[12] = pieces1;
                                                             // COLOUR 2
        _buttonClips[30].x = 246;
        _buttonClips[30].y = 193;
        _buttonClips[30].w = 44;
        _buttonClips[30].h = 90;

        _buttonClips[31].x = 324;
        _buttonClips[31].y = 294;
        _buttonClips[31].w = 54;
        _buttonClips[31].h = 99;
           // Button declaration for 2nd set of pieces on options screen
        Button pieces2 (30, 5, 54, 99, 262, 410);
        _optionsButtons[13] = pieces2;
                                                             // COLOUR 3
        _buttonClips[32].x = 292;
        _buttonClips[32].y = 193;
        _buttonClips[32].w = 44;
        _buttonClips[32].h = 90;

        _buttonClips[33].x = 379;
        _buttonClips[33].y = 294;
        _buttonClips[33].w = 54;
        _buttonClips[33].h = 99;
           // Button declaration for 3rd set of pieces on options screen
        Button pieces3 (32, 3, 54, 99, 322, 410);
        _optionsButtons[14] = pieces3;
                                                             // COLOUR 4
        _buttonClips[34].x = 337;
        _buttonClips[34].y = 193;
        _buttonClips[34].w = 44;
        _buttonClips[34].h = 90;

        _buttonClips[35].x = 434;
        _buttonClips[35].y = 294;
        _buttonClips[35].w = 54;
        _buttonClips[35].h = 99;
           // Button declaration for 4th set of pieces on options screen
        Button pieces4 (34, 0, 54, 99, 382, 410);
        _optionsButtons[15] = pieces4;
                                                                  //HOME
        _buttonClips[36].x = 570;
        _buttonClips[36].y = 297;
        _buttonClips[36].w = 59;
        _buttonClips[36].h = 48;

        _buttonClips[37].x = 570;
        _buttonClips[37].y = 350;
        _buttonClips[37].w = 59;
        _buttonClips[37].h = 48;
                 // Button declaration for home button on options screen
        Button title(36, static_cast<int>(TITLE), 57, 48, 20, 20);
        _optionsButtons[16] = title;
    }

                                                            //Load music
    _music = Mix_LoadMUS("Puzzle_Theme1.wav");
    if(_music ==  nullptr) {
        printf("Failed to load beat music! SDL_mixer Error: %s\n",
               Mix_GetError());
        success = false;
    }
                                                    //Load sound effects
    _click = Mix_LoadWAV("clickSound.wav");
    if(_click == nullptr) {
        printf("Failed to load scratch sound effect!"
               "SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

	_winSound = Mix_LoadWAV("winSound.wav");
    if(_click == nullptr) {
        printf("Failed to load scratch sound effect!"
               "SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}

MainGame::~MainGame() {
                                                    //Free loaded images
    _gameSpriteSheet.free();
    _gameAlphaSheet.free();
    _buttonSpriteSheet.free();
                                                     //Free sound effect
    Mix_FreeChunk(_click);
	Mix_FreeChunk(_winSound);
    Mix_FreeMusic(_music);
    _click = nullptr;
	_winSound = nullptr;
    _music = nullptr;
                                           //Destroy window and renderer
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	_window = nullptr;
	_renderer = nullptr;
                                                   //Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}

void MainGame::gameLoopAi() {
                                    // Init random engine for first move
    mt19937 rng(time(NULL));
    uniform_real_distribution<> startMove(0, 6);

    int curMoveCol = 0;  // Set that column starts at left side for user
    _victory = 0;                             // Set that no-one has won

    _board.setDifficulty(_difficulty);         // Set diff at game start

    SDL_Event event;                                    // Event handler

    while(_gameState == PLAYAI) {
                                       // Allow placement for human turn
        if((_currentTurn == _humanTurn) && (_gameState == PLAYAI)) {
                                                   // Get mouse position
            int x, y;
            SDL_GetMouseState( &x, &y );
                 // Continually update current column based on mouse pos
            if(x < 85) {
                curMoveCol = 0;
                _board.placeMarker(curMoveCol);
            }
            else if((x > 85) && (x < 170)) {
                curMoveCol = 1;
                _board.placeMarker(curMoveCol);
            }
            else if((x > 170) && (x < 255)) {
                curMoveCol = 2;
                _board.placeMarker(curMoveCol);
            }
            else if((x > 255) && (x < 340)) {
                curMoveCol = 3;
                _board.placeMarker(curMoveCol);
            }
            else if((x > 340) && (x < 425)) {
                curMoveCol = 4;
                _board.placeMarker(curMoveCol);
            }
            else if((x > 425) && (x < 510)) {
                curMoveCol = 5;
                _board.placeMarker(curMoveCol);
            }
            else if (x > 510) {
                curMoveCol = 6;
                _board.placeMarker(curMoveCol);
            }
        }

        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                _gameState = EXIT;              // Quit game by xing out
            }
							  // Escape to main menu if user pressed ESC
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				_board.clearBoard();
				_gameState = TITLE;
			}
                                 //If players turn draw marker on screen
            if( event.type == SDL_MOUSEBUTTONDOWN) {
                if((_currentTurn == _humanTurn) &&
                   (_gameState == PLAYAI)) {
                                      // Only place if column isn't full
                    if(_board.getColumnHeight(curMoveCol) < HEIGHT) {
                        _board.placeMove(curMoveCol, _currentTurn);
                                                          // Update turn
                        _currentTurn = _aiTurn;
                                        // Update depth limit for minmax
                        if(_board.getDepthLimit() < 42) {
                            _board.incDepthLimit();
                        }
                                               // Play click sound if on
                        if(_soundBool) {
                            Mix_PlayChannel( -1, _click, 0 );
                        }
                                                         //Check victory
                        _victory = _board.checkWin();
                    }
                }
            }
        }
                                                // Stop if there's a win
        if(_victory == PLAYER1 || _victory == PLAYER2 ||
           _victory == TIE) {
			if(_soundBool && _victory == _humanTurn) {
                Mix_PlayChannel( -1, _winSound, 0 );
            }
                              // Draw game again so user can see victory
            drawAiGame();
            SDL_Delay(5000);
                                   // Clear board and head back to title
            _gameState = TITLE;
            _board.clearBoard();
            break;
        }
                             // Only continue drawing if game isn't over
        else {
            drawAiGame();
        }

        if(_currentTurn == _aiTurn) {
                               // Random move first, to increase variety
            if(_board.getPiecesCount() == 1) {
                              // Delay on move so user can see placement
				if (_difficulty != DIFF5) {
                    SDL_Delay(800);
				}
                _board.placeMove(startMove(rng), _aiTurn);
                _currentTurn = _humanTurn;
				     // inc depth limit for base case board eval to work
                _board.incDepthLimit();
            }
            else {
                if (_difficulty != DIFF5) {
                    SDL_Delay(800);
				}
                                        // Call MinMax for move decision
                _board.placeMove(_ai.MinMax(_board), _aiTurn);
                _currentTurn = _humanTurn;
                if(_board.getDepthLimit() < 42) {
                    _board.incDepthLimit();
                }
            }
            if(_soundBool) {
                Mix_PlayChannel( -1, _click, 0 );
            }
            _victory = _board.checkWin();
        }
    }
}

void MainGame::drawAiGame() {

    int curX = 10;
    int curY = 10;
                                              // Clear screen with white
    SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer);
                                            // Draw background rectangle
    switch(_backColour) {
        case BLUE:
        SDL_SetRenderDrawColor( _renderer, 0x1E, 0x90, 0xFF, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
        case TURQ:
        SDL_SetRenderDrawColor( _renderer, 0x17, 0xB5, 0xCE, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
        case BROWN:
        SDL_SetRenderDrawColor( _renderer, 0xA1, 0x8A, 0x6A, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
    }
                      // For each x y draw piece evenly apart dep on val
    for(int x = 0; x < WIDTH; x++) {
        curY = 10;
        for(int y = 0; y < HEIGHT; y++) {
             // Fill board with white pieces first so marker alpha shows
            _gameSpriteSheet.render(curX, curY,
                &_gameClips[2], _renderer);
            switch(_board.getBoardValue(y, x)) {
                case PLAYER1:
                    _gameSpriteSheet.render(curX, curY,
                        &_gameClips[0 + _pieceOffset], _renderer);
                    break;
                case PLAYER2:
                    _gameSpriteSheet.render(curX, curY,
                        &_gameClips[1 + _pieceOffset], _renderer);
                    break;
                case MARKER:         // Only draw marker if human's turn
                    if(_currentTurn == _humanTurn) {
                        if(_humanTurn == PLAYER1) {
                            _gameAlphaSheet.render(curX, curY,
                              &_gameClips[0 + _pieceOffset], _renderer);
                        }
                        else {
                            _gameAlphaSheet.render(curX, curY,
                              &_gameClips[1 + _pieceOffset], _renderer);
                        }
                    }
                    break;
            }
               // 85 since texture is 75x75 so leave a 10 pixel wide gap
            curY += 85;
        }
        curX += 85;
    }
                                                 // Draw victory prompts
    if(_victory == TIE) {
        _buttonSpriteSheet.render(270, 242, &_txtClips[8], _renderer);
    }
    if(_victory == PLAYER1) {
        _buttonSpriteSheet.render(154, 237, &_txtClips[9], _renderer);
    }
    if(_victory == PLAYER2) {
        _buttonSpriteSheet.render(154, 237, &_txtClips[10], _renderer);
    }
                                                         //Update screen
    SDL_RenderPresent(_renderer);
}

void MainGame::selectLoop() {

    SDL_Event event;

    while(_gameState == SELECT) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                _gameState = EXIT;
            }
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				_gameState = TITLE;
			}
            for(int i = 0; i < 2; i++) {
                                                // Button event handling
                int temp = _selectButtons[i].handleEvent(&event);
                if(temp != -1) {
                    if(_soundBool) {
                        Mix_PlayChannel( -1, _click, 0 );
                    }
                                        // Goto playing comp from select
                    _gameState = PLAYAI;
                               // Assign players turn from button return
                    if(temp == PLAYER1) {
                        _humanTurn = PLAYER1;
                        _aiTurn = PLAYER2;
                        _ai.setAiPlayer(PLAYER2);
                    }
                    else {
                        _humanTurn = PLAYER2;
                        _aiTurn = PLAYER1;
                        _ai.setAiPlayer(PLAYER1);
                    }
                    _currentTurn = PLAYER1;
                }
            }
        }
        drawSelect();
    }
}

void MainGame::drawSelect() {

    SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer);

    switch(_backColour) {
        case BLUE:
        SDL_SetRenderDrawColor( _renderer, 0x1E, 0x90, 0xFF, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
        case TURQ:
        SDL_SetRenderDrawColor( _renderer, 0x17, 0xB5, 0xCE, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
        case BROWN:
        SDL_SetRenderDrawColor( _renderer, 0xA1, 0x8A, 0x6A, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
    }
                           // Render screen title "Player" in the middle
    _buttonSpriteSheet.render(236, 60, &_txtClips[1], _renderer);
                                             // Render buttons to screen
    for(int i = 0; i < 2; i++) {
        _selectButtons[i].render( &_buttonSpriteSheet,
            _buttonClips, _renderer);
    }

    SDL_RenderPresent(_renderer);
}

void MainGame::gameLoop2() {

    int curMoveCol = 0;
    _victory = 0;

    SDL_Event event;

    while(_gameState == PLAYFRIEND) {

        int x, y;
        SDL_GetMouseState( &x, &y );

        if(x < 85) {
            curMoveCol = 0;
            _board.placeMarker(curMoveCol);
        }
        else if((x > 85) && (x < 170)) {
            curMoveCol = 1;
            _board.placeMarker(curMoveCol);
        }
        else if((x > 170) && (x < 255)) {
            curMoveCol = 2;
            _board.placeMarker(curMoveCol);
        }
        else if((x > 255) && (x < 340)) {
            curMoveCol = 3;
            _board.placeMarker(curMoveCol);
        }
        else if((x > 340) && (x < 425)) {
            curMoveCol = 4;
            _board.placeMarker(curMoveCol);
        }
        else if((x > 425) && (x < 510)) {
            curMoveCol = 5;
            _board.placeMarker(curMoveCol);
        }
        else if (x > 510) {
            curMoveCol = 6;
            _board.placeMarker(curMoveCol);
        }

        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                _gameState = EXIT;
            }
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				_board.clearBoard();
				_gameState = TITLE;
			}
            if( event.type == SDL_MOUSEBUTTONDOWN) {
                                  // Only check to see if column is full
                if(_board.getColumnHeight(curMoveCol) < HEIGHT) {
                                             // Place move based on user
                    _board.placeMove(curMoveCol, _currentTurn);
                                                   // Switching of turns
                    if(_currentTurn == PLAYER1) {
                        _currentTurn = PLAYER2;
                    }
                    else {
                        _currentTurn = PLAYER1;
                    }
                    if(_soundBool) {
                        Mix_PlayChannel( -1, _click, 0 );
                    }
                    _victory = _board.checkWin();
                }
            }
        }

        if(_victory == PLAYER1 || _victory == PLAYER2 ||
           _victory == TIE) {
			if(_soundBool) {
                Mix_PlayChannel( -1, _winSound, 0 );
            }
            draw2Game();
            SDL_Delay(5000);
            _gameState = TITLE;
            _board.clearBoard();
        }
        else {
            draw2Game();
        }
    }
}

void MainGame::draw2Game() {
                 // Virtually Identical to Drawing AI except for Markers
    int curX = 10;
    int curY = 10;

    SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer);

    switch(_backColour) {
        case BLUE:
        SDL_SetRenderDrawColor( _renderer, 0x1E, 0x90, 0xFF, 0xFF );
        break;
        case TURQ:
        SDL_SetRenderDrawColor( _renderer, 0x17, 0xB5, 0xCE, 0xFF );
        break;
        case BROWN:
        SDL_SetRenderDrawColor( _renderer, 0xA1, 0x8A, 0x6A, 0xFF );
        break;
    }

    SDL_RenderFillRect(_renderer, &_backGround);

    for(int x = 0; x < WIDTH; x++)
    {
        curY = 10;
        for(int y = 0; y < HEIGHT; y++)
        {
            _gameSpriteSheet.render(curX, curY,
                &_gameClips[2], _renderer);

            switch(_board.getBoardValue(y, x))
            {
                case PLAYER1:
                    _gameSpriteSheet.render(curX, curY,
                        &_gameClips[0 + _pieceOffset], _renderer);
                    break;
                case PLAYER2:
                    _gameSpriteSheet.render(curX, curY,
                        &_gameClips[1 + _pieceOffset], _renderer);
                    break;
                case MARKER:
                    if(_currentTurn == PLAYER1) {
                        _gameAlphaSheet.render(curX, curY,
                            &_gameClips[0 + _pieceOffset], _renderer);
                    }
                    else {
                        _gameAlphaSheet.render(curX, curY,
                            &_gameClips[1 + _pieceOffset], _renderer);
                    }
                    break;
            }
            curY += 85;
        }
        curX += 85;
    }

    if(_victory == TIE) {
        _buttonSpriteSheet.render(270, 242, &_txtClips[8], _renderer);
    }
    if(_victory == PLAYER1) {
        _buttonSpriteSheet.render(154, 237, &_txtClips[9], _renderer);
    }
    if(_victory == PLAYER2) {
        _buttonSpriteSheet.render(154, 237, &_txtClips[10], _renderer);
    }

    SDL_RenderPresent(_renderer);
}

void MainGame::titleLoop() {

    SDL_Event event;

    while(_gameState == TITLE) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                _gameState = EXIT;
            }
                                 // Handle button events for game states
            for(int i = 0; i < 4; i++) {
                int temp = _titleButtons[i].handleEvent(&event);
                if(temp != -1) {
                    if(_soundBool) {
                        Mix_PlayChannel( -1, _click, 0 );
                    }
                                                    // Update game state
                    _gameState = temp;
                }
            }
        }
        drawTitle();
    }
}

void MainGame::drawTitle() {

    SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer);

    switch(_backColour) {
        case BLUE:
        SDL_SetRenderDrawColor( _renderer, 0x1E, 0x90, 0xFF, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
        case TURQ:
        SDL_SetRenderDrawColor( _renderer, 0x17, 0xB5, 0xCE, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
        case BROWN:
        SDL_SetRenderDrawColor( _renderer, 0xA1, 0x8A, 0x6A, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
    }
                                               // Render Connect 4 title
    _buttonSpriteSheet.render(190, 60, &_txtClips[0], _renderer);
                                                // Render Logo to screen
    _gameSpriteSheet.render(50, 30,
                            &_gameClips[1 + _pieceOffset], _renderer);
    _gameSpriteSheet.render(100, 30,
                            &_gameClips[0 + _pieceOffset], _renderer);

                                             // Render buttons on screen
    for(int i = 0; i < 4; i++) {
        _titleButtons[i].render( &_buttonSpriteSheet,
            _buttonClips, _renderer);
    }

    SDL_RenderPresent(_renderer);
}

void MainGame::optLoop() {

    SDL_Event event;

    while(_gameState == OPTIONS) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                _gameState = EXIT;
            }
                                      // Handle difficulty button events
            for(int i = 0; i < 5; i++) {
                int temp = _optionsButtons[i].handleEvent(&event);
                if(temp != -1) {
                    if(_soundBool) {
                        Mix_PlayChannel( -1, _click, 0 );
                    }
                    _difficulty = temp;
                }
            }
                               // Handle background colour button events
            for(int i = 5; i < 8; i++) {
                int temp = _optionsButtons[i].handleEvent(&event);
                if(temp != -1) {
                    if(_soundBool) {
                        Mix_PlayChannel( -1, _click, 0 );
                    }
                    _backColour = temp;
                }
            }
                                         // Handle music on button event
            int temp = _optionsButtons[8].handleEvent(&event);
            if(temp != -1) {
                if(_soundBool) {
                    Mix_PlayChannel( -1, _click, 0 );
                }
                Mix_ResumeMusic();
            }
                                         // Handle sound on button event
            temp = _optionsButtons[9].handleEvent(&event);
            if(temp != -1) {
                if(_soundBool) {
                    Mix_PlayChannel( -1, _click, 0 );
                }
                _soundBool = temp;
            }
                                        // Handle music off button event
            temp = _optionsButtons[10].handleEvent(&event);
            if(temp != -1) {
                if(_soundBool) {
                    Mix_PlayChannel( -1, _click, 0 );
                }
                Mix_PauseMusic();
            }
                                        // Handle sound off button event
            temp = _optionsButtons[11].handleEvent(&event);
            if(temp != -1) {
                if(_soundBool) {
                    Mix_PlayChannel( -1, _click, 0 );
                }
                _soundBool = temp;
            }
                                    // Handle piece colour button events
            for(int i = 12; i < 16; i++) {
                int temp = _optionsButtons[i].handleEvent(&event);
                if(temp != -1) {
                    if(_soundBool) {
                        Mix_PlayChannel( -1, _click, 0 );
                    }
                    _pieceOffset = temp;
                }
            }
                                            // Handle home button events
            temp = _optionsButtons[16].handleEvent(&event);
            if(temp != -1) {
                if(_soundBool) {
                    Mix_PlayChannel( -1, _click, 0 );
                }
                _gameState = temp;
            }

        }
        drawOpts();
    }
}

void MainGame::drawOpts() {

    SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer);

    switch(_backColour) {
        case BLUE:
        SDL_SetRenderDrawColor( _renderer, 0x1E, 0x90, 0xFF, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
        case TURQ:
        SDL_SetRenderDrawColor( _renderer, 0x17, 0xB5, 0xCE, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
        case BROWN:
        SDL_SetRenderDrawColor( _renderer, 0xA1, 0x8A, 0x6A, 0xFF );
        SDL_RenderFillRect(_renderer, &_backGround);
        break;
    }
                                                           // DIFFICULTY
    _buttonSpriteSheet.render(20, 125, &_txtClips[2], _renderer);
                                                                // MUSIC
    _buttonSpriteSheet.render(20, 195, &_txtClips[6], _renderer);
                                                                // SOUND
    _buttonSpriteSheet.render(25, 265, &_txtClips[5], _renderer);
                                                           // BACKGROUND
    _buttonSpriteSheet.render(20, 342, &_txtClips[4], _renderer);
                                                                    // /
    _buttonSpriteSheet.render(197, 202, &_txtClips[7], _renderer);
    _buttonSpriteSheet.render(197, 272, &_txtClips[7], _renderer);
                                                               // COLOUR
    _buttonSpriteSheet.render(20, 410, &_txtClips[3], _renderer);

    for(int i = 0; i < 17; i++) {
        _optionsButtons[i].render(&_buttonSpriteSheet,
								  _buttonClips, _renderer);
    }

    SDL_RenderPresent(_renderer);
}


