#include "dirWorks.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <algorithm>

extern void errorBoxConsole(std::wstring title, std::wstring msg);

std::wstring dirWorks::getExePath()
{
	std::wstring exePath = L"NONE";
	wchar_t exePathArr[MAX_PATH];
	GetModuleFileNameW(NULL, exePathArr, MAX_PATH);
	exePath.assign(exePathArr);
	size_t periodPos = 0;
	periodPos = exePath.find_last_of(L'\\');
	exePath.resize(periodPos);

	return exePath;
}


int dirWorks::makeDir(std::wstring &path)
{
	errno_t Err = 0;															//varaible for handling error
	std::wstring pathCopy = path;
	
	if (path[0] == L'\\')														//check if path provided by user is relative to .exe
	{
		std::wstring temp_path;
		wchar_t exepatharr[MAX_PATH];											//variable for getmoduleFileName
		GetModuleFileNameW(NULL, exepatharr, MAX_PATH);							//get exe filepath
		temp_path.assign(exepatharr);											//work on wstring instead of wchar_t
		size_t exePathFolderPos = 0;											//variable for starting position of .exe filename in string ex. test_folder\\test.exe 
		exePathFolderPos = temp_path.find_last_of(L'\\', temp_path.length());	//search for .exe filename start
		temp_path.replace(exePathFolderPos, -1,path,0, path.length());			//replace .exe filename with path. -1 in 'len' parameter -> every characters from pos to end of string
		//std::wcout << L"\n new path: " << temp_path;
		//wprintf(L"\n new path: %s", temp_path);
		//system("pause");
		pathCopy = temp_path;
	}
		//std::wcout << L"\n fullPath: " << path;
		//wprintf(L"\n fullpath: %s", path);
	if (pathCopy[pathCopy.length()] != L'\\')pathCopy += L'\\'; 

														
	std::wstring tempPath;
	size_t foundBackSlash = 0;													// variable for storing '\\' position in string 
	//foundBackSlash = folderpath.find_first_of(L"\\", 0); //\\in drive letter
	foundBackSlash = pathCopy.find_first_of(L"\\", foundBackSlash);				//	find first occurace of '\\' in drive letter
	//tempPath.assign(pathCopy.begin(), pathCopy.begin() + foundBackSlash + 1);	//  drive root
	//std::wcout << "\n SUBPATH = " << tempPath;
	//wprintf(L"\n SUBPATH: %s", tempPath);
	size_t temp_pos = 0;														// variable needed for calculating nest subfolder length
	int MakeDirSucess = false;
	do //no more subdirectories
	{
		temp_pos = foundBackSlash;
		foundBackSlash = pathCopy.find(L"\\", foundBackSlash + 1);				// search next occurance of '\\' in string. foundBackSlash + 1 for moving next in string
		if (foundBackSlash == -1) break;										// if no more occurances of '\\', break loop statement
		tempPath.assign(pathCopy.begin(), (pathCopy.begin() + temp_pos + (foundBackSlash - temp_pos)));  //assign next subfolder to path for creation
		MakeDirSucess = CreateDirectoryW(tempPath.c_str(), NULL);				// try to create directory // Keeps crying about performance warning, oh well...
		if (MakeDirSucess==false)
		{
			Err = GetLastError();
			
			//if (LastError == ERROR_ALREADY_EXISTS) //wprintf(L"\n Directory is already created."); //nothing is wrong. Directory with subdirectories already exist
			if (Err == ERROR_PATH_NOT_FOUND)							//something is wrong with path //change to elseif when above line is needed
			{
				//wprintf(L"\n Error, path not found");
				return Err;
			}
			if (Err != ERROR_ALREADY_EXISTS)
			{
				SetLastError(ERROR_BAD_PATHNAME);
				return Err;
			}
		}
		
	} while (true);
	//wprintf(L"\n COPYPATH = %s", pathCopy);
	return 0;
}

int dirWorks::makeDir(wchar_t * path)
{
	std::wstring temp_path;
	temp_path.assign(path);
	int retVal;
	retVal = makeDir(temp_path);
	return retVal;
}

int dirWorks::makeFile(std::wstring &path)
{
	errno_t Err = 0;
	size_t fileNamePos = 0;
	fileNamePos = path.find_last_of(L".");
	std::wcout << "\n fileNamePos: " << fileNamePos;
	if (fileNamePos != -1)
		fileNamePos = path.find_last_of(L"\\");
	std::wcout << "\n fileNamePos: " << fileNamePos;
	std::wstring dir_path;
	dir_path.assign(path.begin(), path.begin() + fileNamePos);
	Err = makeDir(dir_path);
	if(Err == ERROR_SUCCESS)
	{
		FILE * fp;
		Err = _wfopen_s(&fp, path.c_str(), L"wb");
		if (Err != ERROR_SUCCESS)
		{
			return Err;
		}
		fclose(fp);
		return 0;
	}
	return Err;
}

