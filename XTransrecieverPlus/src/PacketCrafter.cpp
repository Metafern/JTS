#include "PacketCrafter.h"
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/hmac.h>

using namespace std;
using namespace pcpp;
using namespace util;
using namespace cfg;

uint64_t crft::PiaPacket::nonce = 0;

crft::PiaPacket::PiaPacket(Parser* parserIn) {
	parser = parserIn;
	header.nonce = nonce;
	header.version = 0x84;

	message.field_flags = 0x7f;
	message.msg_flag = 0x09;
}
crft::PiaPacket::PiaPacket() {
	header.nonce = nonce;
	header.version = 0x84;
}

Packet crft::PiaPacket::craftPacket(Packet recieved, vector<uint8_t> data) {
	
	Packet out = recieved;
	
	PayloadLayer* payload = out.getLayerOfType<PayloadLayer>();
	payload->setPayload(data.data(), data.size());
	out.computeCalculateFields();
	return out;
}
/*
Packet crft::Lan::craftBrowseReq() {

	//lazy approach
	vector<uint8_t> data;
	HexToVector("000000023affffffff000400040101ffffffff000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000020201000000000000000488c6f41cf3382b4157089e1c623487b5a031d818765bedd597ca0ce3a3c8ec6fedac053da5f495f0d8002f1cb241513a1d1c194fec6bba421d0c00a09577c90c0b54416ea6e75d62a636459abdcd5f1707dc71e6ea2cfe9a3fe83eb449b74d743b9e6ef38fdb9605004e08716e7c952796f684763a6a49ae13e5bd117261904a3ca85e5f7b1fca70c4df19093806cd2c9edbbb16fa5b902a4bceffdef848594abac7bda09009db083bec22617d7ccf671656fcbc7db01ef34172bcd2c024b19d838db0ae1cf25cd126fe576345860416c716ebf4c53cda1d2875f2ff68310ce25394eb7c60a9077e6a86824ddb594d11c87ec47f509638874827eb77b4cce2fb687c0474cddedac3380b2117255ddb3c32e614c6653b2199b577a400a80e36f2", &data);
	return craftPacket(data, 30000, 30000);
}

Packet crft::Lan::craftBrowseRep() {


	//standard console-specific data
	vector<uint8_t> out;
	HexToVector("010000051100000000003dc00100000000000000000000000000000000000000000000000000010000000407050000e82d500d000002d46119ec17c85000a570046a39a699bc2442a92fe73400108babd9278ec8bbced3ef99039c01306e43006f006f006c000000046a39a699bc2442a92fe7340010081102ac310000090400000042ac11030e4c64b183014d3900024f0a4f47b789470b292b44623b8347eaab2d3e4e47b789470b292b44623b8347eaab2d3e4d47b789470b292b44623b8347eaab2d3e02ff00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000db0038000801000032000000000000000e0025000200000039040200580012000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000168010a00003d000000000000000000000000c0010a00003d0000c001f975acfa003dc0010101536f6d656f6e650000000000000000000000000000000000000000000000000000000000000000000a00003d0000c001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", &out);

	vector<uint8_t> selfKey;
	util::HexToVector("98408530300f066d20cf8fafa062cd87", &selfKey);
	out.insert(out.end(), selfKey.begin(), selfKey.end());
	out.insert(out.end(), parser->browseReply.challengeKey.begin(), parser->browseReply.challengeKey.end());

	vector<uint8_t> cryptoChallenge = parser->browseReply.makeResponse();

	out.insert(out.end(), cryptoChallenge.begin(), cryptoChallenge.end());

	return craftPacket(out, 30000, 30000, parser->udpInfo.srcIP);
}

Packet crft::Lan::craftHostReq() {

	vector<uint8_t> data;
	data.push_back(Lan::GET_HOST_REQ); //Packet ID
	for (int i = 0; i < 11; i++)
		data.push_back(0); //padding
	data.insert(data.end(), parser->sessionID.begin(), parser->sessionID.end());

	message.appendHeader(&data);

	vector<uint8_t> out;

	parser->EncryptPia(data, &out, header);
	return craftPacket(out, 49155, 49152);
}

Packet crft::Lan::craftKeepAlive() {
	vector<uint8_t> data;
	HexToVector("070000000000000000000000", &data);

	message.appendHeader(&data);

	vector<uint8_t> out;
	parser->EncryptPia(data, &out, header);
	return craftPacket(out, 49152, 49152);
}

Packet crft::Lan::craftSessReq() {
	vector<uint8_t> data;
	data.push_back(Lan::GET_SESSION_REQ); //Packet ID
	for (int i = 0; i < 11; i++)
		data.push_back(0); //padding
	data.insert(data.end(), parser->sessionID.begin(), parser->sessionID.end());

	message.appendHeader(&data);

	vector<uint8_t> out;

	parser->EncryptPia(data, &out, header);
	return craftPacket(out, 49155, 49152);
}


Packet crft::Station::craftConnReq() {

	uint8_t conn_id = 0x3f;
	uint8_t version = 0x09;
	uint8_t is_inverse = 0x00;
	uint64_t pid = 0x0a00003d0000c003;
	uint32_t cid = 0x164e9a66;
	uint8_t inverse_cid = 0;

	vector<uint8_t> data;
	data.insert(data.end(), { Station::CONN_REQ, conn_id, version, is_inverse });
	vector<uint8_t> temp = NumToVector(pid, sizeof(pid));
	data.insert(data.end(), temp.begin(), temp.end());

	HexToVector("553c80b900020600000a0000e0c0030000000000000a0000e00000c003e506244a00e0c00300000001", &data); //lazy approach to stationlocation
	HexToVector("d9fe4fba", &data); //lazy approach to ack id
	message.appendHeader(&data);
	HexToVector("000000", &data);
	
	vector<uint8_t> out;
	
	printf("SENT: ");
	for (int i : data)
		printf("%02x", i);
	printf("\n");
	parser->EncryptPia(data, &out, header);
	return craftPacket(out, 49155, 49152);
}
*/