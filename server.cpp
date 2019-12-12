#include "server.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <string.h>
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"
using namespace rapidjson;
server::server(std::string ip, int port)
{
    address_in.sin_family = AF_INET;
    address_in.sin_port = htons(port);
    address_in.sin_addr.s_addr = inet_addr(ip.c_str());
    memset(address_in.sin_zero,'\0',sizeof(address_in.sin_zero));
}
server::~server(){
    close(serverSocket);
}

//std::map<std::string, int> server::dataStore = {"",-1};
void server::creatProcessing(void* arg){
    int newSocket = *((int*)arg);
    //lock
    char client_mess[1024];

    recv(newSocket, client_mess, 1024, 0);
    cout<<"server read from "<<newSocket<<endl;
    processType(string(client_mess), newSocket);
//    std::string talk(client_mess);
//    cout <<"client : "<<std::this_thread::get_id() <<"say : "<<talk<<endl;

}
void server::sendResponse(int fd, char *response){
    send(fd, response, strlen(response), 0);
}

void server::processType(string message, int fd){
    cout<< "processType :  "<<message<<endl;
    const char* mess = message.c_str();
    Document d;
    d.Parse(mess);
    //get start value
    Value& s = d["stars"];
    Value& name = d["project"];
    cout<< "server : type "<<s.GetInt()<<endl;

    char* message1 = "{\"project\":\"rapidjson\",\"stars\":12}";
    char* message13 = "{\"project\":\"connected\",\"stars\":13}";
    switch (s.GetInt()) {
    case 10/*"login"*/:
        userStore.insert(std::pair<string, int>(string(name.GetString()), fd));
        sendResponse(fd, message13);
        break;
    case 11:
        cout<< "server Receive "<<name.GetString()<<endl;
        sendResponse(fd, message1);
        break;
    default:
        break;
    }
}

void server::createSocketListen(){
    cout <<"createSocketListen"<<endl;
    struct sockaddr_storage addr_store;
    socklen_t addr_size;
    //create socket
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    //bind socket
    bind(serverSocket,(struct sockaddr*)&address_in, sizeof(address_in));
    //listen
    if(listen(serverSocket, 40)<-1){
        cout <<"listen socket error"<<endl;
        return;
    }
//    cout <<"createSocketListen 49"<<endl;
    //loop and wait for new incomming connection
//    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
//    Document d;
//    d.Parse(json);
//    //get start value
//    Value& s = d["stars"];
//    cout <<"stars value :"<<s.GetInt()<<endl;
    int i =0;
    while(1){
        //creat acception
        addr_size = sizeof addr_store;
        cout<<"server : still accept"<<endl;
        int newSocket = accept(serverSocket, (struct sockaddr*)&addr_store, &addr_size);

        //then create new thread
        t[i]= std::thread(&server::creatProcessing,this, (void*)&newSocket);
        i++;
        cout<<"server : still accept i="<<i<<endl;
        if(i >=20){
            i =0;
            while(i <20){
                t[i].join();
                i++;
            }
            break;
        }

    }
}
