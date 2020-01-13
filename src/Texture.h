/***********************************************************************
 * Texture.h
 * Header for texture wrapper file, code adapted from lazy foo's
 * tutorials: http://www.lazyfoo.net/tutorials/SDL/index.php
 * Link to specific tutorial:
 * http://www.lazyfoo.net/tutorials/SDL/
 * 11_clip_rendering_and_sprite_sheets/index.php
 * 8 March 2017
 * ********************************************************************/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
using std::string;

#include <SDL.h>
#include <SDL_image.h>
                                                 //Texture wrapper class
class Texture {
	public:
		Texture();
                                       //Deallocates texture on the heap
		~Texture();
		void free();
                                         //Loads image at parameter path
		bool loadFromFile(std::string path, SDL_Renderer* renderer);
                                            //Set transparency for image
		void setAlpha(Uint8 alpha);
                                        //Renders texture at given point
		void render(int x, int y, SDL_Rect* clip, SDL_Renderer* renderer);
                                                 //Gets image dimensions
		int getWidth();
		int getHeight();

	private:
                                                      //Base SDL texture
		SDL_Texture* _texture;
                                                      //Image dimensions
		int _width;
		int _height;
};

#endif // TEXTURE_H
