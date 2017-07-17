#include "network.h"
#include "crypto.h"

Server::Server(const char* encryptionKeyArg)
{
    encryptionKey = encryptionKeyArg;
    port = "none";
    hThread = NULL;
    listenSocket = INVALID_SOCKET;

    sta.encryptionKey = encryptionKeyArg;
    sta.ListenSocket = INVALID_SOCKET;
}

Server& Server::operator=(const Server& server) 
{
    encryptionKey = server.encryptionKey;
    port = server.port;
    hThread = server.hThread;
    listenSocket = server.listenSocket;

    sta.encryptionKey = server.sta.encryptionKey;
    sta.ListenSocket = server.sta.ListenSocket;
    return *this;
}

Network& Network::operator=(const Network& network) 
{
    encryptionKey = network.encryptionKey;
    server = network.server;
    return *this;
}

Network::Network(const char* encryptionKeyArg) 
{
    encryptionKey = encryptionKeyArg;
    server = Server(encryptionKeyArg);
}

std::string Network::RawRequest(const char* serverAddress, const char* port, const char* request) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    const char* sendbuf = request;
    const int bufferlength = 512;
    char recvbuf[bufferlength];
    int iResult;
    std::string response;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) 
        return "Error";

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(serverAddress, port, &hints, &result);
    if (iResult != 0)
    {
        WSACleanup();
        return "Error";
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) 
    {
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) 
        {
            WSACleanup();
            return "Error";
        }

        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) 
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) 
    {
        WSACleanup();
        return "Error";
    }

    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) 
    {
        closesocket(ConnectSocket);
        WSACleanup();
        return "Error";
    }

    do 
    {
        iResult = recv(ConnectSocket, recvbuf, bufferlength, 0);
        if (iResult > 0) 
            response.append(recvbuf);
    } while (iResult > 0);

    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) 
    {
        closesocket(ConnectSocket);
        WSACleanup();
        return "Error";
    }

    closesocket(ConnectSocket);
    WSACleanup();

    return response;
}

const char* Network::ResolveAddress(const char *address) 
{
    const char* addressIP = "";
    WSADATA wsaData;
    struct hostent *remoteHost;
    char *host_name;
    struct in_addr addr;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    host_name = (char *) address;
    remoteHost = gethostbyname(host_name);
    
    if (remoteHost != NULL) 
    {
        if (remoteHost->h_addrtype == AF_INET) 
        {
            int i = 0;
            while (remoteHost->h_addr_list[i] != 0) 
            {
                addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
                addressIP = inet_ntoa(addr);
                break;
            }
        }
    }
    return addressIP;
}

void Server::Start(const char* portArg, bool encrypted) 
{
    DWORD dwThreadId;
    port = portArg;

    if (encrypted)
        sta.encryptionKey = encryptionKey;
    else
        sta.encryptionKey = NULL;

    SOCKET listenSocketTmp = INVALID_SOCKET;

    WSADATA wsaData;
    int iResult;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, portArg, &hints, &result);
    if (iResult != 0) 
        WSACleanup();

    listenSocketTmp = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocketTmp == INVALID_SOCKET) 
    {
        freeaddrinfo(result);
        WSACleanup();
    }

    iResult = bind(listenSocketTmp, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
    {
        freeaddrinfo(result);
        closesocket(listenSocketTmp);
        WSACleanup();
    }

    freeaddrinfo(result);

    iResult = listen(listenSocketTmp, SOMAXCONN);
    if (iResult == SOCKET_ERROR) 
    {
        closesocket(listenSocketTmp);
        WSACleanup();
    }

    listenSocket = listenSocketTmp;
    sta.ListenSocket = listenSocketTmp;

    hThread = CreateThread(0, 0, &ServerThread, (void*)&sta, 0, &dwThreadId);
}

void Server::Stop() 
{
    port = "none";

    closesocket(listenSocket);
    WSACleanup();

    DWORD dwExit;
    GetExitCodeThread(hThread, &dwExit);
    TerminateThread(hThread, dwExit);
}

HANDLE Server::getHhread() 
{
    return hThread;
}

const char* Server::getPort() 
{
    return port;
}

DWORD WINAPI ServerThread(void* arg) 
{
    const char* encryptionKey = (const char*)((*((ServerThreadArgs*)arg)).encryptionKey);
    SOCKET ListenSocket = (SOCKET)((*((ServerThreadArgs*)arg)).ListenSocket);

    SOCKET ClientSocket = INVALID_SOCKET;
    while (true) 
    {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) 
        {
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        } 
        else 
        {
            ClientSocketThreadArgs csa;
            csa.ClientSocket = ClientSocket;
            csa.encryptionKey = encryptionKey;

            CreateThread(0, 0, &ClientSocketThread,(void*)&csa, 0, NULL);
        }
    }
    return 0;
}


DWORD WINAPI ClientSocketThread(void* arg) 
{
    ClientSocketThreadArgs csta = *((ClientSocketThreadArgs*)arg);
    SOCKET ClientSocket = csta.ClientSocket;    
    Crypto<std::string> *crypto = new Crypto<std::string>();
    
    const char* encryptionKey = csta.encryptionKey;

    const int bufferlength = 512;
    int iResult;

    std::string command;
    int iSendResult;
    char recvbuf[bufferlength];
    int recvbuflen = bufferlength;
    std::size_t posSubStr;

    send(ClientSocket, "\nConnection Established\n",24, 0);

    do 
    {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        command.append(recvbuf);

        if (encryptionKey != NULL) 
            command = crypto->Decrypt(command, encryptionKey);

        posSubStr = command.find("\n");
        if (posSubStr != std::string::npos) 
        {
            if (iSendResult == SOCKET_ERROR) 
            {
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
        }
        command = "";
        command.clear();

    } while (iResult > 0);

    send(ClientSocket, "\nConnection Stopped\n",20, 0);

    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) 
    {
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    return 0;
}