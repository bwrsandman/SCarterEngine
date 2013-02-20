/* 
 * File:   SceneGears.hpp
 * Author: sandy
 *
 * Created on February 19, 2013, 1:28 AM
 */

#ifndef SCENEGEARS_HPP
#define	SCENEGEARS_HPP

class SceneGears : public Gtk::GL::DrawingArea
{
public:
    explicit SceneGears(bool is_sync = true);
    virtual ~SceneGears();

protected:
    void gear(GLfloat inner_radius,
              GLfloat outer_radius,
              GLfloat width,
              GLint   teeth,
              GLfloat tooth_depth);

protected:
    // signal handlers:
    virtual void on_realize();
    virtual bool on_configure_event(GdkEventConfigure* event);
    virtual bool on_expose_event(GdkEventExpose* event);
    virtual bool on_map_event(GdkEventAny* event);
    virtual bool on_unmap_event(GdkEventAny* event);
    virtual bool on_visibility_notify_event(GdkEventVisibility* event);
    virtual bool on_idle();
    
    // Added
    virtual bool init_opengl();
    virtual void create_geom();
    virtual void create_light();
    virtual void set_perspective();
    virtual bool create_shaders (const char*) { return true; } //TODO remove
    char* VERTEX_SHADER = NULL; //TODO remove

public:
    // Invalidate whole window.
    void invalidate() {
      get_window()->invalidate_rect(get_allocation(), false);
    }

    // Update window synchronously (fast).
    void update()
    { get_window()->process_updates(false); }

protected:
    // idle signal connection:
    sigc::connection m_ConnectionIdle;

public:
    // get & set view rotation values.
    void get_view_rot(GLfloat& x, GLfloat& y, GLfloat& z)
    { x = m_ViewRotX; y = m_ViewRotY; z = m_ViewRotZ; }

    void set_view_rot(GLfloat x, GLfloat y, GLfloat z)
    { m_ViewRotX = x; m_ViewRotY = y; m_ViewRotZ = z; }

protected:
    // OpenGL scene related variables:
    GLint m_Gear1;
    GLint m_Gear2;
    GLint m_Gear3;

    GLfloat m_ViewRotX;
    GLfloat m_ViewRotY;
    GLfloat m_ViewRotZ;

    GLfloat m_Angle;

    bool m_IsSync;

protected:
    // frame rate evaluation stuff:
    Glib::Timer m_Timer;
    int m_Frames;
};

#endif	/* SCENEGEARS_HPP */

