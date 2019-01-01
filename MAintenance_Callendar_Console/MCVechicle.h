#pragma once

#ifndef __MCVECHICLE_H__
#define __MCVECHICLE_H__

#define MAX_PATH 260
#define LICENSEPLATE_SIZE 11

#include <string>
#include <vector>
#include "MCEvent.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

struct baseDataS
{
	wchar_t licensePlate[LICENSEPLATE_SIZE]; //must be null terminated
	size_t weight; //kilograms
	size_t actualMileage; //Km
	tm techExamDate; //date
	//boost::gregorian::date techExamDateB;
	boost::posix_time::ptime techExamTimeB;
	size_t techExamMileage;
	size_t lastEventID;
	wchar_t fullPath[MAX_PATH];
	//DWORD vechicleType; //used to determine what kind of vechicle is
						//fortesting purposes
};

class MCVechicle
{
public: //methods
	MCVechicle();// after creation, fill with default values
	~MCVechicle();
	int firstSetupConsole();
	void showValues();
	int saveToFile(std::wstring &path);
	int loadFromFile(std::wstring &path);
	int getEvents(std::wstring &path, std::vector<MCEvent> &events_v);
	int addEventConsole();

	//int getLastEventID();

public: //variables
	baseDataS vechicleBasicDataS;
};

#endif

