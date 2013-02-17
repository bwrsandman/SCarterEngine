#ifndef SIMPLESCENE_H
#define SIMPLESCENE_H

#include <gtkmm.h>
#include <gtkglmm.h>

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
	/* OpenGL specific */
	virtual bool init_opengl (void);

private:
	/* Colors */
	const float         CLEAR_R     = 0.0f;
	const float         CLEAR_G     = 0.0f;
	const float         CLEAR_B     = 0.0f;
	const float         CLEAR_A     = 1.0f;
};

#endif
