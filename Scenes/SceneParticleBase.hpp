/* 
 * File:   SceneParticleBase.hpp
 * Author: sandy
 *
 * Maintains two sets of vertex buffers (A and B) and swap their uses in each
 * frame. Buffer A provides the positions and velocities as input to the vertex
 * shader. The vertex shader updates the positions and velocities using the 
 * Euler method and sends the results to buffer B using transform feedback
 * In a second pass, we render the particles using buffer B.
 * This technique is called "ping-ponging"
 * 
 * A -> Vertex Shader -> B
 * B -> Vertex Shader -> Fragment Shader -> ...
 * 
 * Process:
 * 1. Send the particle positions to the vertex shader for updating and capture
 *    the results using transform feedback. The input to the vertex shader will
 *    come from buffer A, and the output will be stored in buffer B. During this
 *    pass, we enable GL_RASTERIZER_DISCARD so that nothing is actually rendered
 *    to the framebuffer.
 * 2. Render the particles at their updated positions using buffer B as input to
 *    the vertex shader.
 * 3. Swap the purposes of the buffers.
 * 
 * Created on March 9, 2013, 3:28 PM
 */     

#ifndef SCENEPARTICLEBASE_HPP
#define	SCENEPARTICLEBASE_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "SceneBase.hpp"

const GLuint nParticles = 16000;

class Vector2f;

class SceneParticleBase : public SceneBase
{
public:
    SceneParticleBase();
    SceneParticleBase(const SceneParticleBase& orig);
    virtual ~SceneParticleBase();
protected:
    virtual bool create_shaders (const char*, const char*);
    virtual bool post_shader_compile(void);
    virtual void create_geom (void);
    virtual bool CreateVBO(void);
    virtual bool DestroyVBO(void);
    virtual void create_light(void) { };
    virtual void render (const float);
    virtual void on_pointer_moved(Vector2f);
    
    /* Particle arrays */
    GLuint particles;
    
    const char* VERTEX_SHADER =
        "#version 400\n"
        "layout (location = 0) in vec2 VertexInitVel;\n"
        "layout (location = 1) in float StartTime;\n"
        "layout (location = 2) in vec2 MousePos;\n"
    
        // To fragment shader
        "out float Transp;\n" // Transparency of the particle
        "out vec3 ex_Color;\n"
    
        "uniform float gTime;\n"  // Animation time
        "uniform vec2 origin;\n"
        "vec2 Accel = vec2(0.0,-0.4);\n"
        "float ParticleLifetime = 3.5;\n"
        "uniform mat4 gWorld;\n"
    
        "void main( void ) {\n"
        // Assume the initial position is (0,0,0).
        "  vec2 pos = vec2(0.0);\n"
        "  Transp = 0.0;\n"

        "  if( gTime > StartTime ) {\n"
        "    float t = gTime - StartTime;\n"
        "    if( t < ParticleLifetime ) {\n"
        "      pos += VertexInitVel * t + Accel * t * t;\n"
        "      Transp = 1.0 - t / ParticleLifetime;\n"
        "    }\n"
        "  }\n"
        "  gl_Position = gWorld * vec4(pos, 0.0, 1.0);\n"
        "  ex_Color = clamp(vec3(pos, 0.0), 0.0, 1.0);\n"
        "}\n";

    
    const char* FRAGMENT_SHADER =
    "#version 400\n"
    "in  vec3 ex_Color;\n"
    "in  float Transp;\n"
    
    "layout (location = 0) out vec4 FragColor;\n"
    "void main(void){\n"
    "  FragColor = vec4(ex_Color, Transp);}";
    
    virtual float get_spread(void) = 0;
    
private:
    GLuint startTime;
    GLuint startPos;
    GLuint initVel;
};

#endif	/* SCENEPARTICLEBASE_HPP */

