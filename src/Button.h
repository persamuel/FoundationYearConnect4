/***********************************************************************
 * Button.h
 * Header for button wrapper file, code adapted from lazy foo's
 * tutorials: http://www.lazyfoo.net/tutorials/SDL/index.php
 * Link to specific tutorial:
 * http://www.lazyfoo.net/tutorials/SDL/17_mouse_events/index.php
 * 8 March 2017
 * ********************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_image.h>
#include "Texture.h"
                                                  // Button image states
enum ButtonSprite {
    BUTTON_MOUSE_OFF = 0,
    BUTTON_MOUSE_ON
};

class Button {

    public:
        Button() {};
        Button(int offset, int funct, int width,
            int height, int x, int y);
                                                  //Handles mouse events
		int handleEvent( SDL_Event* );
                       // Renders button sprite based on clips passed in
		void render( Texture * texture, SDL_Rect clips[],
            SDL_Renderer* renderer );

	private:
        SDL_Point _position;                  // Position and dimensions
		int _width;
		int _height;
		int _funct;        // Integer to return from handing mouse click
		int _clipOffset;    // Offset from parameter clip list in render
		ButtonSprite _currentSprite;      // Current button state sprite
};

#endif // BUTTON_H
