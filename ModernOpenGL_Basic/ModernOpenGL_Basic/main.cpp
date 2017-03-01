//
//  main.cpp
//  OpenGL_Essentials
//
//  Created by GEORGE QUENTIN on 15/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Common.h"

#include "Shader.h"
#include "Mesh.h"
#include "Pyramid.h"
#include "Texture.h"
#include "TorusKnotMesh.h"
#include "TorusMesh.h"

#define SCREEN_WIDTH 1020
#define SCREEN_HEIGHT 720



static void MakeCylinder(float radius, float length, unsigned int numSteps, glm::vec3  * m_verts, unsigned int * m_indices)
{
    
    //glm::vec3  *m_verts = new glm::vec3[(numSteps * 2 + 2)];
    m_verts = new glm::vec3[(numSteps * 2 + 2)];
    
    float hl = length * 0.5f;
    
    float a = 0.0f;
    
    float pi2 = glm::two_pi<float>();//TWO_PI
    
    float step = M_2_PI / (float)numSteps;
    
    for (int i = 0; i < numSteps; ++i)
        
    {
        
        float x = cos(a) * radius;
        
        float y = sin(a) * radius;
        
        m_verts[i] = glm::vec3(x, y, hl);
        
        m_verts[i + numSteps] = glm::vec3(x, y, -hl);
        
        a += step;
        
    }
    
    
    
    m_verts[numSteps * 2 + 0] = glm::vec3(0.0f, 0.0f, +hl);
    
    m_verts[numSteps * 2 + 1] = glm::vec3(0.0f, 0.0f, -hl);
    
    
    //unsigned int  *m_indices = new unsigned int[(4 * numSteps * 3)];
    m_indices = new unsigned int[(4 * numSteps * 3)];
    
    
    for (int i = 0; i < numSteps; ++i)
        
    {
        
        unsigned int i1 = i;
        
        unsigned int i2 = (i1 + 1) % numSteps;
        
        unsigned int i3 = i1 + numSteps;
        
        unsigned int i4 = i2 + numSteps;
        
        
        
        // Sides
        
        
        
        m_indices[i * 6 + 0] = i1;
        
        m_indices[i * 6 + 1] = i3;
        
        m_indices[i * 6 + 2] = i2;
        
        
        
        m_indices[i * 6 + 3] = i4;
        
        m_indices[i * 6 + 4] = i2;
        
        m_indices[i * 6 + 5] = i3;
        
        
        
        // Caps
        
        
        
        m_indices[numSteps * 6 + i * 6 + 0] = numSteps * 2 + 0;
        
        m_indices[numSteps * 6 + i * 6 + 1] = i1;
        
        m_indices[numSteps * 6 + i * 6 + 2] = i2;
        
        
        
        m_indices[numSteps * 6 + i * 6 + 3] = numSteps * 2 + 1;
        
        m_indices[numSteps * 6 + i * 6 + 4] = i4;
        
        m_indices[numSteps * 6 + i * 6 + 5] = i3;
        
    }
    
}


int main(int argc, const char * argv[])  {
    
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return 1;
    } 
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//We don't want the old OpenGL and want the core profile
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Open GL", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw std::runtime_error("glewInit failed");
        exit(EXIT_FAILURE);
        return 1;
    }
    
    if(!GLEW_VERSION_3_2){
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    }
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    const GLubyte* vendor = glGetString(GL_VENDOR); // vendor as a string
    const GLubyte* shaderLanguage = glGetString(GL_SHADING_LANGUAGE_VERSION); // shader lang as a string
    // print out some info about the graphics drivers
    std::cout << "OpenGL version supported: " << version << std::endl;
    std::cout << "GLSL version: " << shaderLanguage << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "Vendor: " << vendor << std::endl;
    
    // OpenGL settings
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable the use of our depth buffer 
    glEnable(GL_CULL_FACE);//only front facinf poligons are rendered, forn facing have a clock wise or counter clock wise order
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    //load shader program
    std::string path = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLProject/ModernOpenGL_Basic/ModernOpenGL_Basic";
    Shader shader(path + "/res/shaders/simpleShader");
    
    //load mesh
    Vertex triangleVertices[] = {
        //vertices positions                          //texture             //colors
        Vertex( glm::vec3(0.0f,0.8f,0.0f), glm::vec2(0.0f,0.5f), glm::vec3(0.0f,0.8f,0.0f)   ),
        Vertex( glm::vec3(-0.5f,-0.2f,0.0f), glm::vec2(0.25f,1.0f), glm::vec3(0.8f,0.8f,0.0f) ),
        Vertex( glm::vec3(0.5f,-0.2f,0.0f), glm::vec2(0.5f,0.5f), glm::vec3(0.8f,0.8f,0.0f) )
    };
    
