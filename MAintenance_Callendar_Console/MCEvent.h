#pragma once
#include "time.h"
#include "windows.h"
#include <string>
#ifndef __MCEVENT_H__
#define __MCEVENT_H__

#define MAX_PATH 260

	//--------------------------------------------------------------//
	//                                                              //
	//                           MCEvent                            //
	//                                                              //
	//--------------------------------------------------------------//
	//                                                              //
	// INFO: Class designed to work with various applications for   //
	//       better handling events with other objects/functions.   //
	//       MCEvent objects can have unique ID (no two the same    //
	//       objects). Additional feature is programName parameter  //
	//       for relaying MCEvent and base app/parent object        //
	//       (helps in pairing MCEvent and parent app).             //
	//       Usually MCEvent class should be a class inheriting     //
	//		 from parent object. After this, setting MCEvent		//
	//		 objects is simpler. Either parentName (object or app   //
	//		 name) or other value should be used for this. This is  //
	//		 called the recognision trace. For example:				//
	//																//
	//		class car	     | recognision val	| McEvent class		//
	//      ----------------------------------------------------	//
	//      | licensePlate   |     WE634LE		| licensePlate |	//
	//		----------------------------------------------------	//
	//																//
	//		MCEvent stores each event in separate file with			//
	//		extension *ev and naming convention of free choice		//
	//		(altough it is a good practise to use following			//
	//		template:												//
	//																//
	//      aD.value     _ bD.eventID ->							//
	//		licensePlate _ eventID    ->							//
	//		WE634LE      _ 00164      ->     ex: WE634LE_00164.ev	//
	//																//
	//		This way produces clear naming convention for files.	//
	//																//
	//		MCEvent contains two data structures:					//
	//		1. basicData -> bD -> basicData that shouldn't be		//
	//							  rewriten (adding new vriables).	//
	//		2. additionalData -> aD -> Here, user can add and		//
	//								   delete variables.			//
	//																//
	//		bD.additionalDataStruct flag should be set to false,	//
	//		when no other struct is used.							//

	struct basicData
	{
		//wchar_t parentName[32];
		// aka magic, for checking structure of event. 
		//null_terminated
		size_t eventVersion; //for different behaviour.
		size_t eventID; //not necessary to use
		tm  eventDate;
		tm eventCreationDate;
		enum eventStatus
		{
			//main statuses
			Scheduled = 0,
			Started = 1,
			Processed = 2,
			Finished = 3,
			//additional statuses
			Current = 4,
			Locked = 5,
			Missed = 6,
			Important = 7,
			Commentary = 8,
			//Default = 0
		};
		eventStatus evStatus;
		wchar_t eventCommentaryArr[MAX_PATH];
		//bool additionalDataStruct;
	//};
	//struct additionalData
	//{
		wchar_t licensePlate[11];
		DWORD eventType; //
		size_t eventDuration; //in hours
		wchar_t fullPath[MAX_PATH];
	};

	class MCEvent
	{
	public: //methods
		MCEvent();
		~MCEvent();
		//int setEventConsole(size_t &lastEventID);
		int saveToFile(std::wstring &path);
		int loadFromFile(std::wstring &path);
		

	public: //variables

		basicData bD;
		//additionalData aD;

	};

#endif

