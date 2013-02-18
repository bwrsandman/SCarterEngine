/* 
 * File:   SceneMorph.hpp
 * Author: sandy
 *
 * Created on February 17, 2013, 11:46 PM
 */

#ifndef SCENEMORPH_HPP
#define	SCENEMORPH_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "SceneBase.hpp"

class Matrix4f;

class SceneMorph : public SceneBase 
                 , public Gtk::GL::Widget<SceneMorph>
{
public:
    SceneMorph();
    SceneMorph(const SceneMorph& orig);
    virtual ~SceneMorph();
    inline void set_alpha(float a) { alpha = a; }
    
protected:
	virtual void create_vao (void);
        virtual bool create_shaders (void);
	virtual void render (GLenum);

private:
    float alpha = 0.0f;
    
    /* Uniforms */
    GLuint gAlpha;
    
    const char* VERTEX_SHADER =
        /* morph.vert - interpolates between the model and the unit sphere */
        "#version 330\n"
        "layout (location = 0) in vec3 gl_Vertex;"
        "uniform mat4 gWorld;"
        "out vec3 ex_Color;"
        "uniform float alpha;"  /* 0 - 1 float that transitions the morph */
        "const float r     = 0.75;"

        "vec3 norm( vec3 p) {"
        "   float d = sqrt( gl_Vertex.x * gl_Vertex.x + "
        "                   gl_Vertex.y * gl_Vertex.y + "
        "                   gl_Vertex.z * gl_Vertex.z);"
        "   float theta = acos(gl_Vertex.z/d);"
        "   float fi    = atan(gl_Vertex.y,gl_Vertex.x);"
        "   return        r * vec3( sin(theta) * cos(fi)," 
        "                           sin(theta) * sin(fi), "
        "                           cos(theta));"
        "}"

        "void main( void ) {"
            /* original position */
        "   vec3 p = gl_Vertex.xyz;"
        "   vec3 v;"
            /* in case normalize fails... */
        "   if( p == vec3( 0.0, 0.0, 0.0 ) )"
        "       v = vec3( 0.0, 0.0, r );"
            /* do linear interpolation */
        "   else {"
        "       v = r * normalize(gl_Vertex.xyz) * alpha + p * ( 1.0 - alpha );}"
            /* continue the transformation. */
        "   gl_Position = gWorld * vec4(v, 1.0);"
        "   ex_Color = clamp(gl_Vertex, 0.0, 1.0);"
        "}";
};

#endif	/* SCENEMORPH_HPP */

