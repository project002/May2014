/*
 * CGUIGTK.h
 *
 *  Created on: Mar 9, 2014
 *      Author: root
 */

#ifndef CGUIGTK_H_
#define CGUIGTK_H_
#include <iostream>
#include <gtkmm.h>
#include <gtkmm/dialog.h>
#include <gtkmm/menubar.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include "EmulationWrapper.h"
#include "CSocketNotReadyException.h"
#include "CEmuStatWidget.h"
#include "CEmulationDrawing.h"

class CGUIGTK : public Gtk::Window
{
public:
	CGUIGTK();
	virtual ~CGUIGTK();

protected:
	//single handlers
	void open_file_browser();
	void start_emulation_quick();
	void run_emulation();
	void stop_emulation();
	void create_menu_bar();
	void start_emulation_thread();

	void loop();

	//members
	Gtk::Grid mGrid;
	Gtk::Box* mPackingBox;
	Gtk::MenuBar* mMenuBar;
	Gtk::Button* mStopButton;
	Gtk::Button* mQuickStartButton;
	Gtk::Label mInst;
	CEmuStatWidget* mStateWidget;
	Gtk::Frame mStateWidgetCon;
	//emulation painting
	CEmulationDrawing *mDrawing;
private:
	std::string mImportXMLPath;
	EmulationWrapper* mEmulation;
	bool mEmulationRunning;
	Glib::Thread* EmulationThread;

};

#endif /* CGUIGTK_H_ */
