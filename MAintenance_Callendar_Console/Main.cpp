#include <iostream>
#include "windows.h"
#include "time.h"
#include "MCEvent.h"
#include "MCVechicle.h"
#include <vector>
#include <fstream>
#include "CfgFile.h"
#include "CfgFileWString.h"
//#include "CfgFileWString.cpp"
#include <vector>
#include "MCDataBase.h"
#include "dirWorks.h"
#include "setList.h"

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

//boost test


//change menu for class menu

std::wstring dateToString(tm &date)
{
	std::wstring retVal;
	retVal += std::to_wstring(date.tm_min);
	retVal += L":";
	retVal += std::to_wstring(date.tm_hour);
	retVal += L" ";
	retVal += std::to_wstring(date.tm_mday);
	retVal += L"/";
	retVal += std::to_wstring(date.tm_mon);
	retVal += L"/";
	retVal += std::to_wstring(date.tm_year);
	return retVal;
}

void errorBoxConsole(std::wstring title, std::wstring msg)
{
	system("cls");
	wprintf(L"\n %s", title.c_str());
	wprintf(L"\n -------------------------------------------------------------");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |           %s",msg.c_str());
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n -------------------------------------------------------------\n");
	//system("pause");
	Sleep(3000);
}

int mainmenu(MCDataBase & actualDataBase)
{
	int user_choice = 0;
	while (true)
	{
		system("cls");
		wprintf(L"\n MAINTENANCE CALLENDAR CONSOLE");
		wprintf(L"\n -------------------------------------------------------------");
		//wprintf(L"\n | DataBase loaded: %s\\%s", actualDataBase.bD.path, actualDataBase.bD.name );
		//wprintf(L"\n | Vechicle loaded: %s", actualDataBase.currentVechicle.vechicleBasicDataS.fullPath);
		wprintf(L"\n | DataBase loaded: %s", actualDataBase.bD.name);
		wprintf(L"\n | Vechicle loaded: %s", actualDataBase.currentVechicle.vechicleBasicDataS.licensePlate);
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n | 1. For adding new Vechicle to dataBase.                   |");
		wprintf(L"\n | 2. For choising Vechicle to work with.                    |");
		wprintf(L"\n | 3. For adding new event to chosen Vechicle .              |");
		wprintf(L"\n | 4. For listing all events for Vechicle.                   |");
		wprintf(L"\n | 5. For listing recent events past and future.             |");
		wprintf(L"\n | 6. For SETTINGS.                                          |");
		wprintf(L"\n | 7. For adding new Database.                               |");
		wprintf(L"\n | 8. For choosing Database.                                 |");
		wprintf(L"\n | 9. DEBUG.                                                 |");
		wprintf(L"\n | 0. For EXIT.                                              |");
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n YOUR CHOICE: ");
		std::wcin >> user_choice;
		
		//wscanf_s(L"%d", &user_choice);
		if (user_choice >= 0 && user_choice <= 9)
			return user_choice;
		else
		{
			errorBoxConsole(L"MAINTENANCE CALLENDAR CONSOLE", L"ERROR. Choice not valid. Try again.");
		}
	}
}

std::wstring AddNewDataBase(std::wstring dataBasePath = L"") //0 for internal ask for directory
{
	MCDataBase dB_temp;
	std::wstring dataBaseName = L"";
	if (dataBasePath == L"")
	{
		wprintf(L"\n MAINTENANCE CALLENDAR CONSOLE -> Add New Database");
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n |                                                           |");
		wprintf(L"\n |     Please provide directory path for New Database:       |");
		wprintf(L"\n |     (Without Polish letters)                              |");
		wprintf(L"\n |   ->");
		std::wcin.ignore();
		std::getline(std::wcin, dataBasePath);
		wprintf(L" |                                                           |");
		wprintf(L"\n |     Please provide Name for New Database:                 |");
		wprintf(L"\n |     (Without Polish letters)                              |");
		wprintf(L"\n |   ->");
		std::getline(std::wcin, dataBaseName);
		dB_temp.setName(dataBaseName);
		wprintf(L" |                                                           |");
		wprintf(L"\n -------------------------------------------------------------\n");
		//std::wcout << L"\n dataBaseName: " << dataBaseName;
		system("pause");
	
		wprintf(L"\n MAINTENANCE CALLENDAR CONSOLE -> Add New Database");
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n |                                                           |");
		wprintf(L"\n |                                                           |");
		wprintf(L"\n |        Creating Data Base at:                             |");
		wprintf(L"\n |      ->%s", dataBasePath.c_str());
		if (dirWorks::makeDir(dataBasePath) == ERROR_SUCCESS)
		{
		wprintf(L"\n |        *Directory Created Successfuly.                    |");
		dB_temp.setPath(dataBasePath);
		}
		else
		{
		wprintf(L"\n |        ERROR. Code of Error: %d |",GetLastError());
		}
		wprintf(L"\n |                                                           |");
		if (dB_temp.createFile() == ERROR_SUCCESS)
		{
		wprintf(L"\n |        *Data Base file Created Successfully.              |");
		}
		else
		{
		wprintf(L"\n |        ERROR. Code of Error: %d |", GetLastError());
		}
		wprintf(L"\n |                                                           |");
		wprintf(L"\n -------------------------------------------------------------");
		//wprintf(L"\n Sizeof(MCDataBase): %d |",sizeof(dB_temp));
		system("pause");
		std::wstring retVal(dB_temp.bD.path);
		retVal.append(L"\\");
		retVal.append(dB_temp.bD.name);
		retVal.append(L".dB");
		return retVal;
	}
	else
	{
		if (dirWorks::makeFile(dataBasePath)==ERROR_SUCCESS)
		{
			dB_temp.setPath(dirWorks::fullPathDivider(dataBasePath, RETURN_PATH));
			dB_temp.setName(dirWorks::fullPathDivider(dataBasePath, RETURN_FILENAME));
		}
	}
	return 0;
	}

