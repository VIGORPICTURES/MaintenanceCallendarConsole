#include "CfgReader.h"
#pragma warning(disable: 4996)

CfgReader::CfgReader()
{
	error_state = false;
	CfgReader("");
}

void CfgReader::OpenFile(std::string filename)
{
	/*
	char * chararr = new char[filename.length()];
	//std::cout << "\n TEST: "<< filename.length()<<std::endl;
	for (int i = 0; i <= filename.length(); i++)
	{
		chararr[i] = filename[i];
		//std::cout << chararr[i];
	}
	//std::cout << "\n//TEST";
	cfg_filename = chararr;
	*/
	OpenFile(string_to_constchar(filename));
}

CfgReader::CfgReader(const char * filename)
{
	
	if (filename=="")
	{
		//debug only
		//std::cout << "\n CfgReader: INFO: No filename provided. (You sure what You doin?).";
		//system("pause");
		//debug only
		cfg_filename = "";
		fp = NULL;
		
	}
	else
	{
		zero_values();
		cfg_filename = filename;
		OpenFile(cfg_filename);
	}
}

void CfgReader::OpenFile(const char * filename)
{
	fp = fopen(filename, "rb");
	if (fp != NULL)
	{
		std::cout << "\n CfgReader: INFO: Opened file: " << filename;
		rewind(fp);
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		rewind(fp);
		cfg_filename = filename;
	}
	else
	{
		std::cout << "\n CfgReader: ERROR: Could not open file: "<< filename <<". (Check path or filename).";
		error_state = true;
		std::cout << "\n";
		system("pause");
	}

	//std::cout << "\n filesize: " << filesize;
	//system("pause");
}

void CfgReader::Search(const char * search_val)
{
	size_t search_val_length = 0;
	for (search_val_length; search_val[search_val_length] != '\0'; search_val_length++);
	std::string test;
	if (fp == NULL)
	{
		std::cout << "\n CfGReader: ERROR: File not loaded! Nothing to read in. (Check for file).";
		std::cout << "\n";
		system("pause");
	}
	else
	{
		char first_letter = 0;
		bool match = false;
		size_t counter = 0;
		rewind(fp);
		while (match == false)
		{
			while (first_letter != 13)
			{
				fread(&first_letter, sizeof(char), 1, fp);
			}
			if (first_letter == 13)
			{
				fread(&first_letter, sizeof(char), 1, fp);
				if (first_letter == 10)
				{
					do
					{
						fread(&first_letter, sizeof(char), 1, fp);
						if (first_letter == search_val[counter])
						{
							test += first_letter;
							counter++;
							if(counter==search_val_length)
							{
								match = true;
								break;
							}	
						}
						else
						{
							counter = 0;
							test = "";
							break;
						}
					} while (first_letter != 61);
					if (test == search_val || match == true)
					{
						search_val_s = search_val;
						match = true;
						break;
					}
				}
			}
			//std::cout << "\n ftell(fp): " << ftell(fp); //check if file pointer is working on file
			if (ftell(fp) == filesize) break;
		}
		if (match == false)
		{
			std::cout << "\n CfgReader: INFO: No such a VALUE( "<< search_val<<" ) in file: "<< cfg_filename;
			std::cout << "\n";
			system("pause");
		}
		else
		{
			test = "";
			while (first_letter != 13)
			{
				fread(&first_letter, sizeof(char), 1, fp);
				if (first_letter == 61)continue;
				if (first_letter == 10 || first_letter == 13) break;
				test += first_letter;
			}
			//std::cout << "\n test string  VALUE (string):" << test << std::endl; //check if value found is okay
			switch (search_val_s[0])
			{
			case 'i':
				int_val = std::stoi(test);
				//std::cout << "\n int_val: " << int_val; //check if conversion from string to int is successful
				break;
			case 'f':
				float_val = std::stof(test);
				//std::cout << "\n float_val: " << float_val; //check if conversion from string to float is successful
				break;
			case 'b':
				if (test == "false" || test[0] == '0')bool_val = false;
				else if  (test == "true" || test[0] == '1' )bool_val = true;
				else
				{
					std::cout << "\n CfgReader: INFO: Bool_val readed from file: "<<cfg_filename<<" is invalid, setting bool_val to false.";
					bool_val = false;
				}
				//std::cout << "\n bool_val: " << bool_val;//check if conversion from string to bool is successful
				break;
			case 'l':
				long_val = std::stol(test);
				//std::cout << "\n long_val: " << long_val;//check if conversion from string to long is successful
				break;
			case 's':
				string_val = test;
				//	std::cout << "\n string_val: " << string_val; //check if copy from string to string is successful
				break;
			case 't':
				size_t_val = std::stoul(test);
				//std::cout << "\n size_t_val: " << size_t_val; //check if conversion from string to size_t is successful	
				break;
			}		
		}
	}
}

