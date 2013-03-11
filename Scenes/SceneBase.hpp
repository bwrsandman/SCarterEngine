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
class Vector2f;
const float si = 0.75f;
const int subs = 4;

class SceneBase : public Gtk::GL::DrawingArea
{
public:
    SceneBase();
    SceneBase(const SceneBase& orig);
    virtual ~SceneBase();

protected:
    Matrix4f *World = NULL;
    GLfloat total_time = 0.0f;
    Glib::Timer update_timer;
    Glib::Timer draw_timer;
   
    /* Uniforms */
    GLuint gWorldLocation;
    GLuint gTimeLocation;
    

    /* Vertex arrays */
    GLfloat *vertices = NULL;
    GLubyte *indices = NULL;

    /* Idle signal connection */
    sigc::connection m_ConnectionIdle;
    
    /* Signal handlers */
    virtual bool on_configure_event (GdkEventConfigure* event);
    virtual bool on_expose_event (GdkEventExpose* event);
    virtual bool on_map_event(GdkEventAny* event);
    virtual bool on_unmap_event(GdkEventAny* event);
    virtual bool on_visibility_notify_event(GdkEventVisibility* event);
    virtual bool on_button_press_event(GdkEventButton * event);
    virtual bool on_motion_notify_event (GdkEventMotion* event);
    virtual void on_realize (void);
    virtual bool on_idle (void);
    virtual void on_button_pressed(Vector2f);
    virtual void on_pointer_moved(Vector2f);

    virtual bool init_opengl (void);
    virtual bool create_shaders (const char*, const char*);
    virtual bool post_shader_compile(void);
    virtual void create_geom (void) = 0;
    virtual void create_light(void) = 0;
    virtual void set_perspective(void);
    virtual void render (const float);
    virtual void update (const float);
    virtual void release (void);

    Vector2f px_to_view(int x, int y);

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
    /* Invalidate whole window. */
    void _invalidate() { get_window()->invalidate_rect(get_allocation(), false); }

    /* Update window synchronously (fast). */
    void _update_sync()    { get_window()->process_updates(false); }
    
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
        "uniform float gTime;\n"
        "out vec3 ex_Color;\n"
    
        "void main( void ) {\n"
        "   gl_Position = gWorld * vec4(gl_Vertex, 1.0);\n"
        "   ex_Color = clamp(gl_Vertex, 0.0, 1.0);\n"
        "}";
};

#endif  /* SCENEBASE_HPP */
