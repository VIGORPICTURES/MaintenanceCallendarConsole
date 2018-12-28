#include "CfgFileWString.h"
#include <fstream>
#include <string>
#include <iostream>
#include <windows.h>
#include <assert.h>



void CfgFileWString::lastError(int erro)
{
	if (erro != 0)
	{
		//error could not open file
		wprintf(L"Error, error code: %d", erro);
		assert(erro == 0);
		system("pause");
	}
}

void CfgFileWString::clear()
{
	filePath.clear();
	buffer.clear();
	foundVal.clear();
	buffer_size = 0;
	endianness = 0;
	foundValPos = 0;
}

CfgFileWString::CfgFileWString()
{
	clear();
}

CfgFileWString::CfgFileWString(std::wstring &filepath)
{
	clear();
	openFile(filepath);
}

int CfgFileWString::openFile(std::wstring &path)
{
	errno_t err;
	FILE *fp;
	size_t readCharacters = 0;
	//int endiannees = 0; // 1 for big endian, -1 for little endian,
						//0 for no BOM, opening file in typical read binary 


	err = _wfopen_s(&fp, path.c_str(), L"rb");
	if (err != ERROR_SUCCESS)
	{
		//error could not open file
		lastError(err);
		return err;
	}
	else
	{
		filePath = path;
		fseek(fp, 0, SEEK_END);
		buffer_size = (ftell(fp));
		rewind(fp);
		buffer.resize(buffer_size);
		
		wchar_t * endianness_val = new wchar_t;
		readCharacters = fread_s(endianness_val, sizeof(wchar_t), sizeof(wchar_t), 1, fp);

		if (*endianness_val == (wchar_t)0xFEFF)
		{
			endianness = 1;
			//wprintf(L"\n UNICODE(16bit): Little Endian!");
		}
		else if (*endianness_val == (wchar_t)0xFFFE)
		{
			endianness = -1; //big endian
			//wprintf(L"\n UNICODE(16bit): Big Endian!");
		}
		else
		{
			endianness = 0; //no BOM, little endian default
			//wprintf(L"\n No BOM. Narrow characters (8bit) Assuming Little Endian!");
		}
		
		if (endianness != 0)
		{
			buffer_size -= 2;
		}
		else
		{
			rewind(fp);
		}
		//wprintf(L"\nbuffer size: %d", buffer_size);
		readCharacters = 0;
		if (endianness != 0)
		{
			buffer.resize((buffer_size / 2)); //resize wstring (buffer size is in bytes, stw::wstring is 2byte per character.
			readCharacters = fread_s(&buffer[0], buffer_size, sizeof(wchar_t), buffer_size / 2, fp); //read wide characters to wide buffer
			fclose(fp);
			return 0;
		}
		else                                                                                  //read narrow characters to narrow buffer and then convert and copy to wide buffer
		{
			std::string buffer_narrow;
			buffer_narrow.resize(buffer_size);
			fread_s(&buffer_narrow[0], buffer_size, sizeof(char), buffer_size, fp);
			for (size_t i = 0; i < buffer_size; i++)
			{
				mbtowc(&buffer[i], &buffer_narrow[i], 1);
			}
			fclose(fp);
			return 0;
		}
	}
	
}

int CfgFileWString::openFile(wchar_t * path)
{
	std::wstring temp_path(path);
	int refVal = openFile(temp_path);
	return refVal;
}

int CfgFileWString::displayContent()
{

	if ((buffer[0] == L'\0') && (buffer.length() == 0))
	{
		//wprintf(L"\n ERROR. bufferPTR == NULL");
		//system("pause");
		SetLastError(ERROR_INTERNAL_ERROR);
			return ERROR_INTERNAL_ERROR;
	}
	else
	{
		wprintf(L"\n SHOWING CONTENT:\n");
		for (unsigned int i = 0; i < buffer_size; i++) //print each character separatly 
											  //on screen with end line and carrage return
		{
			//test_val = buffer[i];
			switch (buffer[i])
			{
			case L'\r':
				wprintf(L"[r]");
				break;

			case L'\n':
				wprintf(L"[n]\n");
				break;

			case L'\t':
				wprintf(L"[t]\t");
				break;

			case L'\0':
				wprintf(L"[0]");
				break;

			default:
				wprintf(L"%c", buffer[i]);
			}
		}
		wprintf(L"\n //SHOWING CONTENT:\n");
		return 0;
	}
}