int CfgReader::r_int(const char * search_val)
{
	Search(search_val);
	return int_val;
}
int CfgReader::r_int(std::string search_val)
{
	Search(string_to_constchar(search_val));
	return int_val;
}

char CfgReader::r_char(const char * search_val)
{
	Search(search_val);
	return char_val;
}
char CfgReader::r_char(std::string search_val)
{
	Search(string_to_constchar(search_val));
	return char_val;
}
float CfgReader::r_float(const char * search_val)
{
	Search(search_val);
	return float_val;
}
float CfgReader::r_float(std::string search_val)
{
	Search(string_to_constchar(search_val));
	return float_val;
}
size_t CfgReader::r_size_t(const char * search_val)
{
	Search(search_val);
	return size_t_val;
}
size_t CfgReader::r_size_t(std::string search_val)
{
	Search(string_to_constchar(search_val));
	return size_t_val;
}
long CfgReader::r_long(const char * search_val)
{
	Search(search_val);
	return long_val;
}
long CfgReader::r_long(std::string search_val)
{
	Search(string_to_constchar(search_val));
	return long_val;
}
bool CfgReader::r_bool(const char * search_val)
{
	Search(search_val);
	return bool_val;
}
bool CfgReader::r_bool(std::string search_val)
{
	Search(string_to_constchar(search_val));
	return bool_val;
}
std::string CfgReader::r_string(const char * search_val)
{
	Search(search_val);
	return string_val;
}
std::string CfgReader::r_string(std::string search_val)
{
	Search(string_to_constchar(search_val));
	return string_val;
}

void CfgReader::Show_content(char display_type)
{
	std::cout << "\n //--------------------------CfgReader.Show_content()------------------------\\"<<std::endl;
	std::cout << "\n //--------------------------filename: "<<cfg_filename<<"------------------------\\" << std::endl;
	if (fp != NULL)
	{
		rewind(fp);
		fseek(fp, 0, SEEK_END);
		int filesize = ftell(fp);
		char literka;
		rewind(fp);
		while (ftell(fp) != filesize)
		{
			fread(&literka, sizeof(char), 1, fp);
			if(display_type=='s')std::cout << literka;
			if(display_type=='f')std::cout << "\n" << (short)literka << " - " << literka;
		}
	}
	else
	{
		std::cout << "\n CfgReader: ERROR: fp == NULL. No file opened. (Check for file).";
	}
	std::cout << "\n \\--------------------------/CfgReader.Show_content()------------------------// \n";
}

void CfgReader::CloseFile()
{
	std::cout << "\n CfgReader: INFO: Closed file: " << cfg_filename;
	fclose(fp);
}

CfgReader::~CfgReader()
{
	//nothing to do here (yet?)
}

void CfgReader::zero_values()
{
	search_val_s = "";
	int_val = NULL;
	float_val = NULL;
	char_val = NULL;
	bool_val = false;
	long_val = NULL;
	string_val = "";
	size_t_val = NULL;
	cfg_filename = "";
	error_state = false;
}

