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

const GLuint nParticles = 16;

class SceneParticleBase : public SceneBase
{
public:
    SceneParticleBase();
    SceneParticleBase(const SceneParticleBase& orig);
    virtual ~SceneParticleBase();
protected:
    virtual bool create_shaders (const char* vsh);
    virtual bool post_shader_compile(void);
    virtual void create_geom (void);
    virtual bool CreateVBO(void);
    virtual bool DestroyVBO(void);
    virtual void create_light(void) { };
    virtual void render (const float);
    
    /* Particle arrays */
    GLuint particleArray[2];
    
    const char* VERTEX_SHADER =
        "#version 400\n"
        "subroutine void RenderPassType();\n"
        "subroutine uniform RenderPassType RenderPass;\n"
        "layout (location = 0) in vec3 VertexPosition;\n"
        "layout (location = 1) in vec3 VertexVelocity;\n"
        "layout (location = 2) in float VertexStartTime;\n"
        "layout (location = 3) in vec3 VertexInitialVelocity;\n"    
    
        // To feedback
        "out vec3 Position;\n"
        "out vec3 Velocity;\n"
        "out float StartTime;\n"
    
        // To fragment shader
        "out float Transp;\n" // Transparency of the particle
        "out vec3 ex_Color;\n"
    
        "uniform float gTime;\n"  // Animation time
        "uniform float H;\n"      // Time between frames
        "vec3 Accel = vec3(0.0,-0.4,0.0);\n"
        "float ParticleLifetime = 3.5;\n"
        "uniform mat4 gWorld;\n"
    
        "subroutine (RenderPassType)\n"
    
        "void update( void ) {\n"
        "  Position = VertexPosition;\n"
        "  Velocity = VertexVelocity;\n"    
        "  StartTime = VertexStartTime;\n"
        "  if (gTime >= StartTime) {\n"
        "    float age = gTime - StartTime;\n"
        "    if (age > ParticleLifetime) {"
        // Particle is past its lifetime, recycle
        "      Position = vec3(0.0);\n"
        "      Velocity = VertexInitialVelocity;\n"
        "      StartTime = gTime;\n"
        "    } else {\n"
        // Particle is Alive, update.
        "      Position += Velocity * H;\n"
        "      Velocity += Accel * H;\n"
        "    }\n"
        "  }\n"
        "}\n"
    
        "subroutine (RenderPassType)\n"
        "void render(void) {\n"
        "  float age = gTime - VertexStartTime;\n"
        "  Transp = 1.0 - age / ParticleLifetime;\n"
        "  gl_Position = gWorld * vec4(VertexPosition, 1.0);\n"
        "  ex_Color = clamp(VertexPosition, 0.0, 1.0);\n"
        "}\n"
    
        "void main( void ) {\n"
        // calls either render() or update()
        "  RenderPass();\n"
        "}\n";

    const char* FRAGMENT_SHADER =
    "#version 400\n"
    "in  vec3 ex_Color;\n"
    "in  float Transp;\n"
    
    "layout (location = 0) out vec4 FragColor;\n"
    "void main(void){\n"
    "  FragColor = vec4(ex_Color, Transp);}";
    
private:
    int drawbuff = 0;
    GLfloat *velocities;
    GLfloat *startTimes;
    
    /* Uniforms */
    GLuint gH;
    
    /* Subroutines */
    GLuint updateSub;
    GLuint renderSub;
    
    GLuint feedback[2];         // Transform feedback objects
    GLuint posBuf[2];           // Position buffers (A and B)
    GLuint velBuf[2];           // Velocity buffers (A and B)
    GLuint startTime[2];        // Start time buffers (A and B)
    GLuint initVel;
    virtual void create_feedback_buffers (void);
    
};

#endif	/* SCENEPARTICLEBASE_HPP */

