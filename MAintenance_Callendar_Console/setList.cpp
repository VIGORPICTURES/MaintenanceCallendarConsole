#include "setList.h"
#include <windows.h>

#define MAX_PATH 260

typedef std::deque<std::wstring>::iterator deque_it;

extern void errorBoxConsole(std::wstring title, std::wstring msg);

int setList::loadFromFileW(std::wstring &path)
{
	errno_t Err = 0;
	FILE * fp;

	
	wchar_t character = L'\0';
	std::wstring entry;
	Err = _wfopen_s(&fp, path.c_str(), L"rb");
	
	if(Err == ERROR_SUCCESS)
	{
		filePath = path;
		int read_OK = true;

		fseek(fp, sizeof(wchar_t), SEEK_SET);
		//fread_s(&entry[0], sizeof(wchar_t), sizeof(wchar_t), 1, fp); //skipping bom
		//entry.clear();

		while (read_OK)
		{
			read_OK = fread_s(&character, sizeof(wchar_t), sizeof(wchar_t), 1, fp);
		
			if(character == L'\r')
			{
				if (!read_OK) break;
				//entry += L'\0';
				fileContent.push_back(entry);
				entry.clear();
				fseek(fp, sizeof(wchar_t), SEEK_CUR);
				continue;
			}
			entry += character;
		}
		fclose(fp);
	}
	else
	{
		//error
		return Err;
	}
	
	return 0;
}

int setList::saveToFileW(std::wstring &path)
{

	errno_t Err = 0;
	FILE * fp;
	filePath = path;
	if (fileContent.empty()) return ERROR_INVALID_FUNCTION; //checks if fileContent is empty
	deque_it it = fileContent.begin();

	Err = _wfopen_s(&fp, path.c_str(), L"w, ccs=UNICODE");
	if (Err == ERROR_SUCCESS)
	{
		std::wstring toFile;
		size_t toFileLength = 0;
		wchar_t newLineChar = L'\n';
		wchar_t eofChar = -1;
		while (it != fileContent.end())
		{
			toFile = *it;
			toFileLength = toFile.length();
			fwrite(&toFile[0], sizeof(wchar_t), toFileLength, fp);
			fwrite(&newLineChar, sizeof(wchar_t), 1, fp);
			it++;
		}
		//fwrite(&eofChar, sizeof(wchar_t), 1, fp);
		fclose(fp);
	}
	else
	{
		return Err;
	}

	return 0;
}

std::wstring setList::getMostRecent()
{
	
	std::wstring refVal = L"NONE";
	std::deque<std::wstring>::iterator it = fileContent.begin();
	if (fileContent.empty()) return refVal;
	refVal = *it;
	return refVal;
}

int setList::setMostRecent(std::wstring &entry)
{
	errno_t Err = 0;
	deque_it result = std::find(std::begin(fileContent), std::end(fileContent), entry);
	if (result != std::end(fileContent))
	{
		size_t iterator_pos = std::distance(fileContent.begin(), result);
		if (iterator_pos == 0) return 0;
		std::rotate(fileContent.begin(), fileContent.begin()+iterator_pos, fileContent.end());
	}
	else
	{
		//error, no item avaiable. Use insert item Hybrid instead
		errorBoxConsole(L"setList -> setMostRecent", L"Error, no such file in container! Use insertEntryHybrid()");
		Err = ERROR_INVALID_FUNCTION;
		return Err;
		//fileContent.push_front(entry);
	}

	Err = saveToFileW(filePath);
	if (Err == ERROR_SUCCESS) return 0;
	else return Err;
}

int setList::insertEntryHybrid(std::wstring entry, std::wstring mode)
{
	errno_t Err = 0;
	if (entry == L"NONE")
	{
		wprintf(L"\n setList -> Insert New Entry");
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n |                                                           |");
		wprintf(L"\n |        Please provide new entry:                          |");
		wprintf(L"\n |      ->");
		std::wcin.ignore();
		std::getline(std::wcin, entry);
		wprintf(L"\n |                                                           |");
		wprintf(L"\n -------------------------------------------------------------");
	}
	deque_it result = std::find(fileContent.begin(), fileContent.end(), entry);
	if (result != fileContent.end())
	{
		return 0;
	}
	if (result == fileContent.end())
	{
		if (mode == L"front")
		{
			fileContent.push_front(entry);
		}
		else
		{
			fileContent.push_back(entry);
		}

		Err = saveToFileW(filePath);
		//fileContent.clear(); //new added clears 
		//loadFromFileW(filePath); //new added
	}
	if (Err == ERROR_SUCCESS) return 0;
	else return Err;
}


setList::setList()
{
}


setList::~setList()
{
}
