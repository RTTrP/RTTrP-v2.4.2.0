#include "thirdParty_motion.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

double doubleSwap(vector<unsigned char> toSwap)
{
	vector<unsigned char> swapped;
	
	for (int i = toSwap.size() - 1; i >= 0; i--)
	{
		swapped.push_back(toSwap.at(i));
	}

	double retVal;

	copy(swapped.begin(), swapped.begin() + swapped.size(), (unsigned char*)&retVal);

	return retVal;
}

float floatSwap(vector<unsigned char> toSwap)
{
	vector<unsigned char> swapped;

	for (int i = toSwap.size() - 1; i >= 0; i--)
	{
		swapped.push_back(toSwap.at(i));
	}

	float retVal;

	copy(swapped.begin(), swapped.begin() + swapped.size(), (unsigned char*)&retVal);

	return retVal;
}

Packet::Packet()
{
	this->pkType = 0;
	this->intSig = 0;
	this->fltSig = 0;
}

Packet::Packet(uint16_t intSig, uint16_t fltSig, vector<unsigned char> *data)
{
	copy(data->begin(), data->begin() + 1, (unsigned char*)&this->pkType);
	data->erase(data->begin(), data->begin() + 1);
	
	this->intSig = intSig;
	this->fltSig = fltSig;
}

Packet::Packet(const Packet &toCopy)
{
	intSig = toCopy.intSig;
	fltSig = toCopy.fltSig;
	pkType = toCopy.pkType;
}

Packet::~Packet()
{

}

Trackable::Trackable()
{
	this->size = 0;
	this->nameLen = 0;
	this->numMods = 0;
	this->timeStamp = 0;
}

Trackable::Trackable(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig) : Packet(intSig, fltSig, data)
{
	copy(data->begin(), data->begin() + 2, (unsigned char*)&this->size);
	data->erase(data->begin(), data->begin() + 2);

	copy(data->begin(), data->begin() + 1, (unsigned char*)&this->nameLen);
	data->erase(data->begin(), data->begin() + 1);

	this->name.append(data->begin(), data->begin() + this->nameLen);
	data->erase(data->begin(), data->begin() + sizeof(unsigned char)*this->nameLen);

	if (this->pkType == 0x51)
	{
		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->timeStamp);
		data->erase(data->begin(), data->begin() + 4);
	}
	else
		this->timeStamp = 0;

	copy(data->begin(), data->begin() + 1, (unsigned char*)&this->numMods);
	data->erase(data->begin(), data->begin() + 1);

	// Convert data from Big Endian format to local host endianness
	if (this->intSig == 0x4154)
	{
		this->size = ntohs(this->size);
		this->timeStamp = ntohl(this->timeStamp);
	}
}

Trackable::Trackable(const Trackable &toCopy)
{
	intSig = toCopy.intSig;
	fltSig = toCopy.fltSig;
	size = toCopy.size;
	nameLen = toCopy.nameLen;
	name = toCopy.name;
	numMods = toCopy.numMods;
	timeStamp = toCopy.timeStamp;
}

Trackable::~Trackable()
{
}

void Trackable::printModule()
{
	cout << "==================Trackable Module==================" << endl;
	cout << "Integer Signature: 0x" << hex << this->intSig << endl;
	cout << "Floating Point Signature: 0x" << hex << this->fltSig << endl;
	cout << "Size: " << this->size << endl;
	cout << "Packet Name Length: " << (int) this->nameLen << endl;
	//cout << "Trackable Name: " << (unsigned char) *this->name << endl;
	cout << "Number of Modules: " << (int) this->numMods << endl;
	cout << "Time Stamp: 0x" << hex << this->timeStamp << endl;
	cout << "==================Trackable Module==================" << endl;
}

CentroidMod::CentroidMod()
{
}

CentroidMod::CentroidMod(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig) : Packet(intSig, fltSig, data)
{
	copy(data->begin(), data->begin() + 2, (unsigned char *)&this->size);
	data->erase(data->begin(), data->begin() + 2);

	copy(data->begin(), data->begin() + 2, (unsigned char *)&this->latency);
	data->erase(data->begin(), data->begin() + 2);

	if (this->fltSig == 0x4334)
	{
		vector<unsigned char> toSwap(8, '\0');

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->x = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->y = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->z = doubleSwap(toSwap);
	}
	else
	{
		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->x);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->y);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->z);
		data->erase(data->begin(), data->begin() + 8);
	}

	if (this->intSig == 0x4154)
	{
		this->size = ntohs(this->size);
		this->latency = ntohs(this->latency);
	}
}

CentroidMod::CentroidMod(const CentroidMod &toCopy)
{
	size = toCopy.size;
	latency = toCopy.latency;
	x = toCopy.x;
	y = toCopy.y;
	z = toCopy.z;
}

