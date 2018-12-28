#pragma once
#include <string>

#ifndef __CFGFILEWSTRING_H_INCLUDED__
#define __CFGFILEWSTRING_H_INCLUDED__

#define BUFFER_EMPTY (DWORD) 132
#define NO_MATCH_FOUND (DWORD) 133

class CfgFileWString
{
public: //methods
	CfgFileWString();
	CfgFileWString(std::wstring &filepath);
	~CfgFileWString();

	int openFile(std::wstring &path);
	int openFile(wchar_t * path);

	int displayContent();
	void clear();
	void lastError(int erro);

	int searchVal(std::wstring &searchStr);
	int searchVal(wchar_t * searchStr);
	
	int search(std::wstring &searchStr, int &refVal);
	int search(wchar_t * searchStr, int &refVal);
	int search(std::wstring &searchStr, bool &refVal);
	int search(wchar_t * searchStr, bool &refVal);
	int search(std::wstring &searchStr, float &refVal);
	int search(wchar_t * searchStr, float &refVal);
	int search(std::wstring &searchStr, wchar_t *& refVal);
	int search(wchar_t * searchStr, wchar_t *& refVal);
	int search(std::wstring &searchStr, std::string &refVal);
	int search(wchar_t * searchStr, std::string &refVal);
	int search(std::wstring &searchStr, std::wstring &refVal);
	int search(wchar_t * searchStr, std::wstring &refVal);
	int search(std::wstring &searchStr, char *& refVal); 
	int search(wchar_t * searchStr, char *& refVal);
	//returns 0 for success and non-zero val when error. SetLasError is set and the same error code is returned
	
	int saveFile();
	int saveVal(std::wstring &searchStr, std::wstring &refVal);
	

	template <typename T>
	int saveVal(std::wstring &searchStr, T refVal)
	{
		errno_t Err = 0;
		Err = searchVal(searchStr);
		if (Err == ERROR_SUCCESS)
		{
			foundVal = std::to_wstring(refVal);
			size_t foundValEndPos = buffer.find(L'\r', foundValPos);
			buffer.replace(buffer.begin() + foundValPos, buffer.begin() + foundValEndPos, foundVal.begin(), foundVal.end());
			Err = saveFile();
			if (Err == ERROR_SUCCESS) return 0;
			else return Err;
			//}
		}
		else
			return Err;
	}
	
	template <typename T>
	int saveVal(wchar_t * searchStr, T refVal)
	{
		int retVal = 0;
		std::wstring tempStr(searchStr);
		retVal = saveVal(tempStr, refVal);
		return retVal;
	}


public: //variables

	int endianness; // 1 for big endian, -1 for little endian,
						//0 for no BOM, opening and interpreting file as narrow characters

	std::wstring filePath;
	std::wstring buffer;
	std::wstring foundVal;
	size_t buffer_size;
	size_t foundValPos; // used for changing the config files
};

#endif

