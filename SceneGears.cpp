// -*- C++ -*-
/*
 * 3-D gear wheels.  This program is in the public domain.
 *
 * Brian Paul
 */

/* Conversion to gtkglextmm by Naofumi Yasufuku */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <GL/glew.h>
#include <gtkmm.h>
#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
//#include "GLConfigUtil.hpp"
#include "SceneGears.hpp"

///////////////////////////////////////////////////////////////////////////////
//
// Gears scene.
//
///////////////////////////////////////////////////////////////////////////////

SceneGears::SceneGears(bool is_sync)
  : m_Gear1(0), m_Gear2(0), m_Gear3(0),
    m_ViewRotX(20.0), m_ViewRotY(30.0), m_ViewRotZ(0.0),
    m_Angle(0.0), m_IsSync(is_sync),
    m_Frames(0)
{
  //
  // Configure OpenGL-capable visual.
  //

  Glib::RefPtr<Gdk::GL::Config> glconfig;

  // Try double-buffered visual
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB    |
                                     Gdk::GL::MODE_DEPTH  |
                                     Gdk::GL::MODE_DOUBLE);
  if (!glconfig)
    {
      std::cerr << "*** Cannot find the double-buffered visual.\n"
                << "*** Trying single-buffered visual.\n";

      // Try single-buffered visual
      glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB   |
                                         Gdk::GL::MODE_DEPTH);
      if (!glconfig)
        {
          std::cerr << "*** Cannot find any OpenGL-capable visual.\n";
          std::exit(1);
        }
    }

  // print frame buffer attributes.
  //GLConfigUtil::examine_gl_attrib(glconfig);

  //
  // Set OpenGL-capability to the widget.
  //

  set_gl_capability(glconfig);

  // Add events.
  add_events(Gdk::VISIBILITY_NOTIFY_MASK);
}

SceneGears::~SceneGears()
{
}

/*
 * Draw a gear wheel.  You'll probably want to call this function when
 * building a display list since we do a lot of trig here.
 *
 * Input:  inner_radius - radius of hole at center
 * outer_radius - radius at center of teeth
 * width - width of gear
 * teeth - number of teeth
 * tooth_depth - depth of tooth
 */

void SceneGears::gear(GLfloat inner_radius,
                      GLfloat outer_radius,
                      GLfloat width,
                      GLint   teeth,
                      GLfloat tooth_depth)
{
  GLint i;
  GLfloat r0, r1, r2;
  GLfloat angle, da;
  GLfloat u, v, len;

  r0 = inner_radius;
  r1 = outer_radius - tooth_depth / 2.0;
  r2 = outer_radius + tooth_depth / 2.0;

  da = 2.0 * G_PI / teeth / 4.0;

  glShadeModel(GL_FLAT);

  glNormal3f(0.0, 0.0, 1.0);

  /* draw front face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    if (i < teeth) {
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    }
  }
  glEnd();

  /* draw front sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * G_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
  }
  glEnd();

  glNormal3f(0.0, 0.0, -1.0);

  /* draw back face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    if (i < teeth) {
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    }
  }
  glEnd();

  /* draw back sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * G_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;

    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
  }
  glEnd();

  /* draw outward faces of teeth */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;

    glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
    glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    u = r2 * cos(angle + da) - r1 * cos(angle);
    v = r2 * sin(angle + da) - r1 * sin(angle);
    len = sqrt(u * u + v * v);
    u /= len;
    v /= len;
    glNormal3f(v, -u, 0.0);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
    glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
    u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
    v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
    glNormal3f(v, -u, 0.0);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
    glNormal3f(cos(angle), sin(angle), 0.0);
  }

  glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
  glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

  glEnd();

  glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * G_PI / teeth;
    glNormal3f(-cos(angle), -sin(angle), 0.0);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
  }
  glEnd();

}

// TODO: Function done in base
void SceneGears::on_realize()
{
  // We need to call the base on_realize()
  Gtk::DrawingArea::on_realize();

  //
  // Get GL::Drawable.
  //

  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  //
  // GL calls.
  //

  // *** OpenGL BEGIN ***
  if (!gldrawable->gl_begin(get_gl_context()))
    return;

    if (!init_opengl())
       return;

  gldrawable->gl_end();
  // *** OpenGL END ***

  // Start timer.
  m_Timer.start();
}

// TODO: Function done in base
bool SceneGears::init_opengl()
{
    std::cout << "GL_RENDERER   = " << glGetString(GL_RENDERER)   << std::endl;
    std::cout << "GL_VERSION    = " << glGetString(GL_VERSION)    << std::endl;
    std::cout << "GL_VENDOR     = " << glGetString(GL_VENDOR)     << std::endl;
    std::cout << "GL_EXTENSIONS = " << glGetString(GL_EXTENSIONS) << std::endl;
    std::cout << std::endl;

    /* Initialize GLEW */
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
      std::cerr << "*** Error Initializing GLEW: "
                << glewGetErrorString(err) << std::endl;
      std::exit(1);
    }

    /* Check that the machine supports the 2.1 API. */
    if (!GLEW_VERSION_3_0)
    {
      std::cerr << "*** Machine does not support GLEW 3.0 API.\n";
      std::exit(1);
    }

    /* Check that the machine supports the 2.1 API. */
    if (!GLEW_VERSION_4_3)
      std::cout << "*** WARN: Machine does not support GLEW 4.3 API.\n";

    /* Create Shader and vertex buffer */
    if(!create_shaders(VERTEX_SHADER))
    {
        std::cerr << "*** Error Creating shaders.\n";
        return false;
    }
    
    create_light();
    create_geom();

    glEnable(GL_NORMALIZE);
  
  return true;
}