CentroidMod::~CentroidMod()
{
}

void CentroidMod::printModule()
{
	cout << "==================Centroid Module==================" << endl;
	cout << "Module Size: " << this->size << endl;
	cout << "Latency: " << this->latency << endl;
	cout << "X Position: " << this->x << endl;
	cout << "Y Position: " << this->y << endl;
	cout << "Z Position: " <<this->z << endl;
	cout << "==================Centroid Module==================" << endl;
}

LEDModule::LEDModule()
{
}

LEDModule::LEDModule(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig) : Packet(intSig, fltSig, data)
{

	copy(data->begin(), data->begin() + 2, (unsigned char*)&this->size);
	data->erase(data->begin(), data->begin() + 2);
	
	copy(data->begin(), data->begin() + 2, (unsigned char*)&this->latency);
	data->erase(data->begin(), data->begin() + 2);
	
	if (this->fltSig == 0x4334)
	{
		vector<unsigned char> toSwap(8, '\0');

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->x = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->y = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->z = doubleSwap(toSwap);
	}
	else
	{
		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->x);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->y);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->z);
		data->erase(data->begin(), data->begin() + 8);
	}

	copy(data->begin(), data->begin() + 1, (unsigned char*)&this->index);
	data->erase(data->begin(), data->begin() + 1);

	if (this->intSig == 0x4154)
	{
		this->size = ntohs(this->size);
		this->latency = ntohs(this->latency);
	}
}

LEDModule::LEDModule(const LEDModule &toCopy)
{
	size = toCopy.size;
	latency = toCopy.latency;
	x = toCopy.x;
	y = toCopy.y;
	z = toCopy.z;
	index = toCopy.index;
}

LEDModule::~LEDModule()
{
}

void LEDModule::printModule()
{
	cout << "==================LED Module==================" << endl;
	cout << "Module Size: " << this->size << endl;
	cout << "Latency: " << this->latency << endl;
	cout << "X Position: " << this->x << endl;
	cout << "Y Position: " << this->y << endl;
	cout << "Z Position: " << this->z << endl;
	cout << "LED Index: " << this->index << endl;
	cout << "==================LED Module==================" << endl;
}

QuatModule::QuatModule()
{
}

QuatModule::QuatModule(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig) : Packet(intSig, fltSig, data)
{
	copy(data->begin(), data->begin() + 2, (unsigned char*)&this->size);
	data->erase(data->begin(), data->begin() + 2);

	copy(data->begin(), data->begin() + 2, (unsigned char*)&this->latency);
	data->erase(data->begin(), data->begin() + 2);

	if (this->fltSig == 0x4334)
	{
		vector<unsigned char> toSwap(8, '\0');

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->Qx = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->Qy = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->Qz = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->Qw = doubleSwap(toSwap);
	}
	else
	{
		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->Qx);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->Qy);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->Qz);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->Qw);
		data->erase(data->begin(), data->begin() + 8);
	}
	
	if (this->intSig == 0x4154)
	{
		this->size = ntohs(this->size);
		this->latency = ntohs(this->latency);
	}
}

QuatModule::QuatModule(const QuatModule &toCopy)
{
	size = toCopy.size;
	latency = toCopy.latency;
	Qx = toCopy.Qx;
	Qy = toCopy.Qy;
	Qz = toCopy.Qz;
	Qx = toCopy.Qx;
}

QuatModule::~QuatModule()
{
}

void QuatModule::printModule()
{
	cout << "==================Quaternion Module==================" << endl;
	cout << "Module Size: " << this->size << endl;
	cout << "Latency: " << this->latency << endl;
	cout << "Qx: " << this->Qx << endl;
	cout << "Qy: " << this->Qy << endl;
	cout << "Qz: " << this->Qz << endl;
	cout << "Qw: " << this->Qw << endl;
	cout << "==================Quaternion Module==================" << endl;
}

EulerModule::EulerModule()
{
}

EulerModule::EulerModule(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig) : Packet(intSig, fltSig, data)
{

	copy(data->begin(), data->begin() + 2, (unsigned char*)&this->size);
	data->erase(data->begin(), data->begin() + 2);

	copy(data->begin(), data->begin() + 2, (unsigned char*)&this->latency);
	data->erase(data->begin(), data->begin() + 2);

	copy(data->begin(), data->begin() + 2, (unsigned char*)&this->order);
	data->erase(data->begin(), data->begin() + 2);

	if (this->fltSig == 0x4334)
	{
		vector<unsigned char> toSwap(8, '\0');

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->R1 = doubleSwap(toSwap);
		this->R1 = (this->R1 * 180) / M_PI;

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->R2 = doubleSwap(toSwap);
		this->R2 = (this->R2 * 180) / M_PI;

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->R3 = doubleSwap(toSwap);
		this->R3 = (this->R3 * 180) / M_PI;
	}
	else
	{
		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->R1);
		data->erase(data->begin(), data->begin() + 8);
		this->R1 = (this->R1 * 180) / M_PI;

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->R2);
		data->erase(data->begin(), data->begin() + 8);
		this->R2 = (this->R2 * 180) / M_PI;

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->R3);
		data->erase(data->begin(), data->begin() + 8);
		this->R3 = (this->R3 * 180) / M_PI;
	}

	if (this->intSig == 0x4154)
	{
		this->size = ntohs(this->size);
		this->latency = ntohs(this->latency);
		this->order = ntohs(this->order);
	}
}

