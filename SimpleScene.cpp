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

}

/* Draw Scene */
void SimpleGLScene::render (GLenum draw_type)
{

	const float si = 0.5f;
	const int subs = 4;
	GLfloat vertices[12 * subs * subs] = {0};
	for (int j=0; j < subs; ++j)
		for (int i=0; i < subs; ++i)
		{
			// v0+4i
			vertices[j * subs * 12 + i * 12 +  0] = -si + 0.5f * si * i;		// X
			vertices[j * subs * 12 + i * 12 +  1] = -si + 0.5f * si * j;		// Y
			vertices[j * subs * 12 + i * 12 +  2] = -si + 0.0f * si;			// Z
			// v1+4i
			vertices[j * subs * 12 + i * 12 +  3] = -si + 0.5f * si * i;		// X
			vertices[j * subs * 12 + i * 12 +  4] = -si + 0.5f * si * (j + 1);	// Y
			vertices[j * subs * 12 + i * 12 +  5] = -si + 0.0f * si;			// Z
			// v2+4i
			vertices[j * subs * 12 + i * 12 +  6] = -si + 0.5f * si * (i + 1);	// X
			vertices[j * subs * 12 + i * 12 +  7] = -si + 0.5f * si * j;		// Y
			vertices[j * subs * 12 + i * 12 +  8] = -si + 0.0f * si;			// Z
			// v3+4i
			vertices[j * subs * 12 + i * 12 +  9] = -si + 0.5f * si * (i + 1);	// X
			vertices[j * subs * 12 + i * 12 + 10] = -si + 0.5f * si * (j + 1);	// Y
			vertices[j * subs * 12 + i * 12 + 11] = -si + 0.0f * si;			// Z
		}

	GLubyte indices[6 * subs * subs] = { 0 };
	for (int j=0; j < subs; ++j)
		for (int i=0; i < subs; ++i)	
		{
			indices[j * subs * 6 + i * 6 + 0] = j * subs * subs + i * subs + 0;
			indices[j * subs * 6 + i * 6 + 1] = j * subs * subs + i * subs + 1;
			indices[j * subs * 6 + i * 6 + 2] = j * subs * subs + i * subs + 2;

			indices[j * subs * 6 + i * 6 + 3] = j * subs * subs + i * subs + 2;
			indices[j * subs * 6 + i * 6 + 4] = j * subs * subs + i * subs + 3;
			indices[j * subs * 6 + i * 6 + 5] = j * subs * subs + i * subs + 1;
		}


	/* Uniform update */
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, World->m);

	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	// draw first half, range is 6 - 0 + 1 = 7 vertices used
	glDrawRangeElements(GL_TRIANGLES, 0, 4 * subs * subs - 1, 6 * subs * subs, GL_UNSIGNED_BYTE, indices);

	// draw second half, range is 7 - 1 + 1 = 7 vertices used
	//glDrawRangeElements(draw_type, 1, 7, 18, GL_UNSIGNED_BYTE, indices+18);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
}

/* Releases the context */
void SimpleGLScene::release (void)
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