int CfgFileWString::searchVal(std::wstring &searchStr)
{
	foundValPos = 0;
	//check if buffer is empty
	if ((buffer[0] == L'\0') && (buffer.length() == 0))
	{
		SetLastError(ERROR_INTERNAL_ERROR);
		wprintf(L"\n Error, Buffer empty");
		return ERROR_INTERNAL_ERROR;
	}
	else
	{	
		foundValPos = buffer.find(searchStr);
		if (foundValPos == -1)
		{
			wprintf(L"\n Sequence not found!");
			SetLastError(NO_MATCH_FOUND);
			return NO_MATCH_FOUND;
		}
		else
		{
			foundValPos = foundValPos + searchStr.length() + 1;
			size_t lineDelimiter = 0;
			lineDelimiter = buffer.find_first_of(L'\r', foundValPos);
			foundVal.assign((buffer.begin() + foundValPos), (buffer.begin() + lineDelimiter));
			//wprintf(L"\n foundVal: %s", foundVal.c_str());
			return 0;
		}
	}
}

int CfgFileWString::searchVal(wchar_t * searchStr)
{
	std::wstring temp_searchStr(searchStr);
	int retVal = searchVal(temp_searchStr);
	return retVal;
}

int CfgFileWString::search(std::wstring &searchStr, int &refVal) //INT
{
	if (searchVal(searchStr))
	{
		refVal = stoi(foundVal);
		return 0;
	}
	else
	{
		SetLastError(NO_MATCH_FOUND);
		//wprintf(L"\n No such string!");
		return NO_MATCH_FOUND;
	}
} //INT

int CfgFileWString::search(wchar_t * searchStr, int &refVal) //INT 
{
	std::wstring temp_searchStr(searchStr);
	int retVal = search(temp_searchStr,refVal);
	return retVal;
} 

