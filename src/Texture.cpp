/***********************************************************************
 * Texture.cpp
 * Implementation file for texture wrapper, code adapted from lazy foo's
 * tutorials: http://www.lazyfoo.net/tutorials/SDL/index.php
 * Link to specific tutorial:
 * http://www.lazyfoo.net/tutorials/SDL/
 * 11_clip_rendering_and_sprite_sheets/index.php
 * 8 March 2017
 * ********************************************************************/

#include "Texture.h"

Texture::Texture() {
                                       // Initialize the texture to NULL
	_texture = nullptr;
	_width = 0;
	_height = 0;
}

Texture::~Texture() {

    free();
}

void Texture::free() {

    if(_texture != NULL) {                  // Free texture if it exists
		SDL_DestroyTexture(_texture);
		_texture = nullptr;
		_width = 0;
		_height = 0;
	}
}

bool Texture::loadFromFile(string path, SDL_Renderer* renderer) {
                                 // Get rid of any pre-existing textures
	free();
                                     // Declare final texture for return
	SDL_Texture* newTexture = nullptr;
                                         // Load image at parameter path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL_image Error: %s\n",
            path.c_str(), IMG_GetError() );
	}

	else {
                                   // Colour key image to pure 255 green
		SDL_SetColorKey(loadedSurface, SDL_TRUE,
            SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0));
                   // Create texture from surface pixels of loaded image
        newTexture = SDL_CreateTextureFromSurface(renderer
            , loadedSurface);
		if(newTexture == nullptr) {
			printf( "Unable to create texture from %s! SDL Error: %s\n",
                path.c_str(), SDL_GetError() );
		}
                                                 // Get image dimensions
		else {
			_width = loadedSurface->w;
			_height = loadedSurface->h;
		}
                                        // Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
                       // Return success and set the new texture for use
	_texture = newTexture;
	return _texture != NULL;
}

void Texture::setAlpha(Uint8 alpha) {

	SDL_SetTextureAlphaMod(_texture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip, SDL_Renderer* renderer) {
                          // Set the rendering x y position on the image
	SDL_Rect renderQuad = { x, y, _width, _height };
                // Set rendering portions width and height based on clip
	if( clip != nullptr ) {                // Null means use whole image
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
                               // Use render parameter to draw to window
	SDL_RenderCopy( renderer, _texture, clip, &renderQuad );
}

int Texture::getWidth() {
	return _width;
}

int Texture::getHeight() {
	return _height;
}
