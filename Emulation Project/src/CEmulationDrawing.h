/*
 * CEmulationDrawing.h
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */

#ifndef CEMULATIONDRAWING_H_
#define CEMULATIONDRAWING_H_

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>

class CEmulationDrawing : public Gtk::DrawingArea
{
public:
	CEmulationDrawing();
	virtual ~CEmulationDrawing();
protected:
	//Override default signal handler:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

	Glib::RefPtr<Gdk::Pixbuf> mImage;
};

#endif /* CEMULATIONDRAWING_H_ */
