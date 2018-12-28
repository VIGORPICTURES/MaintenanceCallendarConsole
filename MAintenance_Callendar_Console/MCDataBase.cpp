#include "MCDataBase.h"
#include "dirWorks.h"
#include <windows.h>



MCDataBase::MCDataBase()
{
	bD.extension[0] = L'%';
	bD.extension[1] = L'd';
	bD.extension[2] = L'B';

	//bD.path = new wchar_t[MAX_PATH];
	wmemset(bD.path, L'\0', MAX_PATH);
	//bD.name = new wchar_t[MAX_PATH];
	wmemset(bD.name, L'\0', MAX_PATH);
	bD.version = 1;
	//bD.numberOfVechicles = 0;
	//bD.reserved = L'R';
	vechicleNames_v.clear();
	//it_current_vechicle;
	dataBaseValid = false;
	vechicleValid = false;
}


 
MCDataBase::~MCDataBase()
{
}

void MCDataBase::addStuff(std::wstring& smth)
{
	vechicleNames_v.push_back(smth);
}

int MCDataBase::setPath(std::wstring &path)
{
	errno_t Err = 0;
	size_t pathsize = path.length();
	if (pathsize >= MAX_PATH)
	{
		SetLastError(ERROR_BAD_PATHNAME);
		return ERROR_BAD_PATHNAME;
	}
	else
	{
		wmemset(bD.path, L'\0', MAX_PATH);
		Err = wcscpy_s(bD.path, path.length()*2,path.c_str()); //"path.length()*2" because of working with wchar_t.
		if (Err != ERROR_SUCCESS)
		{
			return Err;
		}
		return 0;
	}
}

int MCDataBase::setName(std::wstring &name)
{
	errno_t Err = 0;
	size_t namesize = name.length();
	if (namesize >= MAX_PATH)
	{
		SetLastError(ERROR_BAD_PATHNAME);
		return ERROR_BAD_PATHNAME;
	}
	else
	{
		wmemset(bD.name, L'\0', MAX_PATH);
		Err = wcscpy_s(bD.name, name.length() * 2, name.c_str()); //"name.length()*2" because of working with wchar_t.
		if (Err != ERROR_SUCCESS)
		{
			return Err;
		}
		return true;
	}
}

int MCDataBase::createFile()
{
	errno_t Err = 0;
	FILE * fp;
	size_t path_valid = 0;
	path_valid = wcslen(bD.path);
	//std::cout << "\n path length: " << path_valid;
	std::wstring temp_path(bD.path);
	Err = dirWorks::makeDir(temp_path);
	if (Err != ERROR_SUCCESS)
	{
		SetLastError(Err);
		return Err;
	}
	temp_path += L"\\";
	temp_path += bD.name;
	temp_path += L".dB";
	Err = _wfopen_s(&fp, temp_path.c_str(), L"wb");
	if (Err != ERROR_SUCCESS)
	{
		SetLastError(Err);
		return Err;
	}
	else
	{
		fwrite(&bD.extension, sizeof(bD.extension), 1, fp);
		fwrite(&bD.version, sizeof(size_t), 1, fp);
		//fwrite(bD.path, sizeof(wchar_t), wcslen(bD.path), fp);
		fwrite(bD.path, sizeof(wchar_t), MAX_PATH, fp);
		//fwrite(bD.name, sizeof(wchar_t), wcslen(bD.name), fp);
		fwrite(bD.name, sizeof(wchar_t), MAX_PATH, fp);
		//fwrite(&bD.numberOfVechicles, sizeof(size_t), 1, fp);
		//fwrite(&bD.reserved, sizeof(wchar_t), 1, fp);
		//if(bD.numberOfVechicles>0)
		//{
		//fwrite(&vechicleNames_v, sizeof(vechicleNames_v)*2, 1, fp);
		//}
			fclose(fp);
			dataBaseValid = true;
			return 0;
	}	
	//return true;
}

int MCDataBase::loadFromFile(std::wstring &filepath)
{
	errno_t Err = 0;
	FILE * fp;
	Err = _wfopen_s(&fp, filepath.c_str(), L"rb");
	if (Err != ERROR_SUCCESS)
	{
		return Err;
	}
	else
	{
		fread(&bD.extension, sizeof(wchar_t), 3, fp);
		fread(&bD.version, sizeof(size_t), 1, fp);
		fread(&bD.path, sizeof(wchar_t), MAX_PATH, fp);
		fread(&bD.name, sizeof(wchar_t), MAX_PATH, fp);
		//fread(&bD.numberOfVechicles, sizeof(size_t), 1, fp);
		//fread(&bD.reserved, sizeof(wchar_t), 1, fp);
		fclose(fp);
		dataBaseValid = true;
		return 0;
	}
}

