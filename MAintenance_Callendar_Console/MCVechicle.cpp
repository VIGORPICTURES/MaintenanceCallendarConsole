#include "MCVechicle.h"
#include "stdafx.h"
#include "time.h"
#include <iostream>
#include <vector>
#include <string>
#include "dirWorks.h"
#include "MCEvent.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
//#include <limits>

namespace boostDate = boost::gregorian;
namespace boostTime = boost::posix_time;

extern void errorBoxConsole(std::wstring title, std::wstring msg);

extern std::wstring dateToString(tm &date);

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	
}

MCVechicle::MCVechicle()
{
	wmemset(vechicleBasicDataS.licensePlate, '\0', LICENSEPLATE_SIZE);
	vechicleBasicDataS.weight			= 0;
	vechicleBasicDataS.actualMileage = 0;
	//------------------------------------old data gathering
	memset(&vechicleBasicDataS.techExamDate,0,sizeof(vechicleBasicDataS.techExamDate));
	//------------------------------------/old data gathering
	//vechicleBasicDataS.techExamDateB = boostDate::day_clock::local_day();
	vechicleBasicDataS.techExamTimeB = boostTime::second_clock::local_time();
	vechicleBasicDataS.techExamMileage = 0;
	vechicleBasicDataS.lastEventID = 0;
	memset(vechicleBasicDataS.fullPath, L'\0', MAX_PATH);
	//vechicleBasicDataS.vechicleType = 0; //currently not in use
	//later - check if memset for entire struct is possible

}


MCVechicle::~MCVechicle()
{
}

int MCVechicle::firstSetupConsole()
{
	wprintf(L"\n-------------------------------------------------------");
	wprintf(L"\n| Adding new Vechicle.                                |");
	wprintf(L"\n-------------------------------------------------------");
	wprintf(L"\n| Please provide following data:                      |");
	wprintf(L"\n| LicensePlate [max 10 characters]:  ");
	std::wstring templicenseplate;
	std::wcin.ignore();
	std::getline(std::wcin, templicenseplate);
	size_t templicenseplate_size = templicenseplate.length();
	
	if (templicenseplate_size > 10)
	{
		templicenseplate.resize(LICENSEPLATE_SIZE);
		templicenseplate[LICENSEPLATE_SIZE-1] = L'\0';
		templicenseplate_size = templicenseplate.length();
	}
	
	wmemcpy_s(vechicleBasicDataS.licensePlate, templicenseplate.length(), templicenseplate.c_str(), templicenseplate_size);
	wprintf(L"\n| Weight [kg]:                       ");
	std::cin >> vechicleBasicDataS.weight;
	wprintf(L"\n| Actual Mileage [km]:               ");
	std::cin >> vechicleBasicDataS.actualMileage;
	wprintf(L"\n| Next tech exam [km]:               ");
	std::cin >> vechicleBasicDataS.techExamMileage;
	//------------------------------------old data gathering
	/*
	wprintf(L"\n| Next tech exam [date](DD/MM/RRRR): ");
		
	std::wstring tempdate;
	std::wcin.ignore();
	std::getline(std::wcin, tempdate);
	std::wstring tempstr;
	
	size_t it_pos1 = 0;
	size_t it_pos2 = 0;
	it_pos1 = tempdate.find(L'/');

	tempstr.assign(tempdate.begin(), tempdate.begin() + it_pos1); //days
	vechicleBasicDataS.techExamDate.tm_mday = stoi(tempstr);
	it_pos1++;
	it_pos2 = tempdate.find(L'/', it_pos1);
	tempstr.assign(tempdate.begin()+it_pos1, tempdate.begin() + it_pos2); //month
	vechicleBasicDataS.techExamDate.tm_mon = stoi(tempstr);
	vechicleBasicDataS.techExamDate.tm_mon = vechicleBasicDataS.techExamDate.tm_mon - 1;
	it_pos2++;
	tempstr.assign(tempdate.begin() + it_pos2, tempdate.end()); //year
	vechicleBasicDataS.techExamDate.tm_year = (stoi(tempstr) - 1900);
	*/
	//------------------------------------/old data gathering
	wprintf(L"\n| Next tech exam [date](RRRR/MM/DD): ");
	std::string tempdateB;
	std::wcin.ignore();
	std::getline(std::cin, tempdateB);
	//vechicleBasicDataS.techExamDateB = boost::gregorian::from_string(tempdateB);
	//boost ptime from std string without hours and minutes
	tempdateB += " 00:00:00.000";
	vechicleBasicDataS.techExamTimeB = boostTime::time_from_string(tempdateB);

	/*
	tempstr.assign(tempdate.begin(), tempdate.begin() + it_pos1); //days
	vechicleBasicDataS.techExamDate.tm_mday = stoi(tempstr);
	it_pos1++;

	tempstr.assign(tempdate.begin()+3, tempdate.begin() + 5); //month
	vechicleBasicDataS.techExamDate.tm_mon = stoi(tempstr);
	vechicleBasicDataS.techExamDate.tm_mon = vechicleBasicDataS.techExamDate.tm_mon - 1;
	tempstr.assign(tempdate.begin() + 6, tempdate.begin() + 10); //year
	vechicleBasicDataS.techExamDate.tm_year = (stoi(tempstr) - 1900);
	*/
	//place for creating first event - Creation of Vechicle
	showValues();
	return 0;
}

