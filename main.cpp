#include <iostream>
#include <server.h>
#include <client.h>
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    const char* message = "{\"project\":\"rapidjson\",\"stars\":10}";
    server * newServer = new server("127.0.0.1",7777);
    client * cl = new client("127.0.0.1",7777);
    std::thread thServer(&server::createSocketListen, newServer);
//    newServer->createSocketListen();
    int fd =cl->connectServer();
    if(fd != -1){

        cl->sendMessage(message);

        cl->onReceiveMessageCb();
    }
    thServer.join();
    delete newServer;
    delete cl;
    return 0;
}
//#include <iostream>
//#include <thread>
//using namespace std;
//void LaunchRocket()
//{
//    cout << "Launching Rocket" << endl;
//}
//int main(){
//    thread t1(LaunchRocket);
//    //t1.join(); // somehow we forgot to join this to main thread - will cause a crash.
//    return 0;
//}
