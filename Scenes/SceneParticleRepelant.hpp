/* 
 * File:   SceneParticleGravity.hpp
 * Author: sandy
 *
 * Created on March 9, 2013, 3:29 PM
 */

#ifndef SCENEPARTICLEGRAVITY_HPP
#define	SCENEPARTICLEGRAVITY_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "../math3d.hpp"
#include "SceneParticleBase.hpp"

class SceneParticleGravity : public SceneParticleBase
{
public:
    SceneParticleGravity();
    SceneParticleGravity(const SceneParticleGravity& orig);
    virtual ~SceneParticleGravity();
    
protected:
    virtual bool create_shaders (const char*, const char*);
    
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
        "float ParticleLifetime = 3.5;\n"
        "uniform mat4 gWorld;\n"
    
        "void main( void ) {\n"
        // Assume the initial position is (0,0,0).
        "  vec2 pos = vec2(0.0);\n"
        "  Transp = 0.0;\n"
        "  float dist = length(pos - MousePos);\n"
        "  vec2 Accel = pos - MousePos;\n"
        "  normalize(Accel);\n"
        "  Accel *= 1/(dist * dist);\n"
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

        virtual float get_spread(void) { return TWOPI; };
private:

};

#endif	/* SCENEPARTICLEGRAVITY_HPP */

