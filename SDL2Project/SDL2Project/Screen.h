/*
 * Screen.h
 *
 *  Created on: Oct 9, 2016
 *      Author: GeorgeQuentin
 */



#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL2/SDL.h>
#include <iostream>

class Screen {

public:
	int SCREEN_WIDTH{0};
	int SCREEN_HEIGHT{0};

private:
	//using m for making that these are member variables
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_Texture *m_texture;
	Uint32 *m_buffer1;
	Uint32 *m_buffer2;
	const static int screenColor = 60;


public:
	Screen(int width, int height);
	virtual ~Screen();

	bool init();
	bool proccessEvents();
	void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
	void update();
	void boxBlur();
	void clear();
	void close();


};


#endif /* SCREEN_H_ */
