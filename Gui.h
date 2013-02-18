#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>
class SceneBase;

class Gui
{
private:
	const char* const _main_window = "main_window";
	const char* const _gl_frame = "gl_frame";

	Gtk::Main* kit = NULL;
	Gtk::Window* main_win = NULL;
	Gtk::Alignment* gl_container = NULL;
	Gtk::Scale* scl_norm = NULL;
	SceneBase* scene = NULL;

	void on_set_norm_alpha(void);

public:
	Gui(int, char**, const char* const);
	~Gui(void);
	void Draw(void);

};

#endif
