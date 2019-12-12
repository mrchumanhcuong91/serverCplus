#include "client.h"
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
client::client(string ip, int _port)
{
    ip_client = ip;
    port = _port;
    clientSocket = -1;
}
int client::connectServer(){
    //
    //make a socket , bind to address and connect, send data
//    const char* message = "{\"type\":\"login\",\"names\":Cuong}";

    clientSocket = socket(PF_INET,SOCK_STREAM,0);
    //create IP addr
    struct sockaddr_in client_addr;
    socklen_t addr_size;
    addr_size = sizeof client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    client_addr.sin_addr.s_addr = inet_addr(ip_client.c_str());
    memset(client_addr.sin_zero, '\0', sizeof client_addr.sin_zero);
    //connect socket to iP
    int status = connect(clientSocket, (struct sockaddr*)&client_addr,addr_size);
    if(status == -1)
        return -1;
    else
        return clientSocket;
//    sprintf(message,"Client %d :",id_client);

}
void client::onReceiveMessageCb(){
    cout<<"onReceiveMessageCb"<<endl;
    const char* message2 = "{\"project\":\"hello server\",\"stars\":11}";;
    bool isOut = false;
    while(!isOut){
        char buffer[1024];
        memset(buffer,0, 1024);
        if(recv(clientSocket, buffer, 1024, 0) < 0){
            printf("Read data from server failed\n");
        }

        Document d;
        d.Parse(buffer);
        Value& type = d["stars"];
        Value& symbol = d["project"];
        switch (type.GetInt()) {
            case 10/*"login"*/:
                cout<<"client receiver login success "<<type.GetString()<<endl;
                //send hello server

                break;
            case 12/*"close"*/:
                close(clientSocket);
                cout<<"quit receiver"<<endl;
                isOut = true;
                break;
            case 13:
                cout<<"client receiver "<<symbol.GetString()<<endl;
                clientSocket = -1;
                sendMessage(message2);
                break;
            default:
                break;
        }

    }
}

void client::sendMessage(const char *content){
    cout<<"client send "<<content<< " to "<<clientSocket<<endl;
    if(clientSocket == -1)
        clientSocket = connectServer();
    if(send(clientSocket, content, strlen(content), 0) < 0){
        printf("send from client error\n");

    }
}