EulerModule::EulerModule(const EulerModule &toCopy)
{
	size = toCopy.size;
	latency = toCopy.latency;
	order = toCopy.order;
	R1 = toCopy.R1;
	R2 = toCopy.R2;
	R3 = toCopy.R3;
}

EulerModule::~EulerModule()
{
}

void EulerModule::printModule()
{
	cout << "==================Euler Module==================" << endl;
	cout << "Module Size: " << this->size << endl;
	cout << "Latency: " << this->latency << endl;
	cout << "Euler Order: " << this->order << endl;
	cout << "R1: " << this->R1 << endl;
	cout << "R2: " << this->R2 << endl;
	cout << "R3: " << this->R3 << endl;
	cout << "==================Euler Module==================" << endl;
}

CentroidAccVelMod::CentroidAccVelMod()
{
}

CentroidAccVelMod::CentroidAccVelMod(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig) : Packet(intSig, fltSig, data)
{
	copy(data->begin(), data->begin() + 2, (unsigned char *)&this->size);
	data->erase(data->begin(), data->begin() + 2);

	if (this->fltSig == 0x4334)
	{
		vector<unsigned char> toSwap(8, '\0');

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->x = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->y = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->z = doubleSwap(toSwap);

		toSwap.resize(4);
		toSwap.reserve(4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->accx = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->accy = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->accz = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->velx = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->vely = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->velz = floatSwap(toSwap);
	}
	else
	{
		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->x);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->y);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->z);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->accx);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->accy);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->accz);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->velx);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->vely);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->velz);
		data->erase(data->begin(), data->begin() + 4);
	}

	if (this->intSig == 0x4154)
	{
		this->size = ntohs(this->size);
	}
}

CentroidAccVelMod::CentroidAccVelMod(const CentroidAccVelMod &toCopy)
{
	size = toCopy.size;
	x = toCopy.x;
	y = toCopy.y;
	z = toCopy.z;
	accx = toCopy.accx;
	accy = toCopy.accy;
	accz = toCopy.accz;
	velx = toCopy.velx;
	velx = toCopy.vely;
	velz = toCopy.velz;
}

CentroidAccVelMod::~CentroidAccVelMod()
{
}

void CentroidAccVelMod::printModule()
{
	cout << "======Centroid Acceleration/Velocity Module======" << endl;
	cout << "Module Size: " << this->size << endl;
	
	cout << "X Position: " << this->x << endl;
	cout << "Y Position: " << this->y << endl;
	cout << "Z Position: " << this->z << endl;

	cout << "X Velocity: " << this->velx << endl;
	cout << "Y Velocity: " << this->vely << endl;
	cout << "Z Velocity: " << this->velz << endl;
	
	cout << "X Acceleration: " << this->accx << endl;
	cout << "Y Acceleration: " << this->accy << endl;
	cout << "Z Acceleration: " << this->accz << endl;
	cout << "======Centroid Acceleration/Velocity Module======" << endl;
}

LEDAccVelMod::LEDAccVelMod()
{
}

LEDAccVelMod::LEDAccVelMod(vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig) : Packet(intSig, fltSig, data)
{
	copy(data->begin(), data->begin() + 2, (unsigned char *)&this->size);
	data->erase(data->begin(), data->begin() + 2);

	if (this->fltSig == 0x4334)
	{
		vector<unsigned char> toSwap(8, '\0');

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->x = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->y = doubleSwap(toSwap);

		copy(data->begin(), data->begin() + 8, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 8);
		this->z = doubleSwap(toSwap);

		toSwap.resize(4);
		toSwap.reserve(4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->accx = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->accy = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->accz = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->velx = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->vely = floatSwap(toSwap);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&toSwap[0]);
		data->erase(data->begin(), data->begin() + 4);
		this->velz = floatSwap(toSwap);
	}
	else
	{
		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->x);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->y);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 8, (unsigned char *)&this->z);
		data->erase(data->begin(), data->begin() + 8);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->accx);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->accy);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->accz);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->velx);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->vely);
		data->erase(data->begin(), data->begin() + 4);

		copy(data->begin(), data->begin() + 4, (unsigned char*)&this->velz);
		data->erase(data->begin(), data->begin() + 4);
	}

	copy(data->begin(), data->begin() + 1, (unsigned char*)&this->index);
	data->erase(data->begin(), data->begin() + 1);

	if (this->intSig == 0x4154)
	{
		this->size = ntohs(this->size);
	}
}