const char * CfgReader::string_to_constchar(std::string s_value)
{
	char * chararr = new char[s_value.length()];
	for (unsigned int i = 0; i <= s_value.length(); i++)
	{
		chararr[i] = s_value[i];
	}
	return chararr;
}

/*
void CfgReader::Search(const char * search_val) // master function - old. Check with used one.
{
size_t search_val_length = 0;
for (search_val_length; search_val[search_val_length] != '\0'; search_val_length++);
std::string test;
if (fp == NULL)
{
std::cout << "\n Error, file not loaded!";
}
else
{
char first_letter = 0;
bool match = false;
size_t counter = 0;
rewind(fp);
while (match == false)
{
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
}
if (first_letter == 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 10)
{
do
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == search_val[counter])
{
test += first_letter;
counter++;
if(counter==search_val_length)
{
match = true;
break;
}
}
else
{
counter = 0;
test = "";
break;
}
} while (first_letter != 61);
if (test == search_val || match == true)
{
match = true;
break;
}
}
}
std::cout << "\n ftell(fp): " << ftell(fp);
if (ftell(fp) == filesize) break;
}
if (match == false)
{
std::cout << "\n ERROR, no such VALUE!";
}
else
{
std::cout << "\n test string = " << test;
//system("pause");
//Show_content();
///line = test;
///test = "";
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 61)continue;
test += first_letter;
}
std::cout << "\n test string  VALUE (string):" << test << std::endl;
switch (line[0])
{
case 'i':
int_val = std::stoi(test);
std::cout << "\n int_val: " << int_val;
//return int_val;
//a = int_val;
break;
case 'f':
float_val = std::stof(test);
std::cout << "\n float_val: " << float_val;
//return float_val;
//a = float_val;
break;
case 'b':
if (test[0] == 'f' || test[0] == '0')	bool_val = false;
else bool_val = true;
std::cout << "\n bool_val: " << bool_val;
//return bool_val;
//a = bool_val;
break;
case 'l':
long_val = std::stol(test);
std::cout << "\n long_val: " << long_val;
//return long_val;
//a = long_val;
break;
case 's':
string_val = test;
std::cout << "\n string_val: " << string_val;
//return string_val;
//	a = string_val;
break;
case 't':
size_t_val = std::stoul(test);
std::cout << "\n size_t_val: " << size_t_val;
//return size_t_val;
//a = size_t_val;
break;

}

}
}
}
*/

