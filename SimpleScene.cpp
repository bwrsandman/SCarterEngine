#include <iostream>
#include <GL/glew.h>
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
{ 
	release();
}

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
	render();

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
	std::cout << "OpenGL version - " 
              << glGetString(GL_VERSION) << std::endl;

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
	if(!create_shaders())
		return false;
	create_vao();

	/* Clear Frame Buffers */
	glClearColor(CLEAR_R, CLEAR_G, CLEAR_B, CLEAR_A);
	glClearDepth(1.0f);

	/* Configure Viewport to take up the whole window */
	glViewport(0, 0, get_width(), get_height());

	return true;
}

/* Shaders */
bool SimpleGLScene::create_shaders (void)
{
	char shader_error[1024];
	int error_length = 0;
	GLint res;

	/* Generate some IDs for our shader programs */
	SHVERT = glCreateShader(GL_VERTEX_SHADER);
	SHFRAG = glCreateShader(GL_FRAGMENT_SHADER);
	SHPROG = glCreateProgram();

	/* Assign shader source code to these IDs */
	glShaderSource(SHVERT, 1, &VERTEX_SHADER, NULL);
	glShaderSource(SHFRAG, 1, &FRAGMENT_SHADER, NULL);

	/* Compile the code */
	glCompileShader(SHVERT);
	glCompileShader(SHFRAG);

	/* Check if compilation was successful */
	glGetShaderiv(SHVERT, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		std::cerr << "Compilation of vertex shader failed" << std::endl;
		glGetShaderInfoLog(SHVERT, 1024, &error_length, shader_error);
		std::cerr << shader_error << std::endl;
		return false;
	}
	glGetShaderiv(SHFRAG, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		std::cerr << "Compilation of fragment shader failed" << std::endl;
		glGetShaderInfoLog(SHFRAG, 1024, &error_length, shader_error);
		std::cerr << shader_error << std::endl;
		return false;
	}

	/* Attach these shaders to the shader program */
	glAttachShader(SHPROG, SHVERT);
	glAttachShader(SHPROG, SHFRAG);

	/* Flag the shaders to be deleted when the shader program is deleted */
	glDeleteShader(SHVERT);
	glDeleteShader(SHFRAG);

	/* Link the shaders */
	glLinkProgram(SHPROG);
	glGetProgramiv(SHPROG, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
		std::cerr << "Failed to link shader program" << std::endl;

	glUseProgram(SHPROG);

	return true;
}

/* Vertex array objects */
void SimpleGLScene::create_vao (void)
{
	const float quad[8] =
	{
		-0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f,  0.5f,
		 0.5f, -0.5f,
	};

	/* Generate ID for VAO and bind it as the active VAO */
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/* Generate a VBO to store our vertex list */
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

/* Draw Scene */
void SimpleGLScene::render (void)
{
	/* Draw as triangles, from 0 to 4 */
	glBindVertexArray(VBO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}

/* Releases the context */
void SimpleGLScene::release (void)
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
