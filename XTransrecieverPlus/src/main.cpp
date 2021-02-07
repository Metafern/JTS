#include <cstdio>
#include "Config.h"
#include "Transciever.h"
#include <Packet.h>
#include <thread>

using namespace std;


int main(int argc, char* argv[])
{

	//JANK TRADING SYSTEM


	const string path = "D:/ninjhax/main/Documents - HDD/GitHub/XTransrecieverPlus/packets/";
	
	Tx tx1(cfg::interfaceIPAddr1, cfg::switchIP, cfg::searchfilter, false);
	Tx tx2(cfg::interfaceIPAddr2, cfg::switchIP, cfg::searchfilter, true);
	
	tx1.cookie.output = tx2.dev;
	tx2.cookie.output = tx1.dev;

	tx1.Start();
	tx2.Start();


	
	while (true);
}