//
//  QuickTetrahedron.hpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 12/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef QuickTetrahedron_hpp
#define QuickTetrahedron_hpp

#include "Common.h"


class QuickTetrahedron {
    
public:
    QuickTetrahedron();
    QuickTetrahedron(int size);
    ~QuickTetrahedron();
    
    void drawVertices(float size);
    void drawTetrahedron();
    
private:
    std::vector<float> vertices;
    
    std::vector<float> colors;
    
};


#endif /* QuickTetrahedron_hpp */