//////////////////////////////////////////////////////////////////////////
///////////////////////////TORUS INCOMPLETE MODEL/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

    std::vector<Vertex> torusVerts;{

        int numc = 50; 
        int numt = 55;
        int ii, jj, k;
        float s, t, x, y, z, twopi;
        float radius = 2.0f;
        float tick = 1.3f;
        twopi = 2 * glm::pi <float> ();
        
        for (ii = 0; ii < numc; ii++) {
            for (jj = 0; jj <= numt; jj++) {
                for (k = 1; k >= 0; k--) {
                    s = (ii + k) % numc + 0.5;
                    t = jj % numt;
                    
                    x = (radius + tick * cos(s * twopi/numc)) * cos( t * twopi / numt);
                    y = (radius + tick * cos(s * twopi/numc)) * sin( t * twopi / numt);
                    z = tick * sin(s * twopi / numc);
                    
                    glm::vec3 position = glm::vec3(x,y,z);
                    torusVerts.push_back( Vertex( position, glm::vec2(1.0f,0.5f), glm::vec3(1.0f,0.8f,1.0f)  ) );
                }
            }
        }
    }
    TorusMesh torusMesh( torusVerts.data(), torusVerts.size() );
    Shader shader6(path + "/res/shaders/basicShader");
    Texture texture6(path + "/res/textures/space_galaxy.jpg", true);
    Transform transform6;
    
    
