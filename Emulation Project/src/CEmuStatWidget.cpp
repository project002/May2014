/*
 * CEmuStatWidget.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: root
 */

#include "CEmuStatWidget.h"

CEmuStatWidget::CEmuStatWidget(Gtk::Box* MainPack)
{

	MainPack->pack_start(mGrid,true,true,10);
	mGrid.set_row_spacing(20);
	mGrid.set_column_spacing(20);
	mlblPacketsStatus = Gtk::manage(new Gtk::Label());
	mlblPacketsStatus->set_text("Packets Processed:\nPackets Dropped:\nLostPercentage:");
	mStatusFrame.add(*mlblPacketsStatus);
	mStatusFrame.set_label("Packets Status:");
	mStatusFrame.set_shadow_type(Gtk::SHADOW_IN);
	mGrid.add(mStatusFrame);

	mlblPacketsTypes = Gtk::manage(new Gtk::Label());
	mlblPacketsTypes->set_text("IP Packets:\nARP Packets:\nDHCP PAckets:");
	mTypeFrame.set_label("Packets Types:");
	mTypeFrame.set_shadow_type(Gtk::SHADOW_IN);
	mTypeFrame.add(*mlblPacketsTypes);
	mGrid.add(mTypeFrame);

	mtvContainer = Gtk::manage(new Gtk::ScrolledWindow());
	mtvMessages = Gtk::manage(new Gtk::TextView());
	mtvMessagesBuffer = Gtk::TextBuffer::create();
	mtvMessages->set_buffer(mtvMessagesBuffer);
	mtvMessages->set_editable(false);
	mtvContainer->set_policy(Gtk::POLICY_NEVER,Gtk::POLICY_AUTOMATIC);
	mtvContainer->add(*mtvMessages);
	mMessagesFrame.set_label("Messages:");
	mMessagesFrame.set_shadow_type(Gtk::SHADOW_IN);
	mMessagesFrame.add(*mtvContainer);
	mGrid.attach_next_to(mMessagesFrame, mStatusFrame, Gtk::POS_BOTTOM, 2, 1);
}

void CEmuStatWidget::loop()
{
	stringstream ss;
	stringstream ss1;

	int pc = SDataController::getInstance().get_data(SDataController::PACKETPROCCES);
	int pd = SDataController::getInstance().get_data(SDataController::PACKETDROP);
	double lost = SDataController::getInstance().get_dropped_precentage();
	ss << "Packets Processed:" << pc << "\nPackets Dropped:" << pd << "\nLost Percentage:" << lost << "%";
	mlblPacketsStatus->set_text(ss.str());

	int ipc = SDataController::getInstance().get_data(SDataController::IPPACKET);
	int arpc = SDataController::getInstance().get_data(SDataController::ARPPACKET);
	int dhcpc = SDataController::getInstance().get_data(SDataController::DHCPPACKET);
	ss1 << "IP Packets:" << ipc << "\nARP Packets:" << arpc << "\nDHCP Packets:" << dhcpc;
	mlblPacketsTypes->set_text(ss1.str());

	std::string messages = SDataController::getInstance().get_messages();
	//mtvMessagesBuffer->set_text(messages);
}

CEmuStatWidget::~CEmuStatWidget()
{
	// TODO Auto-generated destructor stub
}

