#ifndef UTILITY_H
#define	UTILITY_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <complex>
#include <Windows.h>
#include <cstdio>
#include <shlobj.h>

char genRandom();

std::string RemoveSub(std::string& sInput, const std::string& sub);

bool hasEnding (std::string const &fullString, std::string const &ending);

bool is_file_exist(const char *fileName);

std::string generatoreRandom(int x);

int DisplayResourceNAMessageBox(std::string path, std::string path2);

bool checkExt(std::string ext);

BOOL IsAdmin();
void RequestAdminPrivileges();

BOOL SelfDelete();

std::string checkExePath();        
std::string checkUsersPath();

#endif