//////////////////////////////////////////////////////////////////////////
///////////////////////////TORUS KNOT MODEL/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

    vector<Vertex> torusKnotVertices;
    
    int aSteps = 1024;           // in: Number of steps in the torus knot
    int aFacets = 32;          // in: Number of facets
    float aScale = 4.0f;         // in: Scale of the knot
    float aThickness = 0.5f;     // in: Thickness of the knot
    float aClumps = 0.0f;        // in: Number of clumps in the knot
    float aClumpOffset = 0.0f;   // in: Offset of the clump (in 0..2pi)
    float aClumpScale = 0.0f;    // in: Scale of a clump
    float aUScale = 2.0f;        // in: U coordinate scale
    float aVScale = 32.0f;        // in: V coordinate scale
    float aP = 3;//3.0f;             // in: P parameter of the knot
    float aQ = 2;//-7.0f;             // in: Q parameter of the knot
 
    int i, j;
    aThickness *= aScale;
    float pi2 = glm::two_pi<float>();
    
    GLfloat *vtx      = new GLfloat[((aSteps + 1) * (aFacets + 1) + 1) * 3];
    GLfloat *normal   = new GLfloat[((aSteps + 1) * (aFacets + 1) + 1) * 3];
    GLfloat *texcoord = new GLfloat[((aSteps + 1) * (aFacets + 1) + 1) * 2];
    GLuint  *torusIndices      = new GLuint[(aSteps + 1) * aFacets * 2];
    
    int numIndices = (aSteps + 1) * aFacets * 2;
    int numVertices = ((aSteps + 1) * (aFacets + 1) + 1);
    {
        for (j = 0; j < aFacets; j++)
        {
            for (i = 0; i < aSteps + 1; i++)
            {
                torusIndices[i * 2 + 0 + j * (aSteps + 1) * 2] = ((j + 1) + i * (aFacets + 1));
                torusIndices[i * 2 + 1 + j * (aSteps + 1) * 2] = (j + i * (aFacets + 1));
            }
            
        }
        
        
        for (i = 0; i < aSteps; i++)
        {       
            float centerpoint[3];
            float Pp = aP * i * pi2 / aSteps;
            float Qp = aQ * i * pi2 / aSteps;
            float r = (.5f * (2 + (float)sin(Qp))) * aScale;
            centerpoint[0] = r * (float)cos(Pp);
            centerpoint[1] = r * (float)cos(Qp);
            centerpoint[2] = r * (float)sin(Pp);
            
            float nextpoint[3];
            Pp = aP * (i + 1) * pi2 / aSteps;
            Qp = aQ * (i + 1) * pi2 / aSteps;
            r = (.5f * (2 + (float)sin(Qp))) * aScale;
            nextpoint[0] = r * (float)cos(Pp);
            nextpoint[1] = r * (float)cos(Qp);
            nextpoint[2] = r * (float)sin(Pp);
            
            float T[3];
            T[0] = nextpoint[0] - centerpoint[0];
            T[1] = nextpoint[1] - centerpoint[1];
            T[2] = nextpoint[2] - centerpoint[2];
            
            float N[3];
            N[0] = nextpoint[0] + centerpoint[0];
            N[1] = nextpoint[1] + centerpoint[1];
            N[2] = nextpoint[2] + centerpoint[2];
            
            
            float B[3];
            B[0] = T[1]*N[2] - T[2]*N[1];
            B[1] = T[2]*N[0] - T[0]*N[2];
            B[2] = T[0]*N[1] - T[1]*N[0];
            
            N[0] = B[1]*T[2] - B[2]*T[1];
            N[1] = B[2]*T[0] - B[0]*T[2];
            N[2] = B[0]*T[1] - B[1]*T[0];
            
            float l;
            l = (float)sqrt(B[0] * B[0] + B[1] * B[1] + B[2] * B[2]);
            B[0] /= l;
            B[1] /= l;
            B[2] /= l;
            
            l = (float)sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);
            N[0] /= l;
            N[1] /= l;
            N[2] /= l;
            
            for (j = 0; j < aFacets; j++)
            {
                
                float pointx = (float)sin(j * pi2 / aFacets) * aThickness * (((float)sin(aClumpOffset + aClumps * i * pi2 / aSteps) * aClumpScale) + 1);
                float pointy = (float)cos(j * pi2 / aFacets) * aThickness * (((float)cos(aClumpOffset + aClumps * i * pi2 / aSteps) * aClumpScale) + 1);
                
                vtx[i * (aFacets + 1) * 3 + j * 3 + 0] = N[0] * pointx + B[0] * pointy + centerpoint[0];
                vtx[i * (aFacets + 1) * 3 + j * 3 + 1] = N[1] * pointx + B[1] * pointy + centerpoint[1];
                vtx[i * (aFacets + 1) * 3 + j * 3 + 2] = N[2] * pointx + B[2] * pointy + centerpoint[2];
                
                normal[i * (aFacets + 1) * 3 + j * 3 + 0] = vtx[i * (aFacets + 1) * 3 + j * 3 + 0] - centerpoint[0];
                normal[i * (aFacets + 1) * 3 + j * 3 + 1] = vtx[i * (aFacets + 1) * 3 + j * 3 + 1] - centerpoint[1];
                normal[i * (aFacets + 1) * 3 + j * 3 + 2] = vtx[i * (aFacets + 1) * 3 + j * 3 + 2] - centerpoint[2];
                
                float l;
                l = (float)sqrt(normal[i * (aFacets + 1) * 3 + j * 3 + 0] * normal[i * (aFacets + 1) * 3 + j * 3 + 0] +
                                normal[i * (aFacets + 1) * 3 + j * 3 + 1] * normal[i * (aFacets + 1) * 3 + j * 3 + 1] +
                                normal[i * (aFacets + 1) * 3 + j * 3 + 2] * normal[i * (aFacets + 1) * 3 + j * 3 + 2]);
                
                texcoord[i * (aFacets + 1) * 2 + j * 2 + 0] = ((float)j / aFacets) * aUScale;
                texcoord[i * (aFacets + 1) * 2 + j * 2 + 1] = ((float)i / aSteps) * aVScale;
                
                torusKnotVertices.push_back( 
                Vertex( glm::vec3(vtx[    i * (aFacets + 1) * 3 + j * 3 + 0], 
                                  vtx[    i * (aFacets + 1) * 3 + j * 3 + 1], 
                                  vtx[    i * (aFacets + 1) * 3 + j * 3 + 2]), 
                       glm::vec2(texcoord[i * (aFacets + 1) * 2 + j * 2 + 0],
                                 texcoord[i * (aFacets + 1) * 2 + j * 2 + 1]), 
                       glm::vec3(normal[  i * (aFacets + 1) * 3 + j * 3 + 0], 
                                 normal[  i * (aFacets + 1) * 3 + j * 3 + 1],
                                 normal[  i * (aFacets + 1) * 3 + j * 3 + 2])  
                       ) 
                    );
                
                
                normal[i * (aFacets + 1) * 3 + j * 3 + 0] /= l;
                normal[i * (aFacets + 1) * 3 + j * 3 + 1] /= l;
                normal[i * (aFacets + 1) * 3 + j * 3 + 2] /= l;
            }
            // create duplicate vertex for sideways wrapping
            // otherwise identical to first vertex in the 'ring' except for the U coordinate
            vtx[i * (aFacets + 1) * 3 + aFacets * 3 + 0] = vtx[i * (aFacets + 1) * 3 + 0];
            vtx[i * (aFacets + 1) * 3 + aFacets * 3 + 1] = vtx[i * (aFacets + 1) * 3 + 1];
            vtx[i * (aFacets + 1) * 3 + aFacets * 3 + 2] = vtx[i * (aFacets + 1) * 3 + 2];
            
            normal[i * (aFacets + 1) * 3 + aFacets * 3 + 0] = normal[i * (aFacets + 1) * 3 + 0];
            normal[i * (aFacets + 1) * 3 + aFacets * 3 + 1] = normal[i * (aFacets + 1) * 3 + 1];
            normal[i * (aFacets + 1) * 3 + aFacets * 3 + 2] = normal[i * (aFacets + 1) * 3 + 2];
            
            texcoord[i * (aFacets + 1) * 2 + aFacets * 2 + 0] = aUScale;
            texcoord[i * (aFacets + 1) * 2 + aFacets * 2 + 1] = texcoord[i * (aFacets + 1) * 2 + 1];
            
            torusKnotVertices.push_back( 
            Vertex( glm::vec3(vtx[    i * (aFacets + 1) * 3 + aFacets * 3 + 0], 
                              vtx[    i * (aFacets + 1) * 3 + aFacets * 3 + 1], 
                              vtx[    i * (aFacets + 1) * 3 + aFacets * 3 + 2]), 
                   glm::vec2(texcoord[i * (aFacets + 1) * 2 + aFacets * 2 + 0],
                             texcoord[i * (aFacets + 1) * 2 + aFacets * 2 + 1]), 
                   glm::vec3(normal[  i * (aFacets + 1) * 3 + aFacets * 3 + 0], 
                             normal[  i * (aFacets + 1) * 3 + aFacets * 3 + 1],
                             normal[  i * (aFacets + 1) * 3 + aFacets * 3 + 2])  
                   ) 
            );
        }
        
        // create duplicate ring of vertices for longways wrapping
        // otherwise identical to first 'ring' in the knot except for the V coordinate
        for (j = 0; j < aFacets; j++)
        {
            vtx[aSteps * (aFacets + 1) * 3 + j * 3 + 0] = vtx[j * 3 + 0];
            vtx[aSteps * (aFacets + 1) * 3 + j * 3 + 1] = vtx[j * 3 + 1];
            vtx[aSteps * (aFacets + 1) * 3 + j * 3 + 2] = vtx[j * 3 + 2];
            
            normal[aSteps * (aFacets + 1) * 3 + j * 3 + 0] = normal[j * 3 + 0];
            normal[aSteps * (aFacets + 1) * 3 + j * 3 + 1] = normal[j * 3 + 1];
            normal[aSteps * (aFacets + 1) * 3 + j * 3 + 2] = normal[j * 3 + 2];
            
            texcoord[aSteps * (aFacets + 1) * 2 + j * 2 + 0] = texcoord[j * 2 + 0];
            texcoord[aSteps * (aFacets + 1) * 2 + j * 2 + 1] = aVScale;
            
            torusKnotVertices.push_back( 
            Vertex( glm::vec3(vtx[    aSteps * (aFacets + 1) * 3 + j * 3 + 0], 
                              vtx[    aSteps * (aFacets + 1) * 3 + j * 3 + 1], 
                              vtx[    aSteps * (aFacets + 1) * 3 + j * 3 + 2]), 
                   glm::vec2(texcoord[aSteps * (aFacets + 1) * 2 + j * 2 + 0],
                             texcoord[aSteps * (aFacets + 1) * 2 + j * 2 + 1]), 
                   glm::vec3(normal[  aSteps * (aFacets + 1) * 3 + j * 3 + 0], 
                             normal[  aSteps * (aFacets + 1) * 3 + j * 3 + 1],
                             normal[  aSteps * (aFacets + 1) * 3 + j * 3 + 2])  
                   ) 
            );
        }
        
        // finally, there's one vertex that needs to be duplicated due to both U and V coordinate.
        
        vtx[aSteps * (aFacets + 1) * 3 + aFacets * 3 + 0] = vtx[0];
        vtx[aSteps * (aFacets + 1) * 3 + aFacets * 3 + 1] = vtx[1];
        vtx[aSteps * (aFacets + 1) * 3 + aFacets * 3 + 2] = vtx[2];
        
        normal[aSteps * (aFacets + 1) * 3 + aFacets * 3 + 0] = normal[0];
        normal[aSteps * (aFacets + 1) * 3 + aFacets * 3 + 1] = normal[1];
        normal[aSteps * (aFacets + 1) * 3 + aFacets * 3 + 2] = normal[2];
        
        texcoord[aSteps * (aFacets + 1) * 2 + aFacets * 2 + 0] = aUScale;
        texcoord[aSteps * (aFacets + 1) * 2 + aFacets * 2 + 1] = aVScale;
        
        torusKnotVertices.push_back( 
        Vertex( glm::vec3(vtx[    aSteps * (aFacets + 1) * 3 + aFacets * 3 + 0], 
                          vtx[    aSteps * (aFacets + 1) * 3 + aFacets * 3 + 1], 
                          vtx[    aSteps * (aFacets + 1) * 3 + aFacets * 3 + 2]), 
               glm::vec2(texcoord[aSteps * (aFacets + 1) * 2 + aFacets * 2 + 0],
                         texcoord[aSteps * (aFacets + 1) * 2 + aFacets * 2 + 1]), 
               glm::vec3(normal[  aSteps * (aFacets + 1) * 3 + aFacets * 3 + 0], 
                         normal[  aSteps * (aFacets + 1) * 3 + aFacets * 3 + 1],
                         normal[  aSteps * (aFacets + 1) * 3 + aFacets * 3 + 2])  
               ) 
        );
    }
    TorusKnotMesh torusMeshWithIndices( torusKnotVertices.data(), 
                               numVertices, 
                               torusIndices,
                               numIndices
                               );
    Shader shader5(path + "/res/shaders/basicShader");
    Texture texture5(path + "/res/textures/colors-wallpaper.jpg", true);
    Transform transform5;
    
    
    
