#ifndef SIMPLESCENE_H
#define SIMPLESCENE_H

#include <gtkmm.h>
#include <gtkglmm.h>

class Matrix4f;

class SimpleGLScene : public Gtk::DrawingArea
                    , public Gtk::GL::Widget<SimpleGLScene>
{
public:
	SimpleGLScene (void);
	virtual ~SimpleGLScene (void);

protected:
	virtual void on_realize (void);
	virtual bool on_configure_event (GdkEventConfigure* event);
	virtual bool on_expose_event (GdkEventExpose* event);
	virtual bool init_opengl (void);
	virtual bool create_shaders (void);
	virtual void create_vao (void);
	virtual void render (GLenum);
	virtual void release (void);

private:
    Matrix4f *World = NULL;

	const GLenum draw_type = GL_QUADS;
	/* Colors */
	const float         CLEAR_R     = 1.0f;
	const float         CLEAR_G     = 1.0f;
	const float         CLEAR_B     = 1.0f;
	const float         CLEAR_A     = 1.0f;
	/* Vertex arrays */
	uint              	VAO;
	uint              	VBO;
	/* Shaders */
	uint				SHVERT;
	uint				SHFRAG;
	uint				SHPROG;
	/* Uniforms */
	GLuint gWorldLocation;

	const char* VERTEX_SHADER =
		"#version 330\n"
		"layout (location = 0) in vec3 Position;\n"

		"uniform mat4 gWorld;\n"

		"out vec3 ex_Color;\n"

		"void main()\n"
		"{\n"
		"	gl_Position = gWorld * vec4(Position, 1.0);\n"
		"	ex_Color = clamp(Position, 0.0, 1.0);\n"
		"}";

	const char* FRAGMENT_SHADER =
		/* Fragment Shader – file "minimal.frag" */
		"#version 330\n"
		"in  vec3 ex_Color;\n"
		"out vec4 out_Color;\n"
		"void main(void){\n"
		"		out_Color = vec4(ex_Color, 1.0);}";
};

#endif