inline void chooseDataBaseConsole(MCDataBase & dB, errno_t &Err, setList &LastUsedDatabaseList)
{
	std::wstring user_path;
	std::wstring choosed_path;
	wprintf(L"\n MAINTENANCE CALLENDAR CONSOLE -> Choose Database");
	wprintf(L"\n -------------------------------------------------------------");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |        Please provide path for search:                    |");
	wprintf(L"\n |      ->");
	std::wcin.ignore();
	std::getline(std::wcin, user_path);
	wprintf(L"\n |                                                           |");
	wprintf(L"\n -------------------------------------------------------------");

	choosed_path = dirWorks::BrowseFFConsole(user_path, L"*dB", true);
	Err = dB.loadFromFile(choosed_path);
	if (Err == ERROR_SUCCESS)
	{
		
		LastUsedDatabaseList.insertEntryHybrid(choosed_path);
		LastUsedDatabaseList.setMostRecent(choosed_path);
		//userPrefsIni.saveVal(L"wsLastUsedDataBase", lastUsedDataBase);
		//old in case of failre
		/*
		lastUsedDataBase = choosed_path;
		LastUsedDatabaseList.insertEntryHybrid(lastUsedDataBase);
		LastUsedDatabaseList.setMostRecent(lastUsedDataBase);
		//userPrefsIni.saveVal(L"wsLastUsedDataBase", lastUsedDataBase);
		*/
	}
	//todo


	/*
	1. Check lastUsed.ini for saved databases.
		- if found, display in console (with return option) and:
			* add find dataBase option wchich promts user for entering path to search (folder) or path to specific file
		- if not found, promt for entering folder path or specific file (as above).
	2. After choosing dataBase check if path is present in lastUsed (lastUsed stores paths to specyfic files, not folders)
		- if not, add new path and update ,
		- if so, do nothing.
	3*. Add variables how many times each dataBase was used. Not shure in wchich file. Maybe trashData?
	*/
	//return 0;
}

int addNewVechicleConsole(MCDataBase & dB)
{
	errno_t Err = 0;
	if (dB.checkIfValid())
	{
		dB.currentVechicle.firstSetupConsole();
		std::wstring tempPath;
		tempPath.assign(dB.bD.path);
		tempPath += L"\\";
		tempPath += dB.currentVechicle.vechicleBasicDataS.licensePlate;
		Err = dirWorks::makeDir(tempPath);
		if (Err == ERROR_SUCCESS)
		{
			Err = dB.currentVechicle.saveToFile(tempPath);
		}
		else
		{
			//can't save file.
			return Err;
		}


		//after creating vechicle: set and create first event
		size_t event_no = 0;
		dirData_v temp_dirData;
		std::wstring eventPath = dirWorks::fullPathDivider(dB.currentVechicle.vechicleBasicDataS.fullPath, RETURN_PATH);
		event_no = dirWorks::searchFile(eventPath, temp_dirData, L"*ev");
		event_no++;
		//dirWorks::check how many files with .ev is. Then create last part of name :  LICENSEPLATE_NEXT_EVENT_NUMBER.ev
		eventPath = eventPath + L"\\events\\" + dB.currentVechicle.vechicleBasicDataS.licensePlate + L"_" + std::to_wstring(event_no) + L".ev";

		MCEvent firstEvent;

		firstEvent.bD.eventVersion = 1;
		wmemcpy_s(firstEvent.bD.licensePlate, LICENSEPLATE_SIZE, dB.currentVechicle.vechicleBasicDataS.licensePlate, LICENSEPLATE_SIZE);
		firstEvent.bD.eventID = 0;
		firstEvent.bD.evStatus = basicData::eventStatus::Commentary;
		std::wstring  temp_str = L"FIRST EVENT. Vechicle created.";
		wmemcpy_s(firstEvent.bD.eventCommentaryArr, temp_str.length(), temp_str.c_str(), temp_str.length());

		time_t nowTime = time(0); //now
		time(&nowTime);
		tm tempTime;
		tm * tempTimePTR = &tempTime;
		localtime_s(tempTimePTR, &nowTime);
		firstEvent.bD.eventDate = tempTime;
		firstEvent.bD.eventDuration = 1;

		firstEvent.saveToFile(eventPath);

		//-----------------------------------load new added vechicle ?
		//dB.currentVechicle.loadFromFile()

		return 0;
	}
	else
	{
		//dataBase Invalid (empty), maybe not loaded.
		
		Err = ERROR_INVALID_FUNCTION;
		return Err;
	}

}

