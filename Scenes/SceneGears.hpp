/* 
 * File:   SceneGears.hpp
 * Author: sandy
 *
 * Created on February 19, 2013, 1:28 AM
 * 
 * Based on gtkglextmm example gears by Brian Paul from the public domain
 * 
 */

#ifndef SCENEGEARS_HPP
#define	SCENEGEARS_HPP

class SceneGears : public SceneBase
{
public:
    explicit SceneGears(bool is_sync = true);
    virtual ~SceneGears();

    // get & set view rotation values.
    void get_view_rot(GLfloat& x, GLfloat& y, GLfloat& z)
    { x = m_ViewRotX; y = m_ViewRotY; z = m_ViewRotZ; }

    void set_view_rot(GLfloat x, GLfloat y, GLfloat z)
    { m_ViewRotX = x; m_ViewRotY = y; m_ViewRotZ = z; }
    
protected:
    void gear(GLfloat inner_radius,
              GLfloat outer_radius,
              GLfloat width,
              GLint   teeth,
              GLfloat tooth_depth);

    virtual void create_geom();
    virtual void create_light();
    virtual void set_perspective();
    virtual void render(const float);
    virtual void update(const float);
    virtual void print_framerate(float);

    // OpenGL scene related variables:
    GLint m_Gear1;
    GLint m_Gear2;
    GLint m_Gear3;

    GLfloat m_ViewRotX;
    GLfloat m_ViewRotY;
    GLfloat m_ViewRotZ;

    GLfloat m_Angle;

    bool m_IsSync;

    // frame rate evaluation stuff:
    int m_Frames;
};

#endif	/* SCENEGEARS_HPP */
