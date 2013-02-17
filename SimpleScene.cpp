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

/* Signal to take any necessary actions when the widget is instantiated on a
 * particular display. */
void SimpleGLScene::on_realize (void)
{
	/* We need to call the base on_realize() */
	Gtk::DrawingArea::on_realize();

	/* Get GL::Window. */
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();

	/*** OpenGL BEGIN ***/
	if (!glwindow->gl_begin(get_gl_context()))
		return;

	// TODO: use the bool from this function
	init_opengl();

	glwindow->gl_end();
	/*** OpenGL END ***/
}

/* Signal to take any necessary actions when the widget changes size. */
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

/* Signal to handle redrawing the contents of the widget. */
bool SimpleGLScene::on_expose_event (GdkEventExpose* event)
{
	/* Get GL::Window. */
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();

	/*** OpenGL BEGIN ***/
	if (!glwindow->gl_begin(get_gl_context()))
		return false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Draw here */

	/* Swap buffers. */
	if (glwindow->is_double_buffered())
		glwindow->swap_buffers();
	else
		glFlush();

	glwindow->gl_end();
	/*** OpenGL END ***/

	return true;
}


/* OpenGL specific functions */

/* Initialize OpenGL */
bool SimpleGLScene::init_opengl (void)
{
	// TODO
	/*if(!create_shaders())
		return false;
	create_vao();*/

	/* Clear Frame Buffers */
	glClearColor(CLEAR_R, CLEAR_G, CLEAR_B, CLEAR_A);
	glClearDepth(1.0f);

	/* Configure Viewport to take up the whole window */
	glViewport(0, 0, get_width(), get_height());

	return true;
}