int CfgFileWString::search(std::wstring &searchStr, bool &refVal) //BOOL
{
	std::wstring false_str = L"false";
	std::wstring true_str = L"true";
	
	if (searchVal(searchStr))
	{
		if (foundVal.compare(false_str) == 0)
		{
			refVal = false;
			return 0;
		}
		else if (foundVal.compare(true_str) == 0)
		{
			refVal = true;
			return 0;
		}
		else
		{
			//wprintf(L"\n error bool in file unreadable");
			SetLastError(ERROR_INVALID_FUNCTION);
			return ERROR_INVALID_FUNCTION;
		}
	}
	else
	{
		wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFileWString::search(wchar_t * searchStr, bool &refVal) 
{
	std::wstring temp_searchStr(searchStr);
	int retVal = search(temp_searchStr, refVal);
	return retVal;
}

int CfgFileWString::search(std::wstring &searchStr, float &refVal) //FLOAT
{
	if (searchVal(searchStr))
	{
		refVal = stof(foundVal);
		return 0;
	}
	else
	{
		SetLastError(NO_MATCH_FOUND);
		//wprintf(L"\n No such string!");
	}
	return NO_MATCH_FOUND;
}

int CfgFileWString::search(wchar_t * searchStr, float &refVal)
{
	std::wstring temp_searchStr(searchStr);
	int retVal = search(temp_searchStr, refVal);
	return retVal;
}

int CfgFileWString::search(std::wstring &searchStr, wchar_t *& refVal)
{
	if (searchVal(searchStr))
	{
		refVal = NULL;
		size_t refValBufferSize = 0;
		refValBufferSize = foundVal.length();
		refVal = new wchar_t[refValBufferSize];
		refVal[refValBufferSize] = L'\0';
		wmemcpy_s(refVal, refValBufferSize, foundVal.c_str(), refValBufferSize);
		return 0;
	}
	else
	{
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFileWString::search(wchar_t * searchStr, wchar_t *& refVal)
{
	std::wstring temp_searchStr(searchStr);
	int retVal = search(temp_searchStr, refVal);
	return retVal;
}

int CfgFileWString::search(std::wstring &searchStr, std::string &refVal)
{
	if (searchVal(searchStr))
	{
		refVal.resize(foundVal.length());
		char * temp_arr = new char[foundVal.length()];
		temp_arr[foundVal.length()] = '\0';
		size_t pReturnValue = 0;
		for (size_t i = 0; i < buffer_size; i++)
		{
			wcstombs_s(&pReturnValue, temp_arr, (foundVal.length() + 1), foundVal.c_str(), foundVal.length());
		}
		refVal.assign(temp_arr);
		return 0;
	}
	else
	{
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFileWString::search(wchar_t * searchStr, std::string & refVal)
{
	std::wstring temp_searchStr(searchStr);
	int retVal = search(temp_searchStr, refVal);
	return retVal;
}

int CfgFileWString::search(std::wstring &searchStr, std::wstring &refVal)
{
	if (searchVal(searchStr)==ERROR_SUCCESS)
	{
		refVal.clear();
		refVal.resize(foundVal.length());
		size_t refValBufferSize = 0;
		refVal.assign(foundVal);
		return 0;
	}
	else
	{
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFileWString::search(wchar_t * searchStr, std::wstring & refVal)
{
	std::wstring temp_searchStr(searchStr);
	int retVal = search(temp_searchStr, refVal);
	return retVal;
}

int CfgFileWString::search(std::wstring &searchStr, char * &refVal)
{
	if (searchVal(searchStr))
	{
		//refVal.resize(foundVal.length());
		refVal = new char[foundVal.length()];
		refVal[foundVal.length()] = '\0';
		size_t pReturnValue = 0;
		for (size_t i = 0; i < buffer_size; i++)
		{
			wcstombs_s(&pReturnValue, refVal, (foundVal.length() + 1), foundVal.c_str(), foundVal.length());
		}
		return 0;
	}
	else
	{
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFileWString::search(wchar_t * searchStr, char *& refVal)
{
	std::wstring temp_searchStr(searchStr);
	int retVal = search(temp_searchStr, refVal);
	return retVal;
}

int CfgFileWString::saveFile()
{
	errno_t Err = 0;
	FILE * fp;
	//wprintf(L"\n filepath: %s", filePath.c_str()); //test_only
	Err = _wfopen_s(&fp, filePath.c_str(), L"w, ccs=UNICODE");
	if(Err == ERROR_SUCCESS)
	{
		fwrite(&buffer[0], sizeof(wchar_t), (buffer_size/2), fp);
		fclose(fp);
		openFile(filePath);
		return 0;
	}
	else
	{
		wprintf(L"\n Err: %d", Err);
		return Err;
	}
}

int CfgFileWString::saveVal(std::wstring &searchStr, std::wstring & refVal)
{
	errno_t Err = 0;
	Err = searchVal(searchStr);
	if (Err == ERROR_SUCCESS)
	{
		foundVal = refVal;
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


/*
// saveVal method before creating template method in .h file not used now.
int CfgFileWString::saveVal(std::wstring &searchStr, int refVal)
{
errno_t Err = 0;
Err = searchVal(searchStr);
if (Err == ERROR_SUCCESS)
{
int readVal = stoi(foundVal);
if (readVal == refVal) return 0;
else
{
foundVal = std::to_wstring(refVal);
size_t foundValEndPos = buffer.find(L'\r', foundValPos);
buffer.replace(buffer.begin() + foundValPos, buffer.begin() + foundValEndPos, foundVal.begin(), foundVal.end());
Err = saveFile();
if (Err == ERROR_SUCCESS) return 0;
else return Err;
}
}
else
return Err;
}
*/

CfgFileWString::~CfgFileWString()
{
	clear(); //safety precaution. Clears content of object.
}
