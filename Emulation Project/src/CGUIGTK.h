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
#include <gtkmm/actiongroup.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>

class CGUIGTK : public Gtk::Window
{
public:
	CGUIGTK();
	virtual ~CGUIGTK();
protected:
	//single handlers
	void file_browser();

	//members
	Gtk::Box mPackingBox;
	Gtk::Button btn;
	Glib::RefPtr<Gtk::ActionGroup> mActionGroup;
	Glib::RefPtr<Gtk::UIManager> mUIManager;
};

#endif /* CGUIGTK_H_ */
