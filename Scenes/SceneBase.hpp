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

protected:
    Matrix4f *World = NULL;

    /* Uniforms */
    GLuint gWorldLocation;

    /* Vertex arrays */
    GLfloat *vertices = NULL;
    GLubyte *indices = NULL;

    virtual void on_realize (void);
    virtual bool on_configure_event (GdkEventConfigure* event);
    virtual bool on_expose_event (GdkEventExpose* event);
    virtual bool on_timeout();
    virtual bool init_opengl (void);
    virtual bool create_shaders (const char*);
    virtual void create_vao (void);
    virtual void render (GLenum);
    virtual void release (void);
    
    /* Shaders */
    uint                SHVERT;
    uint                SHFRAG;
    uint                SHPROG;
    
    const char* FRAGMENT_SHADER =
    /* Fragment Shader – file "minimal.frag" */
    "#version 330\n"
    "in  vec3 ex_Color;\n"
    "out vec4 out_Color;\n"
    "void main(void){\n"
    "       out_Color = vec4(ex_Color, 1.0);}";

private:

    const GLenum draw_type = GL_LINES;// GL_POINTS;//GL_QUADS;
    /* Colors */
    const float         CLEAR_R     = 1.0f;
    const float         CLEAR_G     = 1.0f;
    const float         CLEAR_B     = 1.0f;
    const float         CLEAR_A     = 1.0f;
    
    const char* VERTEX_SHADER =
        /* morph.vert - interpolates between the model and the unit sphere */
        "#version 330\n"
        "layout (location = 0) in vec3 gl_Vertex;\n"
        "uniform mat4 gWorld;\n"
        "out vec3 ex_Color;\n"
    
        "void main( void ) {\n"
        "   gl_Position = gWorld * vec4(gl_Vertex, 1.0);\n"
        "   ex_Color = clamp(gl_Vertex, 0.0, 1.0);\n"
        "}";
};

#endif  /* SCENEBASE_HPP */
