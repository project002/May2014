/*
 * CRouter.cpp
 *
 *  Created on: Dec 31, 2013
 *      Author: root
 */

#include "CRouter.h"

CRouter::CRouter():mBufferSize(DEFAULT_ROUTER_BUFFER_SIZE),mPacketCollector(NULL)
{
	try
	{
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
		throw;
	}

}
void CRouter::RequestTables()
{
	try
	{
		list<CConnection const *>::iterator iter;
		//iterate over all connections
		for(iter = mConnections.begin();iter!=mConnections.end();iter++)
		{
			cout << "Print Routing Table For " << (*iter)->GetMAC() << endl;
			//iterate over all ips in the table you got from the connection
			vector< pair<string,string> >& tables=(*iter)->GetTable();
			//print table
			vector< pair<string,string> >::iterator b = tables.begin();
			vector< pair<string,string> >::iterator it=tables.begin();
			for (;it!=tables.end();it++)
			{

				cout << "/t " << it->first << "|" << (*iter)->GetMAC() << "|" << it->second << endl;
				mRoutingTable.insert(pair<string,pair<CConnection const*,string> >(
						it->first,
						pair<CConnection const*,string>((*iter),it->second)
					));
			}
		}
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
		throw;
	}
}

void CRouter::Sniffer()
{
	try
	{
		mPacketCollector= new CPacketCollector(mBufferSize);
		mSniffingThread = boost::thread(&CRouter::Sniff, this);
		mPacketsHandlingThread = boost::thread(&CRouter::PacketHandler, this);
	}
	catch (CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
		throw;
	}
}


bool CRouter::ProcessSendPacket(Packet* packet)
{
//	cout << "[#] before change" << endl;

	map<string,pair< CConnection const*,string> >::iterator table_pos;
	IP* ip_layer = packet->GetLayer<IP>();
	Ethernet* eth_layer = packet->GetLayer<Ethernet>();

	table_pos = mRoutingTable.find(ip_layer->GetDestinationIP());

	string DestMAC = table_pos->second.second;
	string SrcMAC = table_pos->second.first->GetMAC();

	eth_layer->SetSourceMAC(SrcMAC);
	eth_layer->SetDestinationMAC(DestMAC);

	ip_layer->SetTTL(ip_layer->GetTTL()-1);
	return (ip_layer->GetTTL() >= 1);
}

void CRouter::PacketHandler()
{
	Packet* packet;
	map<string,pair< CConnection const*,string> >::iterator pos;
	map<string,pair< CConnection const*,string> >::iterator con_pos;
	try
	{
		while (true)
		{
			//cout<< "[@]1[@]\n";
			packet = mPacketCollector->PopFront();
			//cout<< "[@]1.1[@]\n";
			if (packet != NULL)
			{
				//cout<< "[@]1.2[@]\n";
				IP* ip_layer = packet->GetLayer<IP>();
				//cout<< "[@]1.3[@]\n";
				if (ip_layer != NULL)
				{
					//cout<< "[@]2[@]\n";
					HandleIPv4(packet);
				}
				else
				{
					//cout<< "[@]2.2[@]\n";
					ARP* arp_layer = packet->GetLayer<ARP>();
					//cout<< "[@]2.3@]\n";
					if (arp_layer != NULL) //answer arp requests
					{
						//cout<< "[@]3[@]\n";
						HandleArp(packet);
					}
				}
				if (packet!=NULL)
				{
					//cout<< "[@]5[@]\n";
					delete packet;
					packet=NULL;
				}
			}
			//cout<< "[@]6[@]\n";
		}
	}
	catch (CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
		throw;
	}
}

void CRouter::HandleArp(Packet * pkt)
{
	CConnection* send_connection;
	map<string,pair< CConnection const*,string> >::iterator pos;
	map<string,pair< CConnection const*,string> >::iterator con_pos;
	ARP* arp_layer = pkt->GetLayer<ARP>();
	//cout<<arp_layer->GetTargetIP()<<"\n";
	con_pos = mRoutingTable.find(arp_layer->GetSenderIP());
	pos = mRoutingTable.find(arp_layer->GetTargetIP());
	if (pos != mRoutingTable.end() && con_pos != mRoutingTable.end())
	{
		Ethernet* eth_layer = pkt->GetLayer<Ethernet>();
		if (arp_layer->GetOperation() == ARP::Request && eth_layer != NULL)
		{
			arp_layer->SetOperation(ARP::Reply);
			eth_layer->SetDestinationMAC(arp_layer->GetSenderMAC());
			//cout<<arp_layer->GetTargetIP()<<"\n";
			arp_layer->SetTargetMAC(arp_layer->GetSenderMAC());
			arp_layer->SetSenderMAC(con_pos->second.first->GetMAC());
			arp_layer->SetTargetIP(arp_layer->GetSenderIP());
			arp_layer->SetSenderIP(pos->first);
			eth_layer->SetSourceMAC(pos->second.first->GetMAC());
			send_connection = (const_cast<CConnection*>(con_pos->second.first));
			send_connection->SendPacket(pkt);
		}
	}
}
void CRouter::HandleIPv4(Packet * pkt)
{
	CConnection* send_connection;
	map<string,pair< CConnection const*,string> >::iterator pos;
	IP* ip_layer = pkt->GetLayer<IP>();
	pos = mRoutingTable.find(ip_layer->GetDestinationIP());
	if (pos != mRoutingTable.end())
	{
		send_connection = (const_cast<CConnection*>(pos->second.first));
		string dest_ip = send_connection->getGetwayAddress()->getIpStr();
		if (!dest_ip.compare(pos->first)) //TODO: ignoring packets to the router - maybe we want to respond?
		{;}
		else
		{
			Ethernet* eth_layer = pkt->GetLayer<Ethernet>();
			if (eth_layer != NULL
					&& eth_layer->GetSourceMAC().compare(
							send_connection->GetMAC()))
			{
				if (ProcessSendPacket (pkt))
				{
					Packet * newpkt= new Packet();
					newpkt->PacketFromEthernet(pkt->GetRawPtr(),ETH_HLEN+ip_layer->GetTotalLength());
					send_connection->SendPacket(newpkt);
					delete newpkt;
				}
			}
		}
	}
}
void CRouter::Sniff()
{
	Packet* temp_packet;
	try
	{
		while(true)
		{
			//cout<< "[@]1[@]\n";
			//cout<< "[@]1[@]\n";
			list< CConnection const *>::iterator iter = mConnections.begin();
			//cout<< "[@]2[@]\n";
			for (;iter!=mConnections.end();iter++)
			{

				//cout<< "[@]3[@]\n";
				CConnection * connection = const_cast<CConnection*>(*iter);
				//cout<< "[@]4[@]\n";
				temp_packet= connection->GetPacket();
				//cout<< "[@]5";
				if (temp_packet != NULL)
				{
					//cout<< "[@]6[@]\n";
					mPacketCollector->PushBack(temp_packet);
				}
				//cout<< "[@]7[@]\n";
			}
		}
	}
	catch (CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
		throw;
	}
}
CRouter::~CRouter()
{
	try
	{
		mConnections.clear();
		if(mPacketCollector!=NULL)
		{
			delete mPacketCollector;
			mPacketCollector=NULL;
		}
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
		throw;
	}
}

