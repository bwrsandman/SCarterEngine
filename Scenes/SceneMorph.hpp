/* 
 * File:   SceneMorph.hpp
 * Author: sandy
 *
 * Created on February 17, 2013, 11:46 PM
 */

#ifndef SCENEMORPH_HPP
#define	SCENEMORPH_HPP

#include "SceneBase.hpp"

class Matrix4f;

class SceneMorph : public SceneBase 
{
public:
    SceneMorph();
    SceneMorph(const SceneMorph& orig);
    virtual ~SceneMorph();
    inline void set_alpha(float a) { alpha = a; }
    
protected:
    virtual void create_geom (void);
    virtual bool create_shaders (const char*);
    virtual void render (void);

private:
    float alpha = 0.0f;
    
    /* Uniforms */
    GLuint gAlpha;
    
    const char* VERTEX_SHADER =
        /* morph.vert - interpolates between the model and the unit sphere */
        "#version 330\n"
        "layout (location = 0) in vec3 gl_Vertex;\n"
        "uniform mat4 gWorld;\n"
        "out vec3 ex_Color;\n"
        "uniform float alpha;\n"  /* 0 - 1 float that transitions the morph */
        "const float r     = 0.75;\n"

        "vec3 norm( vec3 p) {\n"
        "   float d = sqrt( gl_Vertex.x * gl_Vertex.x + \n"
        "                   gl_Vertex.y * gl_Vertex.y + \n"
        "                   gl_Vertex.z * gl_Vertex.z);\n"
        "   float theta = acos(gl_Vertex.z/d);\n"
        "   float fi    = atan(gl_Vertex.y,gl_Vertex.x);\n"
        "   return        r * vec3( sin(theta) * cos(fi),\n" 
        "                           sin(theta) * sin(fi), \n"
        "                           cos(theta));\n"
        "}\n\n"

        "void main( void ) {\n"
            /* original position */
        "   vec3 p = gl_Vertex.xyz;\n"
        "   vec3 v;\n"
            /* in case normalize fails... */
        "   if( p == vec3( 0.0, 0.0, 0.0 ) )\n"
        "       v = vec3( 0.0, 0.0, r );\n"
            /* do linear interpolation */
        "   else {\n"
        "       v = r * normalize(gl_Vertex.xyz) * alpha + p * ( 1.0 - alpha );}\n"
            /* continue the transformation. */
        "   gl_Position = gWorld * vec4(v, 1.0);\n"
        "   ex_Color = clamp(gl_Vertex, 0.0, 1.0);\n"
        "}";
};

#endif	/* SCENEMORPH_HPP */