int dirWorks::makeFile(const wchar_t * path)
{
	std::wstring temp_path(path);
	int retVal = makeFile(temp_path);
	return retVal;
}

std::wstring dirWorks::fullPathDivider(std::wstring &path, int typereturn)
{
	size_t fileNamePos = 0;
	fileNamePos = path.find_last_of(L".");
	if (fileNamePos != -1)
		fileNamePos = path.find_last_of(L"\\");
	if (typereturn == RETURN_FILENAME)
	{
		std::wstring f_name;
		f_name.assign(path.begin() + fileNamePos, path.end());
		return f_name;
	}
	else if (typereturn == RETURN_PATH)
	{
		std::wstring dir_path;
		dir_path.assign(path.begin(), path.begin() + fileNamePos);
		return dir_path;
	}
	else
		return L"NONE";

}

std::wstring dirWorks::fullPathDivider(const wchar_t * path, int typereturn)
{
	std::wstring temp_path(path);
	std::wstring retVal = fullPathDivider(temp_path, typereturn);
	return retVal;
}

int dirWorks::getFF(const std::wstring &path, dirData_v & itemsVector, bool consoleOutput)
{
	static size_t count = 0;
	//system("cls");
	//wprintf(L"\n dirWorks\\dirFolderConsole");
	//wprintf(L"\n -------------------------------------------------------------");
	//wprintf(L"\n ---------------------------dirFolder--------------------------------");
	itemData entry;
	int retVal = 0;
	std::wstring pattern(path);
	pattern.append(L"\\*");
	WIN32_FIND_DATAW data;
	HANDLE hFind;
	size_t no_extension = 0;
	//bool retVal = false;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::wstring local = path + L"\\" + data.cFileName;
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				entry.item_extension = L"NULL";
				const size_t len = wcslen(data.cFileName);
				if ((len == 1 && data.cFileName[0] == '.') ||
					(len == 2 && data.cFileName[0] == '.' && data.cFileName[1] == '.'))
					continue;

				retVal = getFF(local,  itemsVector, consoleOutput);//, size_data

			}
			else
			{
				no_extension = local.find_last_of(L".", -1); //if -1, file has no extension
				if (no_extension == -1)
				{
					entry.item_extension = L"NONE";
				}
				else
				{
					entry.item_extension.assign((local.begin() + local.find_last_of(L".", -1)), (local.begin() + local.length()));
				}
				//entry.item_extension.assign((local.begin() + local.find_last_of(L".", -1)), (local.begin() + local.length()));
			}
			
			entry.item_path = local;
			entry.item_size = data.nFileSizeLow;
			entry.item_type = data.dwFileAttributes;
			itemsVector.push_back(entry);
			
			
			//wprintf(L"\n %s", local.c_str());
			if (consoleOutput)
			{
				count++;
				wprintf (L"\n | [%d] %s", count, entry.item_path.c_str());
			}
			
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
	else
	{
		return ERROR_INVALID_FUNCTION;
	}
	//wprintf(L"\n -------------------------------------------------------------");
	return retVal;
}

int dirWorks::getFF(const wchar_t * path, dirData_v & itemsVector, bool consoleOutput)
{
	std::wstring temp_path(path);
	getFF(path, itemsVector, consoleOutput);
	int retVal = 0;
	return retVal;
}

int dirWorks::saveDirDataToFile(const std::wstring & outputFilename, dirData_v &itemsVector, unsigned int mode)
{
	errno_t Err;
	std::wstring toFileString;
	FILE *fp = NULL;
	Err = _wfopen_s(&fp, outputFilename.c_str(), L"w, ccs=UNICODE");
	if (Err == ERROR_SUCCESS)
	{
		for (dirData_v::iterator it = itemsVector.begin(); it != itemsVector.end(); it++)
		{
			toFileString.clear();
			switch (mode)
			{
			case MODE_DEFAULT:
			{
				if (it->item_type == 16) toFileString = L"FOLDER\t";
				else toFileString = L"FILE  \t";
				toFileString += L"SIZE: \t";
				toFileString += std::to_wstring(it->item_size);
				toFileString += L"\t";
				toFileString += it->item_path;	
			}
			break;

			case MODE_PATH:
				toFileString = it->item_path;
					break;

			case MODE_SIZE:
				toFileString = std::to_wstring(it->item_size);
				break;

			case MODE_TYPE:
				if (it->item_type == 16) toFileString = L"FOLDER";
				else toFileString = L"FILE";
				break;

			case MODE_EXTENSION:
				toFileString = it->item_extension;
				break;

			default:
				wprintf(L"\n InvalidMode");
			}
			toFileString += L"\r\n";
			//wprintf L"\n toFileString: %s | sizeof: %d | len: %d", toFileString.c_str(), sizeof(toFileString), toFileString.length());
			fwrite(toFileString.c_str(), (toFileString.length()*sizeof(wchar_t)), 1, fp);
			//save content to file
		}
	}
	else
	{
		wprintf(L"\n Error, couldn't open file. Error Code: %d", Err);
		SetLastError(Err);
		return Err;
		
	}
	wchar_t EOF_char = L'\0';
	fwrite(&EOF_char, sizeof(wchar_t), 1, fp);
	
	fclose(fp);
	return 0;
}