void MCVechicle::showValues()
{
	wprintf(L"\n //-----------showing MCVechicle basic data-----------//");
	wprintf(L"\n licensePlate: ");
	for (int i = 0; i < 10; i++)
	{
		std::wcout<< vechicleBasicDataS.licensePlate[i];
	}
	wprintf(L"\n weight: %d", vechicleBasicDataS.weight);
	wprintf(L"\n actualMileage: %d", vechicleBasicDataS.actualMileage);
	//------------------------------------old data show
	/*
	wprintf(L"\n techExamDate: %d / %d / %d / %d / %d / %d",
		vechicleBasicDataS.techExamDate.tm_year,
		vechicleBasicDataS.techExamDate.tm_mon,
		vechicleBasicDataS.techExamDate.tm_mday,
		vechicleBasicDataS.techExamDate.tm_hour,
		vechicleBasicDataS.techExamDate.tm_min,
		vechicleBasicDataS.techExamDate.tm_sec);
	*/
	//------------------------------------/old data show
	
	//std::string tempStr(boostDate::to_iso_extended_string(vechicleBasicDataS.techExamDateB));
	std::string tempStr = boostTime::to_simple_string(vechicleBasicDataS.techExamTimeB);
	tempStr.resize(20);
	printf("\n techExamDate: %s", tempStr.c_str());

	//printf("\n techExamDate: %s", std::string(boost::gregorian::to_iso_extended_string(vechicleBasicDataS.techExamDateB)).c_str());


	wprintf(L"\n techExamMileage: %d", vechicleBasicDataS.techExamMileage);
	wprintf(L"\n lastEventID: %d",vechicleBasicDataS.lastEventID);
	//std::cout << "\n vechicleType: " << vechicleBasicDataS.vechicleType; //currently not in use
	wprintf(L"\n //-----------/showing MCVechicle basic data-----------//");

}

int MCVechicle::saveToFile(std::wstring &path)
{
	errno_t err = 0;
	//size_t licensePlate_length = 0;
	//licensePlate_length = wcslen(vechicleBasicDataS.licensePlate);
	std::wstring fullName = path + L"\\" + vechicleBasicDataS.licensePlate + L".mcv";
	err = wmemcpy_s(vechicleBasicDataS.fullPath, fullName.length(), fullName.c_str(), fullName.length());

	//wchar_t extensionArr[5] = { L'.',L'M',L'C','V','\0'};
	//licensePlate_length += 5;
	//wchar_t * filename = 0;
	//filename = new wchar_t[licensePlate_length];
	//err = wmemcpy_s(filename, licensePlate_length, vechicleBasicDataS.licensePlate, (licensePlate_length - 4));
	//err = wmemcpy_s(filename + (licensePlate_length - 4), licensePlate_length, extensionArr, 4);
	FILE * fp;
	err = _wfopen_s(&fp, fullName.c_str(), L"wb");
	if (err == ERROR_SUCCESS)
	{
		fwrite(&vechicleBasicDataS, sizeof(vechicleBasicDataS), 1, fp);
		fclose(fp);
		return 0;
	}
	else
	{
		//wprintf(L"\n Error, could not open file for writing");
		SetLastError(ERROR_INVALID_FUNCTION);
		return ERROR_INVALID_FUNCTION;
	}
}