//////////////////////////////////////////////////////////////////////////
///////////////////////////SPHERE MODEL/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
    
    std::vector<Vertex> sphereVerts;
    std::vector<unsigned int> sphereIndices; {
    
        unsigned int StacksIn = 50;
        unsigned int SlicesIn = 50;
        float Radius = 1.0f;
        
        // Calc The Vertices
        for (int stacks = 0; stacks <= StacksIn; ++stacks){
            
            float V   = stacks / (float) StacksIn;
            float phi = V * glm::pi <float> ();
            
            // Loop Through Slices
            for (int slices = 0; slices <= SlicesIn; ++slices){
                
                float U = slices / (float) SlicesIn;
                float theta = U * (glm::pi <float> () * 2);
                
                // Calc The Vertex Positions
                float x = cosf (theta) * sinf (phi);
                float y = cosf (phi);
                float z = sinf (theta) * sinf (phi);
                
                glm::vec3 position = glm::vec3(x,y,z) * Radius;
                glm::vec2 textureCoord = glm::vec2(slices / (float) SlicesIn, stacks / (float) StacksIn);
                glm::vec3 normal = position;
                
                Vertex vertex = Vertex(position , textureCoord, normal  );
                
                // Push Back Vertex Data
                sphereVerts.push_back ( vertex );
            }
        }
        
        // Calc The Index Positions
        for (int i = 0; i < SlicesIn * StacksIn + SlicesIn; ++i){
            
            sphereIndices.push_back (i);
            sphereIndices.push_back (i + SlicesIn + 1);
            sphereIndices.push_back (i + SlicesIn);
            
            sphereIndices.push_back (i + SlicesIn + 1);
            sphereIndices.push_back (i);
            sphereIndices.push_back (i + 1);
        }
    }
    
  
    // next mesh
    Mesh sphereMeshWithIndices( sphereVerts.data(), 
                               sphereVerts.size(), 
                               sphereIndices.data(),
                               sphereIndices.size(),
                               true
                               );
    

    Shader shader4(path + "/res/shaders/basicShader");
    Texture texture4(path + "/res/textures/blue-frozen-water.jpg", true);
    Transform transform4;
    
