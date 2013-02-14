#include <iostream>
#include <GL/glu.h>

#include "GLConfigUtil.h"
#include "SimpleScene.h"

SimpleGLScene::SimpleGLScene (void)
{
	/* Configure OpenGL-capable visual. */
	Glib::RefPtr<Gdk::GL::Config> glconfig;

	/* Try double-buffered visual */
	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB    |
		                             Gdk::GL::MODE_DEPTH  |
		                             Gdk::GL::MODE_DOUBLE);
	if (!glconfig)
	{
	  std::cerr << "*** Cannot find the double-buffered visual.\n"
		        << "*** Trying single-buffered visual.\n";

	  /* Try single-buffered visual */
	  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB   |
		                                 Gdk::GL::MODE_DEPTH);
	  if (!glconfig)
		{
		  std::cerr << "*** Cannot find any OpenGL-capable visual.\n";
		  std::exit(1);
		}
	}

	/* print frame buffer attributes. */
	GLConfigUtil::examine_gl_attrib(glconfig);

	/* Set OpenGL-capability to the widget. */
	set_gl_capability(glconfig);
}

SimpleGLScene::~SimpleGLScene (void)
{ }

void SimpleGLScene::on_realize (void)
{
	/* We need to call the base on_realize() */
	Gtk::DrawingArea::on_realize();

	/* Get GL::Window. */
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();

	/*** OpenGL BEGIN ***/
	if (!glwindow->gl_begin(get_gl_context()))
	return;

	GLUquadricObj* qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glNewList(1, GL_COMPILE);
	gluSphere(qobj, 1.0, 20, 20);
	glEndList();

	static GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};
	static GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);

	glViewport(0, 0, get_width(), get_height());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 1.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 3.0,
		    0.0, 0.0, 0.0,
		    0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -3.0);

	glwindow->gl_end();
	/*** OpenGL END ***/
}

bool SimpleGLScene::on_configure_event (GdkEventConfigure* event)
{
	/* Get GL::Window. */
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();

	/*** OpenGL BEGIN ***/
	if (!glwindow->gl_begin(get_gl_context()))
	return false;

	glViewport(0, 0, get_width(), get_height());

	glwindow->gl_end();
	/*** OpenGL END ***/

	return true;
}

bool SimpleGLScene::on_expose_event (GdkEventExpose* event)
{
	/* Get GL::Window. */
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();

	/*** OpenGL BEGIN ***/
	if (!glwindow->gl_begin(get_gl_context()))
	return false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCallList(1);

	/* Swap buffers. */
	if (glwindow->is_double_buffered())
	glwindow->swap_buffers();
	else
	glFlush();

	glwindow->gl_end();
	/*** OpenGL END ***/

	return true;
}