int dirWorks::saveDirDataToFile(const wchar_t * outputFileName, dirData_v &itemsVector, unsigned int mode)
{
	std::wstring temp_outputFileName(outputFileName);
	int retVal = saveDirDataToFile(temp_outputFileName, itemsVector, mode);
	return retVal;
}

inline void dirWorks::cpyIteratorToItemData(dirData_v::iterator &it, itemData &entry)
{
	entry.item_path = it->item_path;
	entry.item_extension = it->item_extension;
	entry.item_size = it->item_size;
	entry.item_type = it->item_type;
}

int dirWorks::searchFile(const std::wstring &path, dirData_v & resultItemsVector_v, const std::wstring &key, bool consoleOutput)
{
	size_t foundMatches = 0;									//variable for storing found matching files/folders (key) in provided path 
	std::wstring key_cpy = key;				
	itemData entry;												//for storing information
	dirData_v temp_vector;										//vector for storing dirFolder results
	getFF(path, temp_vector, consoleOutput);								// function to list every file and folder in path				
	
	if (key[0] == L'^')											//!!! search key in file NOT CASESENSITIVE
	{
		key_cpy.assign(key.begin() + 1, key.end());
		std::transform(key_cpy.begin(), key_cpy.end(), key_cpy.begin(), ::tolower);
		std::wstring temp_str;
		for (dirData_v::iterator it = temp_vector.begin(); it != temp_vector.end(); it++)
		{
			temp_str = it->item_path;
			std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), ::tolower);
			//std::transform(it->item_path.begin(), it->item_path.end(), it->item_path.begin(), ::tolower);
			//std::transform(it->item_path.begin(), it->item_path.end(), temp_str.begin(), ::tolower); //do not work
			//if (it->item_path.find(key_cpy, 0) != -1)
			if (temp_str.find(key_cpy, 0) != -1)
			{
				cpyIteratorToItemData(it, entry);
				resultItemsVector_v.push_back(entry);
				foundMatches++;
			}
		}
	}
	else if(key_cpy[0] == L'*')								// !!! searching only files with specified extension (extension in key)
	{
		key_cpy[0] = L'.';
		for (dirData_v::iterator it = temp_vector.begin(); it != temp_vector.end(); it++)
		{
			if (it->item_extension == key_cpy)
			{
				cpyIteratorToItemData(it, entry);
				resultItemsVector_v.push_back(entry);
				foundMatches++;
			}
		}
	}
	else													// !!! search key in file CASESENSITIVE
	{
		for (dirData_v::iterator it = temp_vector.begin(); it != temp_vector.end(); it++)
		{
			//testPTR++;
			if (it->item_path.find(key_cpy, 0) != -1)
			{
				//test_val = it->item_path.find(key_cpy, 0);
				cpyIteratorToItemData(it, entry);
				resultItemsVector_v.push_back(entry);
				foundMatches++;
			}
		}
	}

	if (foundMatches) return foundMatches;
	else return false;
}

int dirWorks::searchFile(const wchar_t * path, dirData_v & resultItemsVector_v, const wchar_t *key, bool consoleOutput)
{
	std::wstring temp_path(path);
	std::wstring temp_key(key);
	int retVal = searchFile(temp_path, resultItemsVector_v, temp_key);
	return retVal;
}

