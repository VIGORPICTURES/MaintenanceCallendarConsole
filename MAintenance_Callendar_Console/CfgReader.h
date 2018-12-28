#pragma once
#include "fstream"
#include <iostream>
#include <string>


class CfgReader
{
public:
	FILE *fp;
	const char * cfg_filename;
	size_t filesize;
	std::string search_val_s;
	
	int int_val;
	float float_val;
	char char_val;
	bool bool_val;
	long long_val;
	std::string string_val;
	size_t size_t_val;

	bool error_state;

public:
	//file operations
	void OpenFile(const char * filename);
	void OpenFile(std::string filename);
	void CloseFile();
	void Search(const char * search_val);
	
	//search and return operations 
	int r_int(const char * search_val);
	char r_char(const char * search_val);
	float r_float(const char * search_val);
	size_t r_size_t(const char * search_val);
	long r_long(const char * search_val);
	bool r_bool(const char * search_val);
	std::string r_string(const char * search_val);
	
	int r_int(std::string search_val);
	char r_char(std::string search_val);
	float r_float(std::string search_val);
	size_t r_size_t(std::string search_val);
	long r_long(std::string search_val);
	bool r_bool(std::string search_val);
	std::string r_string(std::string search_val);

	//debug
	void Show_content(char display_type = 's'); // s- as humans see files, f - with all characters

	//constructor/destructor/other
	CfgReader();
	CfgReader(const char *);
	~CfgReader();
	void zero_values();
	const char * string_to_constchar(std::string s_value);

	/*
	void Search(const char * search_val, int &ref_val);
	void Search(const char * search_val, float &ref_val);
	void Search(const char * search_val, char &ref_val);
	void Search(const char * search_val, bool &ref_val);
	void Search(const char * search_val, long &ref_val);
	void Search(const char * search_val, std::string &ref_val);
	void Search(const char * search_val, size_t &ref_val);
	*/
};

