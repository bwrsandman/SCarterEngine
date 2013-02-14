#ifndef SIMPLESCENE_H
#define SIMPLESCENE_H

#include <gtkmm.h>
#include <gtkglmm.h>

class SimpleGLScene : public Gtk::DrawingArea,
                      public Gtk::GL::Widget<SimpleGLScene>
{
public:
  SimpleGLScene();
  virtual ~SimpleGLScene();

protected:
  virtual void on_realize();
  virtual bool on_configure_event(GdkEventConfigure* event);
  virtual bool on_expose_event(GdkEventExpose* event);

};

#endif