int listEventsConsole(std::vector <MCEvent> &Events_v)
{
	std::vector<MCEvent>::iterator it = Events_v.begin();
	std::wstring enumName;

	wprintf(L"\n MAINTENANCE CALLENDAR CONSOLE -> listEventsConsole");
	wprintf(L"\n -------------------------------------------------------------");
	wprintf(L"\n |                                                           |");
	wprintf(L"\n |             Listing Events:                               |");
	for (it = Events_v.begin(); it != Events_v.end(); it++)
	{
		switch (it->bD.evStatus)
		{
		case 0:
			enumName = L"Scheduled";
			break;
		case 1:
			enumName = L"Started";
			break;
		case 2:
			enumName = L"Processed";
			break;
		case 3:
			enumName = L"Finished";
			break;
		case 4:
			enumName = L"Current";
			break;
		case 5:
			enumName = L"Locked";
			break;
		case 6:
			enumName = L"Missed";
			break;
		case 7:
			enumName = L"Important";
			break;
		case 8:
			enumName = L"Commentary";
			break;
		}
	wprintf(L"\n | LicensePlate: %s / EventID: %d / EventStatus: %s / EventType: %d  / EventCommentary: %s", it->bD.licensePlate, it->bD.eventID, enumName.c_str(), it->bD.eventType,  it->bD.eventCommentaryArr);
	}
	wprintf(L"\n |                                                           |");
	wprintf(L"\n -------------------------------------------------------------");
	return 0;
}

