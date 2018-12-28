#include "MCEvent.h"
#include "stdafx.h"

#include "windows.h"
#include "time.h"
//#include "MCVechicle.h"
#include <string>
#include <iostream>
#include "dirWorks.h"



MCEvent::MCEvent()
{
	//basicData Structure Setup
	//wmemset(bD.parentName, L'\0', 32);
	bD.eventVersion = 1;
	bD.eventID = 0;
	time_t begin_time = 0;
	//time(&begin_time);
	localtime_s(&bD.eventDate,&begin_time);
	localtime_s(&bD.eventCreationDate, &begin_time);
	bD.evStatus = basicData::eventStatus::Scheduled;
	wmemset(bD.eventCommentaryArr, L'\0', MAX_PATH);
	wmemset(bD.fullPath, L'\0', MAX_PATH);
	//bD.additionalDataStruct = true;

	//additionalData Structure Setup
	wmemset(bD.licensePlate, L'\0', 11);
	bD.eventType = 0;
	bD.eventDuration = 0;
}


MCEvent::~MCEvent()
{
}

/*
int MCEvent::setEventConsole(size_t &lastEventID)
{
	wprintf(L"\n MCEvent\\setEventConsole");
	wprintf(L"\n -------------------------------------------------------------");
	wprintf(L"\n | Setting new event:                                        |");

	wprintf(L"\n | LicensePlate: %s", bD.licensePlate);
	return 0;
}
*/
int MCEvent::saveToFile(std::wstring &path) //to do
{
	errno_t Err = 0;
	//size_t test_size = sizeof(bD);
	//wprintf(L"sizeof bD: %d", test_size);
	//test_size = sizeof(this);
	//wprintf(L"sizeof this: %d", test_size);
	Err = dirWorks::makeFile(path);
	if (Err == ERROR_SUCCESS)
	{
		FILE * fp;
		Err = _wfopen_s(&fp, path.c_str(), L"wb");
		if (Err == ERROR_SUCCESS)
		{
			wmemcpy_s(bD.fullPath, path.length(), path.c_str(), path.length());
			fwrite(&bD, sizeof(bD), 1, fp);
			fclose(fp);
			
			return 0;
		}
		else
		{
			return Err;
		}
	}
	else
	{
		return Err;
	}
	//addEventConsole  creates path

	//FILE *fp;
	//std::wstring fullName;
	//std::wstring eventIDString;

	//size_t parentNameSize = wcslen(bD.parentName);
	//if (parentNameSize > 32) parentNameSize = 32;
	//parentNameString.assign(bD.parentName);
	//fullName = path + L"\\" + bD.licensePlate + L"_" + std::to_wstring(bD.eventID) + L".ev";


	//eventIDString = std::to_wstring(bD.eventID);
	//filename = parentNameString + L"_" + eventIDString + L".ev";
	//wprintf(L"\n FILENAME: %s", fullName.c_str());

	return 0;
}

int MCEvent::loadFromFile(std::wstring &path)
{
	errno_t Err = 0;
	//size_t test_size = sizeof(bD);
	//wprintf(L"sizeof bD: %d", test_size);
	//test_size = sizeof(this);
	//wprintf(L"sizeof this: %d", test_size);
	//Err = dirWorks::makeFile(path);

		FILE * fp;
		Err = _wfopen_s(&fp, path.c_str(), L"rb");
		if (Err == ERROR_SUCCESS)
		{
			fread(&bD, sizeof(bD), 1, fp);
			fclose(fp);
			return 0;
		}
		else
		{
			return Err;
		}
		return 0;
}
	
	