LEDAccVelMod::LEDAccVelMod(const LEDAccVelMod &toCopy)
{
	size = toCopy.size;
	x = toCopy.x;
	y = toCopy.y;
	z = toCopy.z;
	accx = toCopy.accx;
	accy = toCopy.accy;
	accz = toCopy.accz;
	velx = toCopy.velx;
	velx = toCopy.vely;
	velz = toCopy.velz; 
	index = toCopy.index;
}

LEDAccVelMod::~LEDAccVelMod()
{
}

void LEDAccVelMod::printModule()
{
	cout << "========LED Acceleration/Velocity Module========" << endl;
	cout << "Module Size: " << this->size << endl;

	cout << "X Position: " << this->x << endl;
	cout << "Y Position: " << this->y << endl;
	cout << "Z Position: " << this->z << endl;

	cout << "X Velocity: " << this->velx << endl;
	cout << "Y Velocity: " << this->vely << endl;
	cout << "Z Velocity: " << this->velz << endl;

	cout << "X Acceleration: " << this->accx << endl;
	cout << "Y Acceleration: " << this->accy << endl;
	cout << "Z Acceleration: " << this->accz << endl;
	cout << "========LED Acceleration/Velocity Module========" << endl;
}

ZoneMod::ZoneMod()
{

}

void ZoneMod::ZoneMod(std::vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig)
{
    copy(data->begin(), data->begin() + 2, (unsigned char *)&this->size);
    data->erase(data->begin(), data->begin() + 2);
    copy(data->begin(), data->begin() + 1, (unsigned char*)&this->numofZoneSubModules);
    data->erase(data->begin(), data->begin() + 1);
   
    if (this->intSig == 0x4154)
    {
        this->size = ntohs(this->size);
        this->numofZoneSubModules = ntohs(this->numofZoneSubModules);
    }
}

ZoneMod::ZoneMod(const ZoneMod &toCopy)
{
    size = toCopy.size;
    numofZoneSubModules = toCopy.numofZoneSubModules;
}

ZoneMod::~ZoneMod()
{

}

void ZoneMod::printModule()
{
    cout << "========Colliding Zones Module ========" << endl;

    cout << "Module Size: " << this->size << endl;
    cout << "Number of Zones colliding with the trackable  " << this->numofZoneSubModules << endl;

    cout << "========Colliding Zones Module ========" << endl;

}

class ZoneMod : public Packet
{
public:
    uint16_t size;
    uint8_t numofZoneSubModules;
    ZoneMod();
    ZoneMod(std::vector<unsigned char> *data, uint16_t intSig, uint16_t fltSig);
    ZoneMod(const ZoneMod &toCopy);
    ~ZoneMod();

    void printModule();
};

ZoneSubMod::ZoneSubMod()
{

}

ZoneSubMod::ZoneSubMod(std::vector<unsigned char> *data, uint16_t intSig)
{
    copy(data->begin(), data->begin() + 2, (unsigned char *)&this->size);
    data->erase(data->begin(), data->begin() + 2);
    copy(data->begin(), data->begin() + 1, (unsigned char*)&this->zoneNameLength);
    data->erase(data->begin(), data->begin() + 1);
    if (this->intSig == 0x4154)
    {
        this->size = ntohs(this->size);
        this->zoneNameLength = ntohs(this->zoneNameLength);
    }
    copy(data->begin(), data->begin() + this->zoneNameLength, (unsigned char*)&this->zoneName);
    data->erase(data->begin(), data->begin() + this->zoneNameLength);
}

ZoneSubMod::ZoneSubMod(const ZoneSubMod &toCopy)
{
    intSig = toCopy.intSig;
    size = toCopy.size;
    zoneNameLength = toCopy.zoneNameLength;
    zoneName = toCopy.zoneName;
}

void ZoneSubMod::printModule()
{
    cout << "========Colliding Zones Sub Module ========" << endl;

    cout << "Zone Sub Module Size: " << this->size << endl;
    cout << "Colliding Zone Name length " << this->zoneNameLength << endl;
    cout << "Colliding Zone Name  " << this->zoneName << endl;

    cout << "========Colliding Zones Module ========" << endl;
}

class ZoneSubMod
{
public:
    uint16_t intSig;
    uint8_t size;
    uint8_t zoneNameLength;
    std::string zoneName;
    ZoneSubMod();
    ZoneSubMod(std::vector<unsigned char> *data, uint16_t intSig);
    ZoneSubMod(const ZoneSubMod &toCopy);
    ~ZoneSubMod();

    void printModule();
};
