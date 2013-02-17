#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>

#include "GLConfigUtil.h"
#include "SimpleScene.h"
#include "math3d.h"

SimpleGLScene::SimpleGLScene (void)
	: World(new Matrix4f(1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f))
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
	delete(World); World = NULL;
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
	render(draw_type);

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

	/* Get Uniforms */
    gWorldLocation = glGetUniformLocation(SHPROG, "gWorld");
    if (gWorldLocation == 0xFFFFFFFF)
		return false;

	return true;
}

/* Vertex array objects */
void SimpleGLScene::create_vao (void)
{
	/************************
	 *            
	 *      v2------------v1
	 *     /|            /|
	 *    / |           / |
	 *   /  |          /  |
	 *  v5------------v6  |
	 *  |   |    |_   |   |
	 *  |   |    /    |   |
	 *  |   v3--------|---v0
	 *  |  /          |  /
	 *  | /           | /
	 *  |/            |/
	 *  v4------------v7
	 *      
	 ************************/
	const float si = 0.5f;
	const Vector3f v[] =
	{
		Vector3f( si, -si, -si),		// v0
		Vector3f( si,  si, -si),		// v1
		Vector3f(-si,  si, -si),		// v2
		Vector3f(-si, -si, -si),		// v3
		Vector3f(-si, -si,  si),		// v4
		Vector3f(-si,  si,  si),		// v5
		Vector3f( si,  si,  si),		// v6
		Vector3f( si, -si,  si),		// v7
	};

    Quad3f Quad[] =
	{
    	Quad3f(v[0], v[1], v[2], v[3]),	// Back
    	Quad3f(v[4], v[5], v[6], v[7]),	// Front
    	Quad3f(v[3], v[2], v[5], v[4]),	// Left
    	Quad3f(v[7], v[6], v[1], v[0]),	// Right
    	Quad3f(v[3], v[4], v[7], v[0]),	// Bottom
    	Quad3f(v[5], v[2], v[1], v[6]),	// Top
	};

	/* Generate ID for VAO and bind it as the active VAO */
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/* Generate a VBO to store our vertex list */
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

/* Draw Scene */
void SimpleGLScene::render (GLenum draw_type)
{

	/* Uniform update */
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, World->m);

	/* Draw as triangles, from 0 to 4 */
	glBindVertexArray(VBO);
	glDrawArrays(draw_type, 0, 48);

	glBindVertexArray(0);
}

/* Releases the context */
void SimpleGLScene::release (void)
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
