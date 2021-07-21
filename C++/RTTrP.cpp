#include "RTTrP.h"

using namespace std;

template<typename vectorType>
void print_vector(vector<vectorType> *toPrint)
{
	typename vector<vectorType>::iterator it;

	for (it = toPrint->begin(); it != toPrint->end(); it++)
		(*it)->printModule();
}

RTTrP::RTTrP()
{
}

RTTrP::RTTrP(vector<unsigned char> data)
{
	std::copy(data.begin(), data.begin() + 2, (unsigned char*)&this->intHeader);
	data.erase(data.begin(), data.begin() + 2);

	copy(data.begin(), data.begin() + 2, (unsigned char*)&this->fltHeader);
	data.erase(data.begin(), data.begin() + 2);

	// The header values are always in Network (Big Endian) Format
	this->intHeader = ntohs(this->intHeader);
	this->fltHeader = ntohs(this->fltHeader);
	
	copy(data.begin(), data.begin() + 2, (unsigned char*)&this->version);
	data.erase(data.begin(), data.begin() + 2);

	copy(data.begin(), data.begin() + 4, (unsigned char*)&this->pID);
	data.erase(data.begin(), data.begin() + 4);

	copy(data.begin(), data.begin() + 1, (unsigned char*)&this->pForm);
	data.erase(data.begin(), data.begin() + 1);

	copy(data.begin(), data.begin() + 2, (unsigned char*)&this->pktSize);
	data.erase(data.begin(), data.begin() + 2);

	copy(data.begin(), data.begin() + 4, (unsigned char*)&this->context);
	data.erase(data.begin(), data.begin() + 4);

	copy(data.begin(), data.begin() + 1, (unsigned char*)&this->numMods);
	data.erase(data.begin(), data.begin() + 1);

	this->data.resize(data.size());
	
	if (data.size() > 0)
		copy(data.begin(), data.end(), (unsigned char*)&this->data[0]);
	
	if (this->intHeader == 0x4154)
	{
		this->version = ntohs(this->version);
		this->pID = ntohl(this->pID);
		this->pktSize = ntohs(this->pktSize);
		this->context = ntohl(this->context);
	}
}

RTTrP::~RTTrP()
{
}

void RTTrP::printHeader(void)
{
	cout << "==================RTTrP Packet Header==================" << endl;
	cout << "Integer Header: 0x" << hex << this->intHeader << endl;
	cout << "Floating Point Header: 0x" << hex << this->fltHeader << endl;
	cout << "Version: 0x" << hex << this->version << endl;
	cout << "Packet ID: " << this->pID << endl;

	if (this->pForm == 0)
		cout << "Packet Format: 0x00" << endl;
	else
		cout << "Packet Format: 0x" << hex << this->pForm << endl;

	cout << "Packet Size: " << this->pktSize << endl;
	cout << "Context: 0x" << hex  << this->context << endl;
	cout << "Number of Modules: " << (int) this->numMods << endl;
	cout << "==================RTTrP Packet Header==================" << endl;
}

RTTrPM::RTTrPM()
{
	this->trackable = NULL;
	this->centroidMod = NULL;
	this->ledMod = NULL;
	this->quatMod = NULL;
	this->eulerMod = NULL;
	this->cavMod = NULL;
	this->lavMod = NULL;
    this->zoneMod = NULL;
    this->zoneSubMod = NULL;
}

RTTrPM::~RTTrPM()
{
	delete(this->trackable);
	delete(this->centroidMod);
	delete(this->quatMod);
	delete(this->eulerMod);
	delete(this->cavMod);
    delete(this->zoneMod);

	if (this->ledMod != NULL)
	{
		if (this->ledMod->size() > 0)
		{
			vector<LEDModule*>::iterator led;

			for (led = this->ledMod->begin(); led < this->ledMod->end(); led++)
			{
				delete((*led));
			}

			delete(this->ledMod);
		}
		else
			delete(this->ledMod);
	}
	else
		delete(this->ledMod);

	if (this->lavMod != NULL)
	{
		if (this->lavMod->size() > 0)
		{
			vector<LEDAccVelMod*>::iterator lav;

			for (lav = this->lavMod->begin(); lav < this->lavMod->end(); lav++)
			{
				delete((*lav));
			}

			delete(this->lavMod);
		}
		else
			delete(this->lavMod);
	}
	else
		delete(this->lavMod);

    if (this->zoneSubMod != NULL)
    {
        if (this->zoneSubMod->size() > 0)
        {
            vector<ZoneSubMod*>::iterator zoneSubIter;

            for (zoneSubIter = this->zoneSubMod->begin(); zoneSubIter < this->zoneSubMod->end(); zoneSubIter++)
            {
                delete((*zoneSubIter));
            }

            delete(this->zoneSubMod);
        }
        else
            delete(this->zoneSubMod);
    }
    else
        delete(this->zoneSubMod);
}

void RTTrPM::printPacket(void)
{
	this->header->printHeader();
	this->trackable->printModule();

	if (this->centroidMod != NULL)
		this->centroidMod->printModule();

	if (this->ledMod != NULL)
		print_vector(this->ledMod);
	
	if (this->quatMod != NULL)
		this->quatMod->printModule();

	if (this->eulerMod != NULL)
		this->eulerMod->printModule();

	if (this->cavMod != NULL)
		this->cavMod->printModule();

	if (this->lavMod != NULL)
		print_vector(this->lavMod);

    if (this->zoneMod != NULL)
        this->zoneMod->printModule();

    if (this->zoneSubMod != NULL)
        print_vector(this->zoneSubMod);
}

RTTrPL::RTTrPL()
{
	this->header = NULL;
	this->lightOut = NULL;
	this->sync = NULL;
}

RTTrPL::~RTTrPL()
{
	delete(this->header);
	delete(this->lightOut);
	delete(this->sync);
}

void RTTrPL::printModule()
{
	this->header->printHeader();
	this->lightOut->printModule();
	
	vector<Universe*>::iterator univIt;
	vector<Spot*>::iterator spotIt;

	for (univIt = this->lightOut->uniList->begin(); univIt != this->lightOut->uniList->end(); univIt++)
	{
		(*univIt)->printModule();

		for (spotIt = (*univIt)->spotList->begin(); spotIt != (*univIt)->spotList->end(); spotIt++)
		{
			(*spotIt)->printModule();
			
			print_vector((*spotIt)->chanBlocks);
		}
	}
}
