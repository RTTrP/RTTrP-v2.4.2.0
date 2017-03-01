#ifndef RTTRP_H
#define RTTRP_H

#include "protocol_common.h"
#include "thirdParty_motion.h"
#include "lighting.h"

// Remove if not needed
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#pragma once

using namespace std;

class RTTrP
{
	public:
		uint16_t intHeader, fltHeader, version;
		uint32_t pID;
		uint8_t pForm;
		uint16_t pktSize;
		uint32_t context;
		uint8_t numMods;
		vector<unsigned char> data;

		RTTrP();
		RTTrP(vector<unsigned char>);
		~RTTrP();

		void printHeader(void);
};

class RTTrPM
{
	public:
		RTTrP *header;
		Trackable *trackable;
		CentroidMod *centroidMod;
		vector<LEDModule*> *ledMod;
		QuatModule *quatMod;
		EulerModule *eulerMod;
		CentroidAccVelMod *cavMod;
		vector<LEDAccVelMod*> *lavMod;
		vector<unsigned char> *data;
		vector<uint8_t> pkType;

		RTTrPM();
		~RTTrPM();

		void printPacket(void);
};

class RTTrPL
{
	public:
		RTTrP *header;
		LightingOutput *lightOut;
		LightingSync *sync;

		RTTrPL();
		RTTrPL(RTTrPL* toCopy);
		~RTTrPL();

		void printModule(void);
};

#endif // RTTRP_H