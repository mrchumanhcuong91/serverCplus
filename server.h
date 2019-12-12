#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
using namespace std;
//create server class:
class server
{
public:
    server(std::string ip, int port);
    void createSocketListen();
    void processType(string message, int fd);
    void saveData();
    void creatProcessing(void*);
    void sendResponse(int fd, char* response);
    ~server();
private:
    std::map<std::string, int> userStore;
    int serverSocket;
    std::mutex _mutex;
    struct sockaddr_in address_in;
    std::string ip_addr;
    std::thread t[50];
    int port;
};

#endif // SERVER_H
