/*
 * CEmuStatWidget.h
 *
 *  Created on: Mar 20, 2014
 *      Author: root
 */

#ifndef CEMUSTATWIDGET_H_
#define CEMUSTATWIDGET_H_

#include "BasicIncludes.h"
#include "SDataController.h"
#include <gtkmm.h>
#include <gtkmm/widget.h>
#include <gtkmm/frame.h>
#include <gtkmm/grid.h>


class CEmuStatWidget
{
public:
	CEmuStatWidget(Gtk::Box* MainPack);
	void loop();
	virtual ~CEmuStatWidget();
private:
	Gtk::Grid mGrid;

	Gtk::Frame mStatusFrame;
	Gtk::Label* mlblPacketsStatus;

	Gtk::Frame mTypeFrame;
	Gtk::Label* mlblPacketsTypes;

	Gtk::Frame mMessagesFrame;
	Gtk::ScrolledWindow* mtvContainer;
	Gtk::TextView* mtvMessages;
	Glib::RefPtr<Gtk::TextBuffer> mtvMessagesBuffer;
};

#endif /* CEMUSTATWIDGET_H_ */
