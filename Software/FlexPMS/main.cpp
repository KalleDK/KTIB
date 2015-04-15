#include <string>
#include <iostream>

// MySQL++ connector library includes (mysqlcppconn)
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

// Project includes
#include "Bridge.h"
#include "KarBus.h"
#include "SocketServer.h"


using namespace std;


sql::Connection* mysql_connect() {
    sql::Driver *db_driver;
    sql::Connection *db_conn;
    
    cout << "Connecting to database server" << endl;
    db_driver = get_driver_instance();
    db_conn = db_driver->connect("tcp://localhost", "laerke", "stud");
    
    cout << "Selecting database" << endl;
    db_conn->setSchema("testdb");
    
    return db_conn;
}


void run(sql::Connection *db_conn) {
    // Create threads
    cout << "Creating KarBus thread" << endl;
    KarBus kar_bus;
    kar_bus.start();
    
    cout << "Running Bridge thread" << endl;
    Bridge bridge(db_conn, &kar_bus);
    bridge.start();
    
    cout << "Creating SocketServer thread" << endl;
    SocketServer server(&bridge);
    server.start();
    
    // Join threads
    kar_bus.join();
    bridge.join();
    server.join();
}


int main(int argc, char** argv) {
    sql::Connection *db_conn = mysql_connect();
    run(db_conn);
}
