#pragma once
#include <string>
#include <vector>
#ifndef __DIRWORKS_H__
#define __DIRWORKS_H__

#define PATH_ABSOLUTE 1
#define PATH_RELATIVE 2

#define MODE_DEFAULT	0
#define MODE_PATH		1
#define MODE_SIZE		2
#define MODE_TYPE		3
#define MODE_EXTENSION	4

#define RETURN_PATH		5
#define RETURN_FILENAME 6

struct itemData
{
	std::wstring item_path;
	unsigned int item_type;
	unsigned int item_size;
	std::wstring item_extension;
};

typedef std::vector<itemData> dirData_v;

namespace dirWorks
{

	//#include "dirWorks.cpp"


	//#define PATH_ABSOLUTE 1
	//#define PATH_RELATIVE 2

	

	


	std::wstring getExePath();
	int makeDir(std::wstring &path); //if sucess, returns true, otherwise false. GetLastError() for error code.
	int makeDir(wchar_t * path);
	int makeFile(const wchar_t * path);
	int makeFile(std::wstring &path);
	std::wstring fullPathDivider(const wchar_t * path, int typereturn);
	std::wstring fullPathDivider(std::wstring &path, int typereturn);
	int getFF(const wchar_t * path, dirData_v &itemsVector, bool consoleOutput = false);
	int getFF(const std::wstring &path, dirData_v &itemsVector, bool consoleOutput = false);


	int saveDirDataToFile(const wchar_t * outputFilename,
		dirData_v &itemsVector,
		unsigned int mode = MODE_DEFAULT);

	int saveDirDataToFile(const std::wstring & outputFilename,
		dirData_v &itemsVector,
		unsigned int mode = MODE_DEFAULT);


	inline void cpyIteratorToItemData(dirData_v::iterator &it, itemData &entry);
	//searchFile: if "*extension" provided by parameter, only exact extension files will be searched
				//if "^key" provided by parameter, search will search every occurance of key without beiing case sensitive


	//These two functions return number of found matches instead of typical 0 for success and err when problem obscurrred
	int searchFile(const wchar_t * path, dirData_v & resultItemsVector_v, const wchar_t * key, bool consoleOutput = false);
	int searchFile(const std::wstring &path, dirData_v & resultItemsVector_v, const std::wstring &key, bool consoleOutput = false);

	std::wstring BrowseFFConsole(const wchar_t * path, std::wstring key = L"NONE", bool consoleOutput = false); //lists files in console window. 
																					 // Returns (absolute) path to selected file
																					 //without "key" - list all files / folders in path
																					//single choice only
	std::wstring BrowseFFConsole(std::wstring &path, std::wstring key = L"NONE", bool consoleOutput = false); //lists files in console window. 
																					// Returns (absolute) path to selected file
																					//without "key" - list all files / folders in path
																					//single choice only.

	std::wstring BrowseFFConsole(dirData_v &dir_v);								//lists files int dir_v.
																				//single choice only.

	void showDirDataConsole(dirData_v &itemsVector);				//shows contents of vector.
	
	void test_foo();

}

#endif


