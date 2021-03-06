/*
 * Screen.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: GeorgeQuentin
 */


#include "Screen.h"
Screen::Screen(int width, int height) : SCREEN_WIDTH(width), SCREEN_HEIGHT(height),
	m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(NULL) {
	// TODO Auto-generated constructor stub

	//cout << "SCREEN CONSTRUCTOR " << endl;
}

bool Screen::init() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		//cout << "SDL Init failed." << endl;
		return false;
	}

	m_window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (m_window == NULL) {
		//cout << "WINDOW failed." << SDL_GetError() << endl;
		SDL_Quit();
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (m_renderer == NULL) {
		//cout << "Could not create renderer." << SDL_GetError() << endl;
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return 3;
	}

	if (m_texture == NULL) {
		//cout << "Could not create texture." << SDL_GetError() << endl;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return 4;
	}

	m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

	memset(m_buffer1, screenColor, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	memset(m_buffer2, screenColor, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

//	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
//		m_buffer1[i] = 0x00FFFFFF;
//	}

	return true;
}

void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue){

	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
	{
		return;
	}
	Uint32 color = 0;

	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF;  // this is alpha channel FF, it is opaque

	//RGBA

	//this (y * SCREEN_WIDTH) means we have to multiply y by the screen width, it allows us to cover all the rows. as each row has a screen width, so when we multiply screen width by y value we go down each row
	//we then add x to move to the right, this will cover all the pixel horizontally, it just means go to the right after each column is complete
	m_buffer1[(y * SCREEN_WIDTH) + x] = color;


}

void Screen::clear(){

	memset(m_buffer1, screenColor, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

}

void Screen::boxBlur(){
	//swap the buffers so pixels is in m_buffer2 and we are drawing in m_buffer1.

	Uint32 *temp = m_buffer1; //first we store the location m_buffer1 was pointing at
	m_buffer1 = m_buffer2; //then we change m_buffer1 to point at m_buffer2 memory which is clean.
	m_buffer2 = temp; // then we tell m_buffer2 to point at the original value of stored in m_buffer1

;

	//now we are going to keep blurring the screen more and more
	for (int y=0; y < SCREEN_HEIGHT; y++){

		for(int x=0; x < SCREEN_WIDTH; x++){


			/*
			 * 0 0 0
			 * 0 1 0
			 * 0 0 0
			 *
			 */

			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;

			// this means that whatever pixel we are looking at, we will end up calling the  other pixels around it
			for(int row= -1; row<=1; row++){
				for(int col= -1; col<=1; col++){

					int currentX = x + col;
					int currentY = y + row;

					if (currentX >= 0 && SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT){
						Uint32 color = m_buffer2[currentY*SCREEN_WIDTH + currentX];

						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;


						redTotal += red;
						greenTotal += green;
						blueTotal += blue;

					}

				}
			}


			Uint8 redResult = redTotal / 9;
			Uint8 greenResult = greenTotal / 9;
			Uint8 blueResult = blueTotal / 9;

			setPixel(x, y, redResult, greenResult, blueResult);


		}
	}


}

void Screen::update() {


	SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);

}


bool Screen::proccessEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT){
            return false;
            break;
        }
        
        
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{

            
		    if (event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
		    {
		    	std::cout << "Mouse Down Left = (" << event.motion.x << ", " << event.motion.y << ")\n" ;
		    }
		    if (event.button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
			{
				std::cout << "Mouse Down Right = (" << event.motion.x << ", " << event.motion.y << ")\n" ;
			}

			if (event.button.button == SDL_BUTTON(SDL_BUTTON_MIDDLE))
			{
				std::cout << "Mouse Down Middle = (" << event.motion.x << ", " << event.motion.y << ")\n" ;
			}

		}

		if (event.type == SDL_MOUSEBUTTONUP){

			if (event.button.button == SDL_BUTTON_LEFT)
			{
				std::cout << "Mouse Up Left = (" << event.motion.x << ", " << event.motion.y << ")\n" ;
			}
		}



		if (event.type == SDL_KEYDOWN){

			switch( event.key.keysym.sym ){
				case SDLK_LEFT:
					std::cout << "Key Down Left\n" ;
					break;
				case SDLK_RIGHT:
					std::cout << "Key Down Right \n" ;
					break;
				case SDLK_UP:
					std::cout << "Key Down Up\n" ;
					break;
				case SDLK_DOWN:
					std::cout << "Key Down Down\n" ;
					break;
                case SDLK_SPACE:
                    std::cout << "Space Down\n" ;
                    break;
				default:
					break;
			}

		}

		if (event.type == SDL_KEYUP){

			switch( event.key.keysym.sym ){
				case SDLK_LEFT:
					std::cout << "Key Up Left\n" ;
					break;
				case SDLK_RIGHT:
					std::cout << "Key Up Right \n" ;
					break;
				case SDLK_UP:
					std::cout << "Key Up Up\n" ;
					break;
				case SDLK_DOWN:
					std::cout << "Key Up Down\n" ;
					break;
				case SDLK_SPACE:
					std::cout << "Key space Up\n" ;
					break;
				default:
					break;
			}

		}

	}


	return true;
}


void Screen::close() {

	delete[] m_buffer1;
	delete[] m_buffer2;
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);

	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

Screen::~Screen() {
	// TODO Auto-generated destructor stub

	//cout << "SCREEN DESTRUCTOR " << endl;
}