int main()
{
	errno_t Err = 0;
	//bool DataBaseChoosed = false;
	//bool VechicleChoosed = false;
	//bool AddingEventPossible = DataBaseChoosed && VechicleChoosed;
	
	MCDataBase actualDataBase;
	//MCVechicle actualVechicle;
	std::vector<MCEvent> actualVechicleEventsVector; //maybe obsolete
	std::vector<MCEvent> dataBaseEventsVector;

	
	std::wstring userPrefsPath;
	userPrefsPath = dirWorks::getExePath();
	userPrefsPath += L"\\userPrefs.ini";
	CfgFileWString userPrefsIni(userPrefsPath);
	
	//userPrefsIni.search(L"wsLastUsedDataBase", lastUsedDataBase);
	
	setList LastUsedDatabaseList;
	//std::wstring lastUsedDataBase;
	std::wstring lastUsedDataBasePath = dirWorks::getExePath() + L"\\LastUsed.txt";
	LastUsedDatabaseList.loadFromFileW(lastUsedDataBasePath);

	//lastUsedDataBase = LastUsedDatabaseList.getMostRecent();
	//lastUsedDataBase = LastUsedDatabaseList.getMostRecent(); //old in case off ailure


	if (LastUsedDatabaseList.getMostRecent().compare(std::wstring(L"NONE")) == 0)
		//if (lastUsedDataBase.compare(std::wstring(L"NONE")) == 0) //old in case of failure
	{
		wprintf(L"No last used Database. Search for existing dataBases or add new one.");
		//no Last Used dataBase. Propably first start of program. Maybe ask for start searching dataBases or create new one.
		//nothing to do.
	}
	else
	{
		wprintf(L"LastUsedDataBase: %s", LastUsedDatabaseList.getMostRecent().c_str());
		Err = actualDataBase.loadFromFile(LastUsedDatabaseList.getMostRecent());
		//if (Err == ERROR_SUCCESS) DataBaseChoosed = true;
	}
	Sleep(1000);
	

	int user_choice =  mainmenu(actualDataBase);
	while(user_choice != 0)
	{
	switch (user_choice)
	{
	case 1: //add new vechicle
		if (actualDataBase.dataBaseValid)
		{
			Err = addNewVechicleConsole(actualDataBase);
			actualDataBase.vechicleValid = true;
		}
		else
		{
			system("cls");

		}
		break;

	case 2: //choose vechicle to work with
	{
		size_t foundvechicles = actualDataBase.searchForVechicles();
	}
		break;

	case 3: //add new event to vechicle
		if (actualDataBase.vechicleValid)
		{
			actualDataBase.currentVechicle.addEventConsole();
		}
		else
		{
			errorBoxConsole(L"MAINTENANCE CALLENDAR CONSOLE -> Add New Event", L"Error. No Vechicle choosed");
		}
		break;

	case 4: //list all events for vechicle
	{
		std::wstring eventsPath = dirWorks::fullPathDivider(actualDataBase.currentVechicle.vechicleBasicDataS.fullPath, RETURN_PATH);
		eventsPath += L"\\events";
		actualDataBase.currentVechicle.getEvents(eventsPath, actualVechicleEventsVector);
		listEventsConsole(actualVechicleEventsVector);
	}		//todo
		break;

	case 5: //show system time and most recent events (past and future)
		//todo
		break;

	case 6: //program settings (cfg file)
		//todo
		break;
	
	case 7: //new database
		
		//LastUsedDatabaseList.getMostRecent()
		//old in case o failure
		/*
		system("cls");
		lastUsedDataBase = AddNewDataBase();
		//wprintf(L"\n Last Used Database: %s", lastUsedDataBase.c_str());
		//userPrefsIni.saveVal(L"wsLastUsedDataBase", lastUsedDataBase);
		LastUsedDatabaseList.insertEntryHybrid(lastUsedDataBase);
		LastUsedDatabaseList.setMostRecent(lastUsedDataBase);
		Err = actualDataBase.loadFromFile(lastUsedDataBase);
		//system("pause");
		*/
		system("cls");
		//lastUsedDataBase = AddNewDataBase();
		LastUsedDatabaseList.insertEntryHybrid(AddNewDataBase());
		//LastUsedDatabaseList.setMostRecent(lastUsedDataBase);
		Err = actualDataBase.loadFromFile(LastUsedDatabaseList.getMostRecent());
		//system("pause");
		//todo
		break;

	case 8: //choose database
	{
		chooseDataBaseConsole(actualDataBase, Err, LastUsedDatabaseList);
		/* //old in case of failure
		std::wstring user_path;
		std::wstring choosed_path;
		wprintf(L"\n MAINTENANCE CALLENDAR CONSOLE -> Choose Database");
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n |                                                           |");
		wprintf(L"\n |        Please provide path for search:                    |");
		wprintf(L"\n |      ->");
		std::wcin.ignore();
		std::getline(std::wcin, user_path);
		wprintf(L"\n |                                                           |");
		wprintf(L"\n -------------------------------------------------------------");
		
		choosed_path = dirWorks::BrowseFFConsole(user_path, L"*dB", true);
		Err = actualDataBase.loadFromFile(choosed_path);
		if (Err == ERROR_SUCCESS)
		{
			lastUsedDataBase = choosed_path;
			LastUsedDatabaseList.insertEntryHybrid(lastUsedDataBase);
			LastUsedDatabaseList.setMostRecent(lastUsedDataBase);
			//userPrefsIni.saveVal(L"wsLastUsedDataBase", lastUsedDataBase);
		}
		//todo
		*/
	}
		break;

	case 9: //DEBUG
	{
		boost::posix_time::ptime ptimetest1;
		ptimetest1 = boost::posix_time::second_clock::local_time();

		boost::posix_time::time_duration ptimedurationtest1;
		ptimedurationtest1 = boost::posix_time::hours(1);
		std::cout << "\n ptimetest1      : " << ptimetest1;
		ptimetest1 = ptimetest1 + ptimedurationtest1;
		std::cout << "\n ptimetest1 after: " << ptimetest1;
		//setList testob1;
		//std::wstring savePath = L"D:\\T13\\setList.txt";
		//std::wstring testPath = L"D:\\TEST\\4";
		//testob1.insertEntry();
		//testob1.insertEntry();
		
		//testob1.saveToFileW(savePath);

		//setList grazyna;
		//grazyna.loadFromFileW(savePath);
		//grazyna.insertEntry();
		//grazyna.insertEntry();
		//grazyna.insertEntry();
		//grazyna.setMostRecent(testPath);
		system("pause");
		//dirWorks::test_foo();
		//std::wstring debugPath = L"D:\\Test\\DEBUG";
		//actualVechicle.firstSetup();
		//actualVechicle.saveToFile(debugPath);
		//MCEvent temp_ev;
		//temp_ev.loadFromFile(std::wstring(L"D:\\T13\\WE634LE\\events\\WE634LE_2.ev"));
		
		}
		//todo
		break;

	case 0: //exit
		system("cls");
		wprintf(L"\n EXITING.");
		//saving files etc.
		
		return 0;
		break;

	default:
		//todo
		break;
	}
	//system("pause");
	user_choice = mainmenu(actualDataBase);
}



	

	wprintf(L"\n");
system("pause");
return 0;
}