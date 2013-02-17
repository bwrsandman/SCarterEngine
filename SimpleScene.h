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

	const GLenum draw_type = GL_LINES;// GL_POINTS;//GL_QUADS;
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
/* morph.vert - interpolates between the model and the unit sphere */
		"#version 330\n"
		"layout (location = 0) in vec3 gl_Vertex;"
		"uniform mat4 gWorld;"
		"out vec3 ex_Color;"
		"uniform float alpha;"	// 0 - 1 float that transitions the morph
		"void main( void ) {"
		"	vec3 p = gl_Vertex.xyz;" 			 // original position
		"	float d = sqrt( gl_Vertex.x * gl_Vertex.x + "
		"					gl_Vertex.y * gl_Vertex.y + "
		"					gl_Vertex.z * gl_Vertex.z);"
		"	float theta = acos(gl_Vertex.z/d);"
		"	float fi    = atan(gl_Vertex.y,gl_Vertex.x);"
		"	float r     = 1.0;"
		"	vec3 n      = vec3( r * sin(theta) * cos(fi), "
		"						r * sin(theta) * sin(fi), "
		"						r * cos(theta));"
			// do linear interpolation
		"	vec3 v = n * alpha + p * ( 1.0 - alpha );"
			// in case normalize fails...
		"	if( p == vec3( 0.0, 0.0, 0.0 ) ) {"
		"		v = vec3( 0.0, 1.0, 0.0 );"
		"	}"
			// continue the transformation.
		"	gl_Position = gWorld * vec4(v, 1.0);"
		"	ex_Color = clamp(gl_Vertex, 0.0, 1.0);"
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
