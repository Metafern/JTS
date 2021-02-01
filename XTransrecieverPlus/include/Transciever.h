#pragma once
#include "PacketParser.h"
#include "Responder.h"
#include <PcapLiveDeviceList.h>
class Tx {
public:
	struct Cookie {
		Parser parser;
		Responder responder;
		void getPacket(pcpp::Packet& in) {
			isReady = false;
			in = packet;
			return;
		}
		pcpp::Packet packet;
		bool isReady = false;
		Cookie() {};
		
	} cookie;
	
	

	pcpp::PcapLiveDevice* dev;
	void Start(const std::string interfaceIPAddr, const std::string switchIPAddr, const std::string searchfilter, const bool secondary=false);
};