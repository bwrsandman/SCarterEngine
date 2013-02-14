#ifndef APPLICATION_H
#define APPLICATION_H

#include <gtkmm.h>
#include "SimpleScene.h"

class App : public Gtk::Window
{
public:
	App (void);
	virtual ~App (void);

protected:
	/* Events */
	void on_button_quit_clicked (void);

protected:
	/* Widgets */
	Gtk::VBox m_VBox;
	SimpleGLScene m_SimpleGLScene;
	Gtk::Button m_ButtonQuit;
};

#endif
