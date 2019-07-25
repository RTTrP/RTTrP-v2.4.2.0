/*MIT License

#Copyright(c) 2019 CAST Group of Companies Inc

#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files(the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions :

#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE. */

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

class Packet 
{	
	public:
		uint8_t pkType;
		uint16_t intSig, fltSig;
		
		Packet();
		Packet(uint16_t intSig, uint16_t fltSig, std::vector<unsigned char> *data);
		Packet(const Packet &toCopy);
		~Packet();
};

class Trackable : public Packet
{
	public:
		uint16_t size;
		uint8_t nameLen;
		std::string name;
		uint8_t numMods;
		uint32_t timeStamp;

		Trackable();
		Trackable(std::vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
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
	CentroidMod(std::vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
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
		LEDModule(std::vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
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
		QuatModule(std::vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
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
		EulerModule(std::vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
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
		CentroidAccVelMod(std::vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
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
		LEDAccVelMod(std::vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
		LEDAccVelMod(const LEDAccVelMod &toCopy);
		~LEDAccVelMod();

		void printModule();
};

#endif
