#pragma once
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

class setList
{
public: //methods
	setList();
	~setList();
	int loadFromFileW(std::wstring &path);
	int saveToFileW(std::wstring &path);
	std::wstring getMostRecent();
	int setMostRecent(std::wstring &entry); //saves to file //only when item is avaiable in container!

	int insertEntryHybrid(std::wstring entry = L"NONE", std::wstring mode = L"front"); 
		//works with arguments passed and have own console input.
		//
	//void checkPaths(); //tries to open each file from path. If no success, deletes path or maybe inform user of broken paths
public: //variables

	std::deque <std::wstring> fileContent;
	std::wstring filePath;

};