void SceneGears::create_light()
{
    /* Create lighting */
    static GLfloat pos[4] = {5.0, 5.0, 10.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

void SceneGears::create_geom()
{
    static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0};
    static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0};
    static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0};
    
    // Make the gears.
    m_Gear1 = glGenLists(1);
    glNewList(m_Gear1, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    gear(1.0, 4.0, 1.0, 20, 0.7);
    glEndList();

    m_Gear2 = glGenLists(1);
    glNewList(m_Gear2, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    gear(0.5, 2.0, 2.0, 10, 0.7);
    glEndList();

    m_Gear3 = glGenLists(1);
    glNewList(m_Gear3, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    gear(1.3, 2.0, 0.5, 10, 0.7);
    glEndList();
}

// TODO: Function done in base
bool SceneGears::on_configure_event(GdkEventConfigure* event)
{
  //
  // Get GL::Drawable.
  //

  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  //
  // GL calls.
  //

  // *** OpenGL BEGIN ***
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  glViewport(0, 0, get_width(), get_height());

  set_perspective();
  
  gldrawable->gl_end();
  // *** OpenGL END ***

  return true;
}

void SceneGears::set_perspective()
{
    GLfloat h = (GLfloat)(get_height()) / (GLfloat)(get_width());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -40.0);
}
// TODO: Function done in base
bool SceneGears::on_expose_event2(GdkEventExpose* event)
{
    //
    // Get GL::Drawable.
    //

    Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

    //
    // GL calls.
    //

    // *** OpenGL BEGIN ***
    if (!gldrawable->gl_begin(get_gl_context()))
      return false;

    render();

    // Swap buffers.
    if (gldrawable->is_double_buffered())
      gldrawable->swap_buffers();
    else
      glFlush();

    gldrawable->gl_end();
    // *** OpenGL END ***
    
    return true;
}

bool SceneGears::on_expose_event(GdkEventExpose* event)
{
    bool ret = on_expose_event2(event); // TODO change to base
    print_framerate();                  // TODO put somewhere better than stdout
    return ret;
}

void SceneGears::print_framerate()
{
    //
    // Print frame rate.
    //

    ++m_Frames;

    double seconds = m_Timer.elapsed();
    if (seconds >= 5.0)
      {
        // std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout.precision(3);
        std::cout << m_Frames << " frames in "
                  << seconds << " seconds = "
                  << (m_Frames / seconds) << " FPS\n";
        m_Timer.reset();
        m_Frames = 0;
      }
}

void SceneGears::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glRotatef(m_ViewRotX, 1.0, 0.0, 0.0);
        glRotatef(m_ViewRotY, 0.0, 1.0, 0.0);
        glRotatef(m_ViewRotZ, 0.0, 0.0, 1.0);

        glPushMatrix();
            glTranslatef(-3.0, -2.0, 0.0);
            glRotatef(m_Angle, 0.0, 0.0, 1.0);
            glCallList(m_Gear1);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(3.1, -2.0, 0.0);
            glRotatef(-2.0 * m_Angle - 9.0, 0.0, 0.0, 1.0);
            glCallList(m_Gear2);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-3.1, 4.2, 0.0);
            glRotatef(-2.0 * m_Angle - 25.0, 0.0, 0.0, 1.0);
            glCallList(m_Gear3);
        glPopMatrix();

    glPopMatrix();
}

bool SceneGears::on_map_event(GdkEventAny* event)
{
  if (!m_ConnectionIdle.connected())
    m_ConnectionIdle = Glib::signal_idle().connect(
      sigc::mem_fun(*this, &SceneGears::on_idle), GDK_PRIORITY_REDRAW);

  return true;
}

bool SceneGears::on_unmap_event(GdkEventAny* event)
{
  if (m_ConnectionIdle.connected())
    m_ConnectionIdle.disconnect();

  return true;
}

bool SceneGears::on_visibility_notify_event(GdkEventVisibility* event)
{
  if (event->state == GDK_VISIBILITY_FULLY_OBSCURED)
    {
      if (m_ConnectionIdle.connected())
        m_ConnectionIdle.disconnect();
    }
  else
    {
      if (!m_ConnectionIdle.connected())
        m_ConnectionIdle = Glib::signal_idle().connect(
          sigc::mem_fun(*this, &SceneGears::on_idle), GDK_PRIORITY_REDRAW);
    }

  return true;
}

bool SceneGears::on_idle()
{
  m_Angle += 2.0;

  // Invalidate the whole window.
  invalidate();

  // Update window synchronously (fast).
  if (m_IsSync)
    update();

  return true;
}