#include "Application.h"

App::App (void)
	: m_VBox(false, 0)
	, m_ButtonQuit("Quit")
{
	/* Top-level window. */
	set_title("SCarter engine");

	/* Get automatically if any of their children changed allocation. */
	set_reallocate_redraws(true);

	add(m_VBox);

	/* Simple OpenGL scene. */
	m_SimpleGLScene.set_size_request(200, 200);
	m_VBox.pack_start(m_SimpleGLScene);

	/* Simple quit button. */
	m_ButtonQuit.signal_clicked().connect(
    	sigc::mem_fun(*this, &App::on_button_quit_clicked));

	m_VBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK, 0);

	/* Show window. */
	show_all();
}

App::~App(void)
{ }

void App::on_button_quit_clicked (void)
{
	Gtk::Main::quit();
}