//////////////////////////////////////////////////////////////////////////
///////////////////////////CUBE MODEL/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
    
   
    /////6--------------/5
    ////  .           // |
    //2--------------1   |
    //    .          |   |
    //    .          |   |
    //    .          |   |
    //    .          |   |
    //    7......... |   /4
    //               | //
    //3--------------/0
    
    float size = 1.0f;
    //Specify the 8 VERTICES of A Cube
    vector<Vertex> cubeUniqueVertices = { 
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 0.0f ) ), // 0
        Vertex( glm::vec3( size, size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 0.0f, 1.0f, 1.0f ) ), // 1
        Vertex( glm::vec3( -size, size, size ), glm::vec2(0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 2
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 3
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 0.0f, 0.0f ) ), // 4
        Vertex( glm::vec3( size, size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 0.0f, 1.0f ) ), // 5
        Vertex( glm::vec3( -size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 6
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 1.0f, 1.0f ) ), // 7
    };
    
    Vertex cubeVertices[] = {
        //vertices positions                          //texture             //colors
        //bottom 3,7,4  4,0,3
        Vertex( glm::vec3(  -size,-size,size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 0
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 1
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 2
        Vertex( glm::vec3(  size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 3
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 4
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 5
        
        //top   1,5,6   6,2,1
        Vertex( glm::vec3( size,size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 6
        Vertex( glm::vec3( size, size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 7
        Vertex( glm::vec3(  -size, size,-size ), glm::vec2(0.0f,1.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 8
        Vertex( glm::vec3( -size, size,-size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 9
        Vertex( glm::vec3( -size, size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 10
        Vertex( glm::vec3( size, size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 11
        
        //front 0,1,2    2,3,0
        Vertex( glm::vec3(  size,-size,size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 12
        Vertex( glm::vec3( size, size, size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 13
        Vertex( glm::vec3( -size, size, size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 14
        Vertex( glm::vec3( -size,size, size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 15
        Vertex( glm::vec3(  -size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 16
        Vertex( glm::vec3( size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 17
        
        //back  7,6,5   5,4,7
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 18
        Vertex( glm::vec3( -size, size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 19
        Vertex( glm::vec3( size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 20
        Vertex( glm::vec3( size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 21
        Vertex( glm::vec3( size, -size,-size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 22
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 23
        
        //left   3,2,6  6,7,3
        Vertex( glm::vec3( -size,-size,size), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 24
        Vertex( glm::vec3( -size, size, size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 25
        Vertex( glm::vec3( -size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 26
        Vertex( glm::vec3( -size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 27
        Vertex( glm::vec3( -size, -size, -size), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 28
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 29
        
        //right   4,5,1   1,0,4
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 30
        Vertex( glm::vec3( size, size,-size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 31
        Vertex( glm::vec3( size, size, size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 32
        Vertex( glm::vec3( size, size, size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 33
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 34
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 35
    };
    vector<unsigned int> cubeIndices = { 
        //bottom 
        3,7,4,  
        4,0,3,
        //top   
        1,5,6,
        6,2,1,
        //front 
        0,1,2,
        2,3,0,
        //back  
        7,6,5,
        5,4,7,
        //left   
        3,2,6,
        6,7,3,
        //right   
        4,5,1,
        1,0,4
    }; 
    
  
    bool shouldUseIndices = true;
    Mesh cubeMeshWithIndices( cubeUniqueVertices.data(), 
              cubeUniqueVertices.size(), 
              cubeIndices.data(),
              cubeIndices.size(),
              shouldUseIndices
              );
    

    
    //load texture;
    Texture texture(path + "/res/textures/spiralcolor.jpg", true);
    
    //setup camera
    float aspect = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
    Camera camera(glm::vec3(0.0f, 0.0f,-15.0f), 70.0f, aspect, 0.1f, 5000.0f);
    
    //transform model
    Transform transform;
    float counter = 0.0f;
    
    
    
//////////////////////////////////////////////////////////////////////////
///////////////////////////PYRAMID MODEL/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
    /////---- 4-------
    ////    . . .  .             
    //    .  .   .  .      
    //   .  .     .   .   
    //  .  .       .    .
    // .   2......... |  /3
    //.              . //
    //0--------------/1
    
    vector<Vertex> pyramidVertices = {
        //vertices positions                          //texture             //colors
        //bottom 0,2,3  3,1,0
        Vertex( glm::vec3(  -size,-size,size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 0
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 1
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 2
        Vertex( glm::vec3(  size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 3
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 4
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 5
        
        //front   4,0,1
        Vertex( glm::vec3( 0.0f, size, 0.0f ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 6
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 7
        Vertex( glm::vec3(  size, -size,size ), glm::vec2(0.0f,1.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 8
        
        //back    4,3,2
        Vertex( glm::vec3( 0.0f, size, 0.0f ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 9
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 10
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 11
        
        //left   4,2,0
        Vertex( glm::vec3( 0.0f, size, 0.0f ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 9
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 10
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 11
        
        //right  4,1,3
        Vertex( glm::vec3( 0.0f, size, 0.0f ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 9
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 10
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 11
        
    };
    
    unsigned int indices[] = {0};
    Mesh pyramidmesh( pyramidVertices.data(), 
                     pyramidVertices.size(), 
                     indices,
                     sizeof(indices)/sizeof(indices[0]),
                     false
                     );
    Shader shader2(path + "/res/shaders/basicShader");
    Texture texture2(path + "/res/textures/bricks.jpg", true);
    Transform transform2;
    
//////////////////////////////////////////////////////////////////////////
///////////////////////////MESH MODEL/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
    Shader shader3(path + "/res/shaders/basicShader");
    Texture texture3(path + "/res/textures/space_galaxy.jpg", true);
    Transform transform3;
    Mesh meshModel(path + "/res/model/monkey3.obj");
    
    
    while(!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClearColor(0.1f, 0.4f, 0.4f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//enable and clear the color and depth buffer
        
        //Render cube 
        {
           
            float sinCounter = sinf(counter);
            float conCounter = cosf(counter);
            
            transform.GetPositions()->x = sinCounter;
            transform.GetPositions()->z = conCounter;
            transform.GetRotation()->x = counter * 25;
            transform.GetRotation()->y = counter * 15;
            transform.GetRotation()->z = counter * 5;
            //transform.SetScale(glm::vec3(conCounter, conCounter, conCounter));
            transform2.SetScale(glm::vec3(10,10,10));
            
            transform.SetPositions(glm::vec3(0.0f, -2.0f, -4.0f) );
            
            // bind the shader program 
            shader.Bind();
            
            //update shader, including the tranform of our mesh, and the camera view of the mesh
            shader.Update(transform, camera, true);
            
            //binding texture at zero unit
            texture.Bind(0);
            
            //drawing our meshes
            cubeMeshWithIndices.Draw(true);
            
            //unbind texture
            texture.UnBind();
            
            // unbind the shader program
            shader.UnBind();
            
        }
        
        
        //Render pyramid 
        {
            // bind the shader program 
            shader2.Bind();
            
            transform2.SetPositions(glm::vec3(8.0f, 5.0f, 10.0f) );
            transform2.GetRotation()->y = counter * 5;
            transform2.GetRotation()->z = counter * 5;
            transform2.SetScale(glm::vec3(3,3,3));
            
            //update shader, including the tranform of our mesh, and the camera view of the mesh
            shader2.Update(transform2, camera, true);
            
            //binding texture at zero unit
            texture2.Bind(0);
            
            pyramidmesh.Draw(false);
            
            //unbind texture
            texture2.UnBind();
            
            // unbind the shader program
            shader2.UnBind();
            
        }
        
        //Render model 
        {
            // bind the shader program 
            shader3.Bind();
            
            transform3.SetPositions(glm::vec3(8.0f, -3.0f, 3.0f) );
            transform3.GetRotation()->y = counter * 25;
            transform3.SetScale(glm::vec3(3,3,3));
            
            //update shader, including the tranform of our mesh, and the camera view of the mesh
            shader3.Update(transform3, camera, true);
            
            //binding texture at zero unit
            texture3.Bind(0);
            
            
            meshModel.Draw(true);
            
            //unbind texture
            texture3.UnBind();
            
            // unbind the shader program
            shader3.UnBind();
            
        }
        
        //Render Sphere 
        {
            // bind the shader program 
            shader4.Bind();
            
            transform4.SetPositions(glm::vec3(-8.0f, 3.0f, 1.0f) );
            transform4.GetRotation()->y = counter * 20;
            
            //update shader, including the tranform of our mesh, and the camera view of the mesh
            shader4.Update(transform4, camera, true);
            
            //binding texture at zero unit
            texture4.Bind(0);
            
            
            sphereMeshWithIndices.Draw(true);
            //sphereMeshWithIndices.Draw(true);
            
            //unbind texture
            texture4.UnBind();
            
            // unbind the shader program
            shader4.UnBind();
            
        }
        
        //Render Torus Knot 
        {
            // bind the shader program 
            shader5.Bind();
            
            transform5.SetPositions(glm::vec3(-2.0f, 3.0f, 1.0f) );
            transform5.GetRotation()->y = counter * 20;
            
            //update shader, including the tranform of our mesh, and the camera view of the mesh
            shader5.Update(transform5, camera, true);
            
            //binding texture at zero unit
            texture5.Bind(0);
            
            torusMeshWithIndices.Draw();
            
            //unbind texture
            texture5.UnBind();
            
            // unbind the shader program
            shader5.UnBind();
            
        }
        
        //Render Torus  
        {
            // bind the shader program 
            shader6.Bind();
            
            transform6.SetPositions(glm::vec3(-6.0f, -4.0f, 1.0f) );
            transform6.GetRotation()->z = counter * 20;
            
            //update shader, including the tranform of our mesh, and the camera view of the mesh
            shader5.Update(transform6, camera, true);
            
            //binding texture at zero unit
            texture6.Bind(0);
            
            torusMesh.Draw();
            
            //unbind texture
            texture6.UnBind();
            
            // unbind the shader program
            shader6.UnBind();
            
        }
        
        counter += 0.001f;

        
        // Update Screen, swap the display buffers (displays what was just drawn)
        glfwSwapBuffers(window);
        
        // Check for any input, or window movement
        glfwPollEvents();
        
        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error: " << error << std::endl;
    }
    
    // close GL context and any other GLFW resources
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}


