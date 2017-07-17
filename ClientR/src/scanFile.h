#ifndef SCANFILE_H
#define SCANFILE_H

#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <winbase.h>
#include <winreg.h>
#include <winuser.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <cstring>
#include <string>
#include <iterator>
#include <list>
#include <iostream>
#include <map>
#include <cstdlib>

#include "crypto.h"
#include "network.h"
#include "utility.h"

void leggiCritta(std::string path, std::string psw);

void leggiDecritta(std::string path, std::string psw);

#endif /* SCANFILE_H */