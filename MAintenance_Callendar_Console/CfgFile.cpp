#include "CfgFile.h"
#include <fstream>
#include <string>
#include <iostream>

#include <windows.h>
#include <assert.h>



void CfgFile::clear()
{
	openSuccess = false;
	foundMatch = false;
	filePath = NULL;
	buffer = NULL;
	buffer_size = 0;
}

CfgFile::CfgFile()
{
	clear();
}

CfgFile::CfgFile(wchar_t * filepath)
{
	clear();
	openFileW(filepath);
}


int CfgFile::openFile(wchar_t * path)
{
	errno_t err;
	FILE *fp;

	int endiannees = 0; // 1 for big endian, -1 for little endian,
						//0 for no BOM, opening file in typical read binary 
	
	err = _wfopen_s(&fp, path, L"rb");
	if (err != ERROR_SUCCESS)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return ERROR_INVALID_FUNCTION;
	}
	else
	{
		wchar_t endiannes_BOM = 0;
		fread_s(&endiannes_BOM, sizeof(wchar_t), sizeof(wchar_t), 1, fp);
		switch (endiannes_BOM)
		{
		case 0xFFFE:
			endiannees = 1;
			wprintf(L"\n Little Endian!");
			break;

		case 0xFEFF:
			endiannees = -1;
			wprintf(L"\n Big Endian!");
			break;
		
		default:
			endiannees = 0;
			wprintf(L"\n No BOM");
			break;
		}
		if (endiannees != 0)
		{
			fseek(fp, 0, SEEK_END);
			buffer_size = ftell(fp)-2;
			fseek(fp, 2, SEEK_SET);
			buffer = new wchar_t[buffer_size];
			fread(buffer, sizeof(wchar_t), buffer_size / 2, fp);
		}
		else
		{
			fclose(fp);
			//std::cout << "\n wcslen(path): " << wcslen(path);
			char * path_narrow = new char[wcslen(path)+1];
			path_narrow[wcslen(path)] = '\0';
			//std::cout << "\n strlen(path_narrow): " << strlen(path_narrow);
			size_t no_converted = 0;
			err = wcstombs_s(&no_converted, path_narrow, (wcslen(path)+1), path, strlen(path_narrow));
			//std::cout << "\n no_converted: " << no_converted;
			err = fopen_s(&fp, path_narrow, "rb");
			//std::cout << "\nERR after f_open_s: " << err;
			if (err != ERROR_SUCCESS)
			{
				SetLastError(ERROR_INVALID_FUNCTION);
				return ERROR_INVALID_FUNCTION;
			}
			else
			{
				fseek(fp, 0, SEEK_END);
				buffer_size = ftell(fp);
				rewind(fp);
				char * buffer_narrow = new char[buffer_size];
				fread_s(buffer_narrow, buffer_size, sizeof(char), buffer_size, fp);
				buffer = new wchar_t[buffer_size];
				for (size_t i = 0; i < buffer_size;i++)
				{
				mbtowc(&buffer[i], &buffer_narrow[i], 1);
				}
				fclose(fp);
				return 0;
			}
		}
	}

	return 1;
}

int CfgFile::openFileW(wchar_t * path)
{
	errno_t err;
	FILE *fp;
	
	int endiannees = 0; // 1 for big endian, -1 for little endian,
						//0 for no BOM, opening file in typical read binary 


	err = _wfopen_s(&fp, path, L"rb");
	if (err != 0)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return ERROR_INVALID_FUNCTION;
	}
	else
	{
		filePath = path;
		openSuccess = true;
		fseek(fp, 0, SEEK_END);
		buffer_size = (ftell(fp));
		rewind(fp);
		
		wchar_t * endianness_val = new wchar_t;
		fread_s(endianness_val, sizeof(wchar_t), sizeof(wchar_t), 1, fp);
		
		switch (*endianness_val)
		{
		case 0xFFFE: //little endian
			endiannees = 1;
			wprintf(L"\n Little Endian!");
			break;

		case 0xFEFF:
			endiannees = -1; //big endian
			wprintf(L"\n Big Endian!");
			break;

		default:
			endiannees = 0; //no BOM, little endian default
			wprintf(L"\n No BOM. Assuming Little Endian!");
			break;
		}
		if (endiannees != 0)
		{
			buffer_size -= 2;
		}
		else
		{
			rewind(fp);
		}
			//buffer_size -= ftell(fp);
		wprintf(L"\nbuffer size: %d", buffer_size);
		buffer = new wchar_t[buffer_size];
		//for (int i = 0; i < buffer_size; i++)
		//{
		//	buffer[i] = L'\0';
		//}

		fread_s(buffer, buffer_size, sizeof(wchar_t), buffer_size/2,fp);
		}

	fclose(fp);
	return 0;
}

