#pragma once
#ifndef __MCDATABASE_H__
#define __MCDATABASE_H__

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "MCVechicle.h"
#include "MCEvent.h"


#define MAX_PATH 260

class MCDataBase
{
public: //methods
	MCDataBase();
	~MCDataBase();
	void addStuff(std::wstring& smth);
	int setPath(std::wstring &path); //return
	int setName(std::wstring &name);
	int createFile(); //only when internal path aprameter is valid // replaces saveToFile temporarly
	int loadFromFile(std::wstring &filepath);
	int saveToFile(std::wstring& folderpath);//not used for now
	int searchForVechicles();
	int getVechicleEvents(std::vector<MCEvent> &events_v_ref);
	bool checkIfValid(); //checks if path and name are empty

	//int addVechicle(std::wstring &vechicleName);
	//int chooseVechicle();

public: //variables
	struct basicData
	{
		wchar_t extension[3];
		size_t version;
		wchar_t path[MAX_PATH];
		wchar_t name[MAX_PATH];	
	}bD;
	bool dataBaseValid;
	bool vechicleValid;
	MCVechicle currentVechicle;
	std::vector<MCVechicle> vechicles_v;
	std::vector<std::wstring> vechicleNames_v; //path to every vechicle
	//size_t numberOfVechicles;
	//wchar_t reserved;


};

#endif

