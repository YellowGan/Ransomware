#include "request.h"
#include "crypto.h"

std::string key = "sergio";
std::string port = "8080";
std::string serverAddress = "192.168.27.1";

Network network(key.c_str());

std::string createId(std::string password)
{
    Crypto<std::string> *crypto = new Crypto<std::string>();
    std::string passwordCrypto = crypto->Encrypt(password, key);
    std::string responsecreateIdKey = "";

    while((responsecreateIdKey == "") || (responsecreateIdKey == "Error"))
    {
        std::string reqcreateIdKey = "GET /Server/createIdKey?pass=" +passwordCrypto + "\r\nHTTP/1.1\r\nHost: " + serverAddress + "\r\nConnection: close\r\n\r\n";
        responsecreateIdKey = network.RawRequest(serverAddress.c_str(), port.c_str(), reqcreateIdKey.c_str());
        responsecreateIdKey = crypto->Decrypt(responsecreateIdKey, key);
        
        Sleep(3000);
    }
    
    return responsecreateIdKey;
}

void payment(std::string IdSession)
{
    Crypto<std::string> *crypto = new Crypto<std::string>();
    std::string IdSessionCrypto = crypto->Encrypt(IdSession, key);
    std::string responsePay="";

    while(responsePay==""|| responsePay == "Error")
    {
        std::string reqPay = "GET /Server/payment?Id=" + IdSessionCrypto + "\r\nHTTP/1.1\r\nHost: " + serverAddress + "\r\nConnection: close\r\n\r\n";
        responsePay = network.RawRequest(serverAddress.c_str(), port.c_str(), reqPay.c_str());
        responsePay = crypto->Decrypt(responsePay, key);
        
        Sleep(3000);
    }
}

std::string checkPayment (std::string IdSession)
{
    Crypto<std::string> *crypto = new Crypto<std::string>();
    std::string IdSessionCrypto = crypto->Encrypt(IdSession, key); 
    std::string response = "";

    while(!((response == "fals") || (response == "true")))
    {
        std::string req = "GET /Server/checkPayment?Id=" + IdSessionCrypto + "\r\nHTTP/1.1\r\nHost: " + serverAddress + "\r\nConnection: close\r\n\r\n";
        response = network.RawRequest(serverAddress.c_str(), port.c_str(), req.c_str());
        response = crypto->Decrypt(response, key);
        if ((response.size())>0)
            response = response.erase(4);
        
        Sleep(3000);        
    }
    
    return response;
}

void deleteData (std::string IdSession)
{
    Crypto<std::string> *crypto = new Crypto<std::string>();
    std::string IdSessionCrypto = crypto->Encrypt(IdSession, key); 
    std::string responseDelete = "";

    while(responseDelete == "" || responseDelete == "Error")
    {
        std::string reqDelete = "GET /Server/delete?Id=" + IdSessionCrypto + "\r\nHTTP/1.1\r\nHost: " + serverAddress + "\r\nConnection: close\r\n\r\n";
        responseDelete = network.RawRequest(serverAddress.c_str(), port.c_str(), reqDelete.c_str()); 
        responseDelete = crypto->Decrypt(responseDelete, key);
        
        Sleep(3000);
    }
}