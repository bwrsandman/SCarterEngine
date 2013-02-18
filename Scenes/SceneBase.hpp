/* 
 * File:   SceneBase.hpp
 * Author: sandy
 *
 * Created on February 17, 2013, 11:48 PM
 */

#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <gtkmm.h>
#include <gtkglmm.h>

class Matrix4f;
const float si = 0.75f;
const int subs = 4;

class SceneBase : public Gtk::DrawingArea
                , public Gtk::GL::Widget<SceneBase>
{
public:
    SceneBase();
    SceneBase(const SceneBase& orig);
    virtual ~SceneBase();
    inline void set_alpha(float a) { alpha = a; }

protected:
    Matrix4f *World = NULL;
    float alpha = 0.0f;

    /* Uniforms */
    GLuint gWorldLocation;
    GLuint gAlpha;

    /* Vertex arrays */
    GLfloat *vertices = NULL;
    GLubyte *indices = NULL;

    virtual void on_realize (void);
    virtual bool on_configure_event (GdkEventConfigure* event);
    virtual bool on_expose_event (GdkEventExpose* event);
    virtual bool on_timeout();
    virtual bool init_opengl (void);
    virtual bool create_shaders (void);
    virtual void create_vao (void);
    virtual void render (GLenum);
    virtual void release (void);

private:

    const GLenum draw_type = GL_LINES;// GL_POINTS;//GL_QUADS;
    /* Colors */
    const float         CLEAR_R     = 1.0f;
    const float         CLEAR_G     = 1.0f;
    const float         CLEAR_B     = 1.0f;
    const float         CLEAR_A     = 1.0f;

    /* Shaders */
    uint                SHVERT;
    uint                SHFRAG;
    uint                SHPROG;

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

    const char* FRAGMENT_SHADER =
        /* Fragment Shader – file "minimal.frag" */
        "#version 330\n"
        "in  vec3 ex_Color;\n"
        "out vec4 out_Color;\n"
        "void main(void){\n"
        "       out_Color = vec4(ex_Color, 1.0);}";
};

#endif  /* SCENEBASE_HPP */