int MCDataBase::saveToFile(std::wstring& folderpath) //not used
{
	//wprintf(L"\n fullPath: %s", folderpath);
	if (folderpath[folderpath.length()] != L'\\')folderpath += L'\\';
	unsigned int LastError = 0;
	std::wstring tempPath;
	size_t foundBackSlash = 0;
	//foundBackSlash = folderpath.find_first_of(L"\\", 0); //\\in drive letter
	foundBackSlash = folderpath.find_first_of(L"\\", foundBackSlash);//\\in drive letter
	tempPath.assign(folderpath.begin(), folderpath.begin() + foundBackSlash+1); //drive root
	//wprintf(L"\n SUBPATH = %s", tempPath);
	size_t temp_pos = 0;
	int MakeDirSucess = false;
	do //no more subdirectories
	{
		temp_pos = foundBackSlash;
		foundBackSlash = folderpath.find(L"\\", foundBackSlash+1);//\\in drive letter
		if (foundBackSlash == -1) break;
		tempPath.assign(folderpath.begin(), (folderpath.begin()+temp_pos + (foundBackSlash -temp_pos))); //drive root
		MakeDirSucess = CreateDirectoryW(tempPath.c_str(), NULL);
		if (!MakeDirSucess)
		{
			LastError = GetLastError();
			if (LastError == ERROR_ALREADY_EXISTS) wprintf(L"\n Directory is already created.");
			else if (LastError == ERROR_PATH_NOT_FOUND) wprintf(L"\n Error, path not found");
			
		}
		//wprintf(L"\n SUBPATH = %s", tempPath);
	} while (true);

	folderpath += L"\\dataBase.dB";
	FILE * fp;
	errno_t err = 0;
	err = _wfopen_s(&fp, folderpath.c_str(), L"wb");
	if (err == ERROR_SUCCESS)
	{
		wprintf(L"\n do smth with databasefile");

		fclose(fp);
		return 0;
	}
	else
	{
		//error handling
		//wprintf(L"\n Error, could not open/create file");
		return err;
		//system("pause");
	}
}

int MCDataBase::searchForVechicles()
{
	errno_t Err = 0;
	MCVechicle temp_vechicle;
	dirData_v dB_v;
	size_t numberFoundFiles = 0;
	numberFoundFiles = dirWorks::searchFile(bD.path, dB_v, L"*mcv");
	if (numberFoundFiles == 0)
	{
		wprintf(L"\n No vechicles found in provided dataBase path: %s", bD.path);
		system("pause");
	}
	else
	{
		for (dirData_v::iterator it = dB_v.begin(); it != dB_v.end(); it++)
		{
			Err = temp_vechicle.loadFromFile(it->item_path);
			vechicles_v.push_back(temp_vechicle);
		}

	}
	//dirWorks::showDirDataConsole(dB_v);
	
	std::wstring currentVechicleString = dirWorks::BrowseFFConsole(dB_v);

	wprintf(L"STRING choosed: %s", currentVechicleString.c_str());
	Err = currentVechicle.loadFromFile(currentVechicleString);
	if (Err == ERROR_SUCCESS)
	{
		vechicleValid = true;
		return 0;
	}
	else
	{
		vechicleValid = false;
		return Err;
	}


	/*to do:
	Check bD.path for files with extension .mcv and push them in vechicles_v.

	size_t foundVeciclesInLocation = dB_v.size();				// howa many vechicles in vrector
	vechicles_v.resize(foundVechiclesInLocation);				//resize vector to match size of found vechicles
	vechicles_v.insert (tyle co foundVechiclesInLocation);		//fill vector with  default objects
	itemData::iterator  temp_it = dB_v.begin();							//iterator for dB_v for retrieving path
	for(MCVechicle::iterator it = vechicles_v.begin(); it != vechicles_v.end; it++)
	{
	it->loadfromFile(temp_it->path);

	//to do next. folder tree for database:
												databasefolder\\database.dB
		[examlple vechicle folder (wpr3013)]	databasefolder\\wpr3013\\wpr3013.mcv
												databasefolder\\wpr3013\\events   - here are events
												databasefolder\\wpr3013\\pictures - this is optional

												//to do next check if mcvechicle class is working. add stuff to main menu (add vechicle and event)


	*/
	return ERROR_INVALID_FUNCTION;
}

int MCDataBase::getVechicleEvents(std::vector<MCEvent> &events_v_ref)
{
	/*to do:
	For each element in vechicles_v execute get_event() and getNextEvent() functions and put them in global(?) events vector.

	*/
	return 0;
}

bool MCDataBase::checkIfValid()
{
	size_t size_val = 0;
	size_val = wcslen(bD.name);
	if (size_val > 0)size_val = wcslen(bD.path);
	if (size_val > 0)
	{
		dataBaseValid = true;
		return true;
	}
	else
	{
		dataBaseValid = false;
		return false;
	}
	return false;
}