int CfgFile::displayContent()
{
	wchar_t * bufferPTR = buffer;
	size_t bufferPTRpos = 0;
	if (bufferPTR == NULL)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return ERROR_INVALID_FUNCTION;
	}
	else
	{
		wprintf(L"\n SHOWING CONTENT:\n");
		wchar_t test_val = L'0';
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
				wprintf(L"%c",buffer[i]);
			}
		}
		wprintf(L"\n //SHOWING CONTENT:\n");
		return 0;
	}
}

int CfgFile::searchVal(wchar_t * searchStr)
{
	//check if buffer is empty
	if (buffer == NULL)
	{
		//wprintf(L"\n Error, Buffer empty");
		SetLastError(BUFFER_EMPTY);
		return BUFFER_EMPTY;
	}
	else
	{
		size_t StrLen = wcslen(searchStr);
		wchar_t * foundPTR = 0;
		foundPTR = wcsstr(buffer, searchStr);
		if (foundPTR == NULL)
		{
			foundMatch = false; //  not used now
			wprintf(L"\n Sequence not found!");
			SetLastError(NO_MATCH_FOUND);
			return false;
		}
		else
		{
			foundMatch = true; //not used now
			foundPTR += (StrLen + 1);
			size_t foundValLength = 0;
			for (size_t i = 0; *foundPTR != L'\r'; i++)
			{
				foundPTR++;
				foundValLength++;
			}
			foundValBuff = new wchar_t[foundValLength];
			wmemset(foundValBuff, L'\0', foundValLength);
			foundPTR -= foundValLength;
			wmemcpy_s(foundValBuff, foundValLength, foundPTR, foundValLength);
			foundValBuff[foundValLength] = L'\0';
			return 0;
		}
	}
}

int CfgFile::search(wchar_t * searchStr, int &refVal) //INT
{
	if (searchVal(searchStr))
	{
		refVal = _wtoi(foundValBuff);
		return 0;
	}
	else
	{
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFile::search(wchar_t * searchStr, bool &refVal) //BOOL
{
	wchar_t false_arr[5] = { L'f',L'a',L'l',L's',L'e'};
	wchar_t true_arr[4] = { L't',L'r',L'u',L'e' };
	if (searchVal(searchStr))
	{
		if ((memcmp(false_arr, foundValBuff,10)) == 0)
			{
				refVal = false;
				return 0;
			}
		else if ((memcmp(true_arr, foundValBuff, 8)) == 0)
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
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFile::search(wchar_t * searchStr, float &refVal) //FLOAT
{
	if (searchVal(searchStr))
	{
		refVal = wcstof(foundValBuff, NULL);
		return 0;
	}
	else
	{
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFile::search(wchar_t * searchStr, wchar_t *& refVal)
{
	if (searchVal(searchStr))
	{
		refVal = NULL;
		size_t refValBufferSize = 0;
		refValBufferSize = wcslen(foundValBuff);
		refVal = new wchar_t[refValBufferSize];
		refVal[refValBufferSize] = L'\0';
		wmemcpy_s(refVal, refValBufferSize, foundValBuff, refValBufferSize);
		return 0;
	}
	else
	{
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFile::search(wchar_t * searchStr, std::string &refVal)
{
	if (searchVal(searchStr))
	{
		refVal.clear();
		size_t refValBufferSize = 0;
		refValBufferSize = wcslen(foundValBuff);
		refVal.resize(refValBufferSize);
		size_t pReturnValue = 0;
		char * refValChar = new char[refValBufferSize];
		refValChar[refValBufferSize] = '\0';

		for (size_t i = 0; i < refValBufferSize;i++)
		{
			wcstombs_s(&pReturnValue, refValChar, refValBufferSize+1, foundValBuff, refValBufferSize);
		}
		refVal.assign(refValChar, refValBufferSize);
		return 0;
	}
	else
	{
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFile::search(wchar_t * searchStr, std::wstring &refVal)
{
	if (searchVal(searchStr))
	{
		refVal.clear();
		size_t refValBufferSize = 0;
		refValBufferSize = wcslen(foundValBuff);
		refVal.resize(refValBufferSize);
		refVal.assign(foundValBuff);
		return 0;
	}
	else
	{
		//wprintf(L"\n No such string!");
		SetLastError(NO_MATCH_FOUND);
		return NO_MATCH_FOUND;
	}
}

int CfgFile::search(wchar_t * searchStr, char * &refVal)
{
	if (searchVal(searchStr))
	{
		refVal = 0;
		size_t refValBufferSize = 0;
		refValBufferSize = wcslen(foundValBuff);
		refVal = new char[refValBufferSize];
		refVal[refValBufferSize] = '\0';
		size_t pReturnValue = 0;

		for (size_t i = 0; i < refValBufferSize; i++)
		{
			wcstombs_s(&pReturnValue, refVal, refValBufferSize + 1, foundValBuff, refValBufferSize);
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


CfgFile::~CfgFile()
{
}
