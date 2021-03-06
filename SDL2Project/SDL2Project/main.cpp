//
//  main.cpp
//  SDL2Project
//
//  Created by GEORGE QUENTIN on 27/01/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//


#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Screen.h"
#include <complex>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
    
    int width = 800;
    int height = 600;
    
    
    // Initialise screen
    Screen screen(width, height);
    if (screen.init() == false) {
        cout << "ERROR INITIALIZING SCREEN WITH SDL." << endl;
    }
    cout << "SDL Init succeeded." << endl;
    
    
    int max = 100;
    double scale = 4.0;
    while (true) {
        
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                
                
                double c_re = (col - width/2.0)*scale/width;
                double c_im = (row - height/2.0)*scale/width;
                double x = 0;
                double y = 0;
                int iteration = 0;
                
                while ( (x*x+y*y <= scale ) && iteration < max) {
                    
                    double x_new = x*x - y*y + c_re;
                    y = 2*x*y + c_im;
                    x = x_new;
                    iteration++;
                }
                
                //				std::complex<double> c ( scale * ( x - (width - height) / 2) / height * 4.0 - 2.0, scale * y / height * 4.0 - 2.0);
                //				std::complex<double> z (0.0, 0.0);
                //
                //				int iteration = 0;
                //				while (iteration < max){
                //					z = z * z + c;
                //
                //					if(abs(z) > 2.0)
                //						break;
                //
                //					++iteration;
                //				}
                
                
                if (iteration < max){
                    screen.setPixel(col, row, 0, 255, 255);
                } else {
                    screen.setPixel(col, row, 0, 0, 0);
                }
                
            }
        }
        
        // Draw the screen
        screen.update();
        
        // Check for messages/events
        if(screen.proccessEvents() == false){
            break;
        }
        
    }
    
    screen.close();
    
    
    
    return 0;
}

 
