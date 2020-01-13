/***********************************************************************
 * Button.cpp
 * implementation file for button wrapper, code adapted from lazy foo's
 * tutorials: http://www.lazyfoo.net/tutorials/SDL/index.php
 * Link to specific tutorial:
 * http://www.lazyfoo.net/tutorials/SDL/17_mouse_events/index.php
 * 8 March 2017
 * ********************************************************************/

#include "Button.h"

Button::Button(int offset, int funct, int width, int height,
    int x, int y) {

    _position.x = x;
	_position.y = y;
	_width = width;
	_height = height;
	_clipOffset = offset;
	_funct = funct;
	_currentSprite = BUTTON_MOUSE_OFF;

}
void Button::render(Texture * texture, SDL_Rect clips[],
    SDL_Renderer* renderer) {

    texture->render(_position.x, _position.y,
        &clips[ _currentSprite + _clipOffset], renderer);
}

int Button::handleEvent(SDL_Event* event) {

    if( event->type == SDL_MOUSEMOTION ||
        event->type == SDL_MOUSEBUTTONDOWN) {
                                                   // Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);
                                          // Check if mouse is in button
		bool inside = true;
                                          // Mouse is left of the button
		if(x < _position.x) {
			inside = false;
		}
                                         // Mouse is right of the button
		else if(x > _position.x + _width) {
			inside = false;
		}
                                               // Mouse above the button
		else if(y < _position.y) {
			inside = false;
		}
                                               // Mouse below the button
		else if(y > _position.y + _height) {
			inside = false;
		}
                                              // Mouse is outside button
		if(!inside) {
			_currentSprite = BUTTON_MOUSE_OFF;
		}
		else {
		    _currentSprite = BUTTON_MOUSE_ON;
		    if(event -> type == SDL_MOUSEBUTTONDOWN) {
                           // If user click on button return set integer
                return _funct;
            }
		}
    }
                                       // -1 indicates no click occurred
    return -1;
}
