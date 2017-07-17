#include "scanFile.h"
#include "crypto.h"

void leggiCritta(std::string path, std::string psw)
{
    try
    {
        std::string value;
        char character;

        std::ifstream file(path.c_str(), std::ios::binary);

        if (file.is_open())
        {
            while (file >> std::noskipws >> character)
                value = value + character;

            file.close();
        }

        Crypto<std::string> *crypto = new Crypto<std::string>();
        std::string out;
        out = crypto->Encrypt(value,psw);

        remove(path.c_str());

        std::string filePath1 = path + ".SrgYellow";
        std::ofstream fileo(filePath1.c_str(), std::ios::binary);

        if (fileo.is_open())
        {
            fileo << out;
            fileo.close();
        }
    } catch (...) 
    {

    }
}

void leggiDecritta(std::string path,std::string psw)
{
    std::string value;
    char character;
    std::string enc;
        
    std::ifstream file3(path.c_str(), std::ios::binary);

    if (file3.is_open())
    {
        while (file3 >> std::noskipws >> character)
            enc = enc + character;

        file3.close();
    }

    Crypto<std::string> *crypto = new Crypto<std::string>();
    value = crypto->Decrypt(enc, psw);
    remove(path.c_str());
        
    std::string filePath1 = RemoveSub(path, ".SrgYellow");
    std::ofstream file4(filePath1.c_str(), std::ios::binary);

    if (file4.is_open())
    {
        file4 << value;
        file4.close();
    }
}