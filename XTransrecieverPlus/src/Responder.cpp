#include "Responder.h"
#include <PayloadLayer.h>


using namespace std;
using namespace pcpp;
using namespace crft;


void Responder::setParser(Parser& parserIn) {
	parser = &parserIn;
	lan = Lan(parser);
	station = Station(parser);
}

void Responder::dumpPacket(std::vector<uint8_t>* raw) {}

bool Responder::setPokemonRaw(std::vector<uint8_t>* raw, pcpp::MacAddress dest, Pokemon* original, Pokemon* inject) {
	
	if (raw->size() < 400)
		return false;

	bool output = false;

	for (int i = 0; i < raw->size() - 1; i++) {
	
		if (raw->at(i) == 0xd8) {
			if (raw->at(i+1) == 0x02 && raw->size() > 0x160 + i) {
				printf("FOUND POKEMON! ");
				
				for (int i : *raw)
					printf("%02x", i);
				printf("\n");

				Pokemon found;
				for (int j = 2; j < 0x159; j++) {
					found.data[j - 2] = raw->at(j + i);
					
				}

				if (found.equals(*inject)) {
					for (int j = 2; j < 0x159; j++) {
						raw->at(i + j) = original->data[j - 2];
					}
				}
				else {
					for (int j = 2; j < 0x159; j++) {
						original->data[j - 2] = raw->at(i + j);
						raw->at(i + j) = inject->data[j - 2];
					}
				}


				for (int i : *raw)
					printf("%02x", i);
				printf("\n");

				output = true;
			}
		}
	}

	return output;
}

bool Responder::getResp(vector<Packet>& out) {
	
	hasResp = false;

	for (int i = 0; i < parser->messageVector.size(); i++) {
		
		if (parser->messageVector[i].payload.size() < 1)
			break;
		protocol = parser->messageVector[i].protocol_type;
		msgType = parser->messageVector[i].payload[0];
		
		if (protocol == 0x7c) {
			//exit(1);
			if (msgType == 0x07) {
				int offset = (uint16_t)parser->messageVector[i].payload[2] << 8 | parser->messageVector[i].payload[3];
				if (offset > 0x160 && offset < 0x170) {
					offset = offset - 0x151;
					printf("FOUND 7c:07!!!!! %02x%02x ", parser->messageVector[i].payload[offset], parser->messageVector[i].payload[offset + 1]);
				}
			}
				
		}
		printf("\n");

	}

	
	
	

	return hasResp;
}

void Responder::parseLan(vector<Packet>& packets) {
	
	switch (msgType)
	{
	case Lan::BROWSE_REQ:
		
		packets.push_back(lan.craftBrowseRep());
		packets.push_back(lan.craftBrowseReq());
		packets.push_back(lan.craftKeepAlive());
		break;
	case Lan::BROWSE_REP:
		packets.push_back(lan.craftKeepAlive());
		packets.push_back(lan.craftHostReq());
		break;
	case Lan::GET_HOST_REP:
		packets.push_back(lan.craftKeepAlive());
		packets.push_back(station.craftConnReq());
	default:
		hasResp = false;
		break;
	}

}