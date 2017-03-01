#ifndef THIRDPARTY_MOTION_H
#define THIRDPARTY_MOTION_H

#define _USE_MATH_DEFINES

#include "protocol_common.h"

// Remove if not needed
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#pragma once

using namespace std;

class Packet 
{	
	public:
		uint8_t pkType;
		uint16_t intSig, fltSig;
		
		Packet();
		Packet(uint16_t intSig, uint16_t fltSig, vector<unsigned char> *data);
		Packet(const Packet &toCopy);
		~Packet();
};

class Trackable : public Packet
{
	public:
		uint16_t size;
		uint8_t nameLen;
		string name;
		uint8_t numMods;
		uint32_t timeStamp;

		Trackable();
		Trackable(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		Trackable(const Trackable &toCopy);
		~Trackable();

		void printModule();
};

class CentroidMod : public Packet
{
	public:
		uint16_t size, latency;
		double x, y, z;
	
	CentroidMod();
	CentroidMod(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
	CentroidMod(const CentroidMod &toCopy);
	~CentroidMod();

	void printModule();
};

class LEDModule : public Packet
{
	public:
		uint16_t size, latency;
		double x, y, z;
		uint8_t index;

		LEDModule();
		LEDModule(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		LEDModule(const LEDModule &toCopy);
		~LEDModule();

		void printModule();
};

class QuatModule : public Packet
{
	public:
		uint16_t size, latency;
		double Qx, Qy, Qz, Qw;

		QuatModule();
		QuatModule(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		QuatModule(const QuatModule &toCopy);
		~QuatModule();

		void printModule();
};

class EulerModule : public Packet
{
	public:
		uint16_t size, latency, order;
		double R1, R2, R3;

		EulerModule();
		EulerModule(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		EulerModule(const EulerModule &toCopy);
		~EulerModule();

		void printModule();
};

class CentroidAccVelMod : public Packet
{
	public:
		uint16_t size;
		double x, y, z;
		float accx, accy, accz;
		float velx, vely, velz;

		CentroidAccVelMod();
		CentroidAccVelMod(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		CentroidAccVelMod(const CentroidAccVelMod &toCopy);
		~CentroidAccVelMod();
	
		void printModule();
};

class LEDAccVelMod : public Packet
{
	public:
		uint16_t size;
		double x, y, z;
		float accx, accy, accz;
		float velx, vely, velz;
		uint8_t index;

		LEDAccVelMod();
		LEDAccVelMod(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		LEDAccVelMod(const LEDAccVelMod &toCopy);
		~LEDAccVelMod();

		void printModule();
};

#endif