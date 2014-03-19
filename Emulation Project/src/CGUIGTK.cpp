/*
 * CGUIGTK.cpp
 *
 *  Created on: Mar 9, 2014
 *      Author: root
 */

#include "CGUIGTK.h"

CGUIGTK::CGUIGTK() :mPackingBox(Gtk::manage(new Gtk::Box())),
					mMenuBar(Gtk::manage(new Gtk::MenuBar())),
					mStopButton(Gtk::manage(new Gtk::Button())),
					mQuickStartButton(Gtk::manage(new Gtk::Button())),
					mImportXMLPath(""),
					mEmulation(NULL),
					mEmulationRunning(false)
{
	set_border_width(10);
	set_default_size(530,520);
	set_title("Bandwidth Variance Emulation");

	mPackingBox->set_orientation(Gtk::ORIENTATION_VERTICAL);
	add(*mPackingBox);
	create_menu_bar();

	//quick start button
	mQuickStartButton->set_label("Quick Start");
	mQuickStartButton->signal_clicked().connect(sigc::mem_fun(*this,&CGUIGTK::start_emulation_quick));
	mPackingBox->pack_start(*mQuickStartButton,Gtk::PACK_SHRINK);

	//documentation label
	mInst.set_label("Emulation Status");
	mPackingBox->pack_start(mInst,Gtk::PACK_SHRINK);

	//text view
	create_textview();

	//make stop button
	mStopButton->set_label("Stop Emulation");
	mStopButton->signal_clicked().connect(sigc::mem_fun(*this,&CGUIGTK::stop_emulation));
	mPackingBox->pack_end(*mStopButton,Gtk::PACK_SHRINK);


	mPackingBox->show_all();
}

void CGUIGTK::create_menu_bar()
{
	mPackingBox->pack_start(*mMenuBar,Gtk::PACK_SHRINK,0);

	//menu items top 0
	Gtk::MenuItem* menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
	mMenuBar->append(*menuitem_file);
	Gtk::Menu* file_sub_menu = Gtk::manage(new Gtk::Menu());
	menuitem_file->set_submenu(*file_sub_menu);
	//submenu items
	Gtk::MenuItem* import_file = Gtk::manage(new Gtk::MenuItem("Import..", true));
	//register imoprt file action
	import_file->signal_activate().connect(sigc::mem_fun(*this,&CGUIGTK::open_file_browser));
	file_sub_menu->append(*import_file);
	Gtk::MenuItem *run_emu = Gtk::manage(new Gtk::MenuItem("Run", true));
	//register run emulation action
	run_emu->signal_activate().connect(sigc::mem_fun(*this,&CGUIGTK::start_emulation_thread));
	file_sub_menu->append(*run_emu);
}

void CGUIGTK::create_textview()
{
	mTextScrollView = Gtk::manage(new Gtk::ScrolledWindow());
	mStatusText = Gtk::manage(new Gtk::TextView());
	mStatusTextBuffer = Gtk::TextBuffer::create();
	mStatusText->set_buffer(mStatusTextBuffer);
	mStatusText->set_editable(false);
//	mTextScrollView->set_policy(Gtk::POLICY_NEVER,Gtk::POLICY_AUTOMATIC);
	mTextScrollView->set_policy(Gtk::POLICY_NEVER,Gtk::POLICY_NEVER);
	mTextScrollView->add(*mStatusText);
	mPackingBox->pack_start(*mTextScrollView,Gtk::PACK_EXPAND_WIDGET);
}

void CGUIGTK::start_emulation_quick()
{
	open_file_browser();
	start_emulation_thread();
}

void CGUIGTK::open_file_browser()
{
	  Gtk::FileChooserDialog dialog("Select the Emulation XML file",
	          Gtk::FILE_CHOOSER_ACTION_OPEN);
	  dialog.set_transient_for(*this);

	  //Add response buttons the the dialog:
	  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	  //Add filters, so that only certain file types can be selected:

	  Glib::RefPtr<Gtk::FileFilter> filter_text = Gtk::FileFilter::create();
	  filter_text->set_name("XML Emulation files");
	  filter_text->add_mime_type("application/xml");
	  dialog.add_filter(filter_text);

	  Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
	  filter_any->set_name("Any files");
	  filter_any->add_pattern("*");
	  dialog.add_filter(filter_any);

	  //Show the dialog and wait for a user response:
	  int result = dialog.run();

	  //Handle the response:
	  switch(result)
	  {
	    case(Gtk::RESPONSE_OK):
	    {
	      mImportXMLPath = dialog.get_filename();
	      break;
	    }
	    default:
	    {
	      //how??
	      break;
	    }
	  }
}

void CGUIGTK::stop_emulation()
{
	if (mEmulation!=NULL)
	{
		mEmulationRunning = false;
		mEmulation->StopEmulation();
		delete mEmulation;
		mEmulation = NULL;
	}
}

void CGUIGTK::start_emulation_thread()
{
	mEmulationRunning = true;
	EmulationThread = Glib::Thread::create(sigc::mem_fun(*this,&CGUIGTK::run_emulation),false);
}


void CGUIGTK::run_emulation() //runs in a seperate thread
{
	//while (mEmulationRunning)
	//{
		mInst.set_label("Run Started");
		try
		{
			if (mEmulation==NULL)
			{
				mEmulation = new EmulationWrapper(mImportXMLPath);
				mEmulation->RunEmulation();
				mInst.set_label("Run Ended");
			}
		}
		catch(CException & error)
		{
			mEmulationRunning = false;
			mInst.set_label("Exception On Run");
			//throw CException("Emulation Failed");
		}
	//}
}

CGUIGTK::~CGUIGTK()
{
	// TODO Auto-generated destructor stub
	delete mMenuBar;
	delete mStopButton;
	delete mPackingBox;
	if (mEmulation!=NULL) {delete mEmulation;}

}

