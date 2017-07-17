#ifndef SCANPC_H
#define SCANPC_H

#include <Windows.h>
#include <sys/stat.h>
#include <vector>
#include <iostream>

#include "scanFile.h"
#include "utility.h"

std::vector<std::string> GetFileNamesInDirectory(std::string directory);

void ricerca (std::string path, std::string psw, bool esiste);

int controllo(std::string path);

#endif /* SCANPC_H */