std::wstring dirWorks::BrowseFFConsole(std::wstring &path, std::wstring key, bool consoleOutput)
{
	dirData_v pathList;												//vector conatining list of paths that fulfils requirements. (search path and extension)
	std::wstring retVal;											// absolute path choosed by user.
	size_t noOfPaths = 1;											// stores number of paths. Used for drawing list of paths for user to choose.
	size_t userChoice = 0;
	int searchSuccess = false;										// variable for storing whenever found a match.
	if (key == L"NONE") searchSuccess = getFF(path, pathList, consoleOutput);
	else searchSuccess = searchFile(path, pathList, key, consoleOutput);

	if(searchSuccess)
	{
	
		//DRAWING LIST
		while (true)
		{
			system("cls");
			wprintf(L"\n dirWorks\\listFilesConsole");
			wprintf(L"\n -------------------------------------------------------------");
			wprintf(L"\n | Listing files:                                            |");
			for (dirData_v::iterator it = pathList.begin(); it != pathList.end(); it++)
			{
				wprintf(L"\n | [%d] : %s", noOfPaths, it->item_path.c_str());
				noOfPaths++;
			}
			wprintf(L"\n -------------------------------------------------------------");
			wprintf(L"\n | Enter number of file you want to use: ");
			wscanf_s(L"%d", &userChoice);
			if (userChoice >= 1 && userChoice <= noOfPaths)
			{
				return pathList[userChoice - 1].item_path;
			}
			else
			{
				errorBoxConsole(L"dirWorks\\listFilesConsole", L"Error. Invalid choice. Try again.");
				noOfPaths = 1;
			}
		}
	}
	else
	{
		system("cls");
		wprintf(L"\n dirWorks\\listFilesConsole");
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n |                                                           |");
		wprintf(L"\n |                       Error.                              |");
		wprintf(L"\n |         No matches for querry:                            |");
		wprintf(L"\n |        %s  |  %s |", path.c_str(), key.c_str());
		wprintf(L"\n |                                                           |");
		wprintf(L"\n |                                                           |");
		wprintf(L"\n -------------------------------------------------------------");
		system("pause");
		return std::wstring(L"");
	}
	return std::wstring(L"");
}

std::wstring dirWorks::BrowseFFConsole(const wchar_t * path, std::wstring key, bool consoleOutput)
{
	std::wstring temp_path(path);
	std::wstring retVal = BrowseFFConsole(temp_path, key, consoleOutput);
	return retVal;
}

std::wstring dirWorks::BrowseFFConsole(dirData_v &dir_v)
{
	//dirData_v pathList;												//vector conatining list of paths that fulfils requirements. (search path and extension)
	std::wstring retVal;											// absolute path choosed by user.
	size_t noOfPaths = 1;											// stores number of paths. Used for drawing list of paths for user to choose.
	size_t userChoice = 0;
	int searchSuccess = false;										// variable for storing whenever found a match.
	
	if (dir_v.size() > 0)
	{
		while(true)
		{
		//DRAWING LIST
		system("cls");
		wprintf(L"\n dirWorks\\listFilesConsole");
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n | Listing files:                                            |");
		for (dirData_v::iterator it = dir_v.begin(); it != dir_v.end(); it++)
		{
			wprintf(L"\n | [%d] : %s", noOfPaths, it->item_path.c_str());
			noOfPaths++;
		}
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n | Enter number of file you want to use: ");
		wscanf_s(L"%d", &userChoice);
		if (userChoice >= 1 && userChoice <= noOfPaths)
			{
				return dir_v[userChoice - 1].item_path;
			}
		else
			{
				errorBoxConsole(L"dirWorks\\listFilesConsole", L"Error. Invalid choice. Try again.");
				noOfPaths = 1;
			}
		}
	}
	else
	{
		errorBoxConsole(L"dirWorks\\listFilesConsole", L"dirData_v is empty.");
		return std::wstring(L"");
	}
	return std::wstring(L"");
}

void dirWorks::showDirDataConsole(dirData_v &itemsVector)
{
													//vector conatining list of paths that fulfils requirements. (search path and extension)
												// absolute path choosed by user.
	size_t noOfPaths = 1;											// stores number of paths. Used for drawing list of paths.
	
	if (itemsVector.size() != 0)
	{

		//DRAWING LIST
		system("cls");
		wprintf(L"\n dirWorks\\showDirDataConsole");
		wprintf(L"\n -------------------------------------------------------------");
		wprintf(L"\n | Listing requested files\folders:                          |");
		for (dirData_v::iterator it = itemsVector.begin(); it != itemsVector.end(); it++)
		{
			wprintf(L"\n | [%d] : %s", noOfPaths, it->item_path.c_str());
			noOfPaths++;
		}
		wprintf(L"\n -------------------------------------------------------------");
		system("pause");
	}
	else
	{
		errorBoxConsole(L"dirWorks\\showDirDataConsole", L"ERROR. No elements in itemsVector.");
	}
}

void dirWorks::test_foo()
{
	errorBoxConsole(L"DEBUG TITLE", L"DEBUG ERROR");
	wprintf(L"\n Hi! I'm a foo!");
}