/*
 * CGUIGTK.cpp
 *
 *  Created on: Mar 9, 2014
 *      Author: root
 */

#include "CGUIGTK.h"

CGUIGTK::CGUIGTK() :btn("button"),mActionGroup(Gtk::ActionGroup::create()),mUIManager(Gtk::UIManager::create())
{
	set_border_width(10);
	set_default_size(530,320);
	set_title("Bandwidth Variance Emulation");

	add(mPackingBox);


	mPackingBox.pack_start(btn);
	btn.show();



	mActionGroup->add(Gtk::Action::create("MenuFile", "_File"));
	mActionGroup->add(Gtk::Action::create("ImportData", "Import Data"),sigc::mem_fun(*this, &CGUIGTK::file_browser));

	mUIManager->insert_action_group(mActionGroup);
	add_accel_group(mUIManager->get_accel_group());

	Glib::ustring menuInfo =
			"<ui>"
				"<menubar name='MenuBar'>"
					"<menu name='MenuFile'>"
						"<menuitem name='ImportData'/>"
					"</menu>"
				"</menubar>"
				"<toolbar></toolbar>"
			"</ui>";

	mUIManager->add_ui_from_string(menuInfo);

	Gtk::Widget* menuBar = mUIManager->get_widget("/MenuBar");

	mPackingBox.pack_start(*menuBar);

	menuBar->show();
	mPackingBox.show();
}

void CGUIGTK::file_browser()
{
	std::cout << "open file browser" << std::endl;
}

CGUIGTK::~CGUIGTK()
{
	// TODO Auto-generated destructor stub
}

