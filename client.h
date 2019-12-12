#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
using namespace std;

class client
{
private:
    string ip_client;
    int port;
public:
    client(string ip, int _port);
    int clientSocket;
    string buffer;
    int connectServer();
    void sendMessage(const char* content);
    void onReceiveMessageCb();

};

#endif // CLIENT_H
