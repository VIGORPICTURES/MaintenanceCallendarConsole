#pragma once
#include <string>

#ifndef __CFGFILE_H__
#define __CFGFILE_H__

#define BUFFER_EMPTY (DWORD) 132
#define NO_MATCH_FOUND (DWORD) 133

class CfgFile
{
public: //methods
	CfgFile();
	CfgFile(wchar_t * filepath);
	~CfgFile();

	int openFileW(wchar_t * filepath); //openFile 80-=--wide characters
	int openFile(wchar_t * filepath);
	int displayContent();
	void clear();

	int searchVal(wchar_t * searchStr); 
	int search(wchar_t * searchStr, int &refVal);
	int search(wchar_t * searchStr, bool &refVal);
	int search(wchar_t * searchStr, float &refVal);
	int search(wchar_t * searchStr, wchar_t *& refVal);
	int search(wchar_t * searchStr, std::string &refVal);
	int search(wchar_t * searchStr, std::wstring &refVal);
	int search(wchar_t * searchStr, char *& refVal);
	//returns false when key not found or error, true when found
	//when false: SetLastError flag is set. 132 - buffer empty, 133 - val not found

public: //variables
	
	bool openSuccess;
	bool foundMatch;
	wchar_t * filePath;
	wchar_t * buffer;
	wchar_t * foundValBuff;
	size_t buffer_size;
};

#endif