int MCVechicle::loadFromFile(std::wstring &path)
{
	errno_t Err;
	FILE * fp;
	Err = _wfopen_s(&fp, path.c_str(), L"rb");
	if (Err == ERROR_SUCCESS)
	{
		fread(&vechicleBasicDataS, sizeof(vechicleBasicDataS), 1, fp);
		fclose(fp);
		return 0;
	}
	else
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return ERROR_INVALID_FUNCTION;
	}
}

int MCVechicle::getEvents(std::wstring &path, std::vector<MCEvent> &events_v)
{
	dirData_v eventsPath_v;
	size_t foundMatches = 0;
	foundMatches = dirWorks::searchFile(path, eventsPath_v, L"*ev");
	if (foundMatches)
	{
		MCEvent temp_event;
		for (dirData_v::iterator it = eventsPath_v.begin(); it != eventsPath_v.end(); it++)
		{
			wprintf(L"\ntodo");
			temp_event.loadFromFile(it->item_path);
			events_v.push_back(temp_event);
		}
	}
	return 0;
}

int MCVechicle::addEventConsole()
{
	errno_t Err = 0;
	//L"D:\\T13\\WE634LE\\WE634LE.mcv">

	//------------------------------prepare path for saving event-------------------------------------------------
	size_t event_no = 0;
	dirData_v temp_dirData;
	std::wstring eventPath = dirWorks::fullPathDivider(vechicleBasicDataS.fullPath, RETURN_PATH);
	event_no = dirWorks::searchFile(eventPath, temp_dirData, L"*ev");
	event_no++;
	//dirWorks::check how many files with .ev is. Then create last part of name :  LICENSEPLATE_NEXT_EVENT_NUMBER.ev
	eventPath = eventPath + L"\\events\\" + vechicleBasicDataS.licensePlate + L"_" + std::to_wstring(event_no) + L".ev";


	//------------------------------create 'n fill event object-------------------------------------------------
	MCEvent tempEvent;
	int user_choice = 0;
	{
		
			system("cls");
			tempEvent.bD.eventVersion = 1;
			wprintf(L"\n MCEvent\\setEventConsole");
			wprintf(L"\n -------------------------------------------------------------");
			wprintf(L"\n | Setting new event:                                        |");
			wmemcpy_s(tempEvent.bD.licensePlate, LICENSEPLATE_SIZE, vechicleBasicDataS.licensePlate, LICENSEPLATE_SIZE);
			wprintf(L"\n | LicensePlate: %s", tempEvent.bD.licensePlate);
			tempEvent.bD.eventID = event_no;
			wprintf(L"\n | eventID: %d", tempEvent.bD.eventID);
			do
			{
			wprintf(L"\n | Please choose eventStatus:                                |");
			wprintf(L"\n | [0] - Scheduled.                                          |");
			wprintf(L"\n | [1] - Started.                                            |");
			wprintf(L"\n | [2] - Processed.                                          |");
			wprintf(L"\n | [3] - Finished.                                           |");
			wprintf(L"\n | [4] - Current.                                            |");
			wprintf(L"\n | [5] - Locked.                                             |");
			wprintf(L"\n | [6] - Missed.                                             |");
			wprintf(L"\n | [7] - Important.                                          |");
			wprintf(L"\n | [8] - Commentary.                                         |");
			wprintf(L"\n -------------------------------------------------------------");
			wprintf(L"\n | Enter number corresponding desired eventStatus: ");
			std::wcin >> user_choice;
			//std::wcin.sync();
			//std::wcin.clear();
			std::wcin.ignore();
			if (user_choice >= 0 || user_choice <= 8)
				break;
			else
			{
				errorBoxConsole(L"dirWorks\\listFilesConsole", L"Error. Invalid choice. Try again.");
			}
			} while (true);
			tempEvent.bD.evStatus = static_cast<basicData::eventStatus> (user_choice);

			(GetAsyncKeyState(VK_RETURN) & 0x8000);
			Sleep(100);
			
			//------------------------------fill time-----------------------------
			time_t nowTime = time(0); //now
			time(&nowTime);
			tm tempTime;
			tm * tempTimePTR = &tempTime;

			int val_arr[5] = { 3600, 60, 86400, 2592000, 31536000 };
			int * valPTR = &val_arr[0];

			int current_val = 0;
			do
			{
				//nowTime = time(0);
				localtime_s(tempTimePTR, &nowTime);

				//test.eventDate = *test_tm;

				system("cls");
				wprintf(L"\n -------------------------------------------------------------");
				wprintf(L"\n |                                                           |");
				wprintf(L"\n |       Event date:                                         |");
				wprintf(L"\n |                                                           |");
				wprintf(L"\n |       %02d:%02d %02d/%02d/%02d                             ",
					tempTime.tm_hour, tempTime.tm_min, tempTime.tm_mday,
					tempTime.tm_mon + 1, tempTime.tm_year + 1900);
				wprintf(L"\n |                                                           |");
				wprintf(L"\n |                                                           |");
				wprintf(L"\n -------------------------------------------------------------");
				wprintf(L"\n  [Arrows] to change, [enter] to confirm.                    |");
				//wprintf(L"\n timetics: %d", nowTime);
				gotoxy(62, 6);
				wprintf(L"|");

				switch (current_val)
				{
				case 0:
					gotoxy(10, 5);
					wprintf(L"++");
					gotoxy(10, 7);
					wprintf(L"--");
					break;
				case 1:
					gotoxy(13, 5);
					wprintf(L"++");
					gotoxy(13, 7);
					wprintf(L"--");
					break;
				case 2:
					gotoxy(16, 5);
					wprintf(L"++");
					gotoxy(16, 7);
					wprintf(L"--");
					break;
				case 3:
					gotoxy(19, 5);
					wprintf(L"++");
					gotoxy(19, 7);
					wprintf(L"--");
					break;
				case 4:
					gotoxy(22, 5);
					wprintf(L"++++");
					gotoxy(22, 7);
					wprintf(L"----");
					break;
				}
				Sleep(100);

				if (GetAsyncKeyState(VK_UP))
				{
					nowTime = nowTime + val_arr[current_val];
				}
				else if (GetAsyncKeyState(VK_DOWN))
				{
					nowTime = nowTime - val_arr[current_val];
				}
				else if (GetAsyncKeyState(VK_LEFT))
				{
					current_val--;
					if (current_val <= -1) current_val = 4;
				}
				else if (GetAsyncKeyState(VK_RIGHT))
				{
					current_val++;
					if (current_val > 4) current_val = 0;
					FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				}
				else if (GetAsyncKeyState(VK_RETURN))
				{
					tempEvent.bD.eventDate = tempTime;
					Sleep(100);
					break;
				}
				else
				{
					Sleep(10);
				}

			} while (true);

			//-----------------commentary arr-------------------------------------
			Sleep(10);
			//(GetAsyncKeyState(VK_RETURN) & 0x8000);
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
			Sleep(100);
			wprintf(L"\n -------------------------------------------------------------");
			wprintf(L"\n |                                                           |");
			wprintf(L"\n |     Please type in comment (if necessary):                |");
			wprintf(L"\n |     ");
			std::wstring commentString;
			//std::wcin.sync();
			//std::wcin.ignore();
			std::getline(std::wcin, commentString);
			
			commentString.resize(MAX_PATH);
			wmemcpy_s(tempEvent.bD.eventCommentaryArr, MAX_PATH, commentString.c_str(), MAX_PATH);
			wprintf(L"\n |                                                           |");
			wprintf(L"\n -------------------------------------------------------------");
			wprintf(L"\n |                                                           |");
			wprintf(L"\n |     Please type in event duration (hours):                |");
			wprintf(L"\n |   ->");
			(GetAsyncKeyState(VK_RETURN) & 0x8000);
			//std::wcin.ignore();
			wscanf_s(L"%d", &tempEvent.bD.eventDuration);
			wprintf(L"\n |                                                           |");
			wprintf(L"\n -------------------------------------------------------------");
			
			//---------------------------set eventType-------------------------------
			// currently not used. Maybe switch to enum, with service, check etc

			//---------------------save to file---------------------------------------
			Err = tempEvent.saveToFile(eventPath);
			if (Err == ERROR_SUCCESS)
			{
				return 0;
			}
			else
			{
				return Err;
			}
		
	}


	//------------------------------save event (and check if lasteventID is functional)------------------------------



	return 0;
}