#include <fcntl.h>
#include "SocketServer.h"
#include "SocketClient.h"


using namespace std;


void SocketServer::run() {
    init();
    
    while(1) {
        handle_connection();
    }
}


void SocketServer::init() {
    int s;
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SOCK_PORT);
    
    sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd_ < 0) {
        cout << "ERROR opening socket, quitting..." << endl;
        exit(1);
    }
    
    s = bind(sock_fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (s < 0) {
        cout << "ERROR on binding" << endl;
        exit(1);
    }
    
    listen(sock_fd_, 5);
}


void SocketServer::handle_connection() {
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    SocketClient* client;
    
    // Accept is blocking untill a new client connects
    int client_sock_fd = accept(sock_fd_, (struct sockaddr *) &cli_addr, &clilen);
    if (client_sock_fd < 0) {
        cout << "ERROR on accept" << endl;
        return;
    }
    
    // Spawn client thread
    client = new SocketClient(bridge_, client_sock_fd);
    client->start();
    
    // Add client to list of active (connected) clients
    clients_[client_sock_fd] = client;
    
    // FIXME When does clients get removed?!
}
