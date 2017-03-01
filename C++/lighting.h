#ifndef LIGHTING_H
#define LIGHTING_H

// Remove if not needed
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "protocol_common.h"
#include "thirdParty_motion.h"

#pragma once

using namespace std;

class ChannelBlock : Packet
{
public:
	uint16_t chanOffset, xFade;
	uint8_t value;

	ChannelBlock();
	ChannelBlock(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
	~ChannelBlock();

	void printModule();
};

class Spot : public Packet
{
	public:
		uint16_t size, spotID, spotOffset, channelStruct;
		vector<ChannelBlock*> *chanBlocks;

		Spot();
		Spot(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		~Spot();

		void printModule();
};

class Universe : public Packet
{
	public:
		uint16_t size, universeID, numSpots;
		vector<Spot*> *spotList;

		Universe();
		Universe(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		~Universe();

		void printModule();
};

class LightingOutput : public Packet
{
	public:
		uint16_t size;
		uint32_t lightSeuquence;
		uint8_t action;
		uint32_t holdTime;
		uint16_t numUniverses;
		vector<Universe*> *uniList;

		LightingOutput();
		LightingOutput(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		~LightingOutput();

		void printModule();
};

// This is not currently in use
class LightingSync : public Packet
{
	public:
		uint16_t size;
		uint32_t deviceID, deviceSubID0, deviceSubID1, seqNum;

		LightingSync();
		LightingSync(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		~LightingSync();

		void printModule();
};
#endif // LIGHTING_H