/*//not working functions. Do not use.

void CfgReader::Search(const char * search_val, int &ref_val)
{

size_t search_val_length = 0;
for (search_val_length; search_val[search_val_length] != '\0'; search_val_length++);
std::string test;
if (fp == NULL)
{
std::cout << "\n Error, file not loaded!";
}
else
{
char first_letter = 0;
bool match = false;
size_t counter = 0;
rewind(fp);
while (match == false)
{
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
}
if (first_letter == 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 10)
{
do
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == search_val[counter])
{
test += first_letter;
counter++;
if (counter == search_val_length)
{
match = true;
break;
}
}
else
{
counter = 0;
test = "";
break;
}
} while (first_letter != 61);
if (test == search_val || match == true)
{
match = true;
break;
}
}
}
//std::cout << "\n ftell(fp): " << ftell(fp);
if (ftell(fp) == filesize) break;
}
if (match == false)
{
std::cout << "\n ERROR, no such VALUE!";
}
else
{
std::cout << "\n [INT] FULLstring = " << test;

test = "";
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 61)continue;
if (first_letter == 10 || first_letter == 13) break;
test += first_letter;
}
std::cout << " , VALUE(string) = " << test;
int_val = std::stoi(test);
//ref_val = int_val;
std::cout << " , VALUE(int) = " << int_val;

}
}
}

void CfgReader::Search(const char * search_val, float &ref_val)
{

size_t search_val_length = 0;
for (search_val_length; search_val[search_val_length] != '\0'; search_val_length++);
std::string test;
if (fp == NULL)
{
std::cout << "\n Error, file not loaded!";
}
else
{
char first_letter = 0;
bool match = false;
size_t counter = 0;
rewind(fp);
while (match == false)
{
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
}
if (first_letter == 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 10)
{
do
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == search_val[counter])
{
test += first_letter;
counter++;
if (counter == search_val_length)
{
match = true;
break;
}
}
else
{
counter = 0;
test = "";
break;
}
} while (first_letter != 61);
if (test == search_val || match == true)
{
match = true;
break;
}
}
}
//std::cout << "\n ftell(fp): " << ftell(fp);
if (ftell(fp) == filesize) break;
}
if (match == false)
{
std::cout << "\n ERROR, no such VALUE!";
}
else
{
std::cout << "\n [FLOAT] FULLstring = " << test;

test = "";
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 61)continue;
if (first_letter == 10 || first_letter == 13) break;
test += first_letter;
}
std::cout << " , VALUE(string) = " << test;
float_val = std::stof(test);
ref_val = float_val;
std::cout << " , VALUE(float) = " << float_val;
}
}
}

void CfgReader::Search(const char * search_val, char &ref_val)
{

size_t search_val_length = 0;
for (search_val_length; search_val[search_val_length] != '\0'; search_val_length++);
std::string test;
if (fp == NULL)
{
std::cout << "\n Error, file not loaded!";
}
else
{
char first_letter = 0;
bool match = false;
size_t counter = 0;
rewind(fp);
while (match == false)
{
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
}
if (first_letter == 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 10)
{
do
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == search_val[counter])
{
test += first_letter;
counter++;
if (counter == search_val_length)
{
match = true;
break;
}
}
else
{
counter = 0;
test = "";
break;
}
} while (first_letter != 61);
if (test == search_val || match == true)
{
match = true;
break;
}
}
}
//std::cout << "\n ftell(fp): " << ftell(fp);
if (ftell(fp) == filesize) break;
}
if (match == false)
{
std::cout << "\n ERROR, no such VALUE!";
}
else
{
std::cout << "\n [char] FULLstring = " << test;

test = "";
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 61)continue;
if (first_letter == 10 || first_letter == 13) break;
test += first_letter;
}
std::cout << " , VALUE(string) = " << test;
char_val = test[0];
ref_val = char_val;
std::cout << " , VALUE(char) = " << char_val;
}
}
}

void CfgReader::Search(const char * search_val, bool &ref_val)
{

size_t search_val_length = 0;
for (search_val_length; search_val[search_val_length] != '\0'; search_val_length++);
std::string test;
if (fp == NULL)
{
std::cout << "\n Error, file not loaded!";
}
else
{
char first_letter = 0;
bool match = false;
size_t counter = 0;
rewind(fp);
while (match == false)
{
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
}
if (first_letter == 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 10)
{
do
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == search_val[counter])
{
test += first_letter;
counter++;
if (counter == search_val_length)
{
match = true;
break;
}
}
else
{
counter = 0;
test = "";
break;
}
} while (first_letter != 61);
if (test == search_val || match == true)
{
match = true;
break;
}
}
}
//std::cout << "\n ftell(fp): " << ftell(fp);
if (ftell(fp) == filesize) break;
}
if (match == false)
{
std::cout << "\n ERROR, no such VALUE!";
}
else
{
std::cout << "\n [BOOL] FULLstring = " << test;

test = "";
while (first_letter != 13)
{

fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 61)continue;
if (first_letter == 10 || first_letter == 13) break;
//if (first_letter != 10)
test += first_letter;
}
std::cout << " , VALUE(string) = " << test;
if (test == "false" || test == "0") bool_val = false;
else if (test == "true" || test == "1") bool_val = true;
else
{
system("pause");
bool_val = false;
std::cout << "\n Error: bool_val readed from file is invalid, setting bool_val to false.";
}

ref_val = bool_val;
std::cout << " , VALUE(bool) = " << bool_val;
}
}
}

void CfgReader::Search(const char * search_val, long &ref_val)
{

size_t search_val_length = 0;
for (search_val_length; search_val[search_val_length] != '\0'; search_val_length++);
std::string test;
if (fp == NULL)
{
std::cout << "\n Error, file not loaded!";
}
else
{
char first_letter = 0;
bool match = false;
size_t counter = 0;
rewind(fp);
while (match == false)
{
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
}
if (first_letter == 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 10)
{
do
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == search_val[counter])
{
test += first_letter;
counter++;
if (counter == search_val_length)
{
match = true;
break;
}
}
else
{
counter = 0;
test = "";
break;
}
} while (first_letter != 61);
if (test == search_val || match == true)
{
match = true;
break;
}
}
}
//std::cout << "\n ftell(fp): " << ftell(fp);
if (ftell(fp) == filesize) break;
}
if (match == false)
{
std::cout << "\n ERROR, no such VALUE!";
}
else
{
std::cout << "\n [LONG] FULLstring = " << test;

test = "";
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 61)continue;
if (first_letter == 10 || first_letter == 13) break;
test += first_letter;
}
std::cout << " , VALUE(string) = " << test;
long_val = std::stol(test);
ref_val = long_val;
std::cout << " , VALUE(long) = " << long_val;
}
}
}

void CfgReader::Search(const char * search_val, std::string  &ref_val)
{

size_t search_val_length = 0;
for (search_val_length; search_val[search_val_length] != '\0'; search_val_length++);
std::string test;
if (fp == NULL)
{
std::cout << "\n Error, file not loaded!";
}
else
{
char first_letter = 0;
bool match = false;
size_t counter = 0;
rewind(fp);
while (match == false)
{
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
}
if (first_letter == 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 10)
{
do
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == search_val[counter])
{
test += first_letter;
counter++;
if (counter == search_val_length)
{
match = true;
break;
}
}
else
{
counter = 0;
test = "";
break;
}
} while (first_letter != 61);
if (test == search_val || match == true)
{
match = true;
break;
}
}
}
//std::cout << "\n ftell(fp): " << ftell(fp);
if (ftell(fp) == filesize) break;
}
if (match == false)
{
std::cout << "\n ERROR, no such VALUE!";
}
else
{
std::cout << "\n [STRING] FULLstring = " << test;

test = "";
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 61)continue;
if (first_letter == 10 || first_letter == 13) break;
test += first_letter;
}
std::cout << " , VALUE(string) = " << test;
string_val = test;
ref_val = string_val;
std::cout << " , VALUE(string) = " << string_val;
}
}
}

void CfgReader::Search(const char * search_val, size_t &ref_val)
{

size_t search_val_length = 0;
for (search_val_length; search_val[search_val_length] != '\0'; search_val_length++);
std::string test;
if (fp == NULL)
{
std::cout << "\n Error, file not loaded!";
}
else
{
char first_letter = 0;
bool match = false;
size_t counter = 0;
rewind(fp);
while (match == false)
{
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
}
if (first_letter == 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 10)
{
do
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == search_val[counter])
{
test += first_letter;
counter++;
if (counter == search_val_length)
{
match = true;
break;
}
}
else
{
counter = 0;
test = "";
break;
}
} while (first_letter != 61);
if (test == search_val || match == true)
{
match = true;
break;
}
}
}
//std::cout << "\n ftell(fp): " << ftell(fp);
if (ftell(fp) == filesize) break;
}
if (match == false)
{
std::cout << "\n ERROR, no such VALUE!";
}
else
{
std::cout << "\n [size_t] FULLstring = " << test;

test = "";
while (first_letter != 13)
{
fread(&first_letter, sizeof(char), 1, fp);
if (first_letter == 61)continue;
if (first_letter == 10 || first_letter == 13) break;
test += first_letter;
}
std::cout << " , VALUE(string) = " << test;
size_t_val = std::stoul(test);
ref_val = size_t_val;
std::cout << " , VALUE(size_t) = " << size_t_val;
}
}
}